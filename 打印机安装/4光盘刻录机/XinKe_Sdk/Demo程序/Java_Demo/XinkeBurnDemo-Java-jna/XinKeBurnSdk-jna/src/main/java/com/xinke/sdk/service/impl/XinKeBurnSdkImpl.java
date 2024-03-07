
package com.xinke.sdk.service.impl;

import java.util.HashMap;
import java.util.Map;

import com.xinke.sdk.service.*;
import com.xinke.sdk.utils.Constants;
import com.xinke.sdk.utils.ErrorCode;
import com.sun.jna.Memory;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;
import org.apache.commons.lang.StringUtils;

public class XinKeBurnSdkImpl implements XinKeBurnSdk{
	//static PMYBurnSdk hPMYBurnSDK = null;
	static PMYBurnSdk hPMYBurnSDK = Constants.INSTANCE;
	private boolean isLogin = false;
	private Pointer loginHandle;//登录句柄
	XinKeBurnCallBackListener callBackListener;
	EventCallBack eventCallBack;
	RobotEventCallBack robotEventCallBack;
	public XinKeBurnSdkImpl(XinKeBurnCallBackListener listener)
	{
		this.callBackListener = listener;
	}

	public Map<String, Object> PMYBS_Init()
	{
		//if (hPMYBurnSDK == null){
			//loadLibrary();
			if (hPMYBurnSDK == null){
				return GenResult(ErrorCode.PMYBS_SDK_LOAD_FAIL);
			}
		//}

		//设置JNA编码
		PMYBS_SetJNAEncode();
		//初始化SDK
		int nRet=hPMYBurnSDK.PMYBS_Init();
		
		return GenResult(nRet);
	}

	/* (non-Javadoc)
	 * @see com.pmy.service.XinKeBurnSdk#PMYBS_Destory()
	 */
	public Map<String, Object> PMYBS_Destory() {
		int nRet=hPMYBurnSDK.PMYBS_Destory();
		return GenResult(nRet);	
	}
	
	/**
	 * 设置JNA编码格式为UTF-8,防止中文乱码
	 */
	public void PMYBS_SetJNAEncode(){
		String jna_encode = System.getProperty("jna.encoding");
		System.setProperty("jna.encoding","UTF-8");
		jna_encode = System.getProperty("jna.encoding");
	}
	
	
	/* (non-Javadoc)
	 * @see com.pmy.service.XinKeBurnSdk#PMYBS_Login(java.lang.String, int)
	 */
	public Map<String, Object> PMYBS_Login(String serverIp, int port) {
		 Map<String, Object> result=new HashMap<String, Object>();
		if (port <= 0) {
			port = Constants.PMYBS_SDK_LOGIN_PORT;
		}
		
		if (StringUtils.isEmpty(serverIp)) {
			return GenResult(ErrorCode.PMYBS_SDK_NOT_LOGIN);
		}
		
		try {
			PointerByReference pointer = new PointerByReference(Pointer.NULL);
			int iLoginReturn = hPMYBurnSDK.PMYBS_Login(serverIp,port,pointer);
			
			if (iLoginReturn!=0) {
				return GenResult(ErrorCode.PMYBS_SDK_CONNECT_FAIL);
			}
			//回调
			setCallback();

            result= GenResult(iLoginReturn);
            loginHandle=pointer.getValue();
            isLogin=true;
           // result.put("LoginHandle",pointer.getValue());//登录句柄
            return result;
		}
		catch (Exception e) {
			System.out.println("PMYBS_Login exception: " + e.toString());
			return GenResult(ErrorCode.PMYBS_SDK_CONNECT_FAIL);
		}
	}

	public Map<String, Object> PMYBS_Logout() {
		if (isLogin == false) {
			return GenResult(ErrorCode.PMYBS_SDK_NOT_LOGIN);
		}
		
		int nRet = hPMYBurnSDK.PMYBS_Logout(loginHandle);
		if (nRet==0) {
			isLogin = false;
			System.out.println("logout success");
			return GenResult(nRet);
		} else {
			System.out.println("logout fail. error code:"+ nRet);
			return GenResult(ErrorCode.PMYBS_SDK_CONNECT_FAIL);
		}
	}
	
	public String PMYBS_GetErrorString(int errorCode) {

		String error=hPMYBurnSDK.PMYBS_GetErrorString(errorCode);
		return error;
	}
			
