#include "stdafx.h"
#include "qnewtaskdialog.h"
#include <QMessageBox>
#include "PMYBurnPublic.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include "QPrintValueWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)  
#pragma execution_character_set("utf-8")  
#endif

QString getHostIp()
{
	QString IP;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	for (int i = 0; i < ipAddressesList.size(); ++i)
	{
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address())
		{
			IP = ipAddressesList.at(i).toString();
			break;
		}
	}
	if (IP.isEmpty())
		IP = QHostAddress(QHostAddress::LocalHost).toString();
	return IP;
}

#ifdef WIN32
char GetFreestDisk()
{
	DWORD dwDrivers = GetLogicalDrives();
	ULARGE_INTEGER liMax;
	liMax.QuadPart = 0;
	char cRet = 0;
	for (int i = 0 ; i<26 ; i++)
	{
		if ((dwDrivers & (1<<i))!=0)
		{
			char szDrive[MAX_PATH] = "c:";
			szDrive[0] = 'A' + i;

			if (DRIVE_FIXED == GetDriveTypeA(szDrive))
			{
				ULARGE_INTEGER   uiFreeBytesAvailableToCaller;  
				if (GetDiskFreeSpaceExA(szDrive, &uiFreeBytesAvailableToCaller, NULL, NULL))
				{
					if (uiFreeBytesAvailableToCaller.QuadPart > liMax.QuadPart)
					{
						liMax = uiFreeBytesAvailableToCaller;
						cRet = szDrive[0];
					}
				}
			}		
		}
	}
	return cRet;
}
#endif

QNewTaskDialog::QNewTaskDialog(PMYLoginHandle loginHandle, QWidget *parent /* = 0 */)
	: QDialog(parent)
	, m_LoginHandle(loginHandle)
	, m_TaskHandle(NULL)
	, m_bJsonSubmitTask(false)
	, m_nTotalMB(0)
	, m_nMBPreFile(0)
	, m_nMins(0)
{
	ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
// 	connect(ui.comboBox_BurnType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboxTurn_changed(int)));

	QStringList environment = QProcess::systemEnvironment();  
	ui.lineEdit_Name->setText(QString("%1").arg(QDateTime::currentDateTime().toString("yyMMddHHmmss")));

	ui.comboBox_TaskType->addItem("文件刻录", TASK_TYPE_FILE_BURN);
	ui.comboBox_TaskType->addItem("光盘打印", TASK_TYPE_DISC_PRINT);
	ui.comboBox_TaskType->addItem("实时文件刻录", TASK_TYPE_REAL_FILE_BURN);
	ui.comboBox_TaskType->setCurrentIndex(2);

	ui.comboBox_DiscOutputMode->addItem(QStringLiteral("默认输出"), (int)DISC_OUTPUT_MODE_DEFAULT);
	ui.comboBox_DiscOutputMode->addItem(QStringLiteral("前置输出"), (int)DISC_OUTPUT_MODE_FRONT);

	InitDiscType();
	InitValueTable();
	InitBurnSpeed(ui.comboBox_DiscType->currentData().toInt());
	sltTaskTypeChange(TASK_TYPE_REAL_FILE_BURN);

	connect(ui.comboBox_DiscType, SIGNAL(currentIndexChanged(int)), this, SLOT(sltDiscTypeChange(int)));
	connect(ui.comboBox_TaskType, SIGNAL(currentIndexChanged(int)), this, SLOT(sltTaskTypeChange(int)));

#ifdef WIN32
	ui.lineEdit_SuccessCoverFile->setText("E:/ok.PTF");
	ui.lineEdit_FailCoverFile->setText("E:/fail.PTF");
	ui.lineEdit_cache->setText("F:/XinKeCacheFile");
	//ui.lineEdit_cache->setText(QString("%1:/XinKeCacheFile").arg(GetFreestDisk()));
#else
//	ui.lineEdit_SuccessCoverFile->setText("/home/pmy/code/PMYBurnSvc/bin/gcc-6.3.0/debug/testData/test.PTF");
//	ui.lineEdit_FailCoverFile->setText("/home/pmy/code/PMYBurnSvc/bin/gcc-6.3.0/debug/testData/code.bmp");
#endif

	//ui.checkBox_OnlyBurnIso->hide();
	//ui.checkBox_StreamBurn->hide();
	//ui.checkBox_DeleteFile->hide();
	ui.checkBox_CloseDisc->setChecked(true);
	//ui.checkBox_TestBurn->setChecked(true);
}

