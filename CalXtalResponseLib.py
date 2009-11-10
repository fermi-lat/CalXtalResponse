#$Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/CalXtalResponse/CalXtalResponseLib.py,v 1.2 2008/11/12 19:41:49 ecephas Exp $
def generate(env, **kw):
    if not kw.get('depsOnly', 0):
        env.Tool('addLibrary', library = ['CalXtalResponse'])
    env.Tool('CalUtilLib')
    env.Tool('CalibDataLib')
    env.Tool('GlastSvcLib')
    env.Tool('addLibrary', library=env['rootGuiLibs'])
    env.Tool('addLibrary', library=env['gaudiLibs'])
    env.Tool('ntupleWriterSvcLib')
    env.Tool('LdfEventLib')
    env.Tool('addLibrary', library = env['clhepLibs'])
def exists(env):
    return 1;
