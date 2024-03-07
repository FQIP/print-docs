#include "stdafx.h"
#include "QRobotDeviceWidget.h"
#include "QIconHelper.hpp"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif


void DeviceData::UpdateData(QJsonObject& obj)
{
	id = obj.value("id").toString();
	name = obj.value("name").toString();
	type = obj.value("type").toInt();
	bins_count = obj.value("bins_count").toInt();
	bin1_disc_count = obj.value("bin1_disc_count").toInt();
	bin2_disc_count = obj.value("bin2_disc_count").toInt();
	black_ink = obj.value("black_ink").toInt();
	red_ink = obj.value("red_ink").toInt();
	yellow_ink = obj.value("yellow_ink").toInt();
	blue_ink = obj.value("blue_ink").toInt();
	system_state = obj.value("system_state").toInt();
	system_state_string = obj.value("system_state_string").toString();
	system_error = obj.value("system_error").toInt();
	system_error_string = obj.value("system_error_string").toString();

	QJsonArray ary = obj.value("drives").toArray();

	QStringList listDrives;
	for (int i=0; i<ary.size(); i++)
	{
		QJsonObject obj = ary[i].toObject();
		QString strDevName = obj["dev_name"].toString();

		QMap<QString, DriveData*>::iterator it = m_mapDrive.find(strDevName);
		if (it != m_mapDrive.end())
		{
			it.value()->UpdateData(obj);
		}
		else
		{
			DriveData* data = new DriveData();
			data->UpdateData(obj);
			m_mapDrive[strDevName] = data;
		}
		listDrives.append(strDevName);
	}

	QMap<QString, DriveData*>::iterator it = m_mapDrive.begin();
	while (it != m_mapDrive.end())
	{
		if (!listDrives.contains(it.key()))
		{
			delete it.value();
			it = m_mapDrive.erase(it);
			continue;
		}
		it++;
	}

}

QRobotDeviceWidget::QRobotDeviceWidget(DeviceData* data, QWidget *parent /* = 0 */)
	: QWidget(parent)
	, m_data(data)
{
	ui.setupUi(this);
	UpdateUI();

	ui.widget_Device->installEventFilter(this);
	ui.label_ico->installEventFilter(this);
	SetExpand(true);

// 	QTimer* Timer = new QTimer(this);
// 	Timer->start(1000);
// 	connect(Timer, SIGNAL(timeout()), this, SLOT(sltTimeOut()));
// 
// 	connect(m_pSetDriveAction, SIGNAL(triggered()), this, SLOT(sltSetDriveAction()));
// 
// 	ui.widget_Device->setContextMenuPolicy(Qt::CustomContextMenu);
// 	connect(ui.widget_Device, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(sltProjectTreeMenuRequested(const QPoint&)));


#ifdef WIN32
    ui.comboBox_cmd->addItem("校准打印机", ROBOT_CMD_PTACT_ALIGNPRINTER);
    ui.comboBox_cmd->addItem("忽略低墨盒容量提示", ROBOT_CMD_PTACT_IGNOREINKLOW);
    ui.comboBox_cmd->addItem("重新设置设备刻录机", ROBOT_CMD_PTACT_REINIT_DRIVES);
    ui.comboBox_cmd->addItem("取消设备控制命令", ROBOT_CMD_PTACT_CANCELCMD);
    ui.comboBox_cmd->addItem("重启设备", ROBOT_CMD_PTACT_RESETSYSTEM);
    ui.comboBox_cmd->addItem("检测光盘数量", ROBOT_CMD_PTACT_CHECKDISCS);
    ui.comboBox_cmd->addItem("清洗打印头", ROBOT_CMD_PTACT_CLEANCARTRIDGES);
    ui.comboBox_cmd->addItem("校准光盘桶", ROBOT_CMD_PTACT_CALIBRATE_ONE_DISC);
    ui.comboBox_cmd->addItem("更换墨盒", ROBOT_CMD_PTACT_CHANGE_CARTRIDGE);
    ui.comboBox_cmd->addItem("更新完成墨盒安装", ROBOT_CMD_PTACT_END_CARTRIDGE_CHANGE);
    ui.comboBox_cmd->addItem("初始化到运输模式", ROBOT_CMD_PTACT_SHIP_POSITION);
    ui.comboBox_cmd->addItem("允许无墨盒状态运行", ROBOT_CMD_PTACT_ALLOW_NO_CARTRIDGES);

#else
    ui.comboBox_cmd->addItem("检测光盘数量", ROBOT_CMD_PTACT_CHECKDISCS);
    ui.comboBox_cmd->addItem("重启设备", ROBOT_CMD_PTACT_RESETSYSTEM);
    ui.comboBox_cmd->addItem("初始化到运输模式", ROBOT_CMD_PTACT_SHIP_POSITION);
    ui.comboBox_cmd->addItem("校准光盘桶", ROBOT_CMD_PTACT_CALIBRATE_ONE_DISC);
    ui.comboBox_cmd->addItem("重新设置设备刻录机", ROBOT_CMD_PTACT_REINIT_DRIVES);
    ui.comboBox_cmd->addItem("取消设备控制命令", ROBOT_CMD_PTACT_CANCELCMD);
#endif

}