QNewTaskDialog::~QNewTaskDialog()
{

}

void QNewTaskDialog::sltDiscTypeChange(int nIndex)
{
	//int eDiscType = ui.comboBox_DiscType->currentData().toInt();
	InitBurnSpeed(nIndex);
	return;
}


void QNewTaskDialog::sltTaskTypeChange(int nIndex)
{
	int eTaskType = ui.comboBox_TaskType->currentData().toInt();

	ui.pushButton_AddFile->setEnabled(true);
	ui.pushButton_AddFolder->setEnabled(true);
	ui.pushButton_Add_img->setEnabled(true);
	ui.pushButton_DeleteFile->setEnabled(true);
	ui.label_14->setEnabled(true);
	ui.lineEdit_FailCoverFile->setEnabled(true);
	ui.pushButton_FailCoverFile->setEnabled(true);
	ui.checkBox_SupportNotNullDisc->setEnabled(true);
	ui.checkBox_TestBurn->setEnabled(true);
	ui.checkBox_DeleteFile->setEnabled(true);
	ui.checkBox_VNRWrite->setEnabled(true);
	ui.checkBox_CloseDisc->setEnabled(true);
	ui.checkBox_OnlyBurnIso->setEnabled(true);
	ui.checkBox_VerifyDisc->setEnabled(true);
	ui.checkBox_StreamBurn->setEnabled(true);
	ui.lineEdit_DiscLabel->setEnabled(true);
	ui.comboBox_DiscType->setEnabled(true);
	ui.comboBox_BurnSpeed->setEnabled(true);
	ui.checkBox_StreamBurn->setChecked(false);

	switch (eTaskType)
	{
	case TASK_TYPE_FILE_BURN:
		{
			on_checkBox_OnlyBurnIso_stateChanged(0);
			ui.stackedWidget->setCurrentIndex(0);
			ui.checkBox_StreamBurn->setEnabled(false);
		}
		break;
	case TASK_TYPE_DISC_PRINT:
		{
			ui.stackedWidget->setCurrentIndex(0);
			ui.pushButton_AddFile->setEnabled(false);
			ui.pushButton_AddFolder->setEnabled(false);
			ui.pushButton_Add_img->setEnabled(false);
			ui.pushButton_DeleteFile->setEnabled(false);
			ui.label_14->setEnabled(false);
			ui.lineEdit_FailCoverFile->setEnabled(false);
			ui.pushButton_FailCoverFile->setEnabled(false);
			ui.checkBox_SupportNotNullDisc->setEnabled(false);
			ui.checkBox_TestBurn->setEnabled(false);
			ui.checkBox_DeleteFile->setEnabled(false);
			ui.checkBox_VNRWrite->setEnabled(false);
			ui.checkBox_CloseDisc->setEnabled(false);
			ui.checkBox_OnlyBurnIso->setEnabled(false);
			ui.checkBox_VerifyDisc->setEnabled(false);
			ui.checkBox_StreamBurn->setEnabled(false);
			ui.lineEdit_DiscLabel->setEnabled(false);
			ui.comboBox_DiscType->setEnabled(false);
			ui.comboBox_BurnSpeed->setEnabled(false);
			ui.lineEdit_error_retry->setEnabled(false);
		}
		break;
	case TASK_TYPE_REAL_FILE_BURN:
		{
			ui.checkBox_SupportNotNullDisc->setEnabled(false);
			ui.checkBox_OnlyBurnIso->setEnabled(false);
			ui.stackedWidget->setCurrentIndex(1);
			ui.checkBox_CloseDisc->setChecked(true);
			ui.checkBox_StreamBurn->setChecked(true);
		}
		break;
	}

}

void QNewTaskDialog::on_checkBox_OnlyBurnIso_stateChanged(int nStatus)
{
	if (ui.checkBox_OnlyBurnIso->isChecked())
	{
		ui.pushButton_AddFile->setEnabled(false);
		ui.pushButton_AddFolder->setEnabled(false);
		ui.pushButton_Add_img->setEnabled(true);
	}
	else
	{
		ui.pushButton_AddFile->setEnabled(true);
		ui.pushButton_AddFolder->setEnabled(true);
		ui.pushButton_Add_img->setEnabled(false);
	}
}

