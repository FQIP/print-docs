#include "stdafx.h"
#include <QDomDocument>
#include "PMYBurnSDKDemo.h"
#include "qnewtaskdialog.h"
#include <QFileInfo>
#include "QRobotDeviceWidget.h"
#include "QDataThread.h"
#include "QSystemConfigDlg.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif 

PMYBurnSdkDemo::PMYBurnSdkDemo(QWidget *parent)
	: QDialog(parent)
	, m_strDeviceDes("")
	, m_BurnTaskItemMutex(QMutex::Recursive)
    , m_pLoginHandle(nullptr)
	, m_bOnLine(false)
{
    ui.setupUi(this);
	setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	setWindowTitle("信刻光盘打印刻录系统");
	ui.treeWidget_Task->setColumnWidth(0, 200);
	ui.treeWidget_Task->setColumnWidth(1, 80);

	connect(this, SIGNAL(sigUpdateDynamic(QString)), this, SLOT(sltUpdateDynamic(QString)), Qt::QueuedConnection);
	connect(this, SIGNAL(sigUpdateRobotStatus(QString)), this, SLOT(sltUpdateRobotStatus(QString)), Qt::QueuedConnection);
	connect(this, SIGNAL(sigUpdateTaskStatus(QString)), this, SLOT(sltUpdateTaskStatus(QString)), Qt::QueuedConnection);
	connect(this, SIGNAL(sigUpdateEvent(int, int, QString)), this, SLOT(sltUpdateEvent(int, int, QString)), Qt::QueuedConnection);

	//ui.pushButton_SysConfig->hide();
	ui.pushButton_OpenLog->hide();
	ui.pushButton_GetTaskCfg->hide();
	//ui.listWidget_Log->hide();
	

	ui.label_status->setText("状态：离线");
	m_pTimer = new QTimer(this);
	m_pTimer->start(5000);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(sltTimeOut()));

	QString strFileName = QString("F:/Event/Event_%1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HHmmss"));
	file = new QFile(strFileName);
	file->open(QFile::ReadWrite | QFile::Text);
	//ui.groupBox_13->hide();
	
// 	m_strIp = "127.0.0.1";
// 	on_pushButton_Login_clicked();
// #ifdef _DEBUG
// 	ui.lineEdit_IP->setText("192.168.1.99");
// #endif
}

PMYBurnSdkDemo::~PMYBurnSdkDemo()
{
	SDKFunc().Release();
}

void PMYBurnSdkDemo::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key_Return:
	case Qt::Key_Enter:
		on_pushButton_Login_clicked();
		break;
	default:
		QDialog::keyPressEvent(e);
	}
}

void PMYBS_SDK_APICALL PMYBurnSdkDemo::PMYBS_RobotStatusCallBack(PMYLoginHandle LoginHandle, const char* pszBuffer, void* pUserData)
{
	PMYBurnSdkDemo* pThis = (PMYBurnSdkDemo*)pUserData;
    if (LoginHandle == pThis->m_pLoginHandle)
        emit pThis->sigUpdateRobotStatus(pszBuffer);
}

void PMYBS_SDK_APICALL PMYBurnSdkDemo::PMYBS_TaskStatusCallBack(PMYLoginHandle LoginHandle, const char* pszBuffer, void* pUserData)
{
	PMYBurnSdkDemo* pThis = (PMYBurnSdkDemo*)pUserData;
    if (LoginHandle == pThis->m_pLoginHandle)
        emit pThis->sigUpdateTaskStatus(pszBuffer);
}

void PMYBS_SDK_APICALL PMYBurnSdkDemo::PMYBS_DynamicCallBack(PMYLoginHandle LoginHandle, const char* pszBuffer, void* pUserData)
{
	PMYBurnSdkDemo* pThis = (PMYBurnSdkDemo*)pUserData;
	if (LoginHandle == pThis->m_pLoginHandle)
		emit pThis->sigUpdateDynamic(pszBuffer);
}

void PMYBS_SDK_APICALL PMYBurnSdkDemo::PMYBS_EventCallBack(PMYLoginHandle LoginHandle, int nMajorEvent, int nMinorEvent, const char* pEventStr, void* pUserData)
{
	PMYBurnSdkDemo* pThis = (PMYBurnSdkDemo*)pUserData;
	if (LoginHandle == pThis->m_pLoginHandle)
		emit pThis->sigUpdateEvent(nMajorEvent, nMinorEvent, pEventStr);
}

void PMYBurnSdkDemo::on_pushButton_SysConfig_clicked()
{
	if (!m_bOnLine)
		return;
	QSystemConfigDlg dlg(m_pLoginHandle, this);
	dlg.exec();
}

void PMYBurnSdkDemo::on_pushButton_Restart_clicked()
{
	if (!m_bOnLine)
		return;
	SDKFunc()._PMYBS_ReStartService(m_pLoginHandle);
}

void PMYBurnSdkDemo::sltUpdateTaskStatus(QString strText)
{
	NotifyTask(strText.toStdString());
}

void PMYBurnSdkDemo::sltUpdateRobotStatus(QString strText)
{
	NotifyDevice(strText.toStdString());
}

void PMYBurnSdkDemo::sltUpdateDynamic(QString strJson)
{
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(strJson.toStdString().data(), &json_error);
	if(json_error.error != QJsonParseError::NoError) 
	{
		QMessageBox::warning(this, "error", QString("invalid response. err: %1").arg(json_error.errorString()));
		return;
	}
	QJsonObject obj = parse_doucment.object();
	QJsonObject DynamicObj = obj.value("dynamic").toObject();

	int nLogType = DynamicObj.value("log_type").toInt();
	int nLogLevel = DynamicObj.value("log_level").toInt();
	QString strTime = DynamicObj.value("time").toString();
	QString strUserName = DynamicObj.value("user_name").toString();
	QString strDesc = DynamicObj.value("desc").toString();
	QString strDetail = DynamicObj.value("detail").toString();
	int nTaskType = DynamicObj.value("task_type").toInt();
	QString strTaskId = DynamicObj.value("task_id").toString();
	QString strTaskName = DynamicObj.value("task_name").toString();

	QString strLogTxt = "";
	if (strTime.isEmpty() == false)
	{
		strLogTxt += QString("[%1] ").arg(strTime);
	}
	if (strTaskName.isEmpty() == false)
	{
		strLogTxt += QString("[%1] ").arg(strTaskName);
	}
	if (strDesc.isEmpty() == false)
	{
		strLogTxt += QString("[%1] ").arg(strDesc);
	}
	if (strDetail.isEmpty() == false)
	{
		strLogTxt += QString("[%1] ").arg(strDetail);
	}
	InsertLog(strLogTxt);
}

void PMYBurnSdkDemo::sltUpdateEvent(int nMajorEvent, int nMinorEvent, QString strEvent)
{
	qDebug() << "nMajorEvent : " << nMajorEvent;
	qDebug() << "nMinorEvent : " << nMinorEvent;
	qDebug() << "strEvent : " << strEvent;

	QTextStream out(file);
	out << "nMajorEvent : " << nMajorEvent << endl;
	out << "nMinorEvent : " << nMinorEvent << endl;
	out << "strEvent : " << strEvent << endl;
	out << endl;
}

