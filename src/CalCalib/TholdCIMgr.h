#ifndef TholdCIMgr_H
#define TholdCIMgr_H
// $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/CalCalib/TholdCIMgr.h,v 1.8 2006/04/26 20:23:54 fewtrell Exp $
// LOCAL
#include "CalibItemMgr.h"

// GLAST
#include "CalUtil/CalDefs.h"
#include "CalibData/Cal/CalTholdCI.h"

// EXTLIB

// STD
#include <memory>


class CalCalibSvc;

/** @class TholdCIMgr
    @author Zachary Fewtrell
    
    \brief Manage GLAST Cal charge-injection measured threshold data.
*/

class TholdCIMgr : public CalibItemMgr {
 public:
  TholdCIMgr(CalCalibShared &ccsShared) 
    : CalibItemMgr(ICalibPathSvc::Calib_CAL_TholdCI, ccsShared)
    {};

  const CalibData::CalTholdCI *getTholdCI(CalUtil::FaceIdx faceIdx);

 private:
  StatusCode loadIdealVals();

  
  StatusCode genLocalStore();
  
  /// Validate TDS data entry (for empty ptrs & fun stuff like that)
  bool validateRangeBase(CalibData::CalTholdCI *tholdCI);

  std::auto_ptr<CalibData::CalTholdCI> m_idealTholdCI;
  
};

#endif
