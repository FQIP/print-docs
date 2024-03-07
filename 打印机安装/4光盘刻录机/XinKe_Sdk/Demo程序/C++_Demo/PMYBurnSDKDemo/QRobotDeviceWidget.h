#ifndef QROBOTDEVICEWIDGET_H
#define QROBOTDEVICEWIDGET_H

#include <QWidget>
#include "ui_QRobotDeviceWidget.h"
#include "QRecorderWidget.h"
#include<QMutex>

class DeviceData
{
public:
	DeviceData()
	{
	}
	~DeviceData()
	{
	}
	QString id;
	QString name;
	int type;
	int bins_count;
	int bin1_disc_count;
	int bin2_disc_count;
	int black_ink;
	int red_ink;
	int yellow_ink;
	int blue_ink;
	int system_state;
	QString system_state_string;
	int system_error;
	QString system_error_string;

	QMap<QString, DriveData*> m_mapDrive;

	void UpdateData(QJsonObject&);
};


class QRobotDeviceWidget : public QWidget
{
	Q_OBJECT

public:
	QRobotDeviceWidget(DeviceData* data, QWidget *parent = 0);
	~QRobotDeviceWidget();
	void UpdateData(QJsonObject& obj);
	void UpdateUI();
	void UpdatePTRobotStatus(QDomElement ElementRobot);
	void UpdateDriveStatus(QDomElement DriverElement);
	void SetExpand(bool bExpand);

	bool eventFilter(QObject *obj, QEvent *event);
	void GetRobotDriveMap(QMap<QString, QRecorderWidget*>& mapDrive);
	QRecorderWidget* GetDriveWidget(QString strId);

	QString GetDeviceImage(int nDeviceType, bool& bHideBlack);
	QString GetDiscBoxImage(int nNum);
	QString GetYellowImage(long lRemaining);
	QString GetMagentaImage(long lRemaining);
	QString GetCyanImage(long lRemaining);
	QString GetBlackImage(long lRemaining);
	bool m_bSetDrive;
private:
	Ui::QRobotDeviceWidget ui;
	DeviceData* m_data;
	QMap<QString, QRecorderWidget*> m_mapRecorderWidget;

// 	QString m_strDeviceName;
// 	QPTRobotDeviceUI m_PTRobotDeviceUI;
	QString m_strDeviceTooltip;
	bool m_bExpand;

	QMutex m_Mutex;
	int m_nUpdateTime;
	QAction* m_pSetDriveAction;
signals:
	void sigDelDevice(QString);
    void sigExecCmd(QString, int);
private slots:
	void sltTimeOut();
	void sltSetDriveAction();
	void sltProjectTreeMenuRequested(const QPoint& point);
    void on_pushButton_cmd_clicked();
};

#endif // QROBOTDEVICEWIDGET_H