void PMYBurnSdkDemo::InsertLog(QString text)
{
	static QMutex sm;
	QMutexLocker lock(&sm);

	static QString status;
/*	if (status != text)*/
	{
		//QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
		//QString strLog = QString("%1 %2").arg(strTime).arg(text);

		QListWidgetItem *item = new QListWidgetItem(ui.listWidget_Log);
		item->setText(text);
		ui.listWidget_Log->addItem(item);
		ui.listWidget_Log->setCurrentRow(ui.listWidget_Log->count() - 1);
		status = text;
	}
}

void PMYBurnSdkDemo::NotifyDevice(const std::string& strResponse)
{
	QString strDevice = QString::fromStdString(strResponse);
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(strDevice.toStdString().data(), &json_error);
	if(json_error.error != QJsonParseError::NoError) 
	{
		QMessageBox::warning(this, "error", QString("invalid response. err: %1").arg(strDevice));
		return;
	}
	QJsonObject obj = parse_doucment.object();
	int error_code = obj.value("error_code").toInt();
	if (error_code != 0)
	{
		QMessageBox::warning(this, "error", QString("query fail. err: %1").arg(error_code));
		return;
	}

//    ClearDeviceWidget();
    QStringList lstIds;
	QJsonArray deviceArray = obj.value("devices").toArray();
	for (int i=0; i<deviceArray.size(); i++)
	{
		QJsonValue value = deviceArray[i];
		QJsonObject device = value.toObject();
		QString id = device.value("id").toString();
		QMap<QString, QRobotDeviceWidget*>::Iterator it = m_mapDeviceWidget.find(id);
		if (it == m_mapDeviceWidget.end())
		{
			DeviceData* data = new DeviceData;
			data->UpdateData(device);
			QRobotDeviceWidget* widget = new QRobotDeviceWidget(data, nullptr);
            connect(widget, SIGNAL(sigExecCmd(QString, int)), this, SLOT(sltExecCmd(QString, int)), Qt::QueuedConnection);

			m_mapDeviceWidget[id] = widget;
			ui.verticalLayout_device->addWidget(widget);
		}
		else
		{
			QRobotDeviceWidget* widget = (QRobotDeviceWidget*)(it.value());
			widget->UpdateData(device);
		}

        lstIds.append(id);
	}

    QMap<QString, QRobotDeviceWidget*>::Iterator it = m_mapDeviceWidget.begin();
    while (it != m_mapDeviceWidget.end())
    {
        if (!lstIds.contains(it.key()))
        {
            ui.verticalLayout_device->removeWidget(it.value());
            delete it.value();

            it = m_mapDeviceWidget.erase(it);
        }
        else
        {
            it++;
        }
    }

}

void PMYBurnSdkDemo::NotifyDrive(const std::string& strResponse)
{
	QString strDevice = QString::fromStdString(strResponse);
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(strDevice.toStdString().data(), &json_error);
	if(json_error.error != QJsonParseError::NoError) 
	{
		QMessageBox::warning(this, "error", QString("invalid response. err: %1").arg(strDevice));
		return;
	}
	QJsonObject obj = parse_doucment.object();
	if (!obj.value("drive").isObject())
	{
		return;
	}

	QJsonObject drive = obj.value("drive").toObject();
	QString dev_name = drive.value("dev_name").toString();
	QRecorderWidget* widget = GetDriveWidget(dev_name);
	if (widget == NULL)
	{
		return;
	}

	widget->UpdateData(drive);
}

QRecorderWidget* PMYBurnSdkDemo::GetDriveWidget(QString dev_name)
{
	QMap<QString, QRobotDeviceWidget*>::iterator it = m_mapDeviceWidget.begin();
	while (it != m_mapDeviceWidget.end())
	{
		QRecorderWidget* widget = it.value()->GetDriveWidget(dev_name);
		if (widget)
		{
			return widget;
		}
		it++;
	}
    return nullptr;
}

void PMYBurnSdkDemo::NotifyDisc(const std::string& strResponse)
{
	QString strDevice = QString::fromStdString(strResponse);
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(strDevice.toStdString().data(), &json_error);
	if(json_error.error != QJsonParseError::NoError) 
	{
		QMessageBox::warning(this, "error", QString("invalid response. err: %1").arg(strDevice));
		return;
	}
	QJsonObject obj = parse_doucment.object();
	if (!obj.value("disc").isObject())
	{
		return;
	}

	QJsonObject disc = obj.value("disc").toObject();
	QString id = disc.value("id").toString();
	QString taskId;
	BaseItem* pBaseItem = GetChildItem2(id, taskId);
	if (pBaseItem == NULL)
	{
		return;
	}
	DiscItem* pChildItem = static_cast<DiscItem*>(pBaseItem);
	pChildItem->Update(disc);
	UpdateChildItem(taskId, pChildItem);
}

void PMYBurnSdkDemo::NotifyDownload(const std::string& strResponse)
{
	QString strDevice = QString::fromStdString(strResponse);
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(strDevice.toStdString().data(), &json_error);
	if(json_error.error != QJsonParseError::NoError) 
	{
		QMessageBox::warning(this, "error", QString("invalid response. err: %1").arg(strDevice));
		return;
	}
	QJsonObject obj = parse_doucment.object();
	if (!obj.value("file_upload").isObject())
	{
		return;
	}
	QJsonObject download = obj.value("file_upload").toObject();
	QString id = download.value("id").toString();
	QString taskId;
	BaseItem* pBaseItem = GetChildItem2(id, taskId);
	if (pBaseItem == NULL)
	{
		return;
	}
	UploadItem* pChildItem = static_cast<UploadItem*>(pBaseItem);
	pChildItem->Update(download);
	UpdateChildItem(taskId, pChildItem);
}

void PMYBurnSdkDemo::on_pushButton_QueryTask_clicked()
{
	if (!m_bOnLine)
		return;
	int nLen = 0;
	char* pBuff = new char[nLen];

	int nRet = SDKFunc()._PMYBS_GetTaskStatus(m_pLoginHandle, NULL, pBuff, &nLen);
	if (nRet == PMYBS_SDK_OUTPUT_BUFFER_SMALL)
	{
		delete [] pBuff;
		pBuff = new char[nLen];
		nRet = SDKFunc()._PMYBS_GetTaskStatus(m_pLoginHandle, NULL, pBuff, &nLen);
	}

	if (nRet != PMYBS_SUCCESS)
	{
		delete [] pBuff;
		QMessageBox::warning(this, "error", QString("_PMYBS_GetTaskStatus fail, nRet: %1").arg(nRet));
		return;
	}

	if (!m_bFirst)
	{
		QString strText = QString::fromUtf8(pBuff);
		QMessageBox::information(this, "info", QString("_PMYBS_GetTaskStatus success:\n\n%1").arg(strText));
	}

	std::string strResponse = pBuff;
	NotifyTask(strResponse);
	//InsertLog("GetTaskStatus success");
	delete [] pBuff;
}

