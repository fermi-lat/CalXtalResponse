/** 
 * @file CalXtalResponse_load.cpp
 * @brief This is needed for forcing the linker to load all components
 * of the library.
 *
 *  $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/Dll/CalXtalResponse_load.cxx,v 1.3 2005/04/21 21:44:12 fewtrell Exp $
 */

#include "GaudiKernel/DeclareFactoryEntries.h"
#include "GaudiKernel/IToolFactory.h"

#define DLL_DECL_TOOL(x)       extern const IToolFactory& x##Factory; x##Factory.addRef();

#define DLL_DECL_TOOL(x)       extern const IToolFactory& x##Factory; x##Factory.addRef();

DECLARE_FACTORY_ENTRIES(CalXtalResponse) {
  
  DECLARE_SERVICE( CalCalibSvc );
  
  DECLARE_ALGORITHM( CalXtalRecAlg);

  DLL_DECL_TOOL( XtalDigiTool );
  DLL_DECL_TOOL( XtalEneTool );
  DLL_DECL_TOOL( XtalPosTool );
  
  DLL_DECL_TOOL( CalXtalRespRandom );
} 
