#Python module globals
import string, glob

#Scons module imports
from SCons.Script.SConscript import SConsEnvironment
import pdb

PLATFORMS = {'darwin':'osx','win32':'windows','posix':'linux'}
ARCH_LIST = ['x86','x86_amd64']
ARCH_64 = ['x86_amd64'] # this is using the cross compiler so it is still as 32 bit compiler but with x64 bit extensions turned on
#ARCH_64_LIST = ['x86_amd64','x86_64']
#ARCH_LIST = ['x86','x86_amd64','x64']
#ARCH_64_LIST = ['x86_amd64', 'x64']

DEFAULT_PYTHON = '26'

#msvs_version= str(ARGUMENTS.get("MSVS_VERSION", DEFAULT_MSVS_VERSION))

try: DEFAULT_STATIC_LINK
except NameError:
    DEFAULT_STATIC_LINK = {'windows':'1', 'osx':'1', 'linux':'0' }

try: DEFAULT_ARCH
except NameError:
    DEFAULT_ARCH = {'windows':'x86','osx':'x86','linux':'x86_amd64'}


DEFAULT_OPT_LEVEL = 2
DEFAULT_DEBUG = 0
DEFAULT_DOC = 0
DEFAULT_THREADS = 1
DEFAULT_MSVS_VERSION = '8.0'
DEFAULT_ALTLIBPATH= "/usr/local/lib"
DEFAULT_COMPILER = 'scons'

OS = ARGUMENTS.get("OS", Platform())
platform = PLATFORMS.get(str(OS))

#for different defaults on different platforms
dictType= type({})
if type(DEFAULT_ARCH) == dictType:
    DEFAULT_ARCH= DEFAULT_ARCH.get(platform)
if type(DEFAULT_STATIC_LINK) == dictType:
    DEFAULT_STATIC_LINK= DEFAULT_STATIC_LINK.get(platform)


def CustomEnv(env):
    global ARCH_64_LIST    
    
    TARGET_TYPE = "PROGRAM" # val
    def SGlob(env, pattern):
        path = string.replace(env.GetBuildPath('SConscript'),  'SConscript', '')
        result = []
        for i in glob.glob(path + pattern):
            result.append(string.replace(i, path, ''))
        return result
    
    def GetBuildDir(env, cached=True):
        #platform_dir = env['MY_PLATFORM']
        platform_dir = "RHEL5"
        arch_dir = env['ARCH']    
        thread_dir = int(env['THREADS']) and 'multi' or 'single'
        debug_dir = int(env['DEBUG']) and 'debug' or 'release'
        build_dir = "build/%(platform_dir)s/%(arch_dir)s/%(thread_dir)s/%(debug_dir)s" % vars()        
        return build_dir
         
    def IsWindows(env):
        return env['MY_PLATFORM'] == 'windows'
        
    def IsOSX(env):
        return env['MY_PLATFORM'] == 'osx'
    
    def IsLinux(env):
        return env['MY_PLATFORM'] == 'linux'
    
    def Is64(env):
        return env['ARCH'] in ARCH_64_LIST
    
    def IsWin64(env):
        return env.Is64() and env.IsWindows()
    
    SConsEnvironment.SGlob = SGlob
    SConsEnvironment.GetBuildDir = GetBuildDir
    SConsEnvironment.IsWin64 = IsWin64
    SConsEnvironment.Is64 = Is64     

env = Environment(tools=["default"], CPPPATH=['#include'], MSVS_IGNORE_IDE_PATHS=1, ARCH=DEFAULT_ARCH,)


opts = Variables(ARGUMENTS)
opts.AddVariables(
    EnumVariable('DEBUG', "Debug mode", DEFAULT_DEBUG, allowed_values=('0','1')),
    PathVariable('ALTLIBPATH', "Paths to use when searching for libs", DEFAULT_ALTLIBPATH,),
)


env['OPT_LEVEL']                    = int(ARGUMENTS.get("OPT", DEFAULT_OPT_LEVEL))
env['THREADS']                      = int(ARGUMENTS.get("THREADS", DEFAULT_THREADS))
env['STATIC_LINK']                  = int(ARGUMENTS.get("STATIC_LINK", DEFAULT_STATIC_LINK))
env['OS']                           = str(ARGUMENTS.get("OS", OS))
env['DEBUG']                        = int(ARGUMENTS.get("DEBUG", DEFAULT_DEBUG))
env['ALTLIBPATH']                   = str(ARGUMENTS.get("ALTLIBPATH", DEFAULT_ALTLIBPATH))

env.Tool(CustomEnv)

Help(opts.GenerateHelpText(env))

"""
platform = str(env['MY_PLATFORM'])
threads = int(env['THREADS'])
debug = int(env['DEBUG'])
optlevel = int(env['OPT_LEVEL'])
static_link = int(env['STATIC_LINK'])
arch = env['ARCH']
"""
LIBS = ["fltk", "fltk_gl", "glut", "GL", "GLU", "m", "X11"]
def testing_libs(env, libpath=None, libs=None, unique=None):
    conf = env.Configure( help=False)
    #conf = env.Configure( conf_dir="#conf_cache_%s" %(unique), log_file="#conf_%s.log" %(unique),clean=False, help=False)
    missing_flag = False
    if not libpath == None:
        conf.env.AppendUnique(LIBPATH=libpath)
    
    conf.env.AppendUnique(LIBS=libs)
    for lib in libs:
        if 0 == conf.CheckLib(library=lib):
            missing_flag = True

    env = conf.Finish()
    
    return missing_flag, env


#checking for fltk and opengl libs 
#---------------------------
missing_flag, env = testing_libs(env, libs=LIBS, unique='system')

#---------------------------


if missing_flag:
    imdlibpath = "/imd/tool/app/fltk/fltk-1.1.10/linux64/lib"
    print "\nChecking with IMD lib paths(%s)" %(imdlibpath)
    missing_flag, env = testing_libs(env, libs=LIBS, libpath=imdlibpath, unique="imd")

if missing_flag:
    if env['ALTLIBPATH'] == '/usr/local/lib':
        print "\nChecking user default alturnate lib path(%s)" %(env['ALTLIBPATH'])
    else:
        print "\nChecking user specified lib path(%s)" %(env['ALTLIBPATH'])
    missing_flag, env = testing_libs(env, libs=LIBS, libpath = [env['ALTLIBPATH']], unique="cmdline") 

if missing_flag:
    print "Unable to find one or more libs.  Try adding path to libs as command line argument LIBPATH=PATH_TO_LIB"
    exit(1)

env.AppendUnique(CPPPATH = ["/imd/tool/app/fltk/fltk-1.1.10/linux64/include"]) 


env.SConscript(dirs=['src'], duplicate=0, build_dir = env.GetBuildDir(), exports = {'env':env})