void QNewTaskDialog::on_pushButton_cache_clicked()
{
	QString directory = QFileDialog::getExistingDirectory(this, "请选择缓存目录");
	if (directory.isEmpty())
	{	
		return;
	}
	ui.lineEdit_cache->setText(directory);
}

void QNewTaskDialog::InitDiscType()
{
	ui.comboBox_DiscType->addItem("CD 700MB", PMY_DISC_CD_700M);
	ui.comboBox_DiscType->addItem("DVD 4.7GB", PMY_DISC_DVD_4_7G);
	ui.comboBox_DiscType->addItem("DVD DL 8.5GB", PMY_DISC_DVD_DL_9_4G);
	ui.comboBox_DiscType->addItem("BD 25GB", PMY_DISC_BD_25G);
	ui.comboBox_DiscType->addItem("BD DL 50GB", PMY_DISC_BD_50G);
	ui.comboBox_DiscType->addItem("BD TL 100GB", PMY_DISC_BD_100G);
	ui.comboBox_DiscType->addItem("BD QL 128GB", PMY_DISC_BD_128G);

	ui.comboBox_DiscType->setCurrentIndex(1);

}

void QNewTaskDialog::InitBurnSpeed(int eDiscType)
{
	ui.comboBox_BurnSpeed->clear();
	ui.comboBox_BurnSpeed->addItem("Auto", 0);
	switch (eDiscType)
	{
	case PMY_DISC_CD_700M:
		{
			ui.comboBox_BurnSpeed->addItem("52x", 52);
			ui.comboBox_BurnSpeed->addItem("48x", 48);
			ui.comboBox_BurnSpeed->addItem("40x", 40);
			ui.comboBox_BurnSpeed->addItem("32x", 32);
			ui.comboBox_BurnSpeed->addItem("24x", 24);
			ui.comboBox_BurnSpeed->addItem("20x", 20);
			ui.comboBox_BurnSpeed->addItem("16x", 16);
			ui.comboBox_BurnSpeed->addItem("12x", 12);
			ui.comboBox_BurnSpeed->addItem("8x", 8);
			ui.comboBox_BurnSpeed->addItem("4x", 4);
			ui.comboBox_BurnSpeed->addItem("2x", 2);
			ui.comboBox_BurnSpeed->addItem("1x", 1);
			break;
		}
	case PMY_DISC_DVD_DL_9_4G:
	case PMY_DISC_DVD_4_7G:
		{
			ui.comboBox_BurnSpeed->addItem("24x", 24);
			ui.comboBox_BurnSpeed->addItem("22x", 22);
			ui.comboBox_BurnSpeed->addItem("20x", 20);
			ui.comboBox_BurnSpeed->addItem("18x", 18);
			ui.comboBox_BurnSpeed->addItem("16x", 16);
			ui.comboBox_BurnSpeed->addItem("12x", 12);
			ui.comboBox_BurnSpeed->addItem("8x", 8);
			ui.comboBox_BurnSpeed->addItem("6x", 6);
			ui.comboBox_BurnSpeed->addItem("4x", 4);
			ui.comboBox_BurnSpeed->addItem("2x", 2);
			ui.comboBox_BurnSpeed->addItem("1x", 1);
			break;
		}
	case PMY_DISC_BD_25G:
	case PMY_DISC_BD_50G:
	case PMY_DISC_BD_100G:
	case PMY_DISC_BD_128G:
		{
			ui.comboBox_BurnSpeed->addItem("16x", 16);
			ui.comboBox_BurnSpeed->addItem("14x", 14);
			ui.comboBox_BurnSpeed->addItem("12x", 12);
			ui.comboBox_BurnSpeed->addItem("10x", 10);
			ui.comboBox_BurnSpeed->addItem("8x", 8);
			ui.comboBox_BurnSpeed->addItem("6x", 6);
			ui.comboBox_BurnSpeed->addItem("4x", 4);
			ui.comboBox_BurnSpeed->addItem("2x", 2);
			ui.comboBox_BurnSpeed->addItem("1x", 1);
			break;
		}
	}
	ui.comboBox_BurnSpeed->setCurrentIndex(0);

	return;
}