QRobotDeviceWidget::~QRobotDeviceWidget()
{

}


void QRobotDeviceWidget::on_pushButton_cmd_clicked()
{
    int cmd = ui.comboBox_cmd->currentData().toInt();
    emit sigExecCmd(m_data->name, cmd);
}

void QRobotDeviceWidget::UpdateData(QJsonObject& obj)
{
	m_data->UpdateData(obj);
	UpdateUI();
}


void QRobotDeviceWidget::UpdateUI()
{
	m_strDeviceTooltip = m_data->system_error_string;
	bool bHideBlack = false;
	QString strImage = GetDeviceImage(m_data->type, bHideBlack);
	ui.label_Device->setStyleSheet(QString("image: url(%1);").arg(strImage));
	if (bHideBlack)
	{
		ui.label_Black->hide();
	}

	if (m_data->bins_count == 1)
	{
		ui.label_DiscBoxImage2->hide();
		ui.label_DiscBox2->hide();
	}
	ui.label_RobotDesc->setText(m_data->name);
	ui.label_DeviceStatus->setText(m_data->system_state_string);
	if (m_data->system_state == 0)
		ui.label_DeviceStatusIcon->setStyleSheet("image: url(:/Resources/device_idle.png);");
	else if (m_data->system_state == 1)
		ui.label_DeviceStatusIcon->setStyleSheet("image: url(:/Resources/device_busy.png);");
	else if (m_data->system_state == 2)
		ui.label_DeviceStatusIcon->setStyleSheet("image: url(:/Resources/device_error.png);");
	else
		ui.label_DeviceStatusIcon->setStyleSheet("image: url(:/Resources/device_idle.png);");

	QString strText = "";
	if (-1 == m_data->bin1_disc_count)
		strText = "UNKNOW";
	else
		strText = QString("%1光盘").arg(m_data->bin1_disc_count);	
	ui.label_DiscBox1->setText(strText);					
	ui.label_DiscBoxImage1->setPixmap(QPixmap(GetDiscBoxImage(m_data->bin1_disc_count)));

	if (-1 == m_data->bin2_disc_count)
		strText = "UNKNOW";
	else
		strText = QString("%1光盘").arg(m_data->bin2_disc_count);					
	ui.label_DiscBox2->setText(strText);
	ui.label_DiscBoxImage2->setPixmap(QPixmap(GetDiscBoxImage(m_data->bin2_disc_count)));

	ui.label_Black->setPixmap(QPixmap(GetBlackImage(m_data->black_ink)));
	ui.label_Green->setPixmap(QPixmap(GetCyanImage(m_data->blue_ink)));
	ui.label_Mmagenta->setPixmap(QPixmap(GetMagentaImage(m_data->red_ink)));
	ui.label_Yellow->setPixmap(QPixmap(GetYellowImage(m_data->yellow_ink)));

	QStringList listDrives;

	QMap<QString, DriveData*>::iterator it = m_data->m_mapDrive.begin();
	while (it != m_data->m_mapDrive.end())
	{
		QString strDrive = it.key();
		listDrives.append(strDrive);
		QMap<QString, QRecorderWidget*>::Iterator it2 = m_mapRecorderWidget.find(strDrive);
		if (it2 == m_mapRecorderWidget.end())
		{
			QRecorderWidget* widget = new QRecorderWidget(it.value(), this);
			m_mapRecorderWidget[strDrive] = widget;

			if (it.value()->row == 1)
				ui.widget_Driver->insertWidget(0, widget);
			else if (it.value()->row == 2)
				ui.widget_Driver->insertWidget(1, widget);
			else
				ui.widget_Driver->insertWidget(0, widget);
		}
		else
		{
			QRecorderWidget* widget = (QRecorderWidget*)(it2.value());
			widget->UpdateUI();
		}

		it++;
	}

	QMap<QString, QRecorderWidget*>::iterator it3 = m_mapRecorderWidget.begin();
	while (it3 != m_mapRecorderWidget.end())
	{
		if (!listDrives.contains(it3.key()))
		{
			ui.widget_Driver->removeWidget(it3.value());
			delete it3.value();
			it3 = m_mapRecorderWidget.erase(it3);
			continue;
		}
		it3++;
	}
}

