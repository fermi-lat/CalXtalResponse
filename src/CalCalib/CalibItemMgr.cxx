// $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/CalCalib/CalibItemMgr.cxx,v 1.12 2007/10/09 19:05:40 fewtrell Exp $
/** @file
    @author Zach Fewtrell
*/
// LOCAL
#include "CalibItemMgr.h"
#include "CalCalibSvc.h"

// GLAST
#include "CalibData/DacCol.h"
#include "CalibData/Cal/Xpos.h"


// EXTLIB

// STD
#include <sstream>
#include <algorithm>

using namespace CalUtil;

using namespace std;
///////////////// GENERIC UTILITIES //////////////////////////////////

/**
   functional class deletes a pointer
   fun to use w/ for_each template
     
   I got it from here - Z.F.
   Newsgroups: comp.lang.c++.moderated
   From: Didier Trosset <didier-dot-tros...@acqiris.com>
   Date: 21 Oct 2004 15:39:18 -0400
   Subject: Re: Standard way to delete container of pointers
*/
struct delete_ptr
{
  template <class P>
  void operator() (P p)
  {
    delete p;
    p = 0;
  }
};

/// template function calls delete on all pointers stored in a STL container
template<class T> void del_all_ptrs(T &container) {
  for_each(container.begin(),container.end(),delete_ptr());
}

//////////////////////////////////////////////////////////////////////

StatusCode CalibItemMgr::initialize(const string &flavor) {
  StatusCode sc;

  m_flavor = flavor;

  m_calibPath = m_ccsShared.m_calibPathSvc->getCalibPath(m_calibItem, flavor);
  
  sc = loadIdealVals();
  if (sc.isFailure()) return sc;

  //-- IDEAL MODE --//
  if (m_flavor == "ideal") {
    m_idealMode = true;

    // localstore will not be automatically 
    // generated by standard code b/c we are circumventing it.
    // so we have to call this ourselves.
    // it's a hack, but so be it.
    sc = genLocalStore();
    if (sc.isFailure()) return sc;

    // create MsgStream only when needed (for performance)
    MsgStream msglog(m_ccsShared.m_service->msgSvc(), m_ccsShared.m_service->name()); 
    msglog << MSG::INFO << m_calibPath << " Overriding CalibSvc with ideal calibrations." << endreq;

    m_serNo = SERNO_IDEAL;
  }

  return StatusCode::SUCCESS;
}

StatusCode CalibItemMgr::updateCalib() {
  StatusCode sc;

  // ideal mode never requires update, is never invalid
  if (m_idealMode) return StatusCode::SUCCESS;
  
  // if event is already validated return quickly
  if (m_isValid) return StatusCode::SUCCESS;

  /////////////////////////////////
  //-- CHECK TDS DATA VALIDITY --//
  /////////////////////////////////

  // Get pointer to Gain tree from TDS
  // usually this f() should return immediately
  // if it fails then we have no valid calib data
  // for the current event.
  DataObject *pObject;
  sc = m_ccsShared.m_dataProviderSvc->retrieveObject(m_calibPath, pObject);
  if (!sc.isFailure())
    m_calibBase = (CalibData::CalCalibBase *)(pObject);
  else {
    // create MsgStream only when needed (for performance)
    MsgStream msglog(m_ccsShared.m_service->msgSvc(), m_ccsShared.m_service->name()); 
    
    // else return error (can't find calib)
    msglog << MSG::ERROR << "can't get " 
           << m_calibPath << " from calib db" << endreq;
    return sc;  
  }

  ///////////////////////////////////////
  //-- CHECK IF TDS DATA HAS CHANGED --//
  ///////////////////////////////////////

  // check serial # to see if we're still valid.
  int curSerNo = m_calibBase->getSerNo();
  if (curSerNo != m_serNo) {
    // create MsgStream only when needed (for performance)
    MsgStream msglog(m_ccsShared.m_service->msgSvc(), m_ccsShared.m_service->name()); 
    msglog << MSG::INFO << "Updating " << m_calibPath << endreq;
    m_serNo = curSerNo;
    clearLocalStore();
        
    // generate local data (e.g. splines) if needed
    sc = genLocalStore();
    if (sc.isFailure()) return sc;
  }

  // reset 'in update' flag
  m_isValid = true;

  return StatusCode::SUCCESS;
}

