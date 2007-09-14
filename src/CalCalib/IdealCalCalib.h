#ifndef IdealCalCalib_H
#define IdealCalCalib_H
// $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/CalCalib/IdealCalCalib.h,v 1.5 2006/04/26 20:23:54 fewtrell Exp $

// LOCAL INCLUDES

// GLAST INCLUDES

// EXTLIB
#include "GaudiKernel/StatusCode.h"

// STD
#include <vector>
#include <string>


/** @class IdealCalCalib
    @author Zachary Fewtrell
    \brief Contains all values necessary to generate full set of ideal cal
    calibration constants
    
    contains code to read constants in from IFile XML file.
*/
class IdealCalCalib {
 public:
  IdealCalCalib();

  /// read in calibration values from IFile XML file
  StatusCode readCfgFile(const std::string &path);

  //////////////////
  //// PEDS ///
  //////////////////
  /// 1 pedestal value per range
  std::vector<double> pedVals;       
  /// associated correlated pedestal cosines
  std::vector<double> pedCos;        
  /// sigma/val for all pedestal calibrations
  std::vector<double> pedSigs;       

  //////////////////
  //// ASYMMETRY ///
  //////////////////
  /// val for Large Diode asym spline at neg face
  float asymLrgNeg;            
  /// val for Large Diode asym spline at pos face
  float asymLrgPos;            
        
  /// val for Small Diode asym spline at neg face
  float asymSmNeg;             
  /// val for Small Diode asym spline at pos face
  float asymSmPos;             
        
  /// val for PSNB asym spline at neg face
  float asymPSNBNeg;           
  /// val for PSNB asym spline at pos face
  float asymPSNBPos;           
        
  /// val for NSPB asym spline at neg face
  float asymNSPBNeg;           
  /// val for NSPB asym spline at pos face
  float asymNSPBPos;           
        
  /// sigma/val for all vals in asym group 
  float asymSigPct;            
  

  ///////////////////
  //// MEVPERDAC ////
  ///////////////////
  /// Large diode MevPerDac 
  float mpdLrg;                
  /// Small diode MevPerDac 
  float mpdSm;                 
        
  /// sigma/val for all vals in MPD group 
  float mpdSigPct;             
  

  /////////////////
  //// THOLD_CI ///
  /////////////////
  /// FLE threshold 
  float ciFLE;                 
  /// FHE threshold 
  float ciFHE;                 
  /// LAC threshold 
  float ciLAC;                 
  /// 1 ULD threshold per ADC range
  std::vector<double> ciULD;         
  /// sigma/val for all vals in THOLD_CI group
  float ciSigPct;              
  /// 1 pedestal value per ADC range           
  std::vector<double> ciPeds;        
  

  ///////////////////
  //// THOLD_MUON ///
  ///////////////////
  /// FLE threshold 
  float muonFLE;               
  /// FHE threshold 
  float muonFHE;               
  /// sigma/val for all vals in THOLD_MUON group 
  float muonSigPct;            
  /// 1 pedestal value per ADC range      
  std::vector<double> muonPeds;      

  ///////////////////
  //// INT_NONLIN ///
  ///////////////////
  /// ADC/DAC for each ADC range 
  std::vector<double> inlADCPerCIDAC;          
  /// sigma/val for all vals in INT_NONLIN group
  float inlSigPct;             

 private:

  //-- SECTION DECRIPTION STRINGS --//
  static const std::string PEDS;
  static const std::string ASYM;
  static const std::string THOLD_CI;
  static const std::string THOLD_MUON;
  static const std::string INL;
  static const std::string MPD;
};



#endif
