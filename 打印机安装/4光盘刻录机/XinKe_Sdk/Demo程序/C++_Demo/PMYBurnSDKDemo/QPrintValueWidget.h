#ifndef QPRINTVALUEWIDGET_H
#define QPRINTVALUEWIDGET_H

#include <QWidget>
#include "ui_QPrintValueWidget.h"

typedef struct _PRINT_VALUE_INFO
{
	bool bVariable;	
	QString strValue;
}PRINT_VALUE_INFO, *PPRINT_VALUE_INFO;

class QPrintValueWidget : public QWidget
{
	Q_OBJECT

public:
	QPrintValueWidget(QWidget *parent = 0, int nDefValue = 0);
	~QPrintValueWidget();
	PRINT_VALUE_INFO GetValueInfo();

private:
	Ui::QPrintValueWidget ui;

public slots:
	void sltTypeChange(int nType);

};

#endif // QPRINTVALUEWIDGET_H