void QNewTaskDialog::InitValueTable()
{
	for (int i=0; i<15; i++)
	{
		int row_count = ui.tableWidget_Value->rowCount(); 
		ui.tableWidget_Value->insertRow(row_count);

		QTableWidgetItem *item = new QTableWidgetItem(QString("字段%1").arg(i + 1));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		ui.tableWidget_Value->setItem(row_count, 0, item);

		QPrintValueWidget *comBox = new QPrintValueWidget(ui.tableWidget_Value, i + 1);
		ui.tableWidget_Value->setRowHeight(row_count, 25);
		ui.tableWidget_Value->setCellWidget(row_count, 1, comBox);
	}
}


void QNewTaskDialog::on_buttonBox_rejected()
{
	close();
}

void QNewTaskDialog::on_buttonBox_accepted()
{
	m_strCache = ui.lineEdit_cache->text();
	m_strTaskName = ui.lineEdit_Name->text();
	m_strDiscLabel = ui.lineEdit_DiscLabel->text();

	m_nTaskType = ui.comboBox_TaskType->currentData().toInt();
	m_eDiscType = ui.comboBox_DiscType->currentData().toInt();
	m_nCopyCount = ui.lineEdit_CopyCount->text().toInt();
	m_nBurnSpeed = ui.comboBox_BurnSpeed->currentData().toInt();
	m_nDiscOutputMode = ui.comboBox_DiscOutputMode->currentData().toInt();

	m_bDeleteFile = ui.checkBox_DeleteFile->isChecked();
	m_bVerifyDisc = ui.checkBox_VerifyDisc->isChecked();
	m_bTestBurn = ui.checkBox_TestBurn->isChecked();
	m_bCloseDisc = ui.checkBox_CloseDisc->isChecked();
	m_bRejectIfNotBlank = ui.checkBox_SupportNotNullDisc->isChecked();
	m_bVNRWrite = ui.checkBox_VNRWrite->isChecked();
	m_bBurnImage = ui.checkBox_OnlyBurnIso->isChecked();
	m_bBurnStream = ui.checkBox_StreamBurn->isChecked();
	m_nErrorRetry = ui.lineEdit_error_retry->text().toInt();

	m_nTotalMB = ui.lineEdit_TotalMB->text().toInt();
	m_nMBPreFile = ui.lineEdit_FileSizeMB->text().toInt();
	m_nMins = ui.lineEdit_Mins->text().toInt();

	QTreeWidgetItemIterator it(ui.treeWidget_File);
	while (*it) 
	{
		m_lstFilePath.append((*it)->text(1));
		++it;
	}
	m_strSuccessPrintFile = ui.lineEdit_SuccessCoverFile->text();
	m_strFailPrintFile = ui.lineEdit_FailCoverFile->text();
	int nRowCount = ui.tableWidget_Value->rowCount();
	for (int i=0; i<nRowCount; i++)
	{
		QPrintValueWidget* widget = (QPrintValueWidget*)ui.tableWidget_Value->cellWidget(i, 1);
		m_vPrintValues.push_back(widget->GetValueInfo());
	}

	if (!m_bJsonSubmitTask)
	{
		if (m_nTaskType == TASK_TYPE_DISC_PRINT)
		{
			if (!SubmitPrintTask())
			{
				return;
			}
		}
		else if (m_nTaskType == TASK_TYPE_FILE_BURN)
		{
			int nCount = ui.lineEdit_task_count->text().toInt();
			for (int i = 0; i < nCount; i++)
			{
				QString strName = m_strTaskName;
				if (i > 0)
				{
					strName = QString("%1_%2").arg(m_strTaskName).arg(i);
				}
				if (!SubmitTask(strName))
				{
					return;
				}
			}
		}
	}

	m_strJson = CreateJson();
	done(1);
	close();
}

