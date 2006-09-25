env = Environment()#tools=['default','qt'])

uic = Builder(action= "uic $SOURCES > $TARGET", suffix = '.h', srcsuffix = '.uic' )
moc = Builder(action= "moc $SOURCES > $TARGET", suffix = '.h', srcsuffix = '.moc' )

env['BUILDERS']['UIBuilder'] = uic
env['BUILDERS']['MOCBuilder'] = moc

def CheckPKGConfig( context, pkgname ):
	import SCons.Util, os

	context.Message( "Checking for " + pkgname + "..." )
	ret = context.TryAction( 'pkg-config --exists ' + pkgname )
	if ret[0] == 1:
		tmp = os.popen( 'pkg-config --libs-only-l ' + pkgname ).read().strip()
		env.Append( LIBS = SCons.Util.CLVar( tmp ) )
		tmp = os.popen( 'pkg-config --libs-only-L ' + pkgname ).read().strip()
		env.Append( LIBPATH = SCons.Util.CLVar( tmp ) )
		tmp = ' ' + os.popen( 'pkg-config --cflags ' + pkgname ).read().strip()
		env.Append( CCFLAGS = SCons.Util.CLVar( tmp ) )
	context.Result( ret[0] )
	return ret[0]

conf = Configure( env, custom_tests={'CheckPKGConfig' : CheckPKGConfig } )
#conf.CheckPKGConfig( 'jack' )
conf.CheckPKGConfig( 'QtCore' )
conf.CheckPKGConfig( 'QtGui' )

env = conf.Finish()

env.UIBuilder( target="tapstart_base.h", source="tapstart_base.ui")
env.MOCBuilder( target="tapstart.moc", source="tapstart.h")
env.Program( target='tapStart', source = ['tapstart.cpp','main.cpp'] )

