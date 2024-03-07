#pragma once

#include "PMYBurnSvcSdk.h"

#define TYPEDEF_SDK_FUNC_CONST_CHAR(funcName)  typedef const char* (API_STDCALL* Func_##funcName)
#define TYPEDEF_SDK_FUNC_INT(funcName)  typedef int (API_STDCALL* Func_##funcName)
#define TYPEDEF_SDK_FUNC_BOOL(funcName)  typedef bool (API_STDCALL* Func_##funcName)
#define TYPEDEF_SDK_FUNC_INT_CDECL(funcName)  typedef int (API_CDECL* Func_##funcName)
#define DECLARE_SDK_FUNC(funcName)	Func_##funcName _##funcName
//#define GET_SDK_PROC_ADDRESS(hModule, funcName) _##funcName = (Func_##funcName)PMYSdk::Library::getSymbol(hModule, #funcName); if(_##funcName == NULL) return false;
#define GET_SDK_PROC_ADDRESS(hModule, funcName) _##funcName = (Func_##funcName)hModule.resolve(#funcName); if(_##funcName == NULL) return false;


TYPEDEF_SDK_FUNC_INT (PMYBS_Init)();
TYPEDEF_SDK_FUNC_INT (PMYBS_Destory)();
TYPEDEF_SDK_FUNC_INT (PMYBS_Login)(const char* pszServerIp, int nPort, PMYLoginHandle* pLoginHandle);
TYPEDEF_SDK_FUNC_INT (PMYBS_Logout)(PMYLoginHandle LoginHandle);

TYPEDEF_SDK_FUNC_CONST_CHAR (PMYBS_GetErrorString)(int nErrorNum);

TYPEDEF_SDK_FUNC_INT (PMYBS_ExecuteRobotCmd)(PMYLoginHandle LoginHandle, const char* szRobotName, unsigned long ulCmd);
TYPEDEF_SDK_FUNC_INT (PMYBS_CreateTaskByJson)(PMYLoginHandle LoginHandle, const char* pszJsonText, TASK_ID* ptTaskId);
TYPEDEF_SDK_FUNC_INT (PMYBS_CreateTask)(PMYLoginHandle LoginHandle, int nTaskType, const char* pszTaskName, PMYTaskHandle* pTaskHandle);
TYPEDEF_SDK_FUNC_INT (PMYBS_OpenTask)(PMYLoginHandle LoginHandle, const char* pszTaskId, PMYTaskHandle* pTaskHandle);
TYPEDEF_SDK_FUNC_INT (PMYBS_AddFileEx)(PMYTaskHandle TaskHandle, const char* pszFilePath, int nFlag, const char* pszPathInDisc/* = NULL*/);
TYPEDEF_SDK_FUNC_INT (PMYBS_AddFile)(PMYTaskHandle TaskHandle, const char* pszFilePath, const char* pszPathInDisc /*= NULL*/);
TYPEDEF_SDK_FUNC_INT (PMYBS_AddTree)(PMYTaskHandle TaskHandle, const char* pszDirPath, const char* pszPathInDisc /*= NULL*/);
TYPEDEF_SDK_FUNC_INT (PMYBS_AddDirectory)(PMYTaskHandle TaskHandle, const char* pEmptyDirPath);
TYPEDEF_SDK_FUNC_INT (PMYBS_SetPrintFile)(PMYTaskHandle TaskHandle, const char* pszSusPrintFilePath, const char* pszFailPrintFilePath);
TYPEDEF_SDK_FUNC_INT (PMYBS_SetPrintFile2)(PMYTaskHandle TaskHandle, const char* pszSusPrintFilePath, const char* pszFailPrintFilePath, int nCopyIndex);
TYPEDEF_SDK_FUNC_INT (PMYBS_SetPrintField)(PMYTaskHandle TaskHandle, int nIndex, const char* pszValue /*= NULL*/);
TYPEDEF_SDK_FUNC_INT (PMYBS_SetTaskConfig)(PMYTaskHandle TaskHandle, const char* pszKey, const char* pszValue);
TYPEDEF_SDK_FUNC_INT (PMYBS_SubmitTask)(PMYTaskHandle TaskHandle, TASK_ID* ptTaskId);
TYPEDEF_SDK_FUNC_INT (PMYBS_CancleTaskById)(PMYLoginHandle LoginHandle, const char* pszTaskId);
TYPEDEF_SDK_FUNC_INT (PMYBS_CloseTask)(PMYTaskHandle TaskHandle);
TYPEDEF_SDK_FUNC_INT (PMYBS_GetTaskStatus)(PMYLoginHandle LoginHandle, const char* pszTaskId, char* pszBuffer, int* pnBufferLength);
TYPEDEF_SDK_FUNC_INT (PMYBS_SetTaskStatusCallback)(LPMYBS_TASK_STATUS_CALLBACK pCallbackFunc, void* pUserData);
TYPEDEF_SDK_FUNC_INT (PMYBS_GetRobotStatus)(PMYLoginHandle LoginHandle, char* pszBuffer, int* pnBufferLength);
TYPEDEF_SDK_FUNC_INT (PMYBS_SetRobotStatusCallback)(LPMYBS_ROBOT_STATUS_CALLBACK pCallbackFunc, void* pUserData);
TYPEDEF_SDK_FUNC_INT (PMYBS_GetConfig)(PMYLoginHandle LoginHandle, const char* pszKey, char* pszValue, int* pnValueLength);
TYPEDEF_SDK_FUNC_INT (PMYBS_SetConfig)(PMYLoginHandle LoginHandle, const char* pszKey, char* pszValue);
TYPEDEF_SDK_FUNC_INT (PMYBS_ReStartService)(PMYLoginHandle LoginHandle);
TYPEDEF_SDK_FUNC_BOOL (PMYBS_CheckSvcIsOnline)(PMYLoginHandle LoginHandle);
TYPEDEF_SDK_FUNC_INT (PMYBS_SetDynamicCallback)(LPMYBS_DYNAMIC_CALLBACK pCallbackFunc, void* pUserData);
TYPEDEF_SDK_FUNC_INT (PMYBS_SetEventCallback)(LPMYBS_EVENT_CALLBACK pCallbackFunc, void* pUserData);

