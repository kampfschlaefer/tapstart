
#include "qjack.h"
#include "qjack.moc"

#include <QtCore/QDebug>
#include <math.h>

QJack::QJack( QObject* p ) : QObject( p ), _jackclient( 0 ), _tempo( 120 ), _meterTop( 4 ), _meterBottom( 4 ) {
	qDebug() << "QJack::QJack(" << p << ")";
#ifdef HAVE_JACK
	_jackclient = jack_client_new( "tapStart" );
	if ( _jackclient == 0 )
		qWarning( "Couldn't connect to jackd." );

	if ( _jackclient ) {
		jack_activate( _jackclient );
		int ret = jack_set_timebase_callback( _jackclient, 1, &timebaseCallback, this );
		qDebug() << " jack_set_timebase_callback returned with" << ret;
	}
#endif
}
QJack::~QJack() {
#ifdef HAVE_JACK
	if ( _jackclient ) {
		transport_stop();
		jack_deactivate( _jackclient );
		jack_client_close( _jackclient );
	}
#endif
}

void QJack::transport_start() {
#ifdef HAVE_JACK
	if ( _jackclient ) {
		qDebug() << "QJack::transport_start()";
		jack_transport_start( _jackclient );
	}
#endif
}

void QJack::transport_stop() {
#ifdef HAVE_JACK
	if ( _jackclient )
		jack_transport_stop( _jackclient );
#endif
}

void QJack::transport_setTempo( double tempo ) {
	qDebug() << "QJack::transport_setTempo(" << tempo << ")";
	_tempo = tempo;
}

void QJack::transport_setMeter( int top, int bottom ) {
	qDebug() << "QJack::transport_setMeter(" << top << "," << bottom << ")";
	_meterTop = top;
	_meterBottom = bottom;
}

void QJack::timebaseCallback( jack_transport_state_t, jack_nframes_t nframes, jack_position_t* pos, int new_pos, void* arg ) {
	QJack* qjack = static_cast<QJack*>( arg );

#if 0
	pos->frame_time = pos->frame / double( pos->frame_rate );
	pos->next_time = ( pos->frame + nframes ) / double( pos->frame_rate );
	//qDebug() << "pos->frame =" << pos->frame << " pos->frame_rate =" << pos->frame_rate << " pos->frame_time =" << pos->frame_time;
	//qDebug() << "pos->next_time =" << pos->next_time;
	//qDebug() << "pos->usecs =" << pos->usecs << "  pos->valid =" << pos->valid;
	//qDebug() << "pos->unique_1 =" << pos->unique_1 << "  pos->unique_2 =" << pos->unique_2;

	pos->beat_type = qjack->_meterBottom;
	pos->beats_per_bar = qjack->_meterTop;
	pos->beats_per_minute = qjack->_tempo;

	double current_global_beat = qjack->_tempo / 60 * pos->frame_time;

	pos->bar = int( current_global_beat / pos->beats_per_bar ) + 1;
	pos->beat = fmod( current_global_beat, pos->beats_per_bar ) + 1;
	pos->bar_start_tick = 0;
	pos->tick = 0;
	pos->ticks_per_beat = 120;

	pos->valid = JackPositionBBT;
	qDebug() << QString( "Position %1:%2:%3" ).arg( pos->bar ).arg( pos->beat ).arg( pos->tick );
#endif

	float time_beats_per_bar = qjack->_meterTop;
	float time_beat_type = qjack->_meterBottom;
	double time_ticks_per_beat = 1920.0;
	double time_beats_per_minute = qjack->_tempo;
	volatile int time_reset = 1;

	double min;				/* minutes since frame 0 */
	double abs_tick;		/* ticks since frame 0 */
	double abs_beat;		/* beats since frame 0 */

	if ( new_pos || time_reset ) {

		pos->valid = JackPositionBBT;
		pos->beats_per_bar = time_beats_per_bar;
		pos->beat_type = time_beat_type;
		pos->ticks_per_beat = time_ticks_per_beat;
		pos->beats_per_minute = time_beats_per_minute;

		time_reset = 0;		/* time change complete */

		/* Compute BBT info from frame number.  This is relatively
		 * simple here, but would become complex if we supported tempo
		 * or time signature changes at specific locations in the
		 * transport timeline. */

		min = pos->frame / ( ( double ) pos->frame_rate * 60.0 );
		abs_tick = min * pos->beats_per_minute * pos->ticks_per_beat;
		abs_beat = abs_tick / pos->ticks_per_beat;

		pos->bar = int( abs_beat / pos->beats_per_bar );
		pos->beat = int( abs_beat - ( pos->bar * pos->beats_per_bar ) + 1 );
		pos->tick = int( abs_tick - ( abs_beat * pos->ticks_per_beat ) );
		pos->bar_start_tick = pos->bar * pos->beats_per_bar *
			pos->ticks_per_beat;
		pos->bar++;		/* adjust start to bar 1 */

	} else {
		/* Compute BBT info based on previous period. */
		pos->tick += int(
			nframes * pos->ticks_per_beat * pos->beats_per_minute
			/ ( pos->frame_rate * 60 )
			);

		while ( pos->tick >= pos->ticks_per_beat ) {
			pos->tick -= int( pos->ticks_per_beat );
			if ( ++pos->beat > pos->beats_per_bar ) {
				pos->beat = 1;
				++pos->bar;
				pos->bar_start_tick +=
					pos->beats_per_bar
					* pos->ticks_per_beat;
			}
		}
	}

}

