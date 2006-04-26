#ifndef MPDMgr_H
#define MPDMgr_H
// $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/CalCalib/MPDMgr.h,v 1.6 2006/01/09 21:08:20 fewtrell Exp $
// LOCAL
#include "CalibItemMgr.h"

// GLAST
#include "CalibData/Cal/CalMevPerDac.h"

// EXTLIB

// STD
#include <memory>

using namespace CalUtil;
using namespace idents;
using namespace CalibData;


class CalCalibSvc;

/** \brief Manage MevPerDAC calibration type
    \author Zach Fewtrell
    */
class MPDMgr : public CalibItemMgr {
 public:
  MPDMgr(CalCalibShared &ccsShared) : 
    CalibItemMgr(CAL_MevPerDac, ccsShared) {};

  const CalMevPerDac *getMPD(XtalIdx xtalIdx);

 private:
  StatusCode loadIdealVals();
  
  StatusCode genLocalStore();

  bool validateRangeBase(CalMevPerDac *mpd);

  bool validateRangeBase(RangeBase *rangeBase) {return true;}

  /// ideal data is not available till after construction & I will be responsible
  /// for memory, hence use of auto_ptr<>
  auto_ptr<CalMevPerDac> m_idealMPD;
};

#endif
