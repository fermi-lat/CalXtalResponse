# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/SConscript,v 1.20 2011/02/16 16:57:44 usher Exp $
# Authors: zachary.fewtrell@nrl.navy.mil
# Version: CalXtalResponse-00-23-01-gr01
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('addLinkDeps', package='CalXtalResponse', toBuild='component')

CalXtalResponse = libEnv.SharedLibrary('CalXtalResponse',
                                       listFiles(['src/*.cxx',
					'src/CalCalib/*.cxx',
					'src/CalDigi/*.cxx',
					'src/CalRecon/*.cxx',
                                        'src/CalTuple/*.cxx',
					'src/CalFailureMode/*.cxx',
					'src/Xtalk/*.cxx',
					'src/CalTrig/*.cxx',
					'src/Dll/*.cxx']))

progEnv.Tool('CalXtalResponseLib')

progEnv.Tool('CalibSvcLib')
progEnv.Tool('CalDigiLib')
#progEnv.Tool('addLibrary', library = progEnv['obfLibs'])

test_CalXtalResponse =progEnv.GaudiProgram('test_CalXtalResponse',
                                           listFiles(['src/test/*.cxx']),
                                           test = 1, package='CalXtalResponse')

progEnv.Tool('registerTargets', package = 'CalXtalResponse',
             libraryCxts = [[CalXtalResponse, libEnv]],
             testAppCxts = [[test_CalXtalResponse, progEnv]], 
             includes = listFiles(['CalXtalResponse/*.h']),
             data = ['data/Xtalk/CU06_Neighbor_xtalk.txt'],
             jo = listFiles(['src/*.txt', 'src/test/*.txt']))

