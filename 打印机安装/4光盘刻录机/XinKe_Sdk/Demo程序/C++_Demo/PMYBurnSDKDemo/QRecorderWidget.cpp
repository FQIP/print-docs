#include "stdafx.h"
#include "QRecorderWidget.h"
#include <QDomDocument>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif 
// 
// QRecorderWidget::QRecorderWidget(bool bPrinter, QWidget *parent)
// 	: QWidget(parent)
// 	, m_bPrinter(bPrinter)
// {
// 	ui.setupUi(this);
// 	installEventFilter(this);
// 	if (m_bPrinter)
// 	{
// /*		ui.widget_Size->hide();*/
// 		ui.label_Size->hide();
// 		ui.label_Name->setText("光盘打印机");
// 		ui.progressBar->hide();
// 		QIcon icon = QIcon(":/Resources/printer_64.png");
// 		ui.label_Ico->setPixmap(icon.pixmap(64, 64));
// 	}
// 	else
// 	{
// //		InitSizeRuler();
// 		QIcon icon = QIcon(":/Resources/drive/OpticalDrive.png");
// 		ui.label_Ico->setPixmap(icon.pixmap(64, 64));
// 	}
// }

QRecorderWidget::QRecorderWidget(DriveData* data, QWidget *parent)
	: QWidget(parent)
	, m_data(data)
{
	ui.setupUi(this);
	//InitSizeRuler();
	UpdateUI();
}


QRecorderWidget::~QRecorderWidget()
{

}

void QRecorderWidget::InitSizeRuler()
{
	ui.progressBar->setMinimum(0);
	ui.progressBar->setMaximum(m_u64TotalSize);
	ui.progressBar->setValue(m_u64CurSize);
}

QString QRecorderWidget::GetIconByDriveType(OPTICAL_DRIVE_TYPE eDriveType)
{
	switch(eDriveType)
	{
	case OPTICAL_DRIVE_CD:
		{
			return ":/Resources/drive/OpticalDrive.png";
		}
	case OPTICAL_DRIVE_DVD:
		{
			return ":/Resources/drive/DVD OpticalDrive.png";
		}
	case OPTICAL_DRIVE_HDDVD:
		{
			return ":/Resources/drive/HD DVD OpticalDrive.png";
		}
	case OPTICAL_DRIVE_BD:
		{
			return ":/Resources/drive/BD OpticalDrive.png";
		}
	default:
		{
			return ":/Resources/drive/OpticalDrive.png";
		}
	}
}

QString QRecorderWidget::GetIconByMediaType(PMY_MEDIA_TYPE eMediaType)
{
	switch(eMediaType)
	{
	case MEDIA_CD_ROM:
		{
			return ":/Resources/drive/CD-ROM.png";
		}
	case MEDIA_CD_R:
		{
			return ":/Resources/drive/CD-R.png";
		}
	case MEDIA_CD_RW:
		{
			return ":/Resources/drive/CD-RW.png";
		}
	case MEDIA_DVD_ROM:
		{
			return ":/Resources/drive/DVD-ROM.png";
		}
	case MEDIA_DVD_RAM:
		{
			return ":/Resources/drive/DVD-RAM.png";
		}
	case MEDIA_DVD_PLUS_R:
	case MEDIA_DVD_PLUS_R_DL:
		{
			return ":/Resources/drive/DVD+R.png";
		}
	case MEDIA_DVD_PLUS_RW:
	case MEDIA_DVD_PLUS_RW_DL:
		{
			return ":/Resources/drive/DVD+RW.png";
		}
	case MEDIA_DVD_R:
	case MEDIA_DVD_R_DL:
	case MEDIA_DVD_R_DL_SEQ:
	case MEDIA_DVD_R_DL_JUMP:
		{
			return ":/Resources/drive/DVD-R.png";
		}
	case MEDIA_DVD_RW:
		{
			return ":/Resources/drive/DVD-RW.png";
		}

	case MEDIA_HD_DVD_ROM:
		{
			return ":/Resources/drive/HD DVD-ROM.png";
		}
	case MEDIA_HD_DVD_R:
		{
			return ":/Resources/drive/HD DVD-R.png";
		}
	case MEDIA_HD_DVD_RAM:
		{
			return ":/Resources/drive/HD DVD-R.png";
		}
	case MEDIA_BD_R:
	case MEDIA_BD_R_SRM:
	case MEDIA_BD_R_SRM_POW:
	case MEDIA_BD_R_RRM:
	case MEDIA_BD_ROM:
		{
			return ":/Resources/drive/BD-R.png";
		}
	case MEDIA_BD_RE:
		{
			return ":/Resources/drive/BD-RE.png";
		}
	default:
		{
			return ":/Resources/drive/OpticalDrive.png";
		}
	}
}
/*
<Drive OpticalDriveType="" Letter="" HaveDisc="" MediaType="" DrivePos="" Label="" Status="" Progress="" TotalSize="" FreeSize="" TaskName="" DiscIndex="" DiscId=""/>
*/

