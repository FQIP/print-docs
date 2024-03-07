
package com.xinke.sdk.service;

import java.util.Map;

/** 
* <p>Title: XinKeBurnSdk</p>  
* <p>Description: sdk接口类</p>  
* @author bing
*/ 
public interface XinKeBurnSdk{
	
	/**  
	 * <p>Title: PMYBS_Init</p>  
	 * <p>Description:初始化sdk </p>  
	 * @return      
	 */
	public Map<String, Object> PMYBS_Init();

	/**  
	 * <p>Title: PMYBS_Destory</p>  
	 * <p>Description: 销毁SDK</p>  
	 * @return      
	 */
	public Map<String, Object> PMYBS_Destory();
	
	/**  
	 * <p>Title: PMYBS_Login</p>  
	 * <p>Description: 登录刻录服务</p>  
	 * @param serverIp 刻录服务IP
	 * @param port 刻录服务端口号
	 * @return      
	 */
	public Map<String, Object> PMYBS_Login(String serverIp, int port);

	/**  
	 * <p>Title: PMYBS_Logout</p>  
	 * <p>Description: </p>  
	 * @return      
	 */
	public Map<String, Object> PMYBS_Logout();
	
	/**  
	 * <p>Title: PMYBS_ExecuteRobotCmd</p>  
	 * <p>Description: </p>  
	 * @param robotName
	 * @param command
	 * @return      
	 */
	public Map<String, Object> PMYBS_ExecuteRobotCmd(String robotName, int command);

	/**  
	 * <p>Title: PMYBS_CreateTask</p>  
	 * <p>Description: </p>  
	 * @param taskType 任务类型
	 * @param taskName 任务名称
	 * @return      
	 */
	public XinKeBurnTask PMYBS_CreateTask(int taskType, String taskName);

	/**
	 * 创建任务
	 * @param taskType
	 * @param taskName
	 * @param taskId
	 * @return
	 */
	public Map<String, Object> PMYBS_CreateTask2(int taskType, String taskName, String taskId) ;

	/**
	 * <p>Title: PMYBS_OpenTask</p>
	 * <p>Description: </p>
	 * @param taskId 任务ID
	 * @param taskId
	 * @return
	 */
	public Map<String, Object> PMYBS_OpenTask(String taskId);


	/**  
	 * <p>Title: PMYBS_CancleTaskById</p>  
	 * <p>Description: </p>  
	 * @param taskId 任务ID
	 * @return      
	 */
	public Map<String, Object> PMYBS_CancleTaskById(String taskId);
	
	/**  
	 * <p>Title: PMYBS_GetTaskStatus</p>  
	 * <p>Description: </p>  
	 * @param taskId 任务ID
	 * @return      
	 */
	public Map<String, Object> PMYBS_GetTaskStatus(String taskId);

	/**  
	 * <p>Title: PMYBS_GetRobotStatus</p>  
	 * <p>Description: </p>  
	 * @return      
	 */
	public Map<String, Object> PMYBS_GetRobotStatus();
	
	/**  
	 * <p>Title: PMYBS_GetErrorString</p>  
	 * <p>Description: </p>  
	 * @param errorCode 错误码
	 * @return      
	 */
	public String PMYBS_GetErrorString(int errorCode);

	/**
	 * 获取系统设置
	 * @return
	 */
	public Map<String, Object> PMYBS_GetConfig(String param);

	/**
	 *<p>Title: PMYBS_SetConfig</p>
	 *<p>Description: </p>
	 * @param key
	 * @param value
	 * @return
	 */
	public boolean PMYBS_SetConfig(String key, String value);

	/**
	 * 服务连接状态
	 * @return
	 */
	public Boolean CheckSvcIsOnline();

	/**
	 * 重启服务
	 * @return
	 */
	public Boolean RestartSvc();

}