void QRobotDeviceWidget::SetExpand(bool bExpand)
{
	m_bExpand = bExpand;
	if (m_bExpand)
	{
		//down
		QIconHelper::SetIcon(ui.label_ico, QChar(0xf078), 12);
		ui.widget_ChildDriver->show();
		ui.widget_Device->setStyleSheet("QWidget#widget_Device{background: #E5E5E5;border: 1px solid #d4d5d5;border-left: none;border-right: none;border-top: none;}");
	}
	else
	{
		//right
		QIconHelper::SetIcon(ui.label_ico, QChar(0xf054), 12);
		ui.widget_ChildDriver->hide();
		ui.widget_Device->setStyleSheet("QWidget#widget_Device{background: #E5E5E5;border: none;}");
	}
}

bool QRobotDeviceWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui.widget_Device)
	{
		if (event->type() == QEvent::ToolTip)
		{
			if (m_strDeviceTooltip.isEmpty() == false)
			{
				QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
				QToolTip::showText(helpEvent->globalPos(), m_strDeviceTooltip);
				return true;
			}
		}
	}

	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent->button() == Qt::LeftButton)
		{
			if (obj == ui.label_ico)
			{
				SetExpand(!m_bExpand);
				return true;
			}
		}
	}

	if (event->type() == QEvent::MouseButtonDblClick)
	{
		if (obj == ui.widget_Device)
		{
			SetExpand(!m_bExpand);
			return true;
		}
	}

	return QWidget::eventFilter(obj, event);
}

// void QRobotDeviceWidget::GetRobotDriveMap(QMap<QString, QRecorderWidget*>& mapDrive)
// {
// 	QMap<QChar, QRecorderWidget*>::iterator it = m_mapRecordWidget.begin();
// 	for (; it != m_mapRecordWidget.end(); it++)
// 	{
// 		QRecorderWidget* pRecorderWidget = it.value();
// 		if (pRecorderWidget == NULL)
// 		{
// 			continue;
// 		}
// 		mapDrive[it.key()] = pRecorderWidget;
// 	}
// }

void QRobotDeviceWidget::sltTimeOut()
{
// 	QMutexLocker locker(&m_Mutex);
// 	if (!PMYBurnClient::sMainWidget->CheckSocketStatus())
// 	{
// 		return;
// 	}
// 	m_Mutex.lock();
// 	m_nUpdateTime++;
// 	m_Mutex.unlock();
// 
// 	if (m_nUpdateTime > 30)
// 	{
// 		emit sigDelDevice(m_strDeviceName);
// 	}
}

void QRobotDeviceWidget::sltSetDriveAction()
{
// 	QSetDriveDialog dlg(m_strDeviceName, PMYBurnClient::sMainWidget);
// 	if (dlg.exec() != QDialog::Accepted)
// 	{
// 		return;
// 	}
}

void QRobotDeviceWidget::sltProjectTreeMenuRequested(const QPoint& point)
{
	if (!m_bSetDrive)
		return;

	QMenu *menu = new QMenu(this);
	menu->addAction(m_pSetDriveAction);
	menu->move(cursor().pos());
	menu->show();
}

QRecorderWidget* QRobotDeviceWidget::GetDriveWidget(QString strId)
{
	QMutexLocker lock(&m_Mutex);
	QMap<QString, QRecorderWidget*>::iterator it = m_mapRecorderWidget.find(strId);
	if (it == m_mapRecorderWidget.end())
	{
		return NULL;	
	}
	return it.value();
}

