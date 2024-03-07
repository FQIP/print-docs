#include "stdafx.h"
#include "PMYBurnSDKFunc.h"


CPMYBurnSDKFunc::CPMYBurnSDKFunc(void)
	: m_bInit(false)
	, _PMYBS_Init(NULL)
	, _PMYBS_Destory(NULL)
	, _PMYBS_Login(NULL)
	, _PMYBS_Logout(NULL)
	, _PMYBS_GetErrorString(NULL)
	, _PMYBS_ExecuteRobotCmd(NULL)
	, _PMYBS_CreateTaskByJson(NULL)
	, _PMYBS_CreateTask(NULL)
	, _PMYBS_AddFileEx(NULL)
	, _PMYBS_AddFile(NULL)
	, _PMYBS_AddTree(NULL)
	, _PMYBS_AddDirectory(NULL)
	, _PMYBS_SetPrintFile(NULL)
	, _PMYBS_SetPrintFile2(NULL)
	, _PMYBS_SetPrintField(NULL)
	, _PMYBS_SetTaskConfig(NULL)
	, _PMYBS_SubmitTask(NULL)
	, _PMYBS_CancleTaskById(NULL)
	, _PMYBS_CloseTask(NULL)
	, _PMYBS_GetTaskStatus(NULL)
	, _PMYBS_SetTaskStatusCallback(NULL)
	, _PMYBS_GetRobotStatus(NULL)
	, _PMYBS_SetRobotStatusCallback(NULL)
	, _PMYBS_SetDynamicCallback(NULL)
	, _PMYBS_OpenTask(NULL)
	, _PMYBS_GetConfig(NULL)
	, _PMYBS_SetConfig(NULL)
	, _PMYBS_ReStartService(NULL)
	, _PMYBS_CheckSvcIsOnline(NULL)
	, _PMYBS_SetEventCallback(NULL)
{

}


CPMYBurnSDKFunc::~CPMYBurnSDKFunc(void)
{
}

CPMYBurnSDKFunc& CPMYBurnSDKFunc::Instance()
{
	static CPMYBurnSDKFunc instanceObj;
	if (instanceObj.m_bInit == false)
	{
		instanceObj.m_bInit = instanceObj.Init();
	}
	return instanceObj;
}

void CPMYBurnSDKFunc::Release()
{
	if (m_bInit && _PMYBS_Destory != NULL)
		_PMYBS_Destory();
}

bool CPMYBurnSDKFunc::Init()
{
	if (m_bInit)
		return true;
	std::string strDllFile;

#ifdef QT_NO_DEBUG
#ifdef Q_OS_WIN32
    strDllFile = QCoreApplication::applicationDirPath().toStdString() + "/PMYBurnSvcSdk.dll";
#elif defined (Q_OS_LINUX)
    strDllFile = QCoreApplication::applicationDirPath().toStdString() + "/libPMYBurnSvcSdk.so";
#endif

#else
#ifdef Q_OS_WIN32
    strDllFile = QCoreApplication::applicationDirPath().toStdString() + "/PMYBurnSvcSdkd.dll";
#elif defined (Q_OS_LINUX)
    strDllFile = QCoreApplication::applicationDirPath().toStdString() + "/libPMYBurnSvcSdkd.so";
#endif
#endif

    QLibrary lib(strDllFile.c_str());
    if (lib.load() == false)
    {
        return false;
    }

    GET_SDK_PROC_ADDRESS(lib, PMYBS_Init);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_Destory);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_Login);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_Logout);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_GetErrorString);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_ExecuteRobotCmd);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_CreateTaskByJson);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_CreateTask);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_OpenTask);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_AddFileEx);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_AddFile);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_AddTree);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_AddDirectory);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_SetPrintFile);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_SetPrintFile2);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_SetPrintField);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_SetTaskConfig);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_SubmitTask);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_CancleTaskById);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_CloseTask);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_GetTaskStatus);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_SetTaskStatusCallback);
    GET_SDK_PROC_ADDRESS(lib, PMYBS_GetRobotStatus);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_SetRobotStatusCallback);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_GetConfig);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_SetConfig);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_ReStartService);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_CheckSvcIsOnline);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_SetDynamicCallback);
	GET_SDK_PROC_ADDRESS(lib, PMYBS_SetEventCallback);

	int nRet = _PMYBS_Init();
	if (nRet != PMYBS_SUCCESS)
	{
		return false;
	}

	return true;
}