	public Map<String, Object> PMYBS_ExecuteRobotCmd(String robotName, int command) {
		
		if (isLogin == false) {
			return GenResult(ErrorCode.PMYBS_SDK_NOT_LOGIN);
		}
		
		int nRet=hPMYBurnSDK.PMYBS_ExecuteRobotCmd(loginHandle, robotName,command);
		if (nRet==0) {
			System.out.println("PMYBS_ExecuteRobotCmd success");
			return GenResult(nRet);
		} else {
			System.out.println("PMYBS_ExecuteRobotCmd fail. error code:"+ nRet);
			return GenResult(ErrorCode.PMYBS_SDK_CONNECT_FAIL);
		}
	}
	
	/***TASK****/
	public XinKeBurnTask PMYBS_CreateTask(int taskType, String taskName) {
		if (isLogin == false) {
			return null;
		}
		PointerByReference pTaskHandle = new PointerByReference(Pointer.NULL);
		int nRet=hPMYBurnSDK.PMYBS_CreateTask(loginHandle, taskType, taskName, pTaskHandle);
		if(nRet!=0){
			System.out.println("create task fail! nRet is:"+nRet); 
			return null;
		}

		//任务句柄
		return new XinKeBurnTaskImpl(pTaskHandle.getValue());
	}

	public Map<String, Object> PMYBS_CreateTask2(int taskType, String taskName, String taskId) {
		if (isLogin == false) {
			return GenResult(ErrorCode.PMYBS_SDK_NOT_LOGIN);
		}
		PointerByReference pTaskHandle = new PointerByReference(Pointer.NULL);
		int nRet=hPMYBurnSDK.PMYBS_CreateTask(loginHandle, taskType, taskName, pTaskHandle);
		if(nRet!=0){
			return GenResult(nRet);
		}

		if (StringUtils.isNotEmpty(taskId)){
			nRet=hPMYBurnSDK.PMYBS_SetTaskConfig(pTaskHandle.getValue(), "task_id", taskId);
			if(nRet!=0){
				return GenResult(nRet);
			}
		}

		Map<String, Object> map = new HashMap<String, Object>();
		map = GenResult(nRet);
		map.put("task", new XinKeBurnTaskImpl(pTaskHandle.getValue()));//任务句柄
		return map;
	}

	public Map<String, Object> PMYBS_OpenTask(String taskId) {
		if (isLogin == false) {
			return GenResult(ErrorCode.PMYBS_SDK_NOT_LOGIN);
		}
		PointerByReference pTaskHandle = new PointerByReference(Pointer.NULL);
		int nret=hPMYBurnSDK.PMYBS_OpenTask(loginHandle, taskId, pTaskHandle);
		if (nret != 0)
		{
			System.out.println("PMYBS_CancleTaskById fail. error code:"+ nret);
			return GenResult(nret);
		}

		Map<String, Object> map = new HashMap<String, Object>();
		map = GenResult(nret);
		map.put("task", new XinKeBurnTaskImpl(pTaskHandle.getValue()));
		return map;
	}


	public Map<String, Object> PMYBS_CancleTaskById(String taskId) {
		if (isLogin == false) {
			return GenResult(ErrorCode.PMYBS_SDK_NOT_LOGIN);
		}
		
		int nRet=hPMYBurnSDK.PMYBS_CancleTaskById(loginHandle,taskId);
		
		if (nRet==0) {
			System.out.println("PMYBS_CancleTaskById success");
			return GenResult(nRet);
		} else {
			System.out.println("PMYBS_CancleTaskById fail. error code:"+ nRet);
			return GenResult(ErrorCode.PMYBS_SDK_CONNECT_FAIL);
		}
	}
	

	public Map PMYBS_GetTaskStatus(String taskId) {
		if (isLogin == false) {
			return GenResult(ErrorCode.PMYBS_SDK_NOT_LOGIN);
		}
		String strXml="";
		Pointer pBuffer = new Memory(4096);
		IntByReference nBuffer = new IntByReference(4096);
		int nRet = hPMYBurnSDK.PMYBS_GetTaskStatus(loginHandle, taskId, pBuffer, nBuffer);
		if (nRet == Constants.PMYBS_SDK_OUTPUT_BUFFER_SMALL) {
			int newLen = nBuffer.getValue();
			pBuffer = new Memory(newLen);
			nRet = hPMYBurnSDK.PMYBS_GetTaskStatus(loginHandle, taskId, pBuffer, nBuffer);
		}
		
		if (nRet != 0) {
			return GenResult(nRet);
		}
		
		Map<String, Object> map = new HashMap<String, Object>();
		strXml = pBuffer.getString(0, "UTF-8");
		map=GenResult(nRet);
		map.put("status", strXml);
		return map;
	}

