// LOCAL
#include "CalXtalResponse/IXtalEneTool.h"
#include "CalXtalResponse/ICalCalibSvc.h"

// GLAST
#include "CalUtil/CalDefs.h"

// EXTLIB
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/MsgStream.h"

// STD
#include <cmath>
#include <algorithm>

using namespace CalDefs;

/*! @class XtalEneTool
  \author Zachary Fewtrell
  \brief Simple implementation of IXtalEneTool.  Faithfully pasted from XtalRecAlg v5r6p1
*/

class XtalEneTool : 
  public AlgTool, 
  virtual public IXtalEneTool 
{
public:

  /// default ctor, declares jobOptions.
  XtalEneTool::XtalEneTool( const string& type, 
                            const string& name, 
                            const IInterface* parent);

  /// gets needed parameters and pointers to required services
  virtual StatusCode initialize();

  /// calculate energy deposition given the digi response for both xtal faces
  StatusCode calculate(const CalXtalId &xtalId, 
                       CalXtalId::AdcRange rngP,
                       CalXtalId::AdcRange rngN,
                       int adcP, 
                       int adcN,
                       float &energy,     
                       bool  &rngBelowThresh,
                       bool  &xtalBelowThresh,
                       CalTupleEntry *calTupleEnt
                       );

  /// calculate energy deposition given the digi response for one xtal face
  StatusCode calculate(const CalXtalId &xtalId,
                       int adc, 
                       float position,
                       float &energy,     
                       bool  &rngBelowThresh,
                       bool  &xtalBelowThresh
                       );
private:

  /// name of CalCalibSvc to use for calib constants.
  StringProperty m_calCalibSvcName;                         
  /// pointer to CalCalibSvc object.
  ICalCalibSvc *m_calCalibSvc;                             

};

static ToolFactory<XtalEneTool> s_factory;
const IToolFactory& XtalEneToolFactory = s_factory;

XtalEneTool::XtalEneTool( const string& type,
                          const string& name,
                          const IInterface* parent)
  : AlgTool(type,name,parent) {
  declareInterface<IXtalEneTool>(this);

  declareProperty("CalCalibSvc", m_calCalibSvcName="CalCalibSvc");
}

StatusCode XtalEneTool::initialize() {
  MsgStream msglog(msgSvc(), name());
  msglog << MSG::INFO << "initialize" << endreq;

  StatusCode sc;

  //-- jobOptions --//
  if ((sc = setProperties()).isFailure()) {
    msglog << MSG::ERROR << "Failed to set properties" << endreq;
    return sc;
  }

  // obtain CalCalibSvc
  sc = service(m_calCalibSvcName.value(), m_calCalibSvc);
  if (sc.isFailure()) {
    msglog << MSG::ERROR << "can't get CalCalibSvc." << endreq;
    return sc;
  }

  return StatusCode::SUCCESS;
}

