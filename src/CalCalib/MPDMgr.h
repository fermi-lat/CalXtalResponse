#ifndef MPDMgr_H
#define MPDMgr_H
// $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/CalXtalResponse/src/CalCalib/MPDMgr.h,v 1.10 2008/01/22 20:14:47 fewtrell Exp $
/** @file 
    @author Z.Fewtrell
*/
// LOCAL
#include "CalibItemMgr.h"

// GLAST
#include "CalibData/Cal/CalMevPerDac.h"

// EXTLIB

// STD
#include <memory>

class CalCalibSvc;

/** \brief Manage MevPerDAC calibration type
    \author Z.Fewtrell
    */
class MPDMgr : public CalibItemMgr {
 public:
  MPDMgr(CalCalibShared &ccsShared) :
    CalibItemMgr(ICalibPathSvc::Calib_CAL_MevPerDac,
                 ccsShared,
                 CalUtil::XtalIdx::N_VALS
                 ) {}

  const CalibData::CalMevPerDac *getMPD(const CalUtil::XtalIdx xtalIdx);

 private:
  /// load ideal calibration values from local data store (bypass
  /// calib db)
  StatusCode loadIdealVals();
  
  /// populate all internal calibration data
  StatusCode genLocalStore();

  /// validate single crystal calibration data.
  static bool validateRangeBase(CalibData::CalMevPerDac const*const mpd);

  /// ideal data is not available till after construction & I will be responsible
  /// for memory, hence use of auto_ptr<>
  std::auto_ptr<CalibData::CalMevPerDac> m_idealMPD;
};

#endif
