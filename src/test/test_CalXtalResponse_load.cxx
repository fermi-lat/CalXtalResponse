// $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/src/test/test_CalXtalResponse_load.cxx,v 1.1.1.1 2002/10/30 18:13:51 richard Exp $
//====================================================================
//
//  Description: Implementation of <Package>_load routine.
//               This routine is needed for forcing the linker
//               to load all the components of the library. 
//
//====================================================================

#include "GaudiKernel/ISvcFactory.h"
#include "GaudiKernel/IAlgFactory.h"

#define DLL_DECL_SERVICE(x)    extern const ISvcFactory& x##Factory; x##Factory.addRef();
#define DLL_DECL_ALGORITHM(x)  extern const IAlgFactory& x##Factory; x##Factory.addRef();
#define DLL_DECL_OBJECT(x)     extern const IFactory& x##Factory; x##Factory.addRef();

//! Load all  services: 
void test_CalXtalResponse_load() {
  DLL_DECL_ALGORITHM( test_CalXtalResponse );
} 

extern "C" void test_CalXtalResponse_loadRef()    {
  test_CalXtalResponse_load();
}

