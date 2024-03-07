package com.xinke.sdk.service;

import com.sun.jna.Callback;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

import java.util.Arrays;
import java.util.List;


public interface PMYBurnSdk extends Library {

	//windows环境下加载库
	//PMYBurnSdk INSTANCE = Native.load("PMYBurnSvcSdk64", PMYBurnSdk.class);
	//PMYBurnSdk INSTANCE = Native.load("PMYBurnSvcSdk", PMYBurnSdk.class);
	// PMYBurnSdk INSTANCE = Native.load("PMYBurnSdk", PMYBurnSdk.class);//32λ

	//初始化SDK
	int PMYBS_Init();

	//释放SDK资源
	int PMYBS_Destory();

	//登录服务器
	int PMYBS_Login(String pServerIp, int nPort, PointerByReference pLoginHandle);

	//登出服务器
	int PMYBS_Logout(Pointer LoginHandle);

	String PMYBS_GetErrorString(int nErrorNum);

	//执行机器指令
	int PMYBS_ExecuteRobotCmd(Pointer LoginHandle, String szRobotName, int ulCmd);

	//新建任务
	int PMYBS_CreateTask(Pointer LoginHandle, int nTaskType, String pszTaskName, PointerByReference pTaskHandle);

	int PMYBS_OpenTask(Pointer LoginHandle, String pszTaskId,  PointerByReference pTaskHandle);

	//添加文件到实时流刻录任务中
	int PMYBS_AddFileEx(Pointer TaskHandle, String pszFilePath, int nFlag, String pszPathInDisc);

	//添加文件到刻录任务运行中
	int PMYBS_AddFile(Pointer TaskHandle, String pszFilePath, String pszPathInDisc);

	//添加文件夹（包含子文件）到刻录任务中
	int PMYBS_AddTree(Pointer TaskHandle, String pszDirPath, String pszPathInDisc);

	//添加空文件夹
	int PMYBS_AddDirectory(Pointer TaskHandle, String pEmptyDirPath);

	//添加镜像文件到刻录任务运行中
	int PMYBS_AddImageFile(Pointer TaskHandle, String pszFilePath);

	//添加刻录任务运行模板(std或者jep/jpeg)路径
	int PMYBS_SetPrintFile(Pointer TaskHandle, String pszSusPrintFilePath, String pszFailPrintFilePath);

	//刻录多份时可设置不同的打印模板 nIndex 为份数索引 从0开始
	int PMYBS_SetPrintFile2(Pointer TaskHandle, String pszSusPrintFilePath, String pszFailPrintFilePath, int nCopyIndex);

	//添加刻录任务运行打印字段
	// value使用通配符
	int PMYBS_SetPrintField(Pointer TaskHandle, int nIndex, String pszValue);

	//设置任务运行配置
	int PMYBS_SetTaskConfig(Pointer TaskHandle, String pszKey, String pszValue);

	//提交任务运行
	int PMYBS_SubmitTask(Pointer TaskHandle, TASK_ID ptTaskId);

	//关闭任务运行
	int PMYBS_CloseTask(Pointer TaskHandle);

	int PMYBS_CreateTaskByJson(Pointer LoginHandle, String pszJsonText, TASK_ID ptTaskId);

	//取消任务运行
	int PMYBS_CancleTaskById(Pointer LoginHandle, String pszTaskId);

	//获取刻录任务状态
	int PMYBS_GetTaskStatus(Pointer LoginHandle, String pszTaskId, Pointer pszBuffer, IntByReference pnBufferLength);

	int PMYBS_SetConfig(Pointer LoginHandle, String pszKey, String pszValue);

	int PMYBS_GetConfig(Pointer LoginHandle,  String pszKey, Pointer pszBuffer, IntByReference pnValueLength);

	int PMYBS_ReStartService(Pointer LoginHandle);

	boolean PMYBS_CheckSvcIsOnline(Pointer LoginHandle);

	//刻录任务状态回调
	public static interface FPMYBS_TASK_STATUS_CALLBACK extends Callback {
		public void invoke(Pointer LoginHandle, String pszBuffer, Pointer pUserData);
	}
	//设置刻录任务状态更新回调
	int PMYBS_SetTaskStatusCallback(Callback pCallbackFunc, Pointer pUserData);

	//获取刻录机硬件状态
	int PMYBS_GetRobotStatus(Pointer LoginHandle, Pointer pszBuffer, IntByReference pnBufferLength);

	//设置刻录机硬件状态更新回调
	public static interface FPMYBS_ROBOT_STATUS_CALLBACK extends Callback {
		public void invoke(Pointer LoginHandle, String pszBuffer, Pointer pUserData);
	}

	int PMYBS_SetRobotStatusCallback(Callback pCallbackFunc, Pointer pUserData);

	public class TASK_ID  extends Structure{
		public  byte[] strTaskId = new byte[40];

		@Override
		protected List<String> getFieldOrder() {
			return Arrays.asList(new String[]{"strTaskId"});
		}
	}

}
