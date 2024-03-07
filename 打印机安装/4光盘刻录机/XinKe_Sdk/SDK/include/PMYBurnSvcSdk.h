#pragma once

#ifndef PMYBURNSVCSDK_H
#define PMYBURNSVCSDK_H


#ifdef WIN32
#define DLL_EXPORT_DECL __declspec(dllexport)
#define DLL_IMPORT_DECL __declspec(dllimport)
#define API_STDCALL __stdcall
#define API_CDECL   __cdecl
#define PMYBS_SDK_APICALL		API_STDCALL

#else
#define DLL_EXPORT_DECL
#define DLL_IMPORT_DECL
#define API_STDCALL __attribute__((__stdcall))
#define API_CDECL   __attribute__((__cdecl))
#define PMYBS_SDK_APICALL
#define CALLBACK
#endif

#ifndef WIN32
typedef  int                BOOL;
typedef  unsigned int       DWORD;
typedef  unsigned short     WORD;
typedef  int                LONG;
typedef  unsigned char      BYTE;
typedef  void*              HANDLE;
typedef  wchar_t            WCHAR;

#define __stdcall
#define CALLBACK
#define WINAPI

#endif

#ifdef PMYBS_SDK_EXPORTS
#define PMYBS_SDK_API DLL_EXPORT_DECL
#else
#define PMYBS_SDK_API DLL_IMPORT_DECL
#endif