void PMYBurnSdkDemo::on_pushButton_QueryDevice_clicked()
{
	if (!m_bOnLine)
		return;
	int nLen = 0;
	char* pBuff = new char[nLen];

	int nRet = SDKFunc()._PMYBS_GetRobotStatus(m_pLoginHandle, pBuff, &nLen);
	if (nRet == PMYBS_SDK_OUTPUT_BUFFER_SMALL)
	{
		delete [] pBuff;
		pBuff = new char[nLen];
		nRet = SDKFunc()._PMYBS_GetRobotStatus(m_pLoginHandle, pBuff, &nLen);
	}

	if (nRet != PMYBS_SUCCESS)
	{
		delete [] pBuff;
		QMessageBox::warning(this, "error", QString("_PMYBS_GetRobotStatus fail, nRet: %1").arg(nRet));
		return;
	}

	std::string strResponse = pBuff;

	if (!m_bFirst)
	{
		QString strText = QString::fromUtf8(pBuff);
		QMessageBox::information(this, "info", QString("_PMYBS_GetRobotStatus success:\n\n%1").arg(strText));
	}

	NotifyDevice(strResponse);
	//InsertLog("GetRobotStatus success");
	delete [] pBuff;
}

void PMYBurnSdkDemo::on_pushButton_Login_clicked()
{
	QString strIP = ui.lineEdit_IP->text();
	if (strIP.isEmpty())
	{
		QMessageBox::warning(this, "error", "please input ip!");
		return;
	}
	m_strIp = strIP;

    on_pushButton_Logout_clicked();

	int nRet = SDKFunc()._PMYBS_Login(m_strIp.toStdString().c_str(), PMYBS_SDK_LOGIN_PORT, &m_pLoginHandle);
	if (nRet != PMYBS_SUCCESS)
	{
		QMessageBox::warning(this, "error", QString("login fail, nRet: %1").arg(nRet));
		//exit(0);
		return;
	}

	nRet = SDKFunc()._PMYBS_SetRobotStatusCallback(PMYBS_RobotStatusCallBack, this);
	nRet = SDKFunc()._PMYBS_SetTaskStatusCallback(PMYBS_TaskStatusCallBack, this);
	nRet = SDKFunc()._PMYBS_SetDynamicCallback(PMYBS_DynamicCallBack, this);
	nRet = SDKFunc()._PMYBS_SetEventCallback(PMYBS_EventCallBack, this);

	ClearDeviceWidget();
	ClearTaskWidget();
	ui.listWidget_Log->clear();

	m_bFirst = true;
	on_pushButton_QueryDevice_clicked();
	on_pushButton_QueryTask_clicked();

	m_strVersion = GetConfig(PMYBS_SDK_CFG_VERSION);
	m_bFirst = false;
	m_bOnLine = true;
	sltTimeOut();
}

void PMYBurnSdkDemo::on_pushButton_Logout_clicked()
{
	ClearDeviceWidget();
	ClearTaskWidget();
	ui.listWidget_Log->clear();

    int nRet = SDKFunc()._PMYBS_Logout(m_pLoginHandle);
    if (nRet != PMYBS_SUCCESS)
    {
        QMessageBox::warning(this, "error", QString("logout fail, nRet: %1").arg(nRet));
        return;
    }

}

void PMYBurnSdkDemo::ClearTaskWidget()
{
	ui.treeWidget_Task->clear();

	QMutexLocker lock(&m_BurnTaskItemMutex);
	QMap<QString, TaskItem*>::iterator it = m_mapBurnTaskItem.begin();
	while(it != m_mapBurnTaskItem.end())
	{
		if (it.value())
		{
			delete it.value();
		}
		it++;
	}

	m_mapBurnTaskItem.clear();
}

