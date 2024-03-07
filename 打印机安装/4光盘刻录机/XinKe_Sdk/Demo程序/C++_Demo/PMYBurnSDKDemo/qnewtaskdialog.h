#ifndef QNEWTASKDIALOG_H
#define QNEWTASKDIALOG_H

#include <QVector>
#include <QDialog>
#include "ui_qnewtaskdialog.h"
#include "QPrintValueWidget.h"
#include "PMYBurnSDKFunc.h"

quint64 folderFileSize(const QString& path);
char fileSizeToUnit(quint64);

#define BURN_FILE_FILE			0x01
#define BURN_FILE_TREE			0x02
#define BURN_FILE_DIRECTORY		0x04
#define BURN_FILE_IMAGE			0x08
#define BURN_FILE_ALLDISC		0x10

class QNewTaskDialog : public QDialog
{
	Q_OBJECT

public:
	QNewTaskDialog(PMYLoginHandle loginHandle, QWidget *parent = 0);
	~QNewTaskDialog();

	void InitDiscOutput();
	void InitDiscType();
	void InitBurnSpeed(int eDiscType);
	void InitValueTable();
	QStringList m_lstFilePath;

	QString m_strTaskName;
	int m_nTaskType;
	int m_eDiscType;
	int m_nCopyCount;
	int m_nErrorRetry;

	//删除缓存文件
	bool m_bDeleteFile;
	//光盘卷标
	QString m_strDiscLabel;
	//刻录速度
	int m_nBurnSpeed;
	//校验光盘
	bool m_bVerifyDisc;
	//模拟刻录
	bool m_bTestBurn;
	//关闭光盘
	bool m_bCloseDisc;
	//非空光盘支持
	bool m_bRejectIfNotBlank;
	bool m_bVNRWrite;
	bool m_bBurnStream;
	bool m_bBurnProof;

	bool m_bBurnImage;
	bool m_bNetBurn;

	int m_nDiscOutputMode;

	QString m_strSuccessPrintFile;
	QString m_strFailPrintFile;
	
	QVector<PRINT_VALUE_INFO> m_vPrintValues;
	QString CreateJson();
	QString m_strJson;

	PMYLoginHandle m_LoginHandle;
	QString m_strTaskId;
	PMYTaskHandle m_TaskHandle;
	bool SubmitTask(QString strName);
	bool SubmitPrintTask();

	bool m_bJsonSubmitTask;

	//real
	int m_nTotalMB;
	int m_nMBPreFile;
	int m_nMins;
	QString m_strCache;
public slots: 
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
	void on_comboxTurn_changed(int index);
	void on_pushButton_AddFile_clicked();
	void on_pushButton_AddFolder_clicked();
	void on_pushButton_DeleteFile_clicked();
	void on_pushButton_SuccessCoverFile_clicked();
	void on_pushButton_FailCoverFile_clicked();
	void on_pushButton_Add_img_clicked();
	void sltDiscTypeChange(int nIndex);
	void sltTaskTypeChange(int nIndex);
	void on_checkBox_OnlyBurnIso_stateChanged(int nStatus);
	void on_pushButton_cache_clicked();
private:
	Ui::QNewTaskDialog ui;

	bool AddFile(QStringList);
	void RemoveFile(QList<QTreeWidgetItem *> listSelectItems);
};

#endif // QNEWTASKDIALOG_H
