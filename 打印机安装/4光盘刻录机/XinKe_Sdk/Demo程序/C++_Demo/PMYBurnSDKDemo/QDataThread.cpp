#include "stdafx.h"
#include "QDataThread.h"
#if defined(_MSC_VER) && (_MSC_VER >= 1600)  
#pragma execution_character_set("utf-8")  
#endif 


QDataThread::QDataThread(QObject *parent)
	: QThread(parent)
	, m_nTotalMB(0)
	, m_nMBPreFile(0)
	, m_nMins(0)
	, m_pTaskHandle(NULL)
	, m_pLoginHandle(NULL)
	, m_bBurnStream(false)
{
	m_bStop = false;
	m_u64MaxSizeInDisc = 0;
	m_eDiscType = PMY_DISC_DVD_4_7G;
}

QDataThread::~QDataThread()
{
}

void QDataThread::SetStop()
{
	m_bStop = true;
	m_WaitCondition.wakeAll();
}

void QDataThread::InputLog(const QString& strTaskId, const QString &msg)
{
	static QMutex mutex;
	mutex.lock();
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	QString message = QString("[%1] %2").arg(current_date_time).arg(msg);

	QString strFile = QCoreApplication::applicationDirPath() + "/Log/" + strTaskId + ".log";
	QFile file(strFile);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream text_stream(&file);
	text_stream << message << "\r\n";
	file.flush();
	file.close();
	mutex.unlock();
}

void QDataThread::run()
{
	InputLog(m_strTaskId, "开始取流");
	m_u64MaxSizeInDisc = GetDiscMaxSize(m_eDiscType);
	m_strDir = m_strCache + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd HHmmss");
	QDir dir;
	dir.mkpath(m_strDir);

	if (m_bBurnStream)
	{
		RunStream();
		return;
	}
	

#define BUFF_LEN 1*1024*1024
	BYTE* Buffer = new BYTE[BUFF_LEN];

	int nFileIndex = 0;
	quint64 u64TotalSize = m_nTotalMB;
	quint64 u64GenSize = 0;
	quint64 u64TotalGenSize = 0;
	quint64 n64PreFile = m_nMBPreFile;
	bool bFinish = false;
	int nErr = 0;
	while (1)
	{
		if (m_bStop)
		{
			break;
		}
		nFileIndex++;
		QString strFileName = QString("%1/FileName_%2.dat").arg(m_strDir).arg(nFileIndex);
		QFile file(strFileName);
		if (!file.open(QFile::ReadWrite | QFile::Text))
		{
			InputLog(m_strTaskId, "打开文件失败");
			nErr = 2;
			break;
		}

		memset(Buffer, nFileIndex, BUFF_LEN);
		for (int i = 0; i < n64PreFile; i++)
		{
			if (m_bStop)
			{
				break;
			}
			file.write((char*)Buffer, BUFF_LEN);
			QThread::msleep(1);
		}
		file.close();

		if (m_bStop)
		{
			break;
		}

		u64GenSize += n64PreFile;
		u64TotalGenSize += n64PreFile;

		bool bFullDisc = false;
		bool bFinishTask = false;
		if (((u64GenSize + n64PreFile) * 1024*1024) > m_u64MaxSizeInDisc)
		{
			bFullDisc = true;
			u64GenSize = 0;
		}
		if (u64TotalGenSize >= u64TotalSize)
		{
			bFinishTask = true;
		}

		int nFlag = PMYBS_BURN_FILE_FILE;
		int nRet = SubmitTaskFile(nFileIndex, nFlag, bFullDisc, bFinishTask);
		if (nRet != 0)
		{
			nErr = nRet;
			break;
		}

		if (bFinishTask)
		{
			InputLog(m_strTaskId, "取流完成");
			break;
		}
		QThread::sleep(m_nMins);
	}

	if (Buffer)
	{
		delete [] Buffer;
		Buffer = NULL;
	}
	emit sigFinishData(m_strTaskId, nErr);
}

void QDataThread::RunStream()
{
#define BUFF_LEN 1*1024*1024
	BYTE* Buffer = new BYTE[BUFF_LEN];

	int nFileIndex = 0;
	quint64 u64TotalSize = m_nTotalMB;
	quint64 u64GenSize = 0;
	quint64 u64TotalGenSize = 0;
	quint64 n64PreFile = m_nMBPreFile;
	int nErr = 0;
	while (1)
	{
		if (m_bStop)
		{
			break;
		}
		nFileIndex++;
		QString strFileName = QString("%1/FileName_%2.dat").arg(m_strDir).arg(nFileIndex);
		QFile file(strFileName);
		if (!file.open(QFile::ReadWrite | QFile::Text))
		{
			InputLog(m_strTaskId, "打开文件失败");
			nErr = 2;
			break;
		}

		int nFlag = PMYBS_BURN_FILE_REAL | PMYBS_BURN_FILE_REAL_WRITING;
		int nRet = SubmitTaskFile(nFileIndex, nFlag);
		if (nRet != 0)
		{
			nErr = nRet;
			break;
		}

		memset(Buffer, nFileIndex, BUFF_LEN);
		for (int i = 0; i < n64PreFile; i++)
		{
			if (m_bStop)
			{
				break;
			}
			file.write((char*)Buffer, BUFF_LEN);
			QThread::msleep(70);
		}
		file.close();

		if (m_bStop)
		{
			break;
		}

		u64GenSize += n64PreFile;
		u64TotalGenSize += n64PreFile;

		bool bFullDisc = false;
		bool bFinishTask = false;
		if (((u64GenSize + n64PreFile) * 1024*1024) > m_u64MaxSizeInDisc)
		{
			bFullDisc = true;
			u64GenSize = 0;
		}
		if (u64TotalGenSize >= u64TotalSize)
		{
			bFinishTask = true;
		}

		nFlag = PMYBS_BURN_FILE_REAL | PMYBS_BURN_FILE_REAL_FINISH;
		nRet = SubmitTaskFile(nFileIndex, nFlag, bFullDisc, bFinishTask);
		if (nRet != 0)
		{
			nErr = nRet;
			break;
		}
		
		if (bFinishTask)
		{
			InputLog(m_strTaskId, "取流完成");
			break;
		}
		QThread::sleep(m_nMins);
	}

	if (Buffer)
	{
		delete [] Buffer;
		Buffer = NULL;
	}
	emit sigFinishData(m_strTaskId, nErr);
}