void PMYBurnSdkDemo::on_pushButton_NewTask_clicked()
{
	if (!m_bOnLine)
		return;
	QNewTaskDialog dlg(m_pLoginHandle, this);
	if (dlg.exec() != QDialog::Accepted)
	{
		return;
	}

	QString strTaskId = dlg.m_strTaskId;

	if (dlg.m_bJsonSubmitTask)
	{
		TASK_ID tTaskId;
		QString strJson = dlg.m_strJson;
		std::string strResponse;
		int nRet = SDKFunc()._PMYBS_CreateTaskByJson(m_pLoginHandle, strJson.toStdString().c_str(), &tTaskId);
		if (nRet != 0)
		{
			QMessageBox::warning(this, "error", QString("RPC_QueryDevice fail. err: %1").arg(nRet));
			return;
		}
		strTaskId = tTaskId.strTaskId;
	}

	if (dlg.m_nTaskType == TASK_TYPE_REAL_FILE_BURN)
	{
		PMYTaskHandle taskHandle;
		//创建任务
		int nRet = SDKFunc()._PMYBS_CreateTask(m_pLoginHandle, TASK_TYPE_REAL_FILE_BURN, dlg.m_strTaskName.toStdString().c_str(), &taskHandle);
		if (nRet != PMYBS_SUCCESS)
		{
			QMessageBox::warning(this, "error", QString("_PMYBS_CreateTask fail. err: %1").arg(nRet));
			return;
		}

		do 
		{
			nRet = SDKFunc()._PMYBS_SetPrintFile(taskHandle, dlg.m_strSuccessPrintFile.toStdString().c_str(), dlg.m_strFailPrintFile.toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
			{
				QMessageBox::warning(this, "error", QString("_PMYBS_SetPrintFile fail. err: %1").arg(nRet));
				break;
			}
			//添加打印字段 index 从1开始
			for (int i=0; i<dlg.m_vPrintValues.size(); i++)
			{
				nRet = SDKFunc()._PMYBS_SetPrintField(taskHandle, i + 1, dlg.m_vPrintValues[i].strValue.toStdString().c_str());
				if (nRet != PMYBS_SUCCESS)
				{
					QMessageBox::warning(this, "error", QString("_PMYBS_SetPrintField fail. err: %1").arg(nRet));
					break;
				}
			}
			if (nRet != PMYBS_SUCCESS)
			{
				break;
			}

			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_DISC_TYPE, QString("%1").arg((int)dlg.m_eDiscType).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_CLOSE_DISC, QString("%1").arg(dlg.m_bCloseDisc).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_DISC_LABEL, QString("%1").arg(dlg.m_strDiscLabel).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_TEST_BURN, QString("%1").arg(dlg.m_bTestBurn).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_DISC_OUTPUT_MODE, QString("%1").arg(dlg.m_nDiscOutputMode).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_DELETE_FILES, QString("%1").arg(dlg.m_bDeleteFile).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;

			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_COPY_COUNT, QString("%1").arg(dlg.m_nCopyCount).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_BURN_SPEED, QString("%1").arg(dlg.m_nBurnSpeed).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_VERIFY_DISC, QString("%1").arg(dlg.m_bVerifyDisc).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_REJECT_IF_NOT_BLANK, QString("%1").arg(dlg.m_bRejectIfNotBlank).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_BURN_PROOF, QString("%1").arg(dlg.m_bBurnProof).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_VNR_WRITE, QString("%1").arg(dlg.m_bVNRWrite).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_BURN_ERROR_RETRY, QString("%1").arg(dlg.m_nErrorRetry).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;
			nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_BURN_REAL_BURN_STREAM, QString("%1").arg(dlg.m_bBurnStream).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
				break;

			TASK_ID tTaskId;
			nRet = SDKFunc()._PMYBS_SubmitTask(taskHandle, &tTaskId);
			if (nRet != PMYBS_SUCCESS)
			{
				QMessageBox::warning(this, "error", QString("_PMYBS_SubmitTask fail. err: %1").arg(nRet));
				break;
			}

			nRet = SDKFunc()._PMYBS_CloseTask(taskHandle);
			if (nRet != PMYBS_SUCCESS)
			{
				QMessageBox::warning(this, "error", QString("_PMYBS_CloseTask fail. err: %1").arg(nRet));
				break;
			}

			QDataThread* dataThread = new QDataThread(this);
			connect(dataThread, SIGNAL(finished()), dataThread, SLOT(deleteLater()));
			connect(dataThread, SIGNAL(sigFinishData(QString, int)), this, SLOT(sltFinishData(QString, int)));
			dataThread->m_strCache = dlg.m_strCache;
			dataThread->m_pLoginHandle = m_pLoginHandle;
			dataThread->m_strTaskId = tTaskId.strTaskId;
			dataThread->m_eDiscType = dlg.m_eDiscType;
			dataThread->m_nTotalMB = dlg.m_nTotalMB;
			dataThread->m_nMBPreFile = dlg.m_nMBPreFile;
			dataThread->m_nMins = dlg.m_nMins;
			dataThread->m_bBurnStream = dlg.m_bBurnStream;

			m_pVtWorkThread[tTaskId.strTaskId] = dataThread;
			dataThread->start();

			TaskItem* pTaskItem = GetTaskItem(tTaskId.strTaskId);
			if (pTaskItem == NULL)
			{
				pTaskItem = new TaskItem;
				pTaskItem->id = tTaskId.strTaskId;
				pTaskItem->name = dlg.m_strTaskName;
				m_mapBurnTaskItem[tTaskId.strTaskId] = pTaskItem;
			}
			UpdateTaskItem(pTaskItem);

			return;
		} while (0);
		
		SDKFunc()._PMYBS_CloseTask(taskHandle);
	}

// 	if (strTaskId.isEmpty())
// 		InsertLog("create task fail");
// 	else
// 	{
// 		InsertLog("create task success");
// 	}
}
void PMYBurnSdkDemo::on_pushButton_CancelTask_All_clicked()
{
	if (!m_bOnLine)
		return;

	QTreeWidgetItemIterator it(ui.treeWidget_Task);
	while (*it) 
	{
		QString strTaskId = (*it)->data(0, Qt::UserRole).toString();
		CancelTask(strTaskId);
		++it;
	}

	QMessageBox::information(this, "info", QString("All tasks cancelled successfully"));
}

void PMYBurnSdkDemo::CancelTask(QString strTaskId, bool bShow /* = false */)
{
	{
		QMutexLocker lock(&m_WorkThreadMutex);
		QMap<QString, QDataThread*>::iterator it = m_pVtWorkThread.find(strTaskId);
		if (it != m_pVtWorkThread.end())
		{
			it.value()->SetStop();
		}
	}

	int nRet = SDKFunc()._PMYBS_CancleTaskById(m_pLoginHandle, strTaskId.toStdString().c_str());
	if (nRet != 0)
	{
		if (bShow)
			QMessageBox::warning(this, "error", QString("_PMYBS_CancleTask fail. err: %1").arg(nRet));
		return;
	}
	if (bShow)
		QMessageBox::information(this, "info", QString("Task [%1] cancel sus").arg(strTaskId));
}

void PMYBurnSdkDemo::on_pushButton_CancelTask_clicked()
{
	if (!m_bOnLine)
		return;
	QTreeWidgetItem * item = ui.treeWidget_Task->currentItem();
	if (item == NULL)
	{
		return;
	}
	if (item->parent() != NULL)
	{
		return;
	}

	QString strTaskId = item->data(0, Qt::UserRole).toString();

	CancelTask(strTaskId, true);
}

TaskItem* PMYBurnSdkDemo::GetTaskItem(QString strTaskRunId)
{
	QMutexLocker lock(&m_BurnTaskItemMutex);

	QMap<QString, TaskItem*>::iterator it = m_mapBurnTaskItem.find(strTaskRunId);
	if (it == m_mapBurnTaskItem.end())
	{
		return NULL;
	}
	TaskItem* BurnItem = it.value();
	return BurnItem;
}

BaseItem* PMYBurnSdkDemo::GetChildItem(QString nTaskId, QString strItemId)
{
	QMutexLocker lock(&m_BurnTaskItemMutex);

	QMap<QString, TaskItem*>::iterator it = m_mapBurnTaskItem.find(nTaskId);
	if (it == m_mapBurnTaskItem.end())
	{
        return NULL;
	}
	TaskItem* BurnItem = it.value();

	QMap<QString, BaseItem*>::iterator itDisc = BurnItem->mapChildItems.find(strItemId);
	if (itDisc == BurnItem->mapChildItems.end())
	{
        return NULL;
	}
	return itDisc.value();
}

BaseItem* PMYBurnSdkDemo::GetChildItem2(QString strItemId, QString& strTaskId)
{
	QMutexLocker lock(&m_BurnTaskItemMutex);
	QMap<QString, TaskItem*>::iterator it = m_mapBurnTaskItem.begin();
	while (it != m_mapBurnTaskItem.end())
	{
		BaseItem* item = GetChildItem(it.key(), strItemId);
		if (item)
		{
			strTaskId = it.key();
			return item;
		}
		it++;
	}

	return NULL;
}

void PMYBurnSdkDemo::InsertChildItem(QString strTaskRunId, QString strChildId, BaseItem* pItem)
{
	QMutexLocker lock(&m_BurnTaskItemMutex);

	QMap<QString, TaskItem *>::iterator it = m_mapBurnTaskItem.find(strTaskRunId);
	if (it == m_mapBurnTaskItem.end())
	{
		return;
	}
	TaskItem * BurnItem = it.value();
	BurnItem->mapChildItems[strChildId] = pItem;
	return;
}

void PMYBurnSdkDemo::UpdateTaskItem(TaskItem* pTask)
{
	QTreeWidgetItem* Item = pTask->m_Item;
    if (pTask->m_Item == NULL)
	{
		Item = new QTreeWidgetItem(QStringList(pTask->name));

		Item->setData(0, Qt::UserRole, pTask->id);
		ui.treeWidget_Task->addTopLevelItem(Item);
		Item->setExpanded(true);
		pTask->m_Item = Item;
	}
	Item->setText(1, QString("%1 ").arg(FormatSize(pTask->total_size)));
	Item->setText(2, pTask->status_string);
	return;
}

void PMYBurnSdkDemo::UpdateChildItem(QString strTaskId, BaseItem* pBaseItem)
{
	QTreeWidgetItem* pTreeItem = pBaseItem->m_Item;
    if (pBaseItem->m_Item == NULL)
	{
		TaskItem* pTaskItem = GetTaskItem(strTaskId);
        if (pTaskItem == NULL)
		{
			return;
		}
		pTreeItem = new QTreeWidgetItem(pTaskItem->m_Item);
// 		QProgressBar* widget = new QProgressBar(ui.treeWidget_Task);	
// 		ui.treeWidget_Task->setItemWidget(pTreeItem, 2, widget);
		pTaskItem->m_Item->addChild(pTreeItem);
		pBaseItem->m_Item = pTreeItem;
	}
	if (pBaseItem->m_eType == ITEM_DISC_BURN)
	{
		DiscItem* item = static_cast<DiscItem*>(pBaseItem);
		pTreeItem->setText(0, QString("%1(%2)").arg(item->sn).arg(item->drive));
		pTreeItem->setData(0, Qt::UserRole, item->id);
		QString strSize = QString("%1").arg(FormatSize(item->total_size));
		pTreeItem->setText(1, strSize);
// 		QProgressBar* bar = (QProgressBar*)ui.treeWidget_Task->itemWidget(pTreeItem, 2);
// 		bar->setValue(item->progress);
		pTreeItem->setText(2, item->status_string);
	}
	else if (pBaseItem->m_eType == ITEM_UPLOAD)
	{
		UploadItem* item = static_cast<UploadItem*>(pBaseItem);
		pTreeItem->setText(0, "upload file");
		pTreeItem->setData(0, Qt::UserRole, item->id);
		QString strSize = QString("%1").arg(FormatSize(item->total_size));
		pTreeItem->setText(1, strSize);

// 		QProgressBar* bar = (QProgressBar*)ui.treeWidget_Task->itemWidget(pTreeItem, 2);
// 		bar->setValue(item->progress);
		pTreeItem->setText(2, item->status_string);
	}
	else if (pBaseItem->m_eType == ITEM_DOWNLOAD)
	{
		UploadItem* item = static_cast<UploadItem*>(pBaseItem);
		pTreeItem->setText(0, "download file");
		pTreeItem->setData(0, Qt::UserRole, item->id);
		QString strSize = QString("%1").arg(FormatSize(item->total_size));
		pTreeItem->setText(1, strSize);

		// 		QProgressBar* bar = (QProgressBar*)ui.treeWidget_Task->itemWidget(pTreeItem, 2);
		// 		bar->setValue(item->progress);
		pTreeItem->setText(2, item->status_string);
	}
	return;
}
void PMYBurnSdkDemo::NotifyTask(const std::string& strResponse)
{
	QString strJson = QString::fromStdString(strResponse);
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(strJson.toStdString().data(), &json_error);
	if(json_error.error != QJsonParseError::NoError) 
	{
		QMessageBox::warning(this, "error", QString("invalid response. err: %1").arg(json_error.errorString()));
		return;
	}
	QJsonObject obj = parse_doucment.object();
	int error_code = obj.value("error_code").toInt(0);
	QString error_msg = obj.value("error_msg").toString("");
	if (error_code != 0)
	{
		QMessageBox::warning(this, "error", QString("query fail. err: %1 %2").arg(error_code).arg(error_msg));
		return;
	}

	QJsonArray tasks = obj.value("tasks").toArray();
	for (int i=0; i<tasks.size(); i++)
	{
		QJsonValue value = tasks[i];
		QJsonObject obj = value.toObject();
		QString taskId = obj.value("id").toString();
		TaskItem* pTaskItem = GetTaskItem(taskId);
		if (pTaskItem == NULL)
		{
			pTaskItem = new TaskItem;
			pTaskItem->id = taskId;
			m_mapBurnTaskItem[taskId] = pTaskItem;
		}

		pTaskItem->type = obj.value("type").toInt();
		pTaskItem->name = obj.value("name").toString();
		pTaskItem->is_finish = obj.value("is_finish").toBool();
		pTaskItem->status_code = obj.value("status_code").toInt(0);
		pTaskItem->status_string = obj.value("status_string").toString("");
		pTaskItem->total_disc = obj.value("total_disc").toInt(0);
		pTaskItem->success_disc = obj.value("success_disc").toInt(0);
		pTaskItem->fail_disc = obj.value("fail_disc").toInt(0);
		pTaskItem->total_size = obj.value("total_size").toDouble();

		if (pTaskItem->is_finish)
		{
			CloseTask(taskId);
			FinishTask(taskId);
			continue;
		}

		UpdateTaskItem(pTaskItem);

		if (obj.value("burn").isObject())
		{
			QJsonObject burn = obj.value("burn").toObject();
			if (burn.value("discs").isArray())
			{
				QJsonArray discs = burn.value("discs").toArray();
				for (int i=0; i<discs.size(); i++)
				{
					QJsonValue value = discs[i];
					QJsonObject disc = value.toObject();

					QString id = disc.value("id").toString();
					BaseItem* pBaseItem = GetChildItem(pTaskItem->id, id);
					DiscItem* pChildItem = static_cast<DiscItem*>(pBaseItem);
					if (pChildItem == NULL)
					{
						pChildItem = new DiscItem;
						InsertChildItem(taskId, id, pChildItem);
					}
					pChildItem->Update(disc);
					UpdateChildItem(taskId, pChildItem);
				}
			}
		}

		if (obj.value("file_upload").isObject())
		{
			QJsonObject file_upload = obj.value("file_upload").toObject();
			QString id = file_upload.value("id").toString();
			BaseItem* pBaseItem = GetChildItem(pTaskItem->id, id);
			UploadItem* pChildItem = static_cast<UploadItem*>(pBaseItem);
			if (pChildItem == NULL)
			{
				pChildItem = new UploadItem(ITEM_DOWNLOAD);
				InsertChildItem(taskId, id, pChildItem);
			}
			pChildItem->Update(file_upload);
			UpdateChildItem(taskId, pChildItem);
		}
	}
}

void PMYBurnSdkDemo::GetPrintStatus()
{

}

void PMYBurnSdkDemo::FinishTask(QString strTaskId)
{
	TaskItem* pTaskItem = GetTaskItem(strTaskId);

	int nIndex = ui.treeWidget_Task->indexOfTopLevelItem(pTaskItem->m_Item);
	if (nIndex != -1)
	{
		ui.treeWidget_Task->takeTopLevelItem(nIndex);
	}

	{
		QMutexLocker lock(&m_WorkThreadMutex);
		QMap<QString, QDataThread*>::iterator it = m_pVtWorkThread.find(strTaskId);
		if (it != m_pVtWorkThread.end())
		{
			it.value()->SetStop();
		}
	}
	
	QMutexLocker lock(&m_BurnTaskItemMutex);

	QMap<QString, TaskItem*>::iterator it = m_mapBurnTaskItem.find(strTaskId);
	if (it != m_mapBurnTaskItem.end())
	{
		if (it.value())
		{
			delete it.value();
		}
		m_mapBurnTaskItem.erase(it);
	}
	return;
}

void PMYBurnSdkDemo::CloseTask(QString strTaskId)
{
	PMYTaskHandle TaskHandle = GetTaskHandle(strTaskId);
	if (TaskHandle == NULL)
		return;
	SDKFunc()._PMYBS_CloseTask(TaskHandle);
}

void PMYBurnSdkDemo::on_pushButton_GetTaskCfg_clicked()
{
// 	QTreeWidgetItem * item = ui.treeWidget_Task->currentItem();
//     if (item == NULL)
// 	{
// 		return;
// 	}
//     if (item->parent() != NULL)
// 	{
// 		return;
// 	}
// 	QString strTaskId = item->data(0, Qt::UserRole).toString();
// 	TASK_CONFIG tTaskConfig;
// 	int nRet = SDKFunc()._PMYBS_GetTaskConfig(strTaskId.toStdString().c_str(), &tTaskConfig);
// 	if (nRet != 0)
// 	{
// 		QMessageBox::warning(this, "error", QString("_PMYBS_GetTaskConfig fail. err: %1").arg(nRet));
// 		return;
// 	}
// 
// 	QMessageBox::information(this, "info", "_PMYBS_GetTaskConfig success.");

// 
//     char* Buffer = NULL;
// 	int nBufferLen = 0;
// 	char szTaskId[100] = {0};
// 	int nGroupIdLen = 100;
//     int nRet = GET_FUNC(PMYBURN_GetTaskId)(strTaskRunId.toStdString().c_str(), szTaskId, &nGroupIdLen);
// 	if (nRet != 0)
// 	{
// 		QMessageBox::warning(this, "error", QString("PMYBURN_GetTaskId fail. error code:%1").arg(nRet));
// 		return;
// 	}
//     nRet = GET_FUNC(PMYBURN_GetTaskConfigXml)(szTaskId, Buffer, &nBufferLen);
// 	if (nRet != PMYBURN_SDK_OUTPUT_BUFFER_SMALL)
// 	{
// 		QMessageBox::warning(this, "error", QString("GetTaskConfigXml fail. error code:%1").arg(nRet));
// 		return;
// 	}
// 	Buffer = new char[nBufferLen];
// 	memset(Buffer, 0, nBufferLen);
//     nRet = GET_FUNC(PMYBURN_GetTaskConfigXml)(szTaskId, Buffer, &nBufferLen);
// 	if (nRet == 0)
// 	{
// 		QString strXml = QString::fromUtf8(Buffer);
// 		QMessageBox::information(this, "info", QString("GetTaskConfigXml success:\n\n%1").arg(strXml));
// 	}
// 	else
// 	{
// 		QMessageBox::warning(this, "error", QString("GetTaskConfigXml fail. error code:%1").arg(nRet));
// 	}
// 	delete[]Buffer;
}

void PMYBurnSdkDemo::on_pushButton_GetTaskStatus_clicked()
{
	if (!m_bOnLine)
		return;
	QTreeWidgetItem * item = ui.treeWidget_Task->currentItem();
    if (item == nullptr)
	{
		return;
	}
    if (item->parent() != nullptr)
	{
		return;
	}
	QString strTaskId = item->data(0, Qt::UserRole).toString();

	int nLen = 0;
	char* pBuff = new char[nLen];
	int nRet = SDKFunc()._PMYBS_GetTaskStatus(m_pLoginHandle, strTaskId.toStdString().c_str(), pBuff, &nLen);
	if (nRet == PMYBS_SDK_OUTPUT_BUFFER_SMALL)
	{
		delete [] pBuff;
		pBuff = new char[nLen];
		nRet = SDKFunc()._PMYBS_GetTaskStatus(m_pLoginHandle, strTaskId.toStdString().c_str(), pBuff, &nLen);
	}
	if (nRet != 0)
	{
		QMessageBox::warning(this, "error", QString("_PMYBS_GetTaskStatus fail. err: %1").arg(nRet));
		delete [] pBuff;
		return;
	}

	QString strText = QString::fromUtf8(pBuff);
	QMessageBox::information(this, "info", QString("_PMYBS_GetTaskStatus success:\n\n%1").arg(strText));
	delete [] pBuff;
}

void PMYBurnSdkDemo::sltExecCmd(QString name, int cmd)
{
    int nRet = SDKFunc()._PMYBS_ExecuteRobotCmd(m_pLoginHandle, name.toStdString().c_str(), cmd);

    if (nRet != 0)
    {
        QMessageBox::warning(this, "error", QString("_PMYBS_ExecuteRobotCmd fail. err: %1").arg(nRet));
        return;
    }

    QMessageBox::information(this, "info", QString("_PMYBS_ExecuteRobotCmd success"));

}

QString PMYBurnSdkDemo::GetConfig(const QString& strKey)
{
	int nLen = 0;
	char* pBuff = new char[nLen];
	int nRet = SDKFunc()._PMYBS_GetConfig(m_pLoginHandle, strKey.toStdString().c_str(), pBuff, &nLen);
	if (nRet == PMYBS_SDK_OUTPUT_BUFFER_SMALL)
	{
		delete [] pBuff;
		pBuff = new char[nLen];
		nRet = SDKFunc()._PMYBS_GetConfig(m_pLoginHandle, strKey.toStdString().c_str(), pBuff, &nLen);
	}
	if (nRet != 0)
	{
		if (!m_bFirst)
			QMessageBox::warning(this, "error", QString("_PMYBS_GetConfig fail. strKey: %1, err: %2").arg(strKey).arg(nRet));
		delete [] pBuff;
		return "";
	}
	return QString(pBuff);
}

void PMYBurnSdkDemo::sltTimeOut()
{
	static bool bClear = false;
	m_bOnLine = SDKFunc()._PMYBS_CheckSvcIsOnline(m_pLoginHandle);
	if (m_bOnLine)
	{
		QString strStatus = "状态：在线";
		if (!m_strVersion.isEmpty())
		{
			strStatus = QString("状态：在线, 版本：%1").arg(m_strVersion);
		}
		m_bFirst = true;
		QString strTmp = GetConfig(PMYBS_SDK_CFG_PROC_CPU);
		if (strTmp.isEmpty() == false)
		{
			strStatus += QString(", cpu: %1").arg(strTmp);
		}

		strTmp = GetConfig(PMYBS_SDK_CFG_PROC_MEMORY);
		if (strTmp.isEmpty() == false)
		{
			strStatus += QString(", 内存: %1 KB").arg(strTmp);
		}

		strTmp = GetConfig(PMYBS_SDK_CFG_TOTAL_CPU);
		if (strTmp.isEmpty() == false)
		{
			strStatus += QString(", 总cpu: %1").arg(strTmp);
		}

		strTmp = GetConfig(PMYBS_SDK_CFG_TOTAL_MEMORY);
		if (strTmp.isEmpty() == false)
		{
			strStatus += QString(", 总内存: %1").arg(strTmp);
		}

		strTmp = GetConfig(PMYBS_SDK_CFG_CACHE_TOTAL_SIZE);
		if (strTmp.isEmpty() == false)
		{
			strStatus += QString(", 总大小: %1 MB").arg(strTmp);
		}

		strTmp = GetConfig(PMYBS_SDK_CFG_CACHE_FREE_SIZE);
		if (strTmp.isEmpty() == false)
		{
			strStatus += QString(", 剩余: %1 MB").arg(strTmp);
		}
		m_bFirst = false;

		ui.label_status->setText(strStatus);
		if (bClear)
		{
			ClearTaskWidget();
			m_bFirst = true;
			on_pushButton_QueryTask_clicked();
			m_bFirst = false;
			bClear = false;
		}
	}
	else
	{
		bClear = true;
		ui.label_status->setText("状态：离线");
	}
}

void PMYBurnSdkDemo::sltFinishData(QString strTaskId, int nErr)
{
	if (nErr != 0)
	{
		QMessageBox::warning(this, "error", QString("实时取流失败. strTaskId: %1, err: %2").arg(strTaskId).arg(nErr));
	}
	QMutexLocker lock(&m_WorkThreadMutex);
	QMap<QString, QDataThread*>::iterator it = m_pVtWorkThread.find(strTaskId);
	if (it != m_pVtWorkThread.end())
	{
		m_pVtWorkThread.erase(it);
	}
}

void PMYBurnSdkDemo::on_pushButton_RunPtfEdit_clicked()
{
    QString strExe;
 #ifdef Q_OS_WIN32
    strExe = QCoreApplication::applicationDirPath() + "/PTFEditorUI/PTFEditorUI.exe";
 #elif defined (Q_OS_LINUX)
    strExe = QCoreApplication::applicationDirPath() + "/PTFEditorUI/PTFEditorUI";
 #endif

    QProcess::startDetached(strExe);

////	QString path = QDir::currentPath();
//    QString str = QCoreApplication::applicationDirPath() + "/PTFEditorUI/PTFEditorUI";
////	QDir::setCurrent(QCoreApplication::applicationDirPath() + "/PTFEditorUI");

//    QProcess process;
//    process.setWorkingDirectory( "./PTFEditorUI" );
//#ifdef Q_OS_WIN32
//    strExe = QCoreApplication::applicationDirPath() + "/PTFEditorUI/PTFEditorUI.exe";
//	QProcess::startDetached("PTFEditorUI.exe");
//#elif defined (Q_OS_LINUX)
//    QProcess::startDetached(strExe);
//   // process.startDetached( "/run/media/root/linux_data/code/PMYBurnSvc/debug/PTFEditorUI/PTFEditorUI" );
//#endif
////	QDir::setCurrent(path);
}

void PMYBurnSdkDemo::on_pushButton_OpenLog_clicked()
{
	QTreeWidgetItem * item = ui.treeWidget_Task->currentItem();
    if (item == NULL)
	{
		return;
	}
    if (item->parent() != NULL)
	{
		return;
	}
	QString strTaskRunId = item->data(0, Qt::UserRole).toString();
	QString strFile = QCoreApplication::applicationDirPath() + "/Log/" + strTaskRunId + ".log";
	QFile file(strFile);
	if (file.exists()) 
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile(strFile));
	}
	else
	{
		QMessageBox::warning(this, "error", QString("open log file fail. \n%1").arg(strFile));
	}
}

