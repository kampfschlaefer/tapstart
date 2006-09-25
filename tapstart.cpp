
#include "tapstart.h"
#include "tapstart.moc"

#include <QtCore/QDebug>
#include <QtGui/QAction>
#include <QtCore/QTime>

TapStart::TapStart( QWidget* p ) : QWidget( p ) {
	qDebug() << "TabStart::TabStart(" << p << ")";
	setupUi( this );

	QAction* actionquit = new QAction( "Quit", this );
	actionquit->setShortcut( tr( "Ctrl + Q" ) );
	connect( actionquit, SIGNAL( triggered() ), this, SLOT( on_btnTap_clicked() ) );
}
TapStart::~TapStart() {
	qDebug() << "TabStart::~TabStart()";
}

void TapStart::on_btnTap_clicked() {
	//qDebug() << "TabStart::on_btnTap_clicked()";
	static QTime time = QTime::currentTime();
	int time_elapsed = time.elapsed();
	if ( time_elapsed < 5000 ) {
		while ( _times.size() > 10 )
			_times.erase( _times.begin() );
		_times.push_back( time_elapsed );
		double mean=0;
		for ( int i=0; i<_times.size(); ++i )
			mean += _times[ i ];
		mean /= _times.size();
		double tempo = 60000/mean;
		lblTempo->setText( QString::number( tempo, 'f', 1 ) );
	} else {
		lblTempo->setText( "XXX" );
		_times.clear();
	}
	time.restart();
}