int QDataThread::SubmitTaskFile(int nFileIndex, int nFlag, bool bFullDisc, bool bFinishTask)
{
	int nRet = 0;
	do 
	{
		nRet = SDKFunc()._PMYBS_OpenTask(m_pLoginHandle, m_strTaskId.toStdString().c_str(), &m_pTaskHandle);
		if (nRet != PMYBS_SUCCESS)
		{
			InputLog(m_strTaskId, "打开任务失败");
			break;
		}

		QString strDirectory = QString("/video/dir_%1").arg(nFileIndex);
		nRet = SDKFunc()._PMYBS_AddDirectory(m_pTaskHandle, strDirectory.toStdString().c_str());
		if (nRet != PMYBS_SUCCESS)
		{
			InputLog(m_strTaskId, "添加文件夹失败");
			break;
		}

		QString strFileName = QString("%1/FileName_%2.dat").arg(m_strDir).arg(nFileIndex);
		QString strPathInImage = QString("%1/FileName_%2.dat").arg(strDirectory).arg(nFileIndex);
		nRet = SDKFunc()._PMYBS_AddFileEx(m_pTaskHandle, strFileName.toStdString().c_str(), nFlag, strPathInImage.toStdString().c_str());
		if (nRet != PMYBS_SUCCESS)
		{
			InputLog(m_strTaskId, "添加文件失败");
			break;
		}

		if (bFullDisc)
		{
			nRet = SDKFunc()._PMYBS_SetTaskConfig(m_pTaskHandle, PMYBS_SDK_TASK_CFG_BURN_REAL_FINISH_DISC, QString("%1").arg(1).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
			{
				break;
			}
		}

		if (bFinishTask)
		{
			nRet = SDKFunc()._PMYBS_SetTaskConfig(m_pTaskHandle, PMYBS_SDK_TASK_CFG_BURN_REAL_FINISH_TASK, QString("%1").arg(1).toStdString().c_str());
			if (nRet != PMYBS_SUCCESS)
			{
				break;
			}
		}

		TASK_ID tTaskId;
		nRet = SDKFunc()._PMYBS_SubmitTask(m_pTaskHandle, &tTaskId);
		if (nRet != PMYBS_SUCCESS)
		{
			InputLog(m_strTaskId, "提交任务失败");
			break;
		}
		nRet = SDKFunc()._PMYBS_CloseTask(m_pTaskHandle);
		if (nRet != PMYBS_SUCCESS)
		{
			InputLog(m_strTaskId, "关闭任务失败");
			break;
		}
	} while (0);

	return nRet;
}

quint64 QDataThread::GetDiscMaxSize(int enDiscType)
{
	QString strIniFile = qApp->applicationDirPath() + "/assign_disc.ini";

	quint64 u64CD_MaxMB = 690;
	quint64 u64DVD_MaxMB = 4100;
	quint64 u64DVDDL_MaxMB = 7987;
	quint64 u64BD25_MaxMB = 22528;
	quint64 u64BD50_MaxMB = 45056;
	quint64 u64BD100_MaxMB = 94924;
	quint64 u64BD128_MaxMB = 121344;

	QFileInfo file(strIniFile);
	if (file.exists())
	{
		QSettings* ini = new QSettings(strIniFile, QSettings::IniFormat);
		u64CD_MaxMB = ini->value("CD_MaxMB", 690).toInt();
		u64DVD_MaxMB = ini->value("DVD_MaxMB", 4403).toInt();
		u64DVDDL_MaxMB = ini->value("DVDDL_MaxMB", 7987).toInt();
		u64BD25_MaxMB = ini->value("BD25_MaxM", 22528).toInt();
		u64BD50_MaxMB = ini->value("BD50_MaxMB", 45056).toInt();
		u64BD100_MaxMB = ini->value("BD100_MaxMB", 94924).toInt();
		u64BD128_MaxMB = ini->value("BD128_MaxMB", 121344).toInt();
	}
		

#define ONE_MB (1024*1024)

	switch (enDiscType)
	{
	case PMY_DISC_CD_700M:
		{
			return u64CD_MaxMB * ONE_MB;
		}
	case PMY_DISC_DVD_4_7G:
		{
			return u64DVD_MaxMB * ONE_MB;
		}
	case PMY_DISC_DVD_DL_9_4G:
		{
			return u64DVDDL_MaxMB * ONE_MB;
		}
	case PMY_DISC_BD_25G:
		{
			return u64BD25_MaxMB * ONE_MB;
		}
	case PMY_DISC_BD_50G:
		{
			return u64BD50_MaxMB * ONE_MB;
		}
	case PMY_DISC_BD_100G:
		{
			return u64BD100_MaxMB * ONE_MB;
		}
	case PMY_DISC_BD_128G:
		{
			return u64BD128_MaxMB * ONE_MB;
		}
	default:
		break;
	}
	return 0;
}