void QRecorderWidget::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

bool QRecorderWidget::eventFilter(QObject *object, QEvent *event)
{
	QMouseEvent *mouse = static_cast<QMouseEvent*>(event);
	if(mouse)
	{
		if(mouse->type() == QEvent::Enter)
		{
			//setStyleSheet("QWidget{background-color: rgb(248, 248, 248);}");
			return QWidget::eventFilter(object,event);
		}
		else if(mouse->type() == QEvent::Leave)
		{
			//setStyleSheet("QWidget{background-color: rgb(255, 255, 255);}");
			return QWidget::eventFilter(object,event);;
		}
	}
	return QWidget::eventFilter(object,event);
}

void QRecorderWidget::UpdateData(QJsonObject& obj)
{
	m_data->UpdateData(obj);
	UpdateUI();

}

void QRecorderWidget::UpdateUI()
{
	QString strIconPath;
	if (m_data->is_ready)
	{
		strIconPath = GetIconByMediaType((PMY_MEDIA_TYPE)m_data->disc_media_type);
	}
	else
	{
		strIconPath = GetIconByDriveType((OPTICAL_DRIVE_TYPE)m_data->optical_drive_type);
	}
	QIcon icon = QIcon(strIconPath);
	ui.label_Ico->setPixmap(icon.pixmap(64, 64));

    QString strDisplayName = m_data->display_name;
	int nPos = strDisplayName.lastIndexOf("(");
	if (nPos != -1)
	{
		strDisplayName = strDisplayName.left(nPos);
		strDisplayName = strDisplayName.simplified();
	}
	QString strText = strDisplayName;
	strText += QString(" (%1)").arg(m_data->dev_name);
	if (m_data->disc_volume_name.isEmpty() == false)
	{
		strText += " ";
		strText += m_data->disc_volume_name;
	}

    if (m_data->row == 0)
	{
		strText += " (Top)";
	}
    else if (m_data->row == 1)
	{
		strText += " (Bottom)";
	}
	ui.label_Name->setText(strText);
	QString strStatus;
	if (m_data->is_ready == false || m_data->disc_total_space == 0)
	{
		ui.progressBar->hide();
		ui.label_Size->hide();
// 		if (m_data->is_burning == false)
// 		{
// 			strStatus = "空闲、无盘";
// 		}
	}
	else
	{
		double db = (m_data->disc_total_space - m_data->disc_free_space) / m_data->disc_total_space;
		db *= 100;
		ui.progressBar->show();
		ui.label_Size->show();
		ui.progressBar->setValue(db);
		ui.label_Size->setText(QString("%1 可用，共 %2").arg(FormatSize(m_data->disc_free_space, 2)).arg(FormatSize(m_data->disc_total_space, 2)));
	}

	ui.label_Task->setText("任务：" + m_data->task_name);
	ui.label_DIsc->setText("光盘：" + m_data->burn_disc_sn);
	ui.label_Status->setText("状态：" + m_data->status_string);
}

void DriveData::UpdateData(QJsonObject& obj)
{
	dev_name = obj.value("dev_name").toString();
	dev_desc = obj.value("dev_desc").toString();
	row = obj.value("row").toInt();
	column = obj.value("column").toInt();
	disable = obj.value("disable").toBool();
	optical_drive_type = obj.value("optical_drive_type").toInt();
	display_name = obj.value("display_name").toString();
	is_ready = obj.value("is_ready").toBool();
	disc_media_type = obj.value("disc_media_type").toInt();
	disc_media_type_string = obj.value("disc_media_type_string").toString();
	disc_total_space = obj.value("disc_total_space").toDouble();
	disc_free_space = obj.value("disc_free_space").toDouble();
	disc_volume_name = obj.value("disc_volume_name").toString();
	is_burning = obj.value("is_burning").toBool();
	status_string = obj.value("status_string").toString();
	
// 	burn_size = obj.value("burn_size").toDouble();
// 	burn_progress = obj.value("burn_progress").toInt();
	burn_disc_id = obj.value("burn_disc_id").toString();
	burn_disc_sn = obj.value("burn_disc_sn").toString();
	task_id = obj.value("task_id").toString();
	task_name = obj.value("task_name").toString();

	if (obj.value("burn_disc").isObject())
	{
		QJsonObject burn_disc = obj.value("burn_disc").toObject();

	}
}
