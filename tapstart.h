#ifndef TABSTART_H
#define TABSTART_H

#include <QtGui/QWidget>
#include <QtCore/QList>
#include <QtCore/QPointer>
#include "tapstart_base.h"
#include "tapstart_oscpath_base.h"

class OscPath : public QWidget, private Ui_OscPath_Base
{
	Q_OBJECT
	public:
		OscPath( QWidget* );
};

class TapStart : public QWidget, private Ui_TapStart_Base
{
	Q_OBJECT
	public:
		TapStart( QWidget* =0 );
		~TapStart();

	signals:
		void startTransport();
		void setTempo( double );

	private slots:
		void on_btnTap_clicked();
		void startMusic();

		void on_btnMoreOsc_clicked();

		void removeOscPath( QObject* );

	private:
		double tempo() const;
		double mean() const;
		QList<int> _times;

		QList< QPointer<OscPath> > _oscpaths;
};

#endif // TABSTART_H