bool QNewTaskDialog::SubmitTask(QString strName)
{
    PMYTaskHandle taskHandle;
	//创建任务
	int nRet = SDKFunc()._PMYBS_CreateTask(m_LoginHandle, TASK_TYPE_FILE_BURN, strName.toStdString().c_str(), &taskHandle);
	if (nRet != PMYBS_SUCCESS)
	{
		QMessageBox::warning(this, "error", QString("_PMYBS_CreateTask fail. err: %1").arg(nRet));
		return false;
	}
// 	if (m_bBurnImage)
// 	{
// 		if (m_lstFilePath.size() > 0)
// 		{
// 			nRet = SDKFunc()._PMYBS_AddImageFile(taskHandle, m_lstFilePath[0].toStdString().c_str());
// 			if (nRet != PMYBS_SUCCESS)
// 			{
// 				QMessageBox::warning(this, "error", QString("_PMYBS_AddImageFile fail. err: %1").arg(nRet));
// 				return false;
// 			}
// 		}
// 	}
// 	else
	{
		//添加刻录文件
		for (int i=0; i<m_lstFilePath.size(); i++)
		{
			QFileInfo fileInfo(m_lstFilePath[i]);
			if (fileInfo.isDir())
			{
				nRet = SDKFunc()._PMYBS_AddTree(taskHandle, m_lstFilePath[i].toStdString().c_str(), NULL);
			}
			else
			{
				nRet = SDKFunc()._PMYBS_AddFile(taskHandle, m_lstFilePath[i].toStdString().c_str(), NULL);
			}

			if (nRet != PMYBS_SUCCESS)
			{
				QMessageBox::warning(this, "error", QString("AddTree or AddFile fail. err: %1").arg(nRet));
				return false;
			}
		}
	}
	

	//添加打印模板
	nRet = SDKFunc()._PMYBS_SetPrintFile(taskHandle, m_strSuccessPrintFile.toStdString().c_str(), m_strFailPrintFile.toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
	{
		QMessageBox::warning(this, "error", QString("_PMYBS_SetPrintFile fail. err: %1").arg(nRet));
		return false;
	}

// 	nRet = SDKFunc()._PMYBS_SetPrintFile2(taskHandle, "E:/ok1.PTF", "E:/fail1.PTF", 1);
// 	if (nRet != PMYBS_SUCCESS)
// 	{
// 		QMessageBox::warning(this, "error", QString("_PMYBS_SetPrintFile fail. err: %1").arg(nRet));
// 		return false;
// 	}

	//添加打印字段 index 从1开始
	for (int i=0; i<m_vPrintValues.size(); i++)
	{
		nRet = SDKFunc()._PMYBS_SetPrintField(taskHandle, i + 1, m_vPrintValues[i].strValue.toStdString().c_str());
		if (nRet != PMYBS_SUCCESS)
		{
			QMessageBox::warning(this, "error", QString("_PMYBS_SetPrintField fail. err: %1").arg(nRet));
			return "";
		}
	}

	//设置任务配置
	//无特殊需求可不用配置
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_BURN_IMAGE, QString("%1").arg(m_bBurnImage).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_DISC_TYPE, QString("%1").arg((int)m_eDiscType).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_CLOSE_DISC, QString("%1").arg(m_bCloseDisc).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_DISC_LABEL, QString("%1").arg(m_strDiscLabel).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_TEST_BURN, QString("%1").arg(m_bTestBurn).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_DISC_OUTPUT_MODE, QString("%1").arg(m_nDiscOutputMode).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_DELETE_FILES, QString("%1").arg(m_bDeleteFile).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;

	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_COPY_COUNT, QString("%1").arg(m_nCopyCount).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_BURN_SPEED, QString("%1").arg(m_nBurnSpeed).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_VERIFY_DISC, QString("%1").arg(m_bVerifyDisc).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_REJECT_IF_NOT_BLANK, QString("%1").arg(m_bRejectIfNotBlank).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_BURN_PROOF, QString("%1").arg(m_bBurnProof).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_VNR_WRITE, QString("%1").arg(m_bVNRWrite).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_BURN_ERROR_RETRY, QString("%1").arg(m_nErrorRetry).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;

    TASK_ID tTaskId;
	nRet = SDKFunc()._PMYBS_SubmitTask(taskHandle, &tTaskId);
	if (nRet != PMYBS_SUCCESS)
	{
		QMessageBox::warning(this, "error", QString("_PMYBS_SubmitTask fail. err: %1").arg(nRet));
		return false;
	}
	m_TaskHandle = taskHandle;
	m_strTaskId = tTaskId.strTaskId;

	return true;
}

bool QNewTaskDialog::SubmitPrintTask()
{
	PMYTaskHandle taskHandle;
	//创建任务
	int nRet = SDKFunc()._PMYBS_CreateTask(m_LoginHandle, TASK_TYPE_DISC_PRINT, m_strTaskName.toStdString().c_str(), &taskHandle);
	if (nRet != PMYBS_SUCCESS)
	{
		QMessageBox::warning(this, "error", QString("_PMYBS_CreateTask fail. err: %1").arg(nRet));
		return false;
	}

	//添加打印模板
	nRet = SDKFunc()._PMYBS_SetPrintFile(taskHandle, m_strSuccessPrintFile.toStdString().c_str(), "");
	if (nRet != PMYBS_SUCCESS)
	{
		QMessageBox::warning(this, "error", QString("_PMYBS_SetPrintFile fail. err: %1").arg(nRet));
		return false;
	}

// 	nRet = SDKFunc()._PMYBS_SetPrintFile2(taskHandle, "E:/ok1.PTF", "", 1);
// 	if (nRet != PMYBS_SUCCESS)
// 	{
// 		QMessageBox::warning(this, "error", QString("_PMYBS_SetPrintFile fail. err: %1").arg(nRet));
// 		return false;
// 	}

	//添加打印字段 index 从1开始
	for (int i=0; i<m_vPrintValues.size(); i++)
	{
		nRet = SDKFunc()._PMYBS_SetPrintField(taskHandle, i + 1, m_vPrintValues[i].strValue.toStdString().c_str());
		if (nRet != PMYBS_SUCCESS)
		{
			QMessageBox::warning(this, "error", QString("_PMYBS_SetPrintField fail. err: %1").arg(nRet));
			return "";
		}
	}

	//设置任务配置
	//无特殊需求可不用配置
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_DISC_OUTPUT_MODE, QString("%1").arg(m_nDiscOutputMode).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;
	nRet = SDKFunc()._PMYBS_SetTaskConfig(taskHandle, PMYBS_SDK_TASK_CFG_COPY_COUNT, QString("%1").arg(m_nCopyCount).toStdString().c_str());
	if (nRet != PMYBS_SUCCESS)
		return false;

	TASK_ID tTaskId;
	nRet = SDKFunc()._PMYBS_SubmitTask(taskHandle, &tTaskId);
	if (nRet != PMYBS_SUCCESS)
	{
		QMessageBox::warning(this, "error", QString("_PMYBS_SubmitTask fail. err: %1").arg(nRet));
		return false;
	}
	m_TaskHandle = taskHandle;
	m_strTaskId = tTaskId.strTaskId;
	return true;
}

QString QNewTaskDialog::CreateJson()
{
	QJsonObject obj;
	QJsonObject task;

	task.insert("type", m_nTaskType);
	task.insert("name", m_strTaskName);
	task.insert("remote", m_bNetBurn);
	QJsonObject burn_src;
	QJsonArray local_files;
	for (int i=0; i<m_lstFilePath.size(); i++)
	{
		QFileInfo fileInfo(m_lstFilePath[i]);
		QJsonObject obj;
		obj.insert("flag", fileInfo.isDir() ? BURN_FILE_TREE : BURN_FILE_FILE);
		obj.insert("src_file_path", m_lstFilePath[i]);
		local_files.append(obj);
	}
	burn_src.insert("local_files", local_files);
	task.insert("burn_src", burn_src);

	if (m_bNetBurn)
	{
		QJsonObject upload;
		upload.insert("burn_files", true);
		upload.insert("print_file", true);
		task.insert("upload", upload);
	}

	QJsonObject setting;

	setting.insert("copy_count", m_nCopyCount);
	setting.insert("disc_label", m_strDiscLabel);
	setting.insert("disc_type", m_eDiscType);
	setting.insert("burn_speed", m_nBurnSpeed);
	setting.insert("verify_disc", m_bVerifyDisc);
	setting.insert("test_burn", m_bTestBurn);
	setting.insert("close_disc", m_bCloseDisc);
	setting.insert("disc_output_mode", m_nDiscOutputMode);
	setting.insert("delete_files", m_bDeleteFile);
	setting.insert("reject_if_not_blank", m_bRejectIfNotBlank);
	setting.insert("burn_stream", m_bBurnStream);
	setting.insert("burn_proof", m_bBurnProof);
	setting.insert("vnr_write", m_bVNRWrite);
	setting.insert("burn_image", m_bBurnImage);

	QJsonObject print_setting;
	QJsonArray print_files;
	{
		QJsonObject obj;
		obj.insert("success", m_strSuccessPrintFile);
		obj.insert("fail", m_strFailPrintFile);
		print_files.append(obj);
	}
	print_setting.insert("print_files", print_files);

	QJsonArray field_values;
	for (int i=0; i<m_vPrintValues.size(); i++)
	{
		QJsonObject obj;
		obj.insert("index", i + 1);
		obj.insert("variable", m_vPrintValues[i].bVariable);
		obj.insert("value", m_vPrintValues[i].strValue);
		field_values.append(obj);
	}
	print_setting.insert("field_values", field_values);
	setting.insert("print_setting", print_setting);
	task.insert("setting", setting);
	obj.insert("task", task);


	QJsonDocument doc(obj);
	return doc.toJson();

}

void QNewTaskDialog::on_comboxTurn_changed(int index)
{
	ui.stackedWidget->setCurrentIndex(index);
}

void QNewTaskDialog::on_pushButton_AddFile_clicked()
{
	QStringList FileList = QFileDialog::getOpenFileNames(this, "请选择刻录文件");
	if (FileList.isEmpty())
	{
		return;	
	} 
	AddFile( (QStringList)FileList);
	return;
}

void QNewTaskDialog::on_pushButton_AddFolder_clicked()
{
	QString directory = QFileDialog::getExistingDirectory(this, "请选择刻录文件目录");
	if (directory.isEmpty())
	{	
		return;
	}
	AddFile( (QStringList)directory);
	return;
}

bool QNewTaskDialog::AddFile(QStringList fileList)
{
	for (int i=0; i<fileList.size();)
	{
		QList<QTreeWidgetItem *> listItem = ui.treeWidget_File->findItems(fileList[i], Qt::MatchFixedString, 1);
		if (listItem.size() > 0)
		{
			fileList.removeAt(i);
			continue;
		}
		i++;
	}
	if (fileList.size() == 0)
	{
		return false;
	}
	
	for (int i=0; i<fileList.size(); i++)
	{
		QFileInfo fileInfo(fileList[i]);
		if (fileInfo.isDir() == false && fileInfo.size() + 2048 > CommonCfg().GetDiscUseSize(ui.comboBox_DiscType->currentData().toInt()))
		{
			QString strText = QString("%1文件大小大于剩余光盘容量，无法添加文件！").arg(fileList[i]);
			QMessageBox::warning(this, "error", strText);
			return false;
		}
		 // 设置展示项目
		QTreeWidgetItem* FileItem = new QTreeWidgetItem(ui.treeWidget_File);
		FileItem->setText(0, fileInfo.fileName());
		FileItem->setText(1, fileInfo.absoluteFilePath());
	}
	return true;
}

void QNewTaskDialog::on_pushButton_DeleteFile_clicked()
{
	QTreeWidgetItem* item = ui.treeWidget_File->currentItem();
	delete item;
}

void QNewTaskDialog::on_pushButton_SuccessCoverFile_clicked()
{
    QString strType = "光盘封面打印模板(*.ptf);;图片(*.bmp *.jpg *.jpeg *.png)";
	QString strFilePath = QFileDialog::getOpenFileName(this, "请添加光盘封面打印模板",
		"",
		strType);
	if (strFilePath.isEmpty())
	{
		return;
	}
	ui.lineEdit_SuccessCoverFile->setText(strFilePath);
}

void QNewTaskDialog::on_pushButton_FailCoverFile_clicked()
{
    QString strType = "光盘封面打印模板(*.ptf);;图片(*.bmp *.jpg *.jpeg *.png)";
	QString strFilePath = QFileDialog::getOpenFileName(this, "请添加光盘封面打印模板",
		"",
		strType);
	if (strFilePath.isEmpty())
	{
		return;
	}
	ui.lineEdit_FailCoverFile->setText(strFilePath);
}

void QNewTaskDialog::on_pushButton_Add_img_clicked()
{
// 	if (ui.treeWidget_File->it > 0)
// 	{
// 		return;
// 	}
	QString strFilePath = QFileDialog::getOpenFileName(this, "请选择镜像文件", "", "镜像(*.iso)");
	if (strFilePath.isEmpty())
	{
		return;
	} 
	AddFile( (QStringList)strFilePath);
	return;
}

quint64 folderFileSize(const QString &path) 
{ 
	QDir dir(path); 
	quint64 size = 0; 
	foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files)) 
	{ 
		size += fileInfo.size(); 
	} 
	foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) 
	{ 
		size += folderFileSize(path + QDir::separator() + subDir); 
	} 
	return size; 
}



