#include "stdafx.h"
#include "QSystemConfigDlg.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif 

QSystemConfigDlg::QSystemConfigDlg(PMYLoginHandle pLoginHandle, QWidget *parent /* = 0 */)
	: QDialog(parent)
	, m_pLoginHandle(pLoginHandle)
{
	ui.setupUi(this);

	InitComboxDiscType(ui.comboBox_bin1);
	InitComboxDiscType(ui.comboBox_bin2);
	InitComboxDiscType(ui.comboBox_bin3);
	InitComboxDiscType(ui.comboBox_bin4);

	on_pushButton_getsysconfig_clicked();

	ui.comboBox_bin3->hide();
	ui.comboBox_bin4->hide();
	ui.label_3->hide();
	ui.label_4->hide();
}

QSystemConfigDlg::~QSystemConfigDlg()
{

}

void QSystemConfigDlg::on_buttonBox_accepted()
{
	bool bLRDiscBin = ui.groupBox_SetDiscBinType->isChecked();
	if (!SetConfig(PMYBS_SDK_CFG_SET_DISC_TYPE, QString("%1").arg((int)bLRDiscBin)))
		return;
	if (bLRDiscBin)
	{
		QStringList listBin;
		listBin.append(ui.comboBox_bin1->currentData().toString());
		listBin.append(ui.comboBox_bin2->currentData().toString());
		listBin.append(ui.comboBox_bin3->currentData().toString());
		listBin.append(ui.comboBox_bin4->currentData().toString());

		QString strBins = listBin.join(",");
		if (!SetConfig(PMYBS_SDK_CFG_DISC_TYPE, strBins))
			return;
	}
	
	done(1);
	close();
}

void QSystemConfigDlg::on_buttonBox_rejected()
{
	close();
}

void QSystemConfigDlg::on_pushButton_getsysconfig_clicked()
{
	QString strTmp;
	if (!GetConfig(PMYBS_SDK_CFG_VERSION, strTmp))
		return;
	ui.label_version->setText(QString("°æ±¾£º%1").arg(strTmp));

	if (!GetConfig(PMYBS_SDK_CFG_SET_DISC_TYPE, strTmp))
		return;
	bool bLFDiscBin = (bool)strTmp.toInt();
	ui.groupBox_SetDiscBinType->setChecked(bLFDiscBin);

	if (!GetConfig(PMYBS_SDK_CFG_DISC_TYPE, strTmp))
		return;

	QStringList listBin = strTmp.split(",");

	if (listBin.size() >= 1)
		ui.comboBox_bin1->setCurrentIndex(GetComBoxIndex(ui.comboBox_bin1, listBin[0].toInt()));
	if (listBin.size() >= 2)
		ui.comboBox_bin2->setCurrentIndex(GetComBoxIndex(ui.comboBox_bin2, listBin[1].toInt()));
	if (listBin.size() >= 3)
		ui.comboBox_bin3->setCurrentIndex(GetComBoxIndex(ui.comboBox_bin3, listBin[2].toInt()));
	if (listBin.size() >= 4)
		ui.comboBox_bin4->setCurrentIndex(GetComBoxIndex(ui.comboBox_bin4, listBin[3].toInt()));
}

bool QSystemConfigDlg::GetConfig(const QString& strKey, QString& strValue)
{
	strValue = "";
	int nLen = 0;
	char* pBuff = new char[nLen];
	int nRet = SDKFunc()._PMYBS_GetConfig(m_pLoginHandle, strKey.toStdString().c_str(), pBuff, &nLen);
	if (nRet == PMYBS_SDK_OUTPUT_BUFFER_SMALL)
	{
		delete [] pBuff;
		pBuff = new char[nLen];
		nRet = SDKFunc()._PMYBS_GetConfig(m_pLoginHandle, strKey.toStdString().c_str(), pBuff, &nLen);
	}
	if (nRet != 0)
	{
		QMessageBox::warning(this, "error", QString("_PMYBS_GetConfig fail. strKey: %1, err: %2").arg(strKey).arg(nRet));
		delete [] pBuff;
		return false;
	}
	strValue = QString(pBuff);
	return true;
}

bool QSystemConfigDlg::SetConfig(const QString& strKey, QString strValue)
{
	int nRet = SDKFunc()._PMYBS_SetConfig(m_pLoginHandle, strKey.toStdString().c_str(), (char*)strValue.toStdString().c_str());
	if (nRet != 0)
	{
		QMessageBox::warning(this, "error", QString("_PMYBS_SetConfig fail. strKey: %1, err: %2").arg(strKey).arg(nRet));
		return false;
	}
	return true;
}

void QSystemConfigDlg::InitComboxDiscType(QComboBox* pCombox)
{
	if (pCombox == NULL)
	{
		return;
	}
	pCombox->addItem("CD 700MB", PMY_DISC_CD_700M);
	pCombox->addItem("DVD 4.7GB", PMY_DISC_DVD_4_7G);
	pCombox->addItem("DVD DL 8.5GB", PMY_DISC_DVD_DL_9_4G);
	pCombox->addItem("BD 25GB", PMY_DISC_BD_25G);
	pCombox->addItem("BD DL 50GB", PMY_DISC_BD_50G);
	pCombox->addItem("BD TL 100GB", PMY_DISC_BD_100G);
	pCombox->addItem("BD QL 128GB", PMY_DISC_BD_128G);
}

int QSystemConfigDlg::GetComBoxIndex(QComboBox* pCombox, int nType)
{
	if (pCombox == NULL)
	{
		return 0;
	}
	int nIndex = pCombox->findData(QVariant(nType));
	return nIndex == -1 ? 0 : nIndex;
}