QString QRobotDeviceWidget::GetDeviceImage(int nDeviceType, bool& bHideBlack)
{
	switch(nDeviceType)
	{
	case ROBOT_DISCPUBLISHER_SE:
		{
			bHideBlack = true;
			return ":/Resources/PTRobot/DPSE.png";
		}
		break;
	case ROBOT_DISCPUBLISHER_4100:
		{
			bHideBlack = false;
			return ":/Resources/PTRobot/DPPRO.png";
		}
		break;
	case ROBOT_DISCPUBLISHER_4100_XRP:
		{
			bHideBlack = false;
			return ":/Resources/PTRobot/DPXRP.png";
		}
		break;
	case ROBOT_DISCPUBLISHER_4051:
		{
			bHideBlack = false;
			return ":/Resources/PTRobot/DPPRO.png";
		}
		break;
	case ROBOT_DISCPUBLISHER_SE3:
		{
			bHideBlack = true;
			return ":/Resources/PTRobot/DPSE.png";
		}
		break;
	case ROBOT_DISCPUBLISHER_4200:
		{
			bHideBlack = true;
            return ":/Resources/PTRobot/DPXRP.png";
		}
		break;
	case ROBOT_DISCPUBLISHER_4200_XRP:
		{
			bHideBlack = true;
			return ":/Resources/PTRobot/DPXRP.png";
		}
		break;
	case ROBOT_DISCPUBLISHER_4052:
		{
			bHideBlack = true;
			return ":/Resources/PTRobot/DPPRO.png";
		}
		break;
	default:
		{
			bHideBlack = true;
			return ":/Resources/PTRobot/NoneFound.png";
		}
		break;
	}
}

QString QRobotDeviceWidget::GetDiscBoxImage(int nNum)
{
	if (nNum == -1)
	{
		return ":/Resources/PTRobot/bins_unk.png";
	}
	else if (nNum > 90)
	{
		return ":/Resources/PTRobot/bins_100.png";
	}
	else if (nNum > 70)
	{
		return ":/Resources/PTRobot/bins_80.png";
	}
	else if (nNum > 50)
	{
		return ":/Resources/PTRobot/bins_60.png";
	}
	else if (nNum > 30)
	{
		return ":/Resources/PTRobot/bins_40.png";
	}
	else if (nNum > 10)
	{
		return ":/Resources/PTRobot/bins_20.png";
	}
	else
	{
		return ":/Resources/PTRobot/bins_0.png";
	}
}

QString QRobotDeviceWidget::GetYellowImage(long lRemaining)
{
	int nPercent = lRemaining;// / 10000;
	if (lRemaining == -1)
	{
		return ":/Resources/PTRobot/Myellow_unknown.png";
	}
	else if (nPercent > 95)
	{
		return ":/Resources/PTRobot/Myellow_100.png";
	}
	else if (nPercent > 85)
	{
		return ":/Resources/PTRobot/Myellow_90.png";
	}
	else if (nPercent > 75)
	{
		return ":/Resources/PTRobot/Myellow_80.png";
	}
	else if (nPercent > 65)
	{
		return ":/Resources/PTRobot/Myellow_70.png";
	}
	else if (nPercent > 55)
	{
		return ":/Resources/PTRobot/Myellow_60.png";
	}
	else if (nPercent > 45)
	{
		return ":/Resources/PTRobot/Myellow_50.png";
	}
	else if (nPercent > 35)
	{
		return ":/Resources/PTRobot/Myellow_40.png";
	}
	else if (nPercent > 25)
	{
		return ":/Resources/PTRobot/Myellow_30.png";
	}
	else if (nPercent > 18)
	{
		return ":/Resources/PTRobot/Myellow_20.png";
	}
	else if (nPercent > 10)
	{
		return ":/Resources/PTRobot/Myellow_15.png";
	}
	else if (nPercent > 0)
	{
		return ":/Resources/PTRobot/Myellow_5.png";
	}
	else
	{
		return ":/Resources/PTRobot/Myellow_0.png";
	}
}

