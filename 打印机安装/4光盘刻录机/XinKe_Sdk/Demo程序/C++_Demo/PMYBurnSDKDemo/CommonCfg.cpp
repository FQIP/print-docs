#include "stdafx.h"
#include "CommonCfg.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)  
# pragma execution_character_set("utf-8")  
#endif 

CCommonCfg::CCommonCfg()
	: m_bInit(false)
{
	InitParamValue();
}

CCommonCfg::~CCommonCfg()
{
}

CCommonCfg& CCommonCfg::Instance()
{
	static CCommonCfg instanceObj;
	if (instanceObj.m_bInit == false)
	{
		instanceObj.m_bInit = instanceObj.Init();
	}
	return instanceObj;
}

void CCommonCfg::InitParamValue()
{
	m_fSize_CD = 690.0f; //690MB
	m_fSize_DVD = 4.3*1024.0f; //4.3GB
	m_fSize_DVD_DL = 7.8*1024.0f; //7.8GB
	m_fSize_BD_25 = 22*1024.0f;  //22GB
	m_fSize_BD_50 = 44*1024.0f; //44GB
	m_fSize_BD_100 = 92.7*1024.0f; //92.7GB
	m_fSize_BD_128 = 118.5*1024.0f; //118.5GB
}

bool CCommonCfg::Init()
{
	InitParamValue();
    QString strCfgPath = qApp->applicationDirPath() + "/CommonCfg.xml";
    QFile file(strCfgPath);
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return false;
	do
	{
		if (!m_doc.setContent(&file))
			break;

		QDomElement NdCommonCfg = m_doc.documentElement();
		if (NdCommonCfg.isNull())
			break;

		QDomElement NdDisc = NdCommonCfg.firstChildElement("Disc");
		if (NdDisc.isNull())
			break;

		QDomElement NdCapacity = NdDisc.firstChildElement("Capacity");
		if (NdCapacity.isNull())
			break;

		QDomElement NdItem = NdCapacity.firstChildElement("Item");
		if (NdItem.isNull())
			break;

		while (!NdItem.isNull())
		{
			int enDiscType = NdItem.attribute("Type").toInt();
			SetSpecTypeDiscDeftSize(enDiscType, NdItem.attribute("DefaultSize", "0").toFloat());
			NdItem = NdItem.nextSiblingElement("Item");
		}

	}while (0);
	file.close();
	return true;
}

void CCommonCfg::SetSpecTypeDiscDeftSize(int enDiscType, float fDeftSize)
{
	if (fDeftSize <= 0.0f)
		return;
	
	switch (enDiscType)
	{
	case PMY_DISC_CD_700M:
		{
			m_fSize_CD = fDeftSize;
			break;
		}
	case PMY_DISC_DVD_4_7G:
		{
			m_fSize_DVD = fDeftSize;
			break;
		}
	case PMY_DISC_BD_25G:
		{
			m_fSize_BD_25 = fDeftSize;
			break;
		}
	case PMY_DISC_BD_50G:
		{
			m_fSize_BD_50 = fDeftSize;
			break;
		}
	case PMY_DISC_BD_100G:
		{
			m_fSize_BD_100 = fDeftSize;
			break;
		}
	case PMY_DISC_DVD_DL_9_4G:
		{
			m_fSize_DVD_DL = fDeftSize;
			break;
		}
	case PMY_DISC_BD_128G:
		{
			m_fSize_BD_128 = fDeftSize;
			break;
		}
	default:
		break;
	}
}

quint64 CCommonCfg::GetDiscUseSize(int enDiscType)
{
	quint64 n64Relt = 0;
	switch (enDiscType)
	{
	case PMY_DISC_CD_700M:
		{
			n64Relt = m_fSize_CD;
			break;
		}
	case PMY_DISC_DVD_4_7G:
		{
			n64Relt = m_fSize_DVD;
			break;
		}
	case PMY_DISC_BD_25G:
		{
			n64Relt = m_fSize_BD_25;
			break;
		}
	case PMY_DISC_BD_50G:
		{
			n64Relt = m_fSize_BD_50;
			break;
		}
	case PMY_DISC_BD_100G:
		{
			n64Relt = m_fSize_BD_100;
			break;
		}
	case PMY_DISC_DVD_DL_9_4G:
		{
			n64Relt = m_fSize_DVD_DL;
			break;
		}
	case PMY_DISC_BD_128G:
		{
			n64Relt = m_fSize_BD_128;
			break;
		}
	default:
		break;
	}
	return n64Relt*1024.0f*1024.0f;
}

quint64 CCommonCfg::GetDiscSize(int eType)
{
	/*	return 200*1024*1024;*/
	switch(eType)
	{
	case 0://DISC_CD_700M:
		{
			return 0x2BC00000;
		}
	case 1://DISC_DVD_4_7G:
		{
			return 0x12CCCCCCC;//(UInt64)(4.7*1024.0*1024.0*1024.0);
		}
	case 2://DISC_BD_25G:
		{
			return 0x640000000;//(25*1024*1024*1024);
		}
	case 3://DISC_BD_50G:
		{
			return 0xC80000000;//50*1024*1024*1024;
		}
	case 4://DISC_BD_100G:
		{
			return 0x1900000000;//100*1024*1024*1024;
		}
	case 5://DISC_DVD_DL_8_5G:
		{
			return 0x220000000;//8.5*1024*1024*1024;
		}
	case 6://DISC_BD_128G:
		{
			return 0x2000000000;//128*1024*1024*1024;
		}
	default:
		return 0;
	}
}
