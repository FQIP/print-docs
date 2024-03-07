#ifndef QRECORDERWIDGET_H
#define QRECORDERWIDGET_H
#include <QWidget>
#include "ui_QRecorderWidget.h"
#include "PMYBurnPublic.h"
#include <QDomDocument>


class DriveData
{
public:
	DriveData()
	{

	}
	~DriveData()
	{

	}

	QString dev_name;
	QString dev_desc;
	int row;
	int column;
	bool disable;
	int optical_drive_type;
	QString display_name;
	bool is_ready;
	int disc_media_type;
	QString disc_media_type_string;
	quint64 disc_total_space;
	quint64 disc_free_space;
	QString disc_volume_name;
	bool is_burning;
	QString status_string;

// 	QString burn_status_string;
// 	quint64 burn_size;
// 	int burn_progress;
	QString burn_disc_id;
	QString burn_disc_sn;
	QString task_id;
	QString task_name;



	void UpdateData(QJsonObject&);

};

class QRecorderWidget : public QWidget
{
	Q_OBJECT

public:
	QRecorderWidget(DriveData* data, QWidget *parent);
	~QRecorderWidget();

	void InitSizeRuler();
	void UpdatePrinter(QDomElement strXml);
	QString GetIconByDriveType(OPTICAL_DRIVE_TYPE eDriveType);
	QString GetIconByMediaType(PMY_MEDIA_TYPE eMediaType);
	void UpdateRecorder(QDomElement strXml);
	void paintEvent(QPaintEvent *);
	bool eventFilter(QObject *object, QEvent *event);
	void UpdateData(QJsonObject& obj);
	void UpdateUI();
private:
	Ui::QRecorderWidget ui;
	bool m_bPrinter;
	quint64 m_u64TotalSize;
	quint64 m_u64CurSize;
	quint64 m_u64InitSize;

	QString m_strDriver;
	QString strDiscId;
	DriveData* m_data;
};

#endif // QRECORDERWIDGET_H
