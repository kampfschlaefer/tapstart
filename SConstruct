#!/usr/bin/python

import os

buildenv={}
if os.environ.has_key('PATH'):
	buildenv['PATH']=os.environ['PATH']
else:
	buildenv['PATH']=''
if os.environ.has_key('PKG_CONFIG_PATH'):
	buildenv['PKG_CONFIG_PATH']=os.environ['PKG_CONFIG_PATH']
else:
	buildenv['PKG_CONFIG_PATH']=''

env = Environment( tools=['default','pkgconfig'], toolpath=["admin"], ENV=buildenv )

uic = Builder(action= "uic $SOURCES > $TARGET", suffix = '.h', srcsuffix = '.uic' )
moc = Builder(action= "moc $SOURCES > $TARGET", suffix = '.h', srcsuffix = '.moc' )

env['BUILDERS']['UIBuilder'] = uic
env['BUILDERS']['MOCBuilder'] = moc


conf = Configure( env, custom_tests=env['PKGCONFIG_TESTS'] )

if not conf.CheckForPKGConfig():
	Exit(1)

needed = conf.GetPKGFlags( 'QtCore' )
if needed == 0:
	Exit(1)
env.MergeFlags( needed )
needed = conf.GetPKGFlags( 'QtGui' )
if needed == 0:
	Exit(1)
env.MergeFlags( needed )
needed = conf.GetPKGFlags( 'ofqf', "0.1.1" )
if needed == 0:
	Exit(1)
env.MergeFlags( needed )

tmp = conf.GetPKGFlags( 'jack' )
if not tmp == 0:
	env.MergeFlags( tmp )
	env.MergeFlags( "-DHAVE_JACK" )

env = conf.Finish()

#debug
env.MergeFlags( "-Wall -Werror -g" )

env.UIBuilder( target="tapstart_base.h", source="tapstart_base.ui")
env.UIBuilder( target="tapstart_oscpath_base.h", source="tapstart_oscpath_base.ui")
env.MOCBuilder( target="tapstart.moc", source="tapstart.h")
env.MOCBuilder( target="qjack.moc", source="qjack.h")
env.Program( target='tapStart', source = ['qjack.cpp','tapstart.cpp','main.cpp'] )

