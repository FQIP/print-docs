#ifndef PMYBURNSDKDEMO_H
#define PMYBURNSDKDEMO_H
#include <QtWidgets/QDialog>
#include <QDomElement>
#include <QMutex>
#include "QRecorderWidget.h"
#include "ui_PMYBurnSDKDemo.h"
#include "QRobotDeviceWidget.h"
#include "PMYBurnSDKFunc.h"
#include <QFile>

class QDataThread;
struct GROUP_HISTORY
{
	QString strName;
	QString strId;
	int nType;

};

enum ITEM_TYPE
{
	ITEM_PROJECT = 0,
	ITEM_DISC_BURN,
	ITEM_UPLOAD,
	ITEM_DOWNLOAD,
};

class BaseItem
{
public:
	BaseItem(){
        m_Item = nullptr;
	}
	~BaseItem(){}

	ITEM_TYPE m_eType;
	QTreeWidgetItem* m_Item;
};

class TaskItem : public BaseItem
{
public:
	TaskItem(){
		m_eType = ITEM_PROJECT;
		is_finish = false;
		status_code = 0;
		total_disc = 0;
		total_disc = 0;
		success_disc = 0;
		fail_disc = 0;
		total_size = 0;
	}
	~TaskItem(){}

	QString id;
	int type;
	QString name;
	bool is_finish;
	int status_code;
	QString status_string;
	int total_disc;
	int success_disc;
	int fail_disc;
	double total_size;

	QMap<QString, BaseItem*> mapChildItems;

};

class DiscItem : public BaseItem
{
public:
	DiscItem(){
		m_eType = ITEM_DISC_BURN;

	}
	~DiscItem(){}

	QString id;
	QString sn;
	int disc_index;
	int copy_index;
	QString label;
	int status_code;
	QString status_string;
	bool stream_burn;
	double total_size;
	double burn_size;
	int progress;
	QString drive;


    void Update(QJsonObject& obj);

};

class UploadItem : public BaseItem
{
public:
	UploadItem(){
		m_eType = ITEM_UPLOAD;

	}
	UploadItem(ITEM_TYPE e){
		m_eType = e;
	}
	~UploadItem(){}
	QString id;

	int status_code;
	QString status_string;
	int total_count;
	double total_size;
	double upload_size;
	int progress;
	
	QString cur_file;
	int cur_index;
	int cur_progress;

    void Update(QJsonObject& obj);
};

class PMYBurnSdkDemo : public QDialog
{
	Q_OBJECT

public:
    PMYBurnSdkDemo(QWidget *parent = nullptr);
	~PMYBurnSdkDemo();
	void keyPressEvent(QKeyEvent* e);

	static void PMYBS_SDK_APICALL PMYBS_RobotStatusCallBack(PMYLoginHandle LoginHandle, const char* pszBuffer, void* pUserData);
	static void PMYBS_SDK_APICALL PMYBS_TaskStatusCallBack(PMYLoginHandle LoginHandle, const char* pszBuffer, void* pUserData);
	static void PMYBS_SDK_APICALL PMYBS_DynamicCallBack(PMYLoginHandle LoginHandle, const char* pszBuffer, void* pUserData);
	static void PMYBS_SDK_APICALL PMYBS_EventCallBack(PMYLoginHandle LoginHandle, int nMajorEvent, int nMinorEvent, const char* pEventStr, void* pUserData);

	void NotifyTask(const std::string& strResponse);
	void NotifyDevice(const std::string& strResponse);
	void NotifyDrive(const std::string& strResponse);
	QRecorderWidget* GetDriveWidget(QString dev_name);
	void NotifyDisc(const std::string& strResponse);


	void NotifyDownload(const std::string& strResponse);
	void InsertLog(QString text);

	void UpdateRobotStatus(QString strXml);
	void UpdateDriverStatus(QString strXml);
	QString GetDeviceImage(int nDeviceType, bool& bHideBlack);
	QString GetDiscBoxImage(int nNum);
	QString GetYellowImage(long lRemaining);
	QString GetMagentaImage(long lRemaining);
	QString GetCyanImage(long lRemaining);
	QString GetBlackImage(long lRemaining);
	bool eventFilter(QObject *obj, QEvent *event);
	void InitTaskGroup();
	void InsertTaskGroup(GROUP_HISTORY history);

