#ifndef PMYBURNPUBLIC_H
#define PMYBURNPUBLIC_H

enum OPTICAL_DRIVE_TYPE
{
	OPTICAL_DRIVE_UNKNOW = 0,
	OPTICAL_DRIVE_CD,
	OPTICAL_DRIVE_DVD,
	OPTICAL_DRIVE_HDDVD,
	OPTICAL_DRIVE_BD,
};

enum FileTypeTask
{
	eExistFileTask = 0,
	eDowningFileTask = 1
};

enum PMY_MEDIA_TYPE
{
	/** Represents an unknown media type (when an error occurred) */
	MEDIA_UNKNOWN = 0x1,

	/** No medium is inserted. */
	MEDIA_NONE = 0x2,

	/** DVD-ROM media */
	MEDIA_DVD_ROM = 0x4,

	MEDIA_DVD_R = 0x8,

	MEDIA_DVD_R_SEQ = 0x10,

	/** Dual Layer DVD-R media. */
	MEDIA_DVD_R_DL = 0x20,

	MEDIA_DVD_R_DL_SEQ = 0x40,

	MEDIA_DVD_R_DL_JUMP = 0x80,

	MEDIA_DVD_RAM = 0x100,

	MEDIA_DVD_RW = 0x200,

	/** DVD-RW media formatted in Restricted Overwrite mode. */
	MEDIA_DVD_RW_OVWR = 0x400,

	/** DVD-RW media formatted in Incremental Sequential mode. */
	MEDIA_DVD_RW_SEQ = 0x800,

	MEDIA_DVD_PLUS_RW = 0x1000,

	MEDIA_DVD_PLUS_R = 0x2000,

	/** Double Layer DVD+R media. */
	MEDIA_DVD_PLUS_R_DL = 0x4000,

	/** Double Layer DVD+RW media. */
	MEDIA_DVD_PLUS_RW_DL = 0x8000,

	MEDIA_CD_ROM = 0x10000,
	MEDIA_CD_R = 0x20000,
	MEDIA_CD_RW = 0x40000,
	MEDIA_HD_DVD_ROM = 0x80000,
	MEDIA_HD_DVD_R = 0x100000,
	MEDIA_HD_DVD_RAM = 0x200000,

	/** Read-only Blu-ray Disc (BD) */
	MEDIA_BD_ROM = 0x400000,

	/** Writable Blu-ray Disc (BD-R) */
	MEDIA_BD_R = 0x800000,

	/** Writable Blu-ray Disc (BD-R) */
	MEDIA_BD_R_SRM = 0x1000000,

	/** Writable Blu-ray Disc (BD-R) */
	MEDIA_BD_R_SRM_POW = 0x2000000,

	/** Writable Blu-ray Disc (BD-R) */
	MEDIA_BD_R_RRM = 0x4000000,

	/** Rewritable Blu-ray Disc (BD-RE) */
	MEDIA_BD_RE = 0x8000000,	
};


///////////////////////////
//
//	Robot Type
//
///////////////////////////
#define ROBOT_DISCPUBLISHER				0
#define ROBOT_DISCPUBLISHERII			1
#define ROBOT_DISCPUBLISHERPRO			2
#define ROBOT_COMPOSERMAX				3
#define ROBOT_RACKMOUNT_DPII			4
#define ROBOT_DISCPUBLISHER_XRP			5
#define ROBOT_DISCPUBLISHER_SE			6
#define ROBOT_DISCPUBLISHERPRO_XI		7
#define ROBOT_DISCPUBLISHER_4100		8
#define ROBOT_DISCPUBLISHER_4100_XRP	9
#define ROBOT_DISCPUBLISHER_4051		10
#define ROBOT_DISCPUBLISHER_SE3			11
#define ROBOT_DISCPUBLISHER_4200		12
#define ROBOT_DISCPUBLISHER_4200_XRP	13
#define ROBOT_DISCPUBLISHER_4052		14


#define ONE_KB	1024
#define ONE_MB	(1024*1024)
#define ONE_GB	(1024*1024*1024)
/*#define ONE_TB	(1024*1024*1024*1024)*/

static QString FormatSize(quint64 u64Value, int nBit = 2, bool bZeroEmpty = false)
{
	if (u64Value == 0)
	{
		if (bZeroEmpty)
		{
			return "";	
		}
		else
		{
			return "0 KB";
		}
	}
	QString strRet;
	float fValue;
	// 	if (u64Value >= (quint64)ONE_TB)
	// 	{
	// 		fValue = (float)((double)u64Value / (double)ONE_TB);
	// 		strRet = QString::number(fValue, 'f', 2) + "TB";
	// 	}
	if (u64Value >= ONE_GB)
	{
		fValue = (float)((double)u64Value / (double)ONE_GB);
		strRet = QString::number(fValue, 'f', nBit) + " GB";
	}
	else if (u64Value >= ONE_MB)
	{
		fValue = (float)((double)u64Value / (double)ONE_MB);
		strRet = QString::number(fValue, 'f', nBit) + " MB";
	}
	else if (u64Value >= ONE_KB)
	{
		fValue = (float)((double)u64Value / (double)ONE_KB);
		strRet = QString::number(fValue, 'f', nBit) + " KB";
	}
	else
	{
		strRet = QString::number(u64Value) + " Bytes";
	}
	return strRet;
}

#endif