// void PMYBurnSdkDemo::sltGenerateData(QString strTaskRunId, quint64 u64GenSize, quint64 u64TotalSize, int nPos)
// {
// 	PTASK_TREE_ITEM pChildItem = GetChildItem(strTaskRunId, "download", ITEM_DOWNLOAD);
//     if (pChildItem == NULL)
// 	{
// 		pChildItem = new TASK_TREE_ITEM;
// 		pChildItem->strName = "下载数据流";
// 		pChildItem->strId = "download";
// 		pChildItem->eItemType = ITEM_DOWNLOAD;
// 		InsertChildItem(strTaskRunId, "download", pChildItem, ITEM_DOWNLOAD);
// 	}
// 	pChildItem->nDiscSpaceUsePos = nPos;
// 	pChildItem->u64DiscUseSpace = u64GenSize;
// 	pChildItem->u64DiscTotalSpace = u64TotalSize;
// 	UpdateChildItem(strTaskRunId, pChildItem);
// }

bool PMYBurnSdkDemo::eventFilter(QObject *obj, QEvent *event)
{
// 	if (obj == ui.widget_Device)
// 	{
// 		if (event->type() == QEvent::ToolTip)
// 		{
// 			if (m_strDeviceTooltip.isEmpty() == false)
// 			{
// 				QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);  
// 				QToolTip::showText(helpEvent->globalPos(), m_strDeviceTooltip);  
// 				return true;  
// 			}
// 		}
// 	}
	return QWidget::eventFilter(obj, event);
}

