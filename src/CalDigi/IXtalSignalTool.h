#ifndef IXtalSignalTool_H
#define IXtalSignalTool_H
// $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/CalDigi/IXtalSignalTool.h,v 1.2 2007/11/06 20:54:00 fewtrell Exp $
/** @file 
    @author Z.Fewtrell
*/
/*! @class IXtalSignalTool
 *
 * \author Z.Fewtrell
 *
 * \brief Abstract interface class for calculcation of Cal crystal diode signal levels from a single energy deposit
 * 
 *
 */

// GLAST INCLUDES
#include "CalUtil/CalDefs.h"
#include "CalUtil/CalVec.h"

// EXTLIB INCLUDES
#include "GaudiKernel/IAlgTool.h"

// STD INCLUDES

// FORWARD DECLARES
namespace Event {
  class McIntegratingHit;
}

static const InterfaceID IID_IXtalSignalTool("IXtalSignalTool", 0, 1);

class IXtalSignalTool : virtual public IAlgTool {
 public:

  static const InterfaceID& interfaceID() { return IID_IXtalSignalTool; }

  /** \brief calculate Adc response for one cal xtalIdx.  also select best rng.

  \param hit input energy depositions.  
  \param cidacArray resulting signal levels (in CIDAC units) for each diode in crystal
  */
  virtual StatusCode calculate(const Event::McIntegratingHit &hit,
                               CalUtil::CalVec<CalUtil::XtalDiode, float> &cidacArray) = 0;

};

#endif //_IXtalSignalTool_H
