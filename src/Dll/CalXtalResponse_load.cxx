//  $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/Dll/CalXtalResponse_load.cxx,v 1.12 2008/01/22 20:14:48 fewtrell Exp $
/** 
 * @file
 * @author Z.Fewtrell
 * @brief This is needed for forcing the linker to load all components
 * of the library.
 *
 */

#include "GaudiKernel/DeclareFactoryEntries.h"
#include "GaudiKernel/IToolFactory.h"

#define DLL_DECL_TOOL(x)       extern const IToolFactory& x##Factory; x##Factory.addRef();

DECLARE_FACTORY_ENTRIES(CalXtalResponse) {
  
  DECLARE_SERVICE( CalCalibSvc );
  DECLARE_SERVICE( CalFailureModeSvc );
  
  DECLARE_ALGORITHM( CalXtalRecAlg );
  DECLARE_ALGORITHM( CalTupleAlg );

  DLL_DECL_TOOL( XtalDigiTool );
  DLL_DECL_TOOL( XtalRecTool );
  DLL_DECL_TOOL( CalTrigTool );
  DLL_DECL_TOOL( PrecalcCalibTool );
  DLL_DECL_TOOL( NeighborXtalkTool );
  DLL_DECL_TOOL( CalSignalTool );
  DLL_DECL_TOOL( XtalSignalTool );
  DLL_DECL_TOOL( CalDiagnosticTool );
  
  DLL_DECL_TOOL( CalXtalRespRandom );
} 
