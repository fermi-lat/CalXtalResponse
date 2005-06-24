#ifndef _IXtalDigiTool_H
#define _IXtalDigiTool_H 1
/*! @class IXtalDigiTool
 *
 * \author Zach Fewtrell
 *
 * \brief Abstract interface class for estimation of the digital response of one calorimeter crystal to a set of energy depositions.
 * 
 *
 */

// GLAST INCLUDES
#include "idents/CalXtalId.h"
#include "Event/MonteCarlo/McIntegratingHit.h"

// EXTLIB INCLUDES
#include "GaudiKernel/IAlgTool.h"

// STD INCLUDES
#include <vector>

static const InterfaceID IID_XtalDigiTool("IXtalDigiTool", 1 , 0);

using namespace std;
using namespace idents;

class IXtalDigiTool : virtual public IAlgTool {
 public:

  static const InterfaceID& interfaceID() { return IID_XtalDigiTool; }

  /// calculate Adc response for one cal xtalId.  also select best rng.
  /// \param CalXtalId specify xtal log
  /// \param hitList input vector of energy depositions.  (const *) is used to save space.
  /// \param rngP output best rng for Positive xtal face
  /// \param rngN output best rng for Negative xtal face
  /// \param adcP output vector of 4 adc responses for each rng for Positive face - rngs always in default order (from 0-3)
  /// \param adcN output vector of 4 adc responses for each rng for Negative face - rngs always in default order (from 0-3)
  /// \param lacP output boolean for log accept on Positive xtal face
  /// \param lacN output boolean for log accept on Negative xtal face
  virtual StatusCode calculate(const CalXtalId &xtalId, 
                               const vector<const Event::McIntegratingHit*> &hitList,
                               bool &lacP,                          // output - log accept.
                               bool &lacN,                          // output - log accept. 
                               CalXtalId::AdcRange &rngP,   // output - best rng
                               CalXtalId::AdcRange &rngN,   // output - best rng
                               vector<int> &adcP,                   // output - ADC's for all rngs 0-3
                               vector<int> &adcN                    // output - ADC's for all rngs 0-3
                               ) = 0;

};

#endif //_IXtalDigiTool_H