#ifndef IXtalRecTool_H
#define IXtalRecTool_H
// $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/CalXtalResponse/IXtalRecTool.h,v 1.4 2006/01/09 21:08:20 fewtrell Exp $
/*! @class IXtalRecTool
 *
 * \author Zach Fewtrell
 *
 * \brief Abstract interface class for calculation of total deposited energy 
 & centroid position for a single calorimeter crystal.
 * 
 *
 */

// GLAST INCLUDES
#include "Event/TopLevel/Event.h"
#include "Event/Digi/CalDigi.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"
#include "CalUtil/CalArray.h"

// EXTLIB INCLUDES
#include "GaudiKernel/IAlgTool.h"

// STD INCLUDES

static const InterfaceID IID_IXtalRecTool("IXtalRecTool", 1, 1);

class IXtalRecTool : virtual public IAlgTool {

 public:
  static const InterfaceID& interfaceID() { return IID_IXtalRecTool; }

  /** \brief estimate total deposited energy given the digital
      response for both faces

      \param digi single input digi readout 

      \param xtalRec expected to contain calxtalid field but no 
      CalRangeRecData objects.  calculate() will attempt 
      to create 1 rangeRec object, but this will not always be possible.
      Caller should check xtalRec.getNReadouts() to see if xtal recon 
      was successful.  It _is_ possible for this function to return 
      StatusCode::SUCCESS along w/ 0 CalRangeRecData objects.

      \param belowThresh returns TRUE for each face signal below noise for any 
      adc range

      \param xtalBelowThresh returns TRUE if either face signal below LEX8 noise level

      \param saturated returns TRUE for each face signal that is >= HEX1 
      saturation level

      \param evtHdr (optional) pointer to current event header (used for RunID 
      & EventID in optional XtalRecTuple)

      \return It _is_ possible for this function to return 
      StatusCode::SUCCESS along w/ 0 CalRangeRecData objects.


  */
  virtual StatusCode calculate(const Event::CalDigi &digi,
                               Event::CalXtalRecData &xtalRec,
                               CalUtil::CalArray<CalUtil::FaceNum, bool> &belowThresh,
                               bool &xtalBelowThresh,
                               CalUtil::CalArray<CalUtil::FaceNum, bool> &saturated,
                               const Event::EventHeader *evtHdr=0
                               ) = 0;
};

#endif //_IXtalRecTool_H
