#ifndef CalCalibSvc_H
#define CalCalibSvc_H
// $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/CalCalib/CalCalibSvc.h,v 1.10 2007/09/14 16:03:51 fewtrell Exp $

// LOCAL 
#include "AsymMgr.h"
#include "IntNonlinMgr.h"
#include "MPDMgr.h"
#include "PedMgr.h"
#include "TholdCIMgr.h"
#include "IdealCalCalib.h"
#include "CalCalibShared.h"


// GLAST 
#include "CalXtalResponse/ICalCalibSvc.h"
#include "CalUtil/CalDefs.h"
#include "CalUtil/CalVec.h"


// EXTLIB
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/IIncidentListener.h"

// STD

/** @class CalCalibSvc
    @author Zachary Fewtrell
    * \brief Instatiates ICalCalibSvc interface, gets data from CalibDataSvc
    *
    * handles:
    * - data storage/destruction
    * - communication with Gleam lower level services
    * - checking of data validity period  
    * - extraction of cal-specific constants out of generic data objects
    * - creation/caching of local meta-data objects where needed (e.g. splines)
    *
    * \author  Zachary Fewtrell
    *
    */

class CalCalibSvc : public Service, virtual public ICalCalibSvc,
    virtual public IIncidentListener  {
  
    public:
  
  CalCalibSvc(const string& name, ISvcLocator* pSvcLocator); 
  
  StatusCode initialize();
  StatusCode execute();
  StatusCode finalize () {return StatusCode::SUCCESS;}



  /// queryInterface - for implementing a Service this is necessary
  StatusCode queryInterface(const InterfaceID& riid, void** ppvUnknown);

  /// return the service type
  const InterfaceID&  CalCalibSvc::type () const {return IID_ICalCalibSvc;}

  const CalibData::CalMevPerDac *getMPD(CalUtil::XtalIdx xtalIdx) {return m_mpdMgr.getMPD(xtalIdx);}

  const std::vector<float> *getInlAdc(CalUtil::RngIdx rngIdx) {
    return m_inlMgr.getInlAdc(rngIdx);}

  const std::vector<float> *getInlCIDAC(CalUtil::RngIdx rngIdx) {
    return m_inlMgr.getInlCIDAC(rngIdx);}

  //  const CalibData::Ped *getPed(CalUtil::RngIdx rngIdx);

  StatusCode getPed(CalUtil::RngIdx rngIdx, float &ped);
  StatusCode getPedSig(CalUtil::RngIdx rngIdx, float &sig);


  const CalibData::CalAsym *getAsym(CalUtil::XtalIdx xtalIdx) {return m_asymMgr.getAsym(xtalIdx);}
  
  const CalibData::Xpos *getAsymXpos() {return m_asymMgr.getXpos();}

  const CalibData::CalTholdCI *getTholdCI(CalUtil::FaceIdx faceIdx) {return m_tholdCIMgr.getTholdCI(faceIdx);}

  StatusCode evalCIDAC (CalUtil::RngIdx rngIdx, float adc,   float &cidac) {
    return m_inlMgr.evalCIDAC(rngIdx, adc, cidac);
  }

  StatusCode evalADC (CalUtil::RngIdx rngIdx, float cidac,   float &adc) {
    return m_inlMgr.evalADC(rngIdx, cidac, adc);
  }

  StatusCode evalAsym(CalUtil::XtalIdx xtalIdx, CalUtil::AsymType asymType, 
                      float pos,   float &asym) {
    return m_asymMgr.evalAsym(xtalIdx, asymType, pos, asym);
  }

  StatusCode evalPos (CalUtil::XtalIdx xtalIdx, CalUtil::AsymType asymType, 
                      float asym,  float &pos) {
    return m_asymMgr.evalPos(xtalIdx, asymType, asym, pos);
  }

  StatusCode evalFaceSignal(CalUtil::RngIdx rngIdx, float adc, float &ene);

  StatusCode getMPDDiode(CalUtil::DiodeIdx diodeIdx, float &mpdDiode);

  StatusCode getAsymCtr(CalUtil::XtalIdx xtalIdx, CalUtil::AsymType asymType, 
                        float &asymCtr) {
    return m_asymMgr.getAsymCtr(xtalIdx, asymType, asymCtr);
  }


    private:
  ////////////////////////////////////////////////
  ////// PARAMETER MANAGEMENT ////////////////////
  ////////////////////////////////////////////////

  // JobOptions PROPERTIES

  ///  default flavor for all calib types, unless otherwise specified.
  StringProperty m_defaultFlavor;        

  /// calib flavor override for int-nonlin constants
  StringProperty m_flavorIntNonlin;      
  /// calib flavor override for asymmetry constants
  StringProperty m_flavorAsym;           
  /// calib flavor override for ped constants
  StringProperty m_flavorPed;            
  /// calib flavor override for MeVPerDac constants
  StringProperty m_flavorMPD;            
  /// calib flavor override for CI measured thresholds
  StringProperty m_flavorTholdCI;
  /// file with CU CAL tower temperature measurements
  StringProperty m_temperatureFile;
  /// file with pedestal temperature correction data
  StringProperty m_pedTempCorFile;

  ///CU temperature measurements table
  vector<int  > m_tempTime;
  vector<float> m_cuTwrTemp[4];
  float m_cur_temp_twr[4];
  int m_nEvent;

  ///CU pedestal temperature correction data
 
    CalUtil::CalVec<CalUtil::RngIdx, float> m_pedT0;
    CalUtil::CalVec<CalUtil::RngIdx, float> m_pedTempCoef;

     


  /// this class is shared amongt the CalibItemMgr classes
  CalCalibShared m_ccsShared;

  PedMgr       m_pedMgr;
  IntNonlinMgr m_inlMgr;
  AsymMgr      m_asymMgr;
  MPDMgr       m_mpdMgr;
  TholdCIMgr   m_tholdCIMgr;

  IDataProviderSvc* m_eventSvc;

  /// set to true when we have retrieved event time for current event.
  bool m_gotPedTime;

  /// hook the BeginEvent so that we can check our validity once per event.
  void handle ( const Incident& inc );

  int getSerNoPed(){
    return m_pedMgr.getSerNo();
  }

  int getSerNoINL(){
    return m_pedMgr.getSerNo();
  }

  int getSerNoAsym(){
    return m_pedMgr.getSerNo();
  }

  int getSerNoMPD(){
    return m_pedMgr.getSerNo();
  }

  int getSerNoTholdCI(){
    return m_pedMgr.getSerNo();
  }
};

#endif // CalCalibSvc_H
