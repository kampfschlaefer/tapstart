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

#ifndef TABSTART_H
#define TABSTART_H

#include <QtGui/QWidget>
#include <QtGui/QMainWindow>
#include <QtCore/QList>
#include <QtCore/QPointer>
#include "tapstart_base.h"
#include "tapstart_oscpath_base.h"

class QJack;

class QOscClient;

class QLineEdit;
class QComboBox;

class OscArgument : public QWidget
{
	Q_OBJECT
	public:
		OscArgument( QWidget* );

		QVariant value( double delay );

	private slots:
		void box_changed();

	private:
		QComboBox* _box;
		QLineEdit* _line;
};

class OscPath : public QWidget, private Ui_OscPath_Base
{
	Q_OBJECT
	public:
		OscPath( QWidget* );

	public slots:
		void setDelay( double );

	private slots:
		void updateOsc();

		void on_btnMore_clicked();
		void on_btnLess_clicked();

	private:
		QOscClient* _client;
		QList< OscArgument* > _arguments;
};

class TapStart : public QMainWindow, private Ui_TapStart_Base
{
	Q_OBJECT
	public:
		TapStart( QJack*, QWidget* =0 );
		~TapStart();

	signals:
		void startTransport();
		void setTempo( double );
		void setDelay( double );

	private slots:
		void on_btnTap_clicked();
		void startMusic();

		void on_btnMoreOsc_clicked();

		void removeOscPath( QObject* );

		void aboutTapStart();

	private:
		double tempo() const;
		double mean() const;
		QList<int> _times;

		QList< QPointer<OscPath> > _oscpaths;

		QJack* _jack;

		QWidget* _mw;
};

#endif // TABSTART_H

