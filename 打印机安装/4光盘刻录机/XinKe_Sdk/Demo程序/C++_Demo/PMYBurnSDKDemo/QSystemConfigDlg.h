#ifndef QSYSTEMCONFIGDLG_H
#define QSYSTEMCONFIGDLG_H

#include <QDialog>
#include "ui_QSystemConfigDlg.h"
#include "PMYBurnSDKFunc.h"

class QSystemConfigDlg : public QDialog
{
	Q_OBJECT

public:
	QSystemConfigDlg(PMYLoginHandle pLoginHandle, QWidget *parent = 0);
	~QSystemConfigDlg();

	bool GetConfig(const QString& strKey, QString& strValue);
	bool SetConfig(const QString& strKey, QString strValue);
	void InitComboxDiscType(QComboBox* pCombox);
	int GetComBoxIndex(QComboBox* pCombox, int nType);
private:
	Ui::QSystemConfigDlg ui;

	PMYLoginHandle m_pLoginHandle;
public slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
	void on_pushButton_getsysconfig_clicked();
};

#endif // QSYSTEMCONFIGDLG_H