	public Map PMYBS_GetRobotStatus() {
		if (isLogin == false) {
			return GenResult(ErrorCode.PMYBS_SDK_NOT_LOGIN);
		}
		
		String strXml="";
		Pointer pBufferRobotStatus = new Memory(4096);
		IntByReference nBufferRobotStatus = new IntByReference(4096);
		int ret = hPMYBurnSDK.PMYBS_GetRobotStatus(loginHandle, pBufferRobotStatus, nBufferRobotStatus);
		if (Constants.PMYBS_SDK_OUTPUT_BUFFER_SMALL == ret) {
			int newLen = nBufferRobotStatus.getValue();
			pBufferRobotStatus = new Memory(newLen);
			ret = hPMYBurnSDK.PMYBS_GetRobotStatus(loginHandle,pBufferRobotStatus, nBufferRobotStatus);
		}
		
		if (ret != 0) {
			return GenResult(ret);
		}
		
		Map<String, Object> map = new HashMap<String, Object>();
		strXml = pBufferRobotStatus.getString(0, "UTF-8");
		map=GenResult(ret);
		map.put("status", strXml);
        return map;
	
	}

	public Map PMYBS_GetConfig(String param) {
		if (isLogin == false) {
			return GenResult(ErrorCode.PMYBS_SDK_NOT_LOGIN);
		}
		String strXml="";
		Pointer pBufferConfig = new Memory(4096);
		IntByReference nBufferConfig = new IntByReference(4096);
		int ret = hPMYBurnSDK.PMYBS_GetConfig(loginHandle, param,pBufferConfig, nBufferConfig);
		if (Constants.PMYBS_SDK_OUTPUT_BUFFER_SMALL == ret) {
			int newLen = nBufferConfig.getValue();
			pBufferConfig = new Memory(newLen);
			ret = hPMYBurnSDK.PMYBS_GetConfig(loginHandle, param,pBufferConfig, nBufferConfig);
		}

		if (ret != 0) {
			return GenResult(ret);
		}

		Map<String, Object> map = new HashMap<String, Object>();
		strXml = pBufferConfig.getString(0, "UTF-8");
		map=GenResult(ret);
		map.put("status", strXml);
		return map;
	}

	public boolean PMYBS_SetConfig(String key, String value) {
		if (isLogin == false) {
			return false;
		}
		int nret=hPMYBurnSDK.PMYBS_SetConfig(loginHandle, key,value);
		if(nret==0){
			System.out.println("PMYBS_SetConfig success!");
			return true;
		}
		System.out.println("PMYBS_SetConfig fail!nret="+nret+";key="+key+";value="+value);
		return false;
	}

	public Boolean CheckSvcIsOnline()
	{
		if (isLogin == false) {
			return false;
		}
		return hPMYBurnSDK.PMYBS_CheckSvcIsOnline(loginHandle);
	}

	public static Map<String, Object> GenResult(int error, String errorMsg){
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("errorCode", error);
		map.put("errorMsg", errorMsg);		
		return map;
	}

	public Boolean RestartSvc()
	{
		if (isLogin == false) {
			return false;
		}
		int nRet= hPMYBurnSDK.PMYBS_ReStartService(loginHandle);
		if (nRet==0) {
			System.out.println("ReStartService success");
			return true;
		} else {
			System.out.println("ReStartService fail. error code:"+ nRet);
			return false;
		}
	}
	
	public static Map<String, Object> GenResult(int error){
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("errorCode", error);
		map.put("errorMsg", hPMYBurnSDK.PMYBS_GetErrorString(error));		
		return map;
	}

	public void setCallback(){
		eventCallBack = new EventCallBack();		//状态回调
		hPMYBurnSDK.PMYBS_SetTaskStatusCallback(eventCallBack, Pointer.NULL);

		robotEventCallBack=new RobotEventCallBack();
		hPMYBurnSDK.PMYBS_SetRobotStatusCallback(robotEventCallBack, Pointer.NULL);
	}

	public class EventCallBack implements PMYBurnSdk.FPMYBS_TASK_STATUS_CALLBACK
	{
		///任务回调函数
		public void invoke(Pointer LoginHandle, String pszBuffer, Pointer pUserData)
		{
			XinKeBurnSdkImpl.this.callBackListener.taskCallBack(pszBuffer);

		}
	}


	public class RobotEventCallBack implements PMYBurnSdk.FPMYBS_ROBOT_STATUS_CALLBACK
	{
		///设备回调函数
		public void invoke(Pointer LoginHandle, String pszBuffer, Pointer pUserData)
		{
			XinKeBurnSdkImpl.this.callBackListener.robotCallBack(pszBuffer);

		}
	}

//	public static void main(String[] args) {
////		VideoBurnServiceImpl vBurnService=new VideoBurnServiceImpl();
////		int loginReturn=vBurnService.login("127.0.0.1");
////		System.out.println("loginReturn="+loginReturn);
//		
//	}
}
