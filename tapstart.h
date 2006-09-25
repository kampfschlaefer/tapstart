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

	private slots:
		void on_btnTap_clicked();

	private:
		QList<int> _times;
};

#endif // TABSTART_H

