#ifndef QDEVICEWIDGET_H
#define QDEVICEWIDGET_H

#include <QWidget>
#include "ui_qdevicewidget.h"
#include "QRecorderWidget.h"

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


class QDeviceWidget : public QWidget
{
	Q_OBJECT

public:
	QDeviceWidget(DeviceData* data, QWidget *parent = 0);
	~QDeviceWidget();
	void UpdateData(QJsonObject& obj);

	void UpdateUI();
	QString GetDeviceImage(int nDeviceType, bool& bHideBlack);
	QString GetDiscBoxImage(int nNum);
	QString GetYellowImage(long lRemaining);
	QString GetMagentaImage(long lRemaining);
	QString GetCyanImage(long lRemaining);
	QString GetBlackImage(long lRemaining);
private:
	Ui::QDeviceWidget ui;
	DeviceData* m_data;

	QMap<QString, QRecorderWidget*> m_mapRecorderWidget;
};

#endif // QDEVICEWIDGET_H
