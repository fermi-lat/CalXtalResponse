/** 
 * @file CalXtalResponse_load.cpp
 * @brief This is needed for forcing the linker to load all components
 * of the library.
 *
 *  $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/Dll/CalXtalResponse_load.cxx,v 1.4 2004/08/25 21:14:56 fewtrell Exp $
 */

#include "GaudiKernel/DeclareFactoryEntries.h"
#include "GaudiKernel/IToolFactory.h"

#define DLL_DECL_TOOL(x)       extern const IToolFactory& x##Factory; x##Factory.addRef();

#define DLL_DECL_TOOL(x)       extern const IToolFactory& x##Factory; x##Factory.addRef();

DECLARE_FACTORY_ENTRIES(CalXtalResponse) {
  
  DECLARE_SERVICE( CalCalibSvc );

  DLL_DECL_TOOL( XtalADCTool );
  DLL_DECL_TOOL( XtalEneTool );
  DLL_DECL_TOOL( XtalPosTool );
} 
