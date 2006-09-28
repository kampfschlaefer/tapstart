#ifndef TABSTART_H
#define TABSTART_H

#include <QtGui/QWidget>
#include <QtCore/QList>
#include "tapstart_base.h"

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

	private:
		double tempo() const;
		double mean() const;
		QList<int> _times;
};

#endif // TABSTART_H