void PMYBurnSdkDemo::UpdateDriverStatus(QString strXml)
{
// 	QDomDocument doc;
// 	if (!doc.setContent(strXml))
// 	{
// 		return;
// 	}
// 
// 	//这里暂时只展示第一个设备
// 	QDomElement ContentElem = doc.documentElement();
// 	QDomElement RobotElem = ContentElem.firstChildElement("Robot");
// 	while (!RobotElem.isNull())
// 	{
// 		if (0 == m_strDeviceDes.compare(RobotElem.attribute("RobotDesc"), Qt::CaseInsensitive))
// 		{
// 			QDomElement DriveElem = RobotElem.firstChildElement("Drive");
// 			while (!DriveElem.isNull())
// 			{
// 				QChar qDrive = '0';
// 				QString strDrive = DriveElem.attribute("Letter", "");
// 				if (!strDrive.isEmpty())
// 					qDrive = strDrive.at(0);
// 				if ('0' != qDrive)
// 				{
// 					PDRIVE_TREE_ITEM pDrive = GetDriveItem(qDrive);
// 					if (!pDrive)
// 					{
// 						pDrive = new DRIVE_TREE_ITEM;
// 						pDrive->cLetter = qDrive;
// 						InsertDriveItem(qDrive, pDrive);
// 					}
// 
// 					pDrive->bHaveDisc = (bool)DriveElem.attribute("HaveDisc", "0").toInt();
// 					pDrive->eMediaType = (MEDIA_PHYSICAL_TYPE)DriveElem.attribute("MediaType", "0").toInt();
// 					pDrive->strDisplayName = DriveElem.attribute("DisplayName", "");
// 					pDrive->nDrivePos = DriveElem.attribute("nDrivePos", "0").toInt();
// 					pDrive->strLabel = DriveElem.attribute("Label", "");
// 					pDrive->bUsing = (bool)DriveElem.attribute("Burning", "0").toInt();
// 					pDrive->strStatus = DriveElem.attribute("Status", "");
// 					pDrive->nProgress = DriveElem.attribute("Progress", "0").toInt();
// 					pDrive->ullDiscTotalSpace = DriveElem.attribute("DiscTotalSpace", "0").toULongLong();
// 					pDrive->ullDiscUseSpace = DriveElem.attribute("DiscUseSpace", "0").toULongLong();
// 					pDrive->strDiscId = DriveElem.attribute("DiscId", "");
// 					pDrive->strTaskName = DriveElem.attribute("TaskName", "");
// 					pDrive->ullTotalBurnSize = DriveElem.attribute("BurnSize", "0").toULongLong();
// 					pDrive->nDiscSpaceUsePos = DriveElem.attribute("DiscSpaceUsePos", "0").toInt();
// 					UpdateDriveItem(pDrive);
// 
// 					UpdateDriveWidget(DriveElem);
// 				}
// 
// 				DriveElem = DriveElem.nextSiblingElement("Drive");
// 			}
// 			break;
// 		}
// 		RobotElem = RobotElem.nextSiblingElement("Robot");
// 	}
}
// 
// PDRIVE_TREE_ITEM PMYBurnSdkDemo::GetDriveItem(QChar cLetter)
// {
// 	m_DriveItemMutex.lock();
// 	QMap<QChar, PDRIVE_TREE_ITEM>::iterator it = m_mapDriveItem.find(cLetter);
// 	if (it == m_mapDriveItem.end())
// 	{
// 		m_DriveItemMutex.unlock();
// 		return NULL;
// 	}
// 	PDRIVE_TREE_ITEM DriveItem = it.value();
// 	m_DriveItemMutex.unlock();
// 	return DriveItem;
// }
// 
// void PMYBurnSdkDemo::InsertDriveItem(QChar cLetter, PDRIVE_TREE_ITEM pItem)
// {
// 	m_DriveItemMutex.lock();
// 	m_mapDriveItem[cLetter] = pItem;
// 	m_DriveItemMutex.unlock();
// 	return;
// }

