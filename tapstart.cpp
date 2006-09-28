
#include "tapstart.h"
#include "tapstart.moc"

#include <QtCore/QDebug>
#include <QtGui/QAction>
#include <QtCore/QTime>
#include <QtCore/QTimer>

TapStart::TapStart( QWidget* p ) : QWidget( p ) {
	qDebug() << "TapStart::TapStart(" << p << ")";
	setupUi( this );

	QAction* actionquit = new QAction( "Quit", this );
	actionquit->setShortcut( tr( "Ctrl + Q" ) );
	connect( actionquit, SIGNAL( triggered() ), this, SLOT( on_btnTap_clicked() ) );
}
TapStart::~TapStart() {
	qDebug() << "TapStart::~TapStart()";
}

void TapStart::on_btnTap_clicked() {
	//qDebug() << "TabStart::on_btnTap_clicked()";
	static QTime time = QTime::currentTime();
	int time_elapsed = time.elapsed();
	if ( time_elapsed < 2000 ) {
		if ( cBBars->checkState() == Qt::Checked ) {
			_times.push_back( time_elapsed );
			while ( _times.size() >= sBBars->value()*sBMeterTop->value() )
				_times.erase( _times.begin() );

			if ( _times.size()+1 == sBBars->value()*sBMeterTop->value() ) {
				qDebug() << endl << "Should set jack's meter to" << sBMeterTop->value() << "/" << sBMeterBottom->value();
				//qDebug() << "Should set jack's tempo to" << tempo();
				emit setTempo( tempo() );
				QTimer::singleShot( int( mean() ), this, SLOT( startMusic() ) );

				lblTempo->setText( QString::number( tempo(), 'f', 1 ) );
				_times.clear();
			}

		} else {
		while ( _times.size() > sBBars->value()*sBMeterTop->value() )
			_times.erase( _times.begin() );
			_times.push_back( time_elapsed );
			lblTempo->setText( QString::number( tempo(), 'f', 1 ) );
		}
	} else {
		lblTempo->setText( "XXX" );
		_times.clear();
	}
	time.restart();
}

void TapStart::startMusic() {
	qDebug() << "TapStart::startMusic()";
	emit startTransport();
}

double TapStart::mean() const {
	double mean=0;
	for ( int i=0; i<_times.size(); ++i )
		mean += _times[ i ];
	mean /= _times.size();
	return mean;
}

double TapStart::tempo() const {
	return 60000/mean();
}

