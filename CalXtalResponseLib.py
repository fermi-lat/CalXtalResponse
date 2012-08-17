#$Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/CalXtalResponse/CalXtalResponseLib.py,v 1.3 2009/11/10 00:53:13 jrb Exp $
def generate(env, **kw):
    if not kw.get('depsOnly', 0):
        env.Tool('addLibrary', library = ['CalXtalResponse'])
        if env['PLATFORM']=='win32' and env.get('CONTAINERNAME','')=='GlastRelease':
	    env.Tool('findPkgPath', package = 'CalXtalResponse') 
    env.Tool('CalUtilLib')
    env.Tool('CalibDataLib')
    env.Tool('GlastSvcLib')
    env.Tool('addLibrary', library=env['rootGuiLibs'])
    env.Tool('addLibrary', library=env['gaudiLibs'])
    env.Tool('ntupleWriterSvcLib')
    env.Tool('LdfEventLib')
    env.Tool('addLibrary', library = env['clhepLibs'])
    if env['PLATFORM']=='win32' and env.get('CONTAINERNAME','')=='GlastRelease':
        env.Tool('findPkgPath', package = 'CalibSvc') 
        env.Tool('findPkgPath', package = 'LdfEvent') 
def exists(env):
    return 1;
