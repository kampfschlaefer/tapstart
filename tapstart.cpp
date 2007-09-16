/*
    Copyright 2007 Arnold Krille <arnold@arnoldarts.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

    This program and libraries are distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this package; see the file gpl-2.0.txt. If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301 USA.
*/

#include "tapstart.h"
#include "tapstart.moc"

#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QtGui>

#include <qoscclient.h>

#include "qjack.h"

OscArgument::OscArgument( QWidget* p ) : QWidget( p ) {
	QVBoxLayout* layout = new QVBoxLayout( this );
	_box = new QComboBox( this );
	_box->setEditable( false );
	_box->addItems( QStringList()
		<< "Custom (int)" << "Custom (double)" << "Custom (string)"
		<< "Tempo (int)" << "Tempo (double)" << "Tempo (string)"
		<< "Delay (int)" << "Delay (double)" << "Delay (string)"
		<< "Delay/2 (int)" << "Delay/2 (double)" << "Delay/2 (string)"
		<< "Delay/3 (int)" << "Delay/3 (double)" << "Delay/3 (string)"
		<< "Delay/4 (int)" << "Delay/4 (double)" << "Delay/4 (string)"
		);
	connect( _box, SIGNAL( currentIndexChanged( int ) ), this, SLOT( box_changed() ) );
	layout->addWidget( _box );

	_line = new QLineEdit( this );
	layout->addWidget( _line );
}
QVariant OscArgument::value( double delay ) {
	QVariant v;
	int div = 1;

	if ( _box->currentText().contains( "/2" ) )
		div = 2;
	if ( _box->currentText().contains( "/3" ) )
		div = 3;
	if ( _box->currentText().contains( "/4" ) )
		div = 4;

	if ( _box->currentText().contains( "Custom" ) )
		v = _line->text();
	if ( _box->currentText().contains( "Tempo" ) )
		v = 60000/delay;
	if ( _box->currentText().contains( "Delay" ) )
		v = delay/div;

	QString type = _box->currentText().section( QRegExp( "[\\(\\)]" ), 1, 1 );
	type.replace( "string", "QString" );
	v.convert( QVariant::nameToType( type.toStdString().c_str() ) );

	return v;
}
void OscArgument::box_changed() {
	if ( _box->currentText().contains( "Custom" ) )
		_line->setEnabled( true );
	else
		_line->setEnabled( false );
}


OscPath::OscPath( QWidget* p ) : QWidget( p ), _client( 0 ) {
	setupUi( this );

	if ( ! _client ) {
		_client = new QOscClient( QHostAddress::LocalHost, port->value(), this );
		connect( hostName, SIGNAL( editingFinished() ), this, SLOT( updateOsc() ) );
		connect( port, SIGNAL( valueChanged( int ) ), this, SLOT( updateOsc() ) );
	}
}

void OscPath::setDelay( double d ) {
	if ( groupBox->isChecked() ) {

		QVariantList args;
		foreach( OscArgument* arg, _arguments )
			args.push_back( arg->value( d ) );

		_client->sendData( path->text(), args );
	}
}

void OscPath::updateOsc() {
	qDebug() << "OscPath::updateOsc()";
	QHostAddress tmp( hostName->text() );
	qDebug() << tmp.toString();
	_client->setAddress( tmp, port->value() );
}

void OscPath::on_btnMore_clicked() {
	_arguments.push_back( new OscArgument( this ) );
	hboxLayout1->addWidget( _arguments.back() );
}
void OscPath::on_btnLess_clicked() {
	delete _arguments.back();
	_arguments.pop_back();
}

TapStart::TapStart( QJack* j, QWidget* p ) : QMainWindow( p ), _jack( j ) , _mw( new QWidget( this ) ) {
	qDebug() << "TapStart::TapStart(" << p << ")";
	setCentralWidget( _mw );
	setupUi( _mw );
	connect( btnTap, SIGNAL( clicked() ), this, SLOT( on_btnTap_clicked() ) );
	connect( btnMoreOsc, SIGNAL( clicked() ), this, SLOT( on_btnMoreOsc_clicked() ) );

	QMenu* filemenu = menuBar()->addMenu( "File" );
	filemenu->addAction( "Quit", this, SLOT( close() ), Qt::CTRL + Qt::Key_Q );

	QMenu* helpmenu = menuBar()->addMenu( "Help" );
	helpmenu->addAction( "About TapStart...", this, SLOT( aboutTapStart() ) );
	helpmenu->addAction( "About Qt...", qApp, SLOT( aboutQt() ) );

	if ( _jack == 0 || _jack->jackEnabled() == false )
		jackBox->setEnabled( false );

	connect( this, SIGNAL( setTempo( double ) ), _jack, SLOT( transport_setTempo( double ) ) );
	connect( this, SIGNAL( startTransport() ), _jack, SLOT( transport_start() ) );

}
TapStart::~TapStart() {
	qDebug() << "TapStart::~TapStart()";
}

void TapStart::on_btnTap_clicked() {
	//qDebug() << "TapStart::on_btnTap_clicked()";
	static QTime time = QTime::currentTime();

	int time_elapsed = time.elapsed();

	if ( time_elapsed < 2000 && time_elapsed > 10 ) {

		_times.push_back( time_elapsed );

		qDebug() << "TapStart::on_btnTap_clicked()" << time_elapsed;

		emit setDelay( mean() );
		emit setTempo( tempo() );

		if	( jackBox->isChecked() && cBBars->isChecked() && _times.size() == sBBars->value()*sBMeterTop->value() -1 )
			QTimer::singleShot( mean(), this, SLOT( startMusic() ) );

		lblTempo->setText( QString::number( tempo(), 'f', 1 ) );

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
	static int last_size=0;
	static double mean=0;
	if ( last_size != _times.size() ) {
		last_size = _times.size();
		mean = 0;
		for ( int i=0; i<last_size; ++i )
			mean += _times[ i ];
		mean /= last_size;
	}
	return mean;
}

double TapStart::tempo() const {
	return 60000/mean();
}

void TapStart::on_btnMoreOsc_clicked() {
	_oscpaths.push_back( new OscPath( this ) );
	vboxLayout->addWidget( _oscpaths.back() );
	connect( _oscpaths.back(), SIGNAL( destroyed( QObject* ) ), this, SLOT( removeOscPath( QObject* ) ) );
	connect( this, SIGNAL( setDelay( double ) ), _oscpaths.back(), SLOT( setDelay( double ) ) );
}

void TapStart::removeOscPath( QObject* ) {
	foreach( QPointer<OscPath> p, _oscpaths )
		if ( p == 0 )
			_oscpaths.removeAll( p );
}

void TapStart::aboutTapStart() {
	QMessageBox::about( this, "About TapStart", "\
<h1>TapStart (v0.1)</h1>\
<p>See <a href=\"http://www.arnoldarts.de/drupal/?q=TapStart\">www.arnoldarts.de/drupal/?q=TapStart</a> \
for more info and documentation.</p>\
<h4>Copyright 2007 Arnold Krille <arnold@arnoldarts.de></h4>\
<p>License: GPLv2 <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html> (a \
copy of the license is also included in the source-package.)</p>\
<h4>First steps:</h4>\
<p>Tap your tempo on the big button. The label below will show the (averaged) \
tempo you tap.</p>\
<p>If jack is running when the app starts, the jack tempo will be updated as you\
tap. If you check the corresponding option the jack transport will also be started.</p>\
<p>TapStart can also send OSC-messages, just click \"More OSC\" to add OSC-messages \
to be send on each tempo update. That way you can control sequencers and delay-effects.</p>\
" );
}

