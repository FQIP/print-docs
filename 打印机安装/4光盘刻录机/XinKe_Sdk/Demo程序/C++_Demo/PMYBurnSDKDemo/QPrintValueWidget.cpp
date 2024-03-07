#include "stdafx.h"
#include "QPrintValueWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)  
# pragma execution_character_set("utf-8")  
#endif 

QPrintValueWidget::QPrintValueWidget(QWidget *parent /* = 0 */, int nDefValue /* = 0 */)
	: QWidget(parent)
{
	ui.setupUi(this);

//	int nHeight = ui.comboBox_Type->height();
// 	ui.comboBox_Type->setView(new QListView());
// 	ui.comboBox_Type->setStyleSheet(QString("QComboBox QAbstractItemView::item {min-height: %1px;}").arg(ui.comboBox_Type->height() - 5));
	ui.comboBox_Type->addItem("自定义内容", "$(CUSTOM_TEXT)");
	ui.comboBox_Type->addItem("任务名称", PRINT_VALUE_TASK_NAME);
	ui.comboBox_Type->addItem("光盘编号", PRINT_VALUE_DISC_INDEX);
	ui.comboBox_Type->addItem("光盘总数", PRINT_VALUE_DISC_COUNT);
	ui.comboBox_Type->addItem("刻录开始时间", PRINT_VALUE_BURN_START_TIME);
	ui.comboBox_Type->addItem("刻录结束时间", PRINT_VALUE_BURN_END_TIME);
	ui.comboBox_Type->addItem("光盘卷标", PRINT_VALUE_DISC_LABEL);
	ui.comboBox_Type->addItem("光盘SN", PRINT_VALUE_DISC_SN);
	ui.comboBox_Type->addItem("刻录大小", PRINT_VALUE_BURN_SIZE);

	connect(ui.comboBox_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(sltTypeChange(int)));

	ui.comboBox_Type->setMaximumWidth(100);
	ui.comboBox_Type->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

	if (nDefValue < ui.comboBox_Type->count())
	{
		ui.comboBox_Type->setCurrentIndex(nDefValue);
		sltTypeChange(nDefValue);
	}
}

QPrintValueWidget::~QPrintValueWidget()
{

}

void QPrintValueWidget::sltTypeChange(int nType)
{
	if (nType != 0)
	{
		ui.lineEdit_Value->hide();
		ui.comboBox_Type->setMaximumWidth(16777215);
		ui.comboBox_Type->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	}
	else
	{
		ui.lineEdit_Value->show();
		ui.comboBox_Type->setMaximumWidth(100);
		ui.comboBox_Type->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	}
}

PRINT_VALUE_INFO QPrintValueWidget::GetValueInfo()
{
	PRINT_VALUE_INFO stValueInfo;
	stValueInfo.bVariable = (ui.comboBox_Type->currentIndex() != 0);
	if (stValueInfo.bVariable)
	{
		stValueInfo.strValue = ui.comboBox_Type->currentData().toString();
	}
	else
	{
		stValueInfo.strValue = ui.lineEdit_Value->text();
	}
	return stValueInfo;
}