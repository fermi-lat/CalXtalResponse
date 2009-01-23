# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/CalXtalResponse/SConscript,v 1.3 2008/11/06 14:45:51 ecephas Exp $
# Authors: zachary.fewtrell@nrl.navy.mil
# Version: CalXtalResponse-00-21-08
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('CalXtalResponseLib', depsOnly = 1)

CalXtalResponse = libEnv.SharedLibrary('CalXtalResponse', listFiles(['src/*.cxx',
					'src/CalCalib/*.cxx',
					'src/CalDigi/*.cxx',
					'src/CalRecon/*.cxx',
                                        'src/CalTuple/*.cxx',
					'src/CalFailureMode/*.cxx',
					'src/Xtalk/*.cxx',
					'src/CalTrig/*.cxx',
					'src/Dll/*.cxx']))

progEnv.Tool('CalXtalResponseLib')
test_CalXtalResponse = progEnv.GaudiProgram('test_CalXtalResponse', listFiles(['src/test/*.cxx']), test = 1)

progEnv.Tool('registerObjects', package = 'CalXtalResponse', libraries = [CalXtalResponse], testApps = [test_CalXtalResponse], 
	includes = listFiles(['CalXtalResponse/*.h']))





