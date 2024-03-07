#ifndef QDATATHRAD_H
#define QDATATHRAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "PMYBurnSDKFunc.h"

class QDataThread : public QThread
{
	Q_OBJECT

public:
	QDataThread(QObject *parent);
	~QDataThread();

	void run();
	void RunStream();
	void SetStop();
	static void InputLog(const QString& strTaskId, const QString &msg);

private:
	int SubmitTaskFile(int nFileIndex, int nFlag, bool bFullDisc = false, bool bFinishTask = false);
	quint64 GetDiscMaxSize(int enDiscType);

public:
	bool m_bStop;
	int m_nTotalMB;
	int m_nMBPreFile;
	int m_nMins;
	quint64 m_u64MaxSizeInDisc;
	PMYTaskHandle m_pTaskHandle;
	PMYLoginHandle m_pLoginHandle;
	QString m_strTaskId;
	int m_eDiscType;
	QString m_strCache;
	bool m_bBurnStream;

	QString m_strDir;
private:
	QMutex m_Mutex;
	QWaitCondition m_WaitCondition;

signals:
	void sigFinishData(QString, int);
};

#endif // QDATATHRAD_H
