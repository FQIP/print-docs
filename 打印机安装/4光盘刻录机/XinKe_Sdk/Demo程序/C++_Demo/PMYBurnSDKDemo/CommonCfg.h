#pragma once

#include "qnewtaskdialog.h"

class CCommonCfg
{
public:
	//virtual
	CCommonCfg();
	~CCommonCfg();

public:
	bool Init();
	static CCommonCfg &Instance();
	quint64 GetDiscUseSize(int enDiscType);

    quint64 GetDiscSize(int eType);
private:
	void InitParamValue();
	void SetSpecTypeDiscDeftSize(int enDiscType, float fDeftSize);

protected:
	bool m_bInit;

private:
	QDomDocument m_doc;
    quint64 m_fSize_CD;
    quint64 m_fSize_DVD;
    quint64 m_fSize_DVD_DL;
    quint64 m_fSize_BD_25;
    quint64 m_fSize_BD_50;
    quint64 m_fSize_BD_100;
    quint64 m_fSize_BD_128;
};

inline CCommonCfg &CommonCfg()
{
	return CCommonCfg::Instance();
}