class CPMYBurnSDKFunc
{
public:
	CPMYBurnSDKFunc(void);
	~CPMYBurnSDKFunc(void);

public:
	bool Init();
	static CPMYBurnSDKFunc &Instance();
	void Release();
protected:
	bool m_bInit;

public:

	DECLARE_SDK_FUNC(PMYBS_Init);
	DECLARE_SDK_FUNC(PMYBS_Destory);
	DECLARE_SDK_FUNC(PMYBS_Login);
	DECLARE_SDK_FUNC(PMYBS_Logout);
	DECLARE_SDK_FUNC(PMYBS_GetErrorString);
	DECLARE_SDK_FUNC(PMYBS_ExecuteRobotCmd);
	DECLARE_SDK_FUNC(PMYBS_CreateTaskByJson);
	DECLARE_SDK_FUNC(PMYBS_CreateTask);
	DECLARE_SDK_FUNC(PMYBS_OpenTask);
	DECLARE_SDK_FUNC(PMYBS_AddFileEx);
	DECLARE_SDK_FUNC(PMYBS_AddFile);
	DECLARE_SDK_FUNC(PMYBS_AddTree);
	DECLARE_SDK_FUNC(PMYBS_AddDirectory);
	DECLARE_SDK_FUNC(PMYBS_SetPrintFile);
	DECLARE_SDK_FUNC(PMYBS_SetPrintFile2);
	DECLARE_SDK_FUNC(PMYBS_SetPrintField);
	DECLARE_SDK_FUNC(PMYBS_SetTaskConfig);
	DECLARE_SDK_FUNC(PMYBS_SubmitTask);
	DECLARE_SDK_FUNC(PMYBS_CancleTaskById);
	DECLARE_SDK_FUNC(PMYBS_CloseTask);
	DECLARE_SDK_FUNC(PMYBS_GetTaskStatus);
	DECLARE_SDK_FUNC(PMYBS_SetTaskStatusCallback);
	DECLARE_SDK_FUNC(PMYBS_GetRobotStatus);
	DECLARE_SDK_FUNC(PMYBS_SetRobotStatusCallback);
	DECLARE_SDK_FUNC(PMYBS_GetConfig);
	DECLARE_SDK_FUNC(PMYBS_SetConfig);
	DECLARE_SDK_FUNC(PMYBS_ReStartService);
	DECLARE_SDK_FUNC(PMYBS_CheckSvcIsOnline);
	DECLARE_SDK_FUNC(PMYBS_SetDynamicCallback);
	DECLARE_SDK_FUNC(PMYBS_SetEventCallback);
};

inline CPMYBurnSDKFunc &SDKFunc()
{
	return CPMYBurnSDKFunc::Instance();
}