QString QRobotDeviceWidget::GetMagentaImage(long lRemaining)
{
	int nPercent = lRemaining;// / 10000;
	if (lRemaining == -1)
	{
		return ":/Resources/PTRobot/Mmagenta_unknown.png";
	}
	else if (nPercent > 95)
	{
		return ":/Resources/PTRobot/Mmagenta_100.png";
	}
	else if (nPercent > 85)
	{
		return ":/Resources/PTRobot/Mmagenta_90.png";
	}
	else if (nPercent > 75)
	{
		return ":/Resources/PTRobot/Mmagenta_80.png";
	}
	else if (nPercent > 65)
	{
		return ":/Resources/PTRobot/Mmagenta_70.png";
	}
	else if (nPercent > 55)
	{
		return ":/Resources/PTRobot/Mmagenta_60.png";
	}
	else if (nPercent > 45)
	{
		return ":/Resources/PTRobot/Mmagenta_50.png";
	}
	else if (nPercent > 35)
	{
		return ":/Resources/PTRobot/Mmagenta_40.png";
	}
	else if (nPercent > 25)
	{
		return ":/Resources/PTRobot/Mmagenta_30.png";
	}
	else if (nPercent > 18)
	{
		return ":/Resources/PTRobot/Mmagenta_20.png";
	}
	else if (nPercent > 10)
	{
		return ":/Resources/PTRobot/Mmagenta_15.png";
	}
	else if (nPercent > 0)
	{
		return ":/Resources/PTRobot/Mmagenta_5.png";
	}
	else
	{
		return ":/Resources/PTRobot/Mmagenta_0.png";
	}
}

QString QRobotDeviceWidget::GetCyanImage(long lRemaining)
{
	int nPercent = lRemaining;// / 10000;
	if (lRemaining == -1)
	{
		return ":/Resources/PTRobot/Mcyan_unknown.png";
	}
	else if (nPercent > 95)
	{
		return ":/Resources/PTRobot/Mcyan_100.png";
	}
	else if (nPercent > 85)
	{
		return ":/Resources/PTRobot/Mcyan_90.png";
	}
	else if (nPercent > 75)
	{
		return ":/Resources/PTRobot/Mcyan_80.png";
	}
	else if (nPercent > 65)
	{
		return ":/Resources/PTRobot/Mcyan_70.png";
	}
	else if (nPercent > 55)
	{
		return ":/Resources/PTRobot/Mcyan_60.png";
	}
	else if (nPercent > 45)
	{
		return ":/Resources/PTRobot/Mcyan_50.png";
	}
	else if (nPercent > 35)
	{
		return ":/Resources/PTRobot/Mcyan_40.png";
	}
	else if (nPercent > 25)
	{
		return ":/Resources/PTRobot/Mcyan_30.png";
	}
	else if (nPercent > 18)
	{
		return ":/Resources/PTRobot/Mcyan_20.png";
	}
	else if (nPercent > 10)
	{
		return ":/Resources/PTRobot/Mcyan_15.png";
	}
	else if (nPercent > 0)
	{
		return ":/Resources/PTRobot/Mcyan_5.png";
	}
	else
	{
		return ":/Resources/PTRobot/Mcyan_0.png";
	}
}

QString QRobotDeviceWidget::GetBlackImage(long lRemaining)
{
	int nPercent = lRemaining;// / 10000;
	if (lRemaining == -1)
	{
		return ":/Resources/PTRobot/Mblack_unknown.png";
	}
	else if (nPercent > 95)
	{
		return ":/Resources/PTRobot/Mblack_100.png";
	}
	else if (nPercent > 85)
	{
		return ":/Resources/PTRobot/Mblack_90.png";
	}
	else if (nPercent > 75)
	{
		return ":/Resources/PTRobot/Mblack_80.png";
	}
	else if (nPercent > 65)
	{
		return ":/Resources/PTRobot/Mblack_70.png";
	}
	else if (nPercent > 55)
	{
		return ":/Resources/PTRobot/Mblack_60.png";
	}
	else if (nPercent > 45)
	{
		return ":/Resources/PTRobot/Mblack_50.png";
	}
	else if (nPercent > 35)
	{
		return ":/Resources/PTRobot/Mblack_40.png";
	}
	else if (nPercent > 25)
	{
		return ":/Resources/PTRobot/Mblack_30.png";
	}
	else if (nPercent > 18)
	{
		return ":/Resources/PTRobot/Mblack_20.png";
	}
	else if (nPercent > 10)
	{
		return ":/Resources/PTRobot/Mblack_15.png";
	}
	else if (nPercent > 0)
	{
		return ":/Resources/PTRobot/Mblack_5.png";
	}
	else
	{
		return ":/Resources/PTRobot/Mblack_0.png";
	}
}