#ifdef __cplusplus
extern "C"
{
#else
#define true 1
#define false 0
typedef unsigned char bool;
#endif


#define PMYBS_SDK_CUR_VERSION	1
#define PMYBS_SDK_LOGIN_PORT	6000

typedef void* PMYLoginHandle;
typedef void* PMYTaskHandle;

typedef struct _TASK_ID
{
	char strTaskId[40];
}TASK_ID, *PTASK_ID;

/***SDK****/
//初始化SDK
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_Init();
//释放SDK资源
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_Destory();

/***SVC*****/
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_Login(const char* pszServerIp, int nPort, PMYLoginHandle* pLoginHandle);
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_Logout(PMYLoginHandle LoginHandle);

PMYBS_SDK_API const char* PMYBS_SDK_APICALL PMYBS_GetErrorString(int nErrorNum);
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_ExecuteRobotCmd(PMYLoginHandle LoginHandle, const char* szRobotName, unsigned long ulCmd);

/***TASK****/
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_CreateTask(PMYLoginHandle LoginHandle, int nTaskType, const char* pszTaskName, PMYTaskHandle* pTaskHandle);

PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_OpenTask(PMYLoginHandle LoginHandle, const char* pszTaskId, PMYTaskHandle* pTaskHandle);

//添加文件到实时流刻录任务中
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_AddFileEx(PMYTaskHandle TaskHandle, const char* pszFilePath, int nFlag, const char* pszPathInDisc = NULL);
//添加文件到刻录任务中
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_AddFile(PMYTaskHandle TaskHandle, const char* pszFilePath, const char* pszPathInDisc = NULL);
//添加文件夹（包含子文件）到刻录任务中
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_AddTree(PMYTaskHandle TaskHandle, const char* pszDirPath, const char* pszPathInDisc = NULL);
//添加空文件夹
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_AddDirectory(PMYTaskHandle TaskHandle, const char* pEmptyDirPath);

//添加刻录任务运行模板(ptf或者jep/jpeg)路径
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_SetPrintFile(PMYTaskHandle TaskHandle, const char* pszSusPrintFilePath, const char* pszFailPrintFilePath);
//刻录多份时可设置不同的打印模板 nIndex 为份数索引 从0开始
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_SetPrintFile2(PMYTaskHandle TaskHandle, const char* pszSusPrintFilePath, const char* pszFailPrintFilePath, int nCopyIndex);
//添加刻录任务运行打印字段
// value使用通配符
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_SetPrintField(PMYTaskHandle TaskHandle, int nIndex, const char* pszValue = NULL);

PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_SetTaskConfig(PMYTaskHandle TaskHandle, const char* pszKey, const char* pszValue);

PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_SubmitTask(PMYTaskHandle TaskHandle, TASK_ID* ptTaskId);
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_CloseTask(PMYTaskHandle TaskHandle);

PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_CreateTaskByJson(PMYLoginHandle LoginHandle, const char* pszJsonText, TASK_ID* ptTaskId);
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_CancleTaskById(PMYLoginHandle LoginHandle, const char* pszTaskId);

/**状态相关**/
//获取刻录任务状态
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_GetTaskStatus(PMYLoginHandle LoginHandle, const char* pszTaskId, char* pszBuffer, int* pnBufferLength);
//设置刻录任务状态更新回调
typedef void (PMYBS_SDK_APICALL *LPMYBS_TASK_STATUS_CALLBACK)(PMYLoginHandle LoginHandle, const char* pszBuffer, void* pUserData);
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_SetTaskStatusCallback(LPMYBS_TASK_STATUS_CALLBACK pCallbackFunc, void* pUserData);

//获取刻录机硬件状态
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_GetRobotStatus(PMYLoginHandle LoginHandle, char* pszBuffer, int* pnBufferLength);
//设置刻录机硬件状态更新回调
typedef void (PMYBS_SDK_APICALL *LPMYBS_ROBOT_STATUS_CALLBACK)(PMYLoginHandle LoginHandle, const char* pszBuffer, void* pUserData);
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_SetRobotStatusCallback(LPMYBS_ROBOT_STATUS_CALLBACK pCallbackFunc, void* pUserData);

PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_SetConfig(PMYLoginHandle LoginHandle, const char* pszKey, char* pszValue);
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_GetConfig(PMYLoginHandle LoginHandle, const char* pszKey, char* pszValue, int* pnValueLength);
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_ReStartService(PMYLoginHandle LoginHandle);
PMYBS_SDK_API bool PMYBS_SDK_APICALL PMYBS_CheckSvcIsOnline(PMYLoginHandle LoginHandle);

typedef void (PMYBS_SDK_APICALL *LPMYBS_DYNAMIC_CALLBACK)(PMYLoginHandle LoginHandle, const char* pszBuffer, void* pUserData);
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_SetDynamicCallback(LPMYBS_DYNAMIC_CALLBACK pCallbackFunc, void* pUserData);

typedef void (PMYBS_SDK_APICALL *LPMYBS_EVENT_CALLBACK)(PMYLoginHandle LoginHandle, int nMajorEvent, int nMinorEvent, const char* pEventStr, void* pUserData);
PMYBS_SDK_API int PMYBS_SDK_APICALL PMYBS_SetEventCallback(LPMYBS_EVENT_CALLBACK pCallbackFunc, void* pUserData);

/*  Error Code Start  */
#define 	PMYBS_SUCCESS                                         0

//PMY SDK错误码
#define 	PMYBS_SDK_CONNECT_FAIL                                11000 //连接刻录服务失败，需确认刻录服务IP端口是否正确，并保证网络通畅
#define 	PMYBS_SDK_PUBLISHER_RUN_FAIL                          11001 //初始化接收服务数据推送模块失败
#define 	PMYBS_SDK_EXCEPTION                                   11002 //异常错误
#define 	PMYBS_SDK_INVALID_PARAMETER                           11003 //无效参数
#define		PMYBS_SDK_PARSE_RESPONSE_BODY_FAIL                    11004 //解析返回包体失败
#define		PMYBS_SDK_INVALID_SYSCONFIG_KEY                       11005 //无效的系统配置项
#define 	PMYBS_SDK_OUTPUT_BUFFER_SMALL                         11006 //输出Buffer长度太小，一般用于获取输出Buffer长度；
#define		PMYBS_SDK_TASK_TYPE_NOT_SUPPORT                       11007 //不支持该类型的任务
#define		PMYBS_SDK_TASK_NOT_FOUND                              11008 //未找到任务
#define 	PMYBS_SDK_FILE_NOT_EXIST                              11009 //文件不存在
#define 	PMYBS_SDK_FILE_OPEN_FAIL                              11010 //文件打开失败
#define 	PMYBS_SDK_ALREADY_LOGIN                               11011 //已登录
#define 	PMYBS_SDK_NOT_LOGIN                                   11012 //未登录
#define 	PMYBS_SDK_UPLOAD_NET_INIT_FAIL                        11013 //文件传输网络初始化失败
#define 	PMYBS_SDK_NO_BURNING_FILES_IN_THE_TASK                11014 //任务中未添加刻录文件
#define 	PMYBS_SDK_TASK_JSON_TOO_LONG                          11015 //任务数据 Buffer 太长（一般是添加的文件太多导致）
#define 	PMYBS_SDK_TASK_CLOSE_FAIL_UPLOADING                   11016 //任务关闭失败，刻录源传输中
/*  Error Code End  */

/*CreateTask TaskType*/
#define TASK_TYPE_UNKNOW                  0
#define TASK_TYPE_FILE_BURN               1
#define TASK_TYPE_DISC_PRINT              2
#define TASK_TYPE_REAL_FILE_BURN          3

//Task Config Key
#define PMYBS_SDK_TASK_CFG_DISC_TYPE					"disc_type"
#define PMYBS_SDK_TASK_CFG_CLOSE_DISC					"close_disc"
#define PMYBS_SDK_TASK_CFG_DISC_LABEL					"disc_label"
#define PMYBS_SDK_TASK_CFG_TEST_BURN					"test_burn"
#define PMYBS_SDK_TASK_CFG_DISC_OUTPUT_MODE				"disc_output_mode"
#define PMYBS_SDK_TASK_CFG_DELETE_FILES					"delete_files"

#define PMYBS_SDK_TASK_CFG_COPY_COUNT					"copy_count"
#define PMYBS_SDK_TASK_CFG_BURN_SPEED					"burn_speed"
#define PMYBS_SDK_TASK_CFG_VERIFY_DISC					"verify_disc"
#define PMYBS_SDK_TASK_CFG_REJECT_IF_NOT_BLANK			"reject_if_not_blank"
#define PMYBS_SDK_TASK_CFG_BURN_PROOF					"burn_proof"
#define PMYBS_SDK_TASK_CFG_VNR_WRITE					"vnr_write"
#define PMYBS_SDK_TASK_CFG_SPLITE_DISC_ACCORD			"split_disc_accord"
#define PMYBS_SDK_TASK_CFG_SPLITE_DISC_UNIT				"split_disc_unit"
#define PMYBS_SDK_TASK_CFG_MIN_FILE_LAYER				"min_file_layer"
#define PMYBS_SDK_TASK_CFG_BURN_IMAGE					"burn_image"
#define PMYBS_SDK_TASK_CFG_BURN_ERROR_RETRY				"error_retry"
#define PMYBS_SDK_TASK_CFG_BURN_REAL_FINISH_DISC		"real_finish_disc"
#define PMYBS_SDK_TASK_CFG_BURN_REAL_FINISH_TASK		"real_finish_task"
#define PMYBS_SDK_TASK_CFG_BURN_REAL_BURN_STREAM		"burn_stream"

//config
#define PMYBS_SDK_CFG_VERSION					"version"
#define PMYBS_SDK_CFG_SET_DISC_TYPE				"set_disc_bin_type"
#define PMYBS_SDK_CFG_DISC_TYPE					"disc_bin_type"
#define PMYBS_SDK_CFG_PROC_CPU					"proc_cpu"
#define PMYBS_SDK_CFG_PROC_MEMORY				"proc_memory"
#define PMYBS_SDK_CFG_TOTAL_CPU					"total_cpu"
#define PMYBS_SDK_CFG_TOTAL_MEMORY				"total_memory"
#define PMYBS_SDK_CFG_CACHE_FREE_SIZE			"cache_free_size"
#define PMYBS_SDK_CFG_CACHE_TOTAL_SIZE			"cache_total_size"

//file type
#define PMYBS_BURN_FILE_FILE			0x01
#define PMYBS_BURN_FILE_TREE			0x02
#define PMYBS_BURN_FILE_DIRECTORY		0x04
#define PMYBS_BURN_FILE_IMAGE			0x08
#define PMYBS_BURN_FILE_ALLDISC			0x10

#define PMYBS_BURN_FILE_REAL			0x20
#define PMYBS_BURN_FILE_REAL_WRITING	0x40
#define PMYBS_BURN_FILE_REAL_FINISH		0x80


//光盘输出模式
#define DISC_OUTPUT_MODE_DEFAULT          0   //默认输出
#define DISC_OUTPUT_MODE_FRONT            200 //前置输出

//分盘依据
#define ASSIGN_ACCORD_FILE_SIZE           0
#define ASSIGN_ACCORD_FILE_NAME           1
#define ASSIGN_ACCORD_FILE_MODIFY_TIME    2

//分盘单位
#define ASSIGN_UNIT_SINGLE_FILE           0
#define ASSIGN_UNIT_FIRST_LEVEL_SUBDIR    1

//Disc Type
#define PMY_DISC_CD_700M                  1
#define PMY_DISC_DVD_4_7G                 2
#define PMY_DISC_DVD_DL_9_4G              3
#define PMY_DISC_BD_25G                   4
#define PMY_DISC_BD_50G                   5
#define PMY_DISC_BD_100G                  6
#define PMY_DISC_BD_128G                  7

//print

#define PRINT_VALUE_TASK_NAME					"$(TASK_NAME)"
#define PRINT_VALUE_BURN_START_TIME				"$(BURN_START_TIME)"
#define PRINT_VALUE_BURN_END_TIME				"$(BURN_END_TIME)"
#define PRINT_VALUE_DISC_INDEX					"$(DISC_INDEX)"
#define PRINT_VALUE_DISC_COUNT					"$(DISC_COUNT)"
#define PRINT_VALUE_DISC_LABEL					"$(DISC_LABEL)"
#define PRINT_VALUE_DISC_SN						"$(DISC_SN)"
#define PRINT_VALUE_BURN_SIZE					"$(BURN_SIZE)"

//Robot System State
#define SYSSTATE_IDLE   0
#define SYSSTATE_BUSY   1
#define SYSSTATE_ERROR  2 

//Robot Device Type
#define ROBOT_DS20        11
#define ROBOT_DS4200      12
#define ROBOT_DS100       13
#define ROBOT_DS4052      14

//Robot Cmd
#define ROBOT_CMD_PTACT_ALIGNPRINTER			0x00000001
#define ROBOT_CMD_PTACT_IGNOREINKLOW			0x00000002
#define ROBOT_CMD_PTACT_DISABLEPWRBUTTON		0x00000004
#define ROBOT_CMD_PTACT_REINIT_DRIVES			0x00000008
#define ROBOT_CMD_PTACT_IDENTIFY				0x00000010
#define ROBOT_CMD_PTACT_CANCELCMD				0x00000020
#define ROBOT_CMD_PTACT_ENABLEPWRBUTTON			0x00000040
#define ROBOT_CMD_PTACT_RESETSYSTEM				0x00000080
#define ROBOT_CMD_PTACT_CHECKDISCS				0x00000100
#define ROBOT_CMD_PTACT_CLEANCARTRIDGES			0x00000200
#define ROBOT_CMD_PTACT_CALIBRATE_ONE_DISC		0x00000400
#define ROBOT_CMD_PTACT_CHANGE_CARTRIDGE		0x00000800
#define ROBOT_CMD_PTACT_END_CARTRIDGE_CHANGE	0x00001000
#define ROBOT_CMD_PTACT_SHIP_POSITION			0x00002000
#define ROBOT_CMD_PTACT_ALLOW_NO_CARTRIDGES		0x00010000

//Dynamic
//type
#define LOG_TYPE		0
#define LOG_TYPE_TASK	1
#define LOG_TYPE_ROBOT	2

//level
#define LOG_LEVEL			0
#define LOG_LEVEL_INFO		1
#define LOG_LEVEL_WARING	2
#define LOG_LEVEL_ERROR		3

//Event type
//Event Major
#define PMYBS_EVENT_MAJOR_TASK				1001
#define PMYBS_EVENT_MAJOR_DEVICE			1002
#define PMYBS_EVENT_MAJOR_BURN_DISC			1003
#define PMYBS_EVENT_MAJOR_PRINT_DISC		1004
#define PMYBS_EVENT_MAJOR_TRANSFER_FILE		1005
#define PMYBS_EVENT_MAJOR_NETWORK			1006

//PMYBS_EVENT_MAJOR_TASK 1001
#define PMYBS_EVENT_MINOR_TASK_WAITING		0
#define PMYBS_EVENT_MINOR_TASK_START		1
#define PMYBS_EVENT_MINOR_TASK_FINISH		2
#define PMYBS_EVENT_MINOR_TASK_ERROR		3
#define PMYBS_EVENT_MINOR_TASK_CANCLE		4

//PMYBS_EVENT_MAJOR_DEVICE 1002
#define PMYBS_EVENT_MINOR_DEVICE_REMOVE		0
#define PMYBS_EVENT_MINOR_DEVICE_INSERT		1
#define PMYBS_EVENT_MINOR_DEVICE_ERROR		2


//PMYBS_EVENT_MAJOR_BURN_DISC 1003
#define PMYBS_EVENT_MINOR_BURN_DISC_START					0
#define PMYBS_EVENT_MINOR_BURN_DISC_FINISH					1
#define PMYBS_EVENT_MINOR_BURN_DISC_ERROR					2
#define PMYBS_EVENT_MINOR_BURN_DISC_CANCLE					3

//PMYBS_EVENT_MAJOR_PRINT_DISC 1004
#define PMYBS_EVENT_MINOR_RINT_DISC_START		0
#define PMYBS_EVENT_MINOR_RINT_DISC_FINISH		1
#define PMYBS_EVENT_MINOR_RINT_DISC_ERROR		2
#define PMYBS_EVENT_MINOR_RINT_DISC_CANCLE		3

//PMYBS_EVENT_MAJOR_TRANSFER_FILE 1005
#define PMYBS_EVENT_MINOR_TRANSFER_START		0
#define PMYBS_EVENT_MINOR_TRANSFER_FINISH		1
#define PMYBS_EVENT_MINOR_TRANSFER_ERROR		2
#define PMYBS_EVENT_MINOR_TRANSFER_CANCLE		3

//PMYBS_EVENT_MAJOR_NETWORK 1006
#define PMYBS_EVENT_MINOR_NETWORK_DISCONNECT		0
#define PMYBS_EVENT_MINOR_NETWORK_RECONNECT_START	1
#define PMYBS_EVENT_MINOR_NETWORK_RECONNECT_FINISH	2

#ifdef __cplusplus
}
#endif
#endif
