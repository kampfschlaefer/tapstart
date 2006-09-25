#include <QtGui/QApplication>
#include <QtGui/QPushButton>

#include "tapstart.h"

int main( int argc, char** argv )
{
	QApplication* app = new QApplication( argc, argv );

	TapStart* w = new TapStart();
	//QObject::connect( w, SIGNAL( clicked() ), w, SLOT( close() ) );

	w->show();

	return app->exec();
}
