#ifndef CalXtalRecAlg_h
#define CalXtalRecAlg_h 
//    $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/XtalRecon/CalXtalRecAlg.h,v 1.4 2006/01/09 21:08:22 fewtrell Exp $


// LOCAL INCLUDES


// GLAST INCLUDES
#include "Event/Digi/CalDigi.h"
#include "Event/Recon/CalRecon/CalXtalRecData.h"


// EXTLIB INCLUDES
#include "GaudiKernel/Algorithm.h"


// STD INCLUDES


class INeighborXtalkTool;
class IXtalRecTool;

namespace Event {
	class EventHeader;
};

/** @class CalXtalRecAlg
    @brief  Calorimeter crystal reconstruction algorithm

    This algorithm reconstructs energy and position in each calorimeter crystal.
    See CalXtalResponse/XtalRecTool

    @author           A.Chekhtman
    @author           Zach Fetwrell

*/
class CalXtalRecAlg : public Algorithm
{
 public:
  CalXtalRecAlg(const std::string& name, ISvcLocator* pSvcLocator);
  /// initialize internal data members.
  StatusCode initialize();
  ///  Reconstruct ene & pos for all xtal hits in event
  StatusCode execute();
  /// required by Gaudi Algorithm class
  StatusCode finalize();

 private:
  ///  function for setting pointers to the input and output data in Gaudi TDS
  StatusCode retrieve();
    
  /// pointer to input data collection in TDS
  Event::CalDigiCol* m_calDigiCol;

  /// pointer to the output data collection in TDS
  Event::CalXtalRecCol* m_calXtalRecCol;
    
  /// pointer to event Header (evtId, runId, etc...)
  Event::EventHeader* m_evtHdr;

  /// pointer to CalResponse tool for converting xtal digi info -> energy 
  IXtalRecTool *m_xtalRecTool;

  /// name of IXtalRecTool instantiation
  StringProperty m_recToolName;

  /// pointer to NeighborXtalk tool for calculating electronic crosstalk from neighboring crystals
  INeighborXtalkTool *m_xtalkTool;

  /// name of INeigbhorXtalkTool instantiation
  StringProperty m_xtalkToolName;
};

#endif