	void DisplayGroupHistory(GROUP_HISTORY history);
	TaskItem* GetTaskItem(QString strTaskId);
	BaseItem* GetChildItem(QString nTaskRunId, QString strItemId);
	BaseItem* GetChildItem2(QString strItemId, QString& strTaskId);
	void InsertChildItem(QString strTaskRunId, QString strChildId, BaseItem* pItem);

	void UpdateTaskItem(TaskItem* pTask);
	void UpdateChildItem(QString strTaskRunId, BaseItem* pBaseItem);
	void InputLog(const QString& strTaskId, const QString &msg);
	void UpdateDownloadStatus(QString strTaskRunId, QString strStatus);
	void TaskStatusCallbackCore(const char* pBuffer);
	void DriveStatusCallbackCore(const char* pBuffer);
	void RobotStatusCallbackCore(const char* pBuffer);
	void EventCallbackCore(int nMajorEvent, int nMinorEvent, const char* pEventXml);
	void ProcTaskRunEvent(int nMinorEvent, const QString& strEventXml);
	void ProcBurnDiscEvent(int nMinorEvent, const QString& strEventXml);
	void FinishTask(QString strTaskId);
	void CloseTask(QString strTaskId);

	QString GetDiscTypeTxtByEm(PMY_MEDIA_TYPE eMediaType);
	void UpdateFileTransferStatus(QString strTaskRunId, QDomElement& FileTransferElement);
	void GetDriveStatus();
	void GetPrintStatus();

	void RPC_PublishDeviceStatus(const std::string& strResponse);
	void RPC_PublishTaskStatus(const std::string& strResponse);

	void RPC_PublishDiscStatus(const std::string& strResponse);
	void RPC_PublishDriveStatus(const std::string& strResponse);

	void RPC_PublishDownloadStatus(const std::string& strResponse);
	bool AddFileToDiscTree(QString strFilePath);

	void ClearDeviceWidget();

	PMYTaskHandle GetTaskHandle(QString strTaskId);
	void AddTaskHandle(QString strTaskId, PMYTaskHandle taskHandle);

	void CancelTask(QString strTaskId, bool bShow = false);
public slots: 
	void on_pushButton_Login_clicked();
	void on_pushButton_Logout_clicked();
	void ClearTaskWidget();
	void on_pushButton_QueryDevice_clicked();
	void on_pushButton_NewTask_clicked();
	void on_pushButton_CancelTask_All_clicked();
	void on_pushButton_CancelTask_clicked();
	void on_pushButton_GetTaskCfg_clicked();
	void on_pushButton_GetTaskStatus_clicked();
	void on_pushButton_RunPtfEdit_clicked();
	void on_pushButton_OpenLog_clicked();
	void on_pushButton_QueryTask_clicked();
	void on_pushButton_SysConfig_clicked();
	void on_pushButton_Restart_clicked();

	void sltUpdateTaskStatus(QString);
	void sltUpdateRobotStatus(QString);
	void sltUpdateDynamic(QString);
	void sltUpdateEvent(int, int, QString);

	void sltExecCmd(QString name, int cmd);
	QString GetConfig(const QString& strKey);
	void sltTimeOut();
	void sltFinishData(QString strTaskId, int nErr);
signals:
	void sigUpdateTaskStatus(QString);
	void sigUpdateRobotStatus(QString);
	void sigUpdateDynamic(QString);
	void sigUpdateEvent(int, int, QString);
private:
	Ui::PMYBurnSdkDemoClass ui;
	int m_nPTRobotDeviceType;
	QString m_strDeviceTooltip;
	QString m_strDeviceDes;

	QMutex m_BurnTaskItemMutex;
	QMap<QString, TaskItem*> m_mapBurnTaskItem;

	QMutex m_BurnTaskMutex;
	QMap<QString, PMYTaskHandle> m_mapBurnTask;

	QMap<QString, QRobotDeviceWidget*> m_mapDeviceWidget;
	QString m_strIp;

	PMYLoginHandle m_pLoginHandle;
	bool m_bFirst;

	QTimer* m_pTimer;
	QString m_strVersion;

	QMutex m_WorkThreadMutex;
	QMap<QString, QDataThread*> m_pVtWorkThread;

	QFile* file;
	bool m_bOnLine;
};

#endif // PMYBURNSDKDEMO_H
