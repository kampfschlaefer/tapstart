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
