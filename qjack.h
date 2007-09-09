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

		bool jackEnabled() { return _jackclient; }

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