StatusCode XtalEneTool::calculate(const CalXtalId &xtalId, 
                                  CalXtalId::AdcRange rngP,
                                  CalXtalId::AdcRange rngN,
                                  int adcP, 
                                  int adcN,
                                  float &energy,
                                  bool &rngBelowThresh,
                                  bool &xtalBelowThresh,
                                  CalTupleEntry *calTupleEnt
                                  ) {
  StatusCode sc;
  XtalIdx xtalIdx(xtalId); // used for array indexing
  TwrNum twr = xtalIdx.getTwr();
  LyrNum lyr = xtalIdx.getLyr();
  ColNum col = xtalIdx.getCol();

  // initial return vals
  energy = 0;
  rngBelowThresh  = false;
 
  //-- GET PED && LAC (POS_FACE) --//
  float pedP, pedN, sigP, sigN, cos;
  RngIdx rngIdxP(xtalIdx, POS_FACE, rngP);
  sc = m_calCalibSvc->getPed(rngIdxP.getCalXtalId(), pedP, sigP, cos);
  if (sc.isFailure()) return sc;
  float adcPedP = adcP - pedP;   // ped subtracted ADC
  
  CalibData::ValSig fle,fhe,lacP, lacN;
  CalXtalId tmpIdP(twr,lyr,col,POS_FACE);
  sc = m_calCalibSvc->getTholdCI(tmpIdP,fle,fhe,lacP);
  
  //-- GET PED && LAC (NEG_FACE) --//
  RngIdx rngIdxN(xtalIdx, NEG_FACE, rngN);
  sc = m_calCalibSvc->getPed(rngIdxN.getCalXtalId(), pedN, sigN, cos);
  if (sc.isFailure()) return sc;
  float adcPedN = adcN - pedN; // ped subtracted ADC

  CalXtalId tmpIdN(twr,lyr,col,NEG_FACE);
  sc = m_calCalibSvc->getTholdCI(tmpIdN,fle,fhe,lacN);
  
  //-- THROW OUT LOW ADC VALS --/
  // LEX8 range is compared against 0.5 * lac threshold
  // we throw out entire xtal if adc is too low 
  if ((rngP == LEX8 && adcPedP < lacP.getVal()*0.5) ||
      (rngN == LEX8 && adcPedN < lacN.getVal()*0.5)) {
    xtalBelowThresh = true;
    rngBelowThresh  = true;
    return StatusCode::SUCCESS;
  }

  // other ranges are compared against 5 sigma.
  // energy estimate for this range is thrown out
  // but overall xtal estimate may still be valid.
  // ene = 0, below thresh = false
  if ((rngP != LEX8 && adcPedP < sigP*5.0) ||
      (rngN != LEX8 && adcPedN < sigN*5.0)) {
    rngBelowThresh = true;
    return StatusCode::SUCCESS;
  }

  //-- CONVERT ADCs -> DAC --//
  double dacP, dacN;
  sc = m_calCalibSvc->evalDAC(rngIdxP.getCalXtalId(), adcPedP, dacP);
  if (sc.isFailure()) return sc;
  sc = m_calCalibSvc->evalDAC(rngIdxN.getCalXtalId(), adcPedN, dacN);
  if (sc.isFailure()) return sc;
  
  //-- CONVERT DIODE SIZE (IF NEEDED) --//
  // if diodes are different, then I need to convert them both to sm diode
  DiodeNum diodeP = RngNum(rngP).getDiode();
  DiodeNum diodeN = RngNum(rngN).getDiode();
  if (diodeP != diodeN) { 
    // get small diode asymmetry at center of xtal (used for both conversions)
    // NOTE:
    // using center of xtal for position since the lrg/sm ratio is 
    // effectively constant throughout the xtal length and this keeps the
    // energy calculation independent of the position calculation.
    double asymSm;
    sc = m_calCalibSvc->evalAsymSm(xtalId, 0.0, asymSm);
    if (sc.isFailure()) return sc;

    if (diodeP == LRG_DIODE) {
      // STRATEGY:
      // convert PL -> PS
      // AsymNSPB = log(PL / NS)
      // AsymSm   = log(PS / NS)
      // exp(sm)/exp(asymNSPB) = (PS/NS)/(PL/NS) = PS/PL
      //      exp(sm-asymNSPB) = PS/PL
      
      double asymNSPB;
      sc = m_calCalibSvc->evalAsymNSPB(xtalId, 0.0, asymNSPB);
      if (sc.isFailure()) return sc;

      dacP *= exp(asymSm - asymNSPB);
      diodeP = SM_DIODE;
    } else { // case: diodeN == LRG
      // STRATEGY:
      // convert NL -> NS
      // asymPSNB = log(PS/NL)
      // asymSm   = log(PS/NS)
      // exp(asymPSNB)/exp(sm) = (PS/NL)/(PS/NS)
      //      exp(asymPSNB-sm) = NS/NL

      double asymPSNB;
      sc = m_calCalibSvc->evalAsymPSNB(xtalId, 0.0, asymPSNB);
      if (sc.isFailure()) return sc;

      dacN *= exp(asymPSNB-asymSm);
      diodeN = SM_DIODE;
    }
  }
  // now both dac vals are for the same diode size

  //-- GET MEVPERDAC --//
  CalibData::ValSig mpdLrg, mpdSm;
  sc = m_calCalibSvc->getMeVPerDac(xtalId, mpdLrg, mpdSm);
  if (sc.isFailure()) return sc;
  
  //-- CALC MEAN DAC & ENERGY --//
  // check for invalid dac vals (i need to take the sqrt)
  if (dacP < 0 || dacN < 0) {
    // create MsgStream only when needed for performance
    MsgStream msglog(msgSvc(), name()); 
    msglog << MSG::WARNING;
    // need to use .stream() to get xtalId to pretty print.
    msglog.stream() << "DAC val < 0, can't calculate energy." 
                    <<  " xtal=[" << xtalId << ']';
    msglog << endreq;
    return StatusCode::FAILURE;
  } 
  double meanDAC = sqrt(dacP*dacN);
  
  if (diodeP == SM_DIODE)
    energy = meanDAC*mpdSm.getVal();
  else
    energy = meanDAC*mpdLrg.getVal();

  ///////////////////////////////////////////
  //-- STEP 5: CALCULATE XTAL-FACE SIGNAL--//
  ///////////////////////////////////////////
  
  // 1st 
  double faceSignalP = dacP*((diodeP == SM_DIODE) ? mpdSm.getVal() : mpdLrg.getVal());
  double faceSignalN = dacN*((diodeN == SM_DIODE) ? mpdSm.getVal() : mpdLrg.getVal());

  // 2nd correct for overall asymmetry of diodes (asym at center of xtal)
  // retrieve asym
  double asymCtrLrg, asymCtrSm;
  sc = m_calCalibSvc->evalAsymLrg(xtalId, 0, asymCtrLrg);
  if (sc.isFailure()) return sc;
  sc = m_calCalibSvc->evalAsymSm(xtalId, 0, asymCtrSm);
  if (sc.isFailure()) return sc;
  
  // move asym out of log scale
  asymCtrLrg = exp(asymCtrLrg);
  asymCtrSm  = exp(asymCtrSm);

  // apply assymetry correction
  faceSignalP *= 1/sqrt((diodeP == LRG_DIODE) ? asymCtrLrg : asymCtrSm);
  faceSignalN *= sqrt((diodeN == LRG_DIODE) ? asymCtrLrg : asymCtrSm);


  //-- POPULATE TUPLE (OPTIONAL) --//
  if (calTupleEnt) {
    calTupleEnt->m_calXtalAdcPed[twr][lyr][col][POS_FACE] = adcPedP;
    calTupleEnt->m_calXtalAdcPed[twr][lyr][col][NEG_FACE] = adcPedN;
        
    calTupleEnt->m_calXtalFaceSignal[twr][lyr][col][POS_FACE] = faceSignalP;
    calTupleEnt->m_calXtalFaceSignal[twr][lyr][col][NEG_FACE] = faceSignalN;
  }

  return StatusCode::SUCCESS;
}

