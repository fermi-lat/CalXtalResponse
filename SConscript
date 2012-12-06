# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/CalXtalResponse/SConscript,v 1.23 2012/09/21 19:09:48 lsrea Exp $
# Authors: zachary.fewtrell@nrl.navy.mil
# Version: CalXtalResponse-01-01-00

Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('addLinkDeps', package='CalXtalResponse', toBuild='component')

CalXtalResponse = libEnv.ComponentLibrary('CalXtalResponse',
                                          listFiles(['src/*.cxx',
                                                     'src/CalCalib/*.cxx',
                                                     'src/CalDigi/*.cxx',
                                                     'src/CalRecon/*.cxx',
                                                     'src/CalTuple/*.cxx',
                                                     'src/CalFailureMode/*.cxx',
                                                     'src/Xtalk/*.cxx',
                                                     'src/CalTrig/*.cxx']))


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