// void PMYBurnSdkDemo::UpdateDriveItem(PDRIVE_TREE_ITEM pDrive)
// {
// 	QTreeWidgetItem* Item = pDrive->DriveItem;
// 	if (pDrive->DriveItem == NULL)
// 	{
// 		Item = new QTreeWidgetItem(QStringList(pDrive->cLetter));
// 
// 		Item->setData(0, Qt::UserRole, pDrive->cLetter);
// 		ui.treeWidget_Driver->addTopLevelItem(Item);
// 		Item->setExpanded(true);
// 		pDrive->DriveItem = Item;
// 	}
// 	
// 	Item->setText(1, pDrive->strDisplayName);
// 	Item->setText(2, pDrive->bHaveDisc?"是":"否");
// 	Item->setText(3, GetDiscTypeTxtByEm(pDrive->eMediaType));
// 	Item->setText(4, pDrive->nDrivePos==0?"上光驱":"下光驱");
// 	Item->setText(5, pDrive->strLabel);
// 	Item->setText(6, pDrive->bUsing?"是":"否");
// 	Item->setText(7, pDrive->strTaskName);
// 	Item->setText(8, pDrive->strDiscId);
// 	Item->setText(9, QString("%1").arg(FormatSize(pDrive->ullTotalBurnSize)));
// 	Item->setText(10, QString().asprintf("%d%", pDrive->nProgress));
// 	Item->setText(11, QString("%1").arg(FormatSize(pDrive->ullDiscTotalSpace)));
// 	Item->setText(12, QString("%1").arg(FormatSize(pDrive->ullDiscUseSpace)));
// 	Item->setText(13, QString().asprintf("%d%", pDrive->nDiscSpaceUsePos));
// 	Item->setText(14, pDrive->strStatus);
//}