StatusCode XtalEneTool::calculate(const CalXtalId &xtalId,
                                  int adc, 
                                  float position,
                                  float &energy,  
                                  bool &rngBelowThresh,
                                  bool &xtalBelowThresh
                                  ) {
  StatusCode sc;

  energy = 0;
  rngBelowThresh  = false;
  
  // check that CalXtalId has face & range fields enabled
  if (!xtalId.validFace() || !xtalId.validRange())
    throw invalid_argument("XtalEneTool: CalXtalId requires valid"
                           " face & range fields");

  RngNum rng(xtalId.getRange());
  
  // get ped
  float ped, sig, cos;
  sc = m_calCalibSvc->getPed(xtalId, ped, sig, cos);
  if (sc.isFailure()) return sc;
  float adcPed = adc - ped;

  // LEX8 range is compared against 0.5 * lac threshold
  // we throw out entire xtal if adc is too low (belowThresh = true)
  if (rng == LEX8) {
    //-- THROW OUT LOW ADCS --/
    CalibData::ValSig fle,fhe,lac;
    
    // retreive threshold
    sc = m_calCalibSvc->getTholdCI(xtalId,fle,fhe,lac);
    
    // set flag if adc val is too low
    if (adcPed < lac.getVal()*0.5) {
      rngBelowThresh  = true;
      xtalBelowThresh = true;
      return StatusCode::SUCCESS;
    }
  } else {
    // other ranges are compared against 5 sigma.
    // energy estimate for this range is thrown out
    // but overall xtal estimate may still be valid.
    // ene = 0, below thresh = false
    if (adcPed < 5.0*sig) {
      rngBelowThresh = true;
      return StatusCode::SUCCESS;
    }
  }

  // convert adc->dac units
  double dac;
  sc = m_calCalibSvc->evalDAC(xtalId, adcPed, dac);
  if (sc.isFailure()) return sc;
  
  // check for invalid dac vals (i need to take the sqrt)
  if (dac < 0) {
    // create MsgStream only when needed for performance
    MsgStream msglog(msgSvc(), name()); 
    msglog << MSG::WARNING << "DAC val < 0, can't calculate energy.  This shouldn't happen." << endreq;
    return StatusCode::FAILURE;
  } 
  
  DiodeNum diode(rng.getDiode());

  // use appropriate asymmetry to calc position based on
  // diode size
  double asym;
  if (diode == SM_DIODE) {
    sc = m_calCalibSvc->evalAsymSm(xtalId, position, asym);
    if (sc.isFailure()) return sc;
  } else {
    sc = m_calCalibSvc->evalAsymLrg(xtalId, position, asym);
    if (sc.isFailure()) return sc;
  }

  // calculate the 'other' dac
  FaceNum face = xtalId.getFace();
  double otherDac;
  if (face == POS_FACE) otherDac = dac/exp(asym);
  else otherDac = dac*exp(asym);

  double meanDAC = sqrt(dac*otherDac);

  CalibData::ValSig mpdLrg, mpdSm;
  sc = m_calCalibSvc->getMeVPerDac(xtalId, mpdLrg, mpdSm);
  if (sc.isFailure()) return sc;

  if (diode == SM_DIODE)
    energy = meanDAC*mpdSm.getVal();
  else
    energy = meanDAC*mpdLrg.getVal();
  
  return StatusCode::SUCCESS;
}