StatusCode CalibItemMgr::evalSpline(int calibType, LATWideIndex idx, 
                                    float x, float &y) {
  StatusCode sc;

  // make sure we have valid calib data for this event.
  sc = updateCalib();
  if (sc.isFailure()) return sc;

  // check that we have a spline for this particular xtal
  // (i.e. when LAT is not fully populated)
  TSpline3 *spline = m_splineLists[calibType][idx];
  if (!spline) {
    ostringstream msg;
    MsgStream msglog(m_ccsShared.m_service->msgSvc(), m_ccsShared.m_service->name()); 
    msglog << MSG::INFO 
           << "No spline data found for " << m_calibPath 
           << " xtal=" << idx.val()
           << endreq;

    return StatusCode::FAILURE;
  }

  // bounds check input to spline function to avoid
  // weird behavior
  x = max<float>(m_splineXMin[calibType][idx],x);
  x = min<float>(m_splineXMax[calibType][idx],x);

  y = spline->Eval(x);

#if 0
  //-- USEFUL FOR DEBUGGING SPLINE BEHAVIOR --//
  // create MsgStream only when needed (for performance)
  if (idx.val()==48) {
    MsgStream msglog(m_ccsShared.m_service->msgSvc(), m_ccsShared.m_service->name()); 
    msglog << MSG::VERBOSE << "Evaluating spline: "
           << spline->GetName()
           << " " << spline
           << " idx=" << idx.val()
           << " X=" << x
           << " Y=" << y
           << endreq;
  }
#endif

  
  return StatusCode::SUCCESS;
}

StatusCode CalibItemMgr::genSpline(int calibType, LATWideIndex idx, const string &name, 
                                   const vector<float> &x, const vector<float> &y) {

  int n = min(x.size(),y.size());

  // create tmp arrays for TSpline ctor
  double *xp = new double[n];
  double *yp = new double[n];

  // copy vector data into temp arrays
  copy(x.begin(),x.begin()+n,xp);
  copy(y.begin(),y.begin()+n,yp);


  TSpline3 *mySpline = new TSpline3(name.c_str(),
                                    xp,yp,n);
  mySpline->SetName(name.c_str());

#if 0
  //-- USEFUL FOR DEBUGGING SPLINE BEHAVIOR --//
  // create MsgStream only when needed (for performance)
  if (idx.val()==48) {
    MsgStream msglog(m_ccsShared.m_service->msgSvc(), m_ccsShared.m_service->name()); 
    msglog << MSG::VERBOSE << "Generating spline: "
           << mySpline->GetName()
           << " " << mySpline
           << " idx=" << idx.val();
    for (int i = 0; i < n; i++)
      msglog << "\tX=" << xp[i]
             << " Y=" << yp[i];
    msglog << endreq;
  }
#endif

  
  // put spline in list
  m_splineLists[calibType][idx] = mySpline;
  // populate x-axis boundaries
  m_splineXMin[calibType][idx] = xp[0];
  m_splineXMax[calibType][idx] = xp[n-1];

  // clear heap variables
  delete [] xp;
  delete [] yp;
  
  return StatusCode::SUCCESS;
}

/// Template function fills any STL type container with zero values
template <class T> static void fill_zero(T &container) {
  fill(container.begin(), container.end(), 0);
}

void CalibItemMgr::clearLocalStore() {   
  m_rngBases.clear();

  // m_splineLists is the 'owner' of the splines, so i need to delete
  // the objects themselves as well as the pointers.
  for (unsigned i = 0; i < m_splineLists.size(); i++) {
    del_all_ptrs(m_splineLists[i]);
    fill_zero(m_splineXMin[i]);
    fill_zero(m_splineXMax[i]);
  }
}