// void PMYBurnSdkDemo::UpdateDriveWidget(QDomElement DriveElement)
// {
//     QRecorderWidget* ItemWidget = NULL;
// 	QChar cLetter = DriveElement.attribute("Letter").toUpper().at(0);
// 	QMap<QChar, QRecorderWidget*>::iterator it = m_mapRecordWidget.find(cLetter);
// 	if (it == m_mapRecordWidget.end())
// 	{
// 		ItemWidget = new QRecorderWidget(false, this);
// 		ui.verticalLayout_Recorder->addWidget(ItemWidget);
// 		m_mapRecordWidget[cLetter] = ItemWidget;
// 	}
// 	else
// 	{
// 		ItemWidget = it.value();
// 	}
// 
// 	ItemWidget->UpdateRecorder(DriveElement);
// }

bool PMYBurnSdkDemo::AddFileToDiscTree(QString strFilePath)
{	
	QFileInfo fileInfo(strFilePath);
	QTreeWidgetItem* FileItem = new QTreeWidgetItem();
	FileItem->setText(0, fileInfo.fileName());
	FileItem->setText(1, QString::number(fileInfo.size()));
	ui.treeWidget_Task->addTopLevelItem(FileItem);
	return true;
}

void PMYBurnSdkDemo::ClearDeviceWidget()
{
	QMap<QString, QRobotDeviceWidget*>::iterator it = m_mapDeviceWidget.begin();
	for (it; it != m_mapDeviceWidget.end(); it++)
	{
		ui.verticalLayout_device->removeWidget(it.value());
		delete it.value();
	}
	m_mapDeviceWidget.clear();
}

PMYTaskHandle PMYBurnSdkDemo::GetTaskHandle(QString strTaskId)
{
	QMutexLocker lock(&m_BurnTaskMutex);
	QMap<QString, PMYTaskHandle>::iterator it = m_mapBurnTask.find(strTaskId);
	if (it == m_mapBurnTask.end())
	{
		return NULL;
	}
	return it.value();
}

void PMYBurnSdkDemo::AddTaskHandle(QString strTaskId, PMYTaskHandle taskHandle)
{
	QMutexLocker lock(&m_BurnTaskMutex);
	m_mapBurnTask[strTaskId] = taskHandle;
}

void DiscItem::Update(QJsonObject &obj)
{
    id = obj.value("id").toString();
    sn = obj.value("sn").toString();
    disc_index = obj.value("disc_index").toInt();
    copy_index = obj.value("copy_index").toInt();
    label = obj.value("label").toString();
    status_code = obj.value("status_code").toInt();
    status_string = obj.value("status_string").toString();
    stream_burn = obj.value("stream_burn").toBool();
    total_size = obj.value("total_size").toDouble();
    burn_size = obj.value("burn_size").toDouble();
    progress = obj.value("progress").toInt();
    drive = obj.value("drive").toString();
}

void UploadItem::Update(QJsonObject &obj)
{
    id = obj.value("id").toString();
    status_string = obj.value("status_string").toString("");
    status_code = obj.value("status_code").toInt(0);
    total_count = obj.value("total_count").toInt(0);
    total_size = obj.value("total_size").toDouble(0);
    upload_size = obj.value("upload_size").toDouble(0);
    cur_file = obj.value("cur_file").toString("");
    cur_index = obj.value("cur_index").toInt(-1);
    cur_progress = obj.value("cur_progress").toInt(0);
    progress = obj.value("progress").toInt(0);
}
