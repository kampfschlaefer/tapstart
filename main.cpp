#include <QtGui/QApplication>
#include <QtGui/QPushButton>

#include "tapstart.h"
#include "qjack.h"

int main( int argc, char** argv )
{
	QApplication* app = new QApplication( argc, argv );

	//QJack* qjack = new QJack( app );

	TapStart* w = new TapStart();

	//QObject::connect( w, SIGNAL( setTempo( double ) ), qjack, SLOT( transport_setTempo( double ) ) );
	//QObject::connect( w, SIGNAL( startTransport() ), qjack, SLOT( transport_start() ) );

	//QObject::connect( w, SIGNAL( clicked() ), w, SLOT( close() ) );

	w->show();

	int ret = app->exec();

	delete w;
	//delete qjack;
	return ret;
}
