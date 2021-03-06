/*
    Copyright 2007 Arnold Krille <arnold@arnoldarts.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

    This program and libraries are distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this package; see the file gpl-2.0.txt.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301 USA.
*/

#include <QtGui/QApplication>
#include <QtGui/QPushButton>

#include "tapstart.h"
#include "qjack.h"

int main( int argc, char** argv )
{
	QApplication* app = new QApplication( argc, argv );

	QJack* qjack = new QJack( app );

	TapStart* w = new TapStart( qjack );

	w->show();

	int ret = app->exec();

	delete w;
	delete qjack;
	return ret;
}
