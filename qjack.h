#ifndef QJACK_H
#define QJACK_H

#include <QtCore/QObject>

#include <jack/jack.h>

class QJack : public QObject
{
	Q_OBJECT
	public:
		QJack( QObject* =0 );
		~QJack();

	public slots:
		void transport_start();
		void transport_stop();
		void transport_setTempo( double );
		void transport_setMeter( int top, int bottom );

	private:
		static void timebaseCallback( jack_transport_state_t, jack_nframes_t, jack_position_t*, int, void* );
		jack_client_t* _jackclient;

		double _tempo;
		int _meterTop, _meterBottom;
};

#endif // QJACK_H

