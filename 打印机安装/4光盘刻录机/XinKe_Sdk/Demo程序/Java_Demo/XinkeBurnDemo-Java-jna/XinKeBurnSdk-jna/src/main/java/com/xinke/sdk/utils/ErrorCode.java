package com.xinke.sdk.utils;

import java.util.HashMap;
import java.util.Map;

public class ErrorCode {

	/*  Error Code Start  */
	public static int 	PMYBS_SUCCESS                                       =   0;
	//PMY SDK错误码
	public static int 	PMYBS_SDK_CONNECT_FAIL                              =   11000; //连接刻录服务失败，需确认刻录服务IP端口是否正确，并保证网络通畅
	public static int 	PMYBS_SDK_PUBLISHER_RUN_FAIL                        =   11001; //初始化接收服务数据推送模块失败
	public static int 	PMYBS_SDK_EXCEPTION                                 =   11002; //异常错误
	public static int 	PMYBS_SDK_INVALID_PARAMETER                         =   11003; //无效参数
	public static int	PMYBS_SDK_PARSE_RESPONSE_BODY_FAIL                  =   11004; //解析返回包体失败
	public static int	PMYBS_SDK_INVALID_SYSCONFIG_KEY                     =   11005; //无效的系统配置项
	public static int 	PMYBS_SDK_OUTPUT_BUFFER_SMALL                       =   11006; //输出Buffer长度太小，一般用于获取输出Buffer长度；
	public static int	PMYBS_SDK_TASK_TYPE_NOT_SUPPORT                     =   11007; //不支持该类型的任务
	public static int	PMYBS_SDK_TASK_NOT_FOUND                            =   11008; //未找到任务
	public static int 	PMYBS_SDK_FILE_NOT_EXIST                            =   11009; //文件不存在
	public static int 	PMYBS_SDK_FILE_OPEN_FAIL                            =   11010; //文件打开失败
	public static int 	PMYBS_SDK_ALREADY_LOGIN                             =   11011; //已登录
	public static int 	PMYBS_SDK_NOT_LOGIN                                 =   11012; //未登录
	public static int 	PMYBS_SDK_UPLOAD_NET_INIT_FAIL        				=   11013;//文件传输网络初始化失败
	public static int 	PMYBS_SDK_NO_BURNING_FILES_IN_THE_TASK				= 	11014;//任务中未添加刻录文件
	public static int 	PMYBS_SDK_TASK_JSON_TOO_LONG              			= 	11015; //任务数据 Buffer 太长（一般是添加的文件太多导致）
	public static int 	PMYBS_SDK_TASK_CLOSE_FAIL_UPLOADING       			= 	11016; //任务关闭失败，刻录源传输中
	public static int	PMYBS_SDK_LOAD_FAIL									=   11100; //底层动态库加载失败

	public static Map<Integer, String> StringTable;

	static{
		// 加载属性文件
		try {
			StringTable = new HashMap<Integer, String>();

			StringTable.put(PMYBS_SUCCESS, "");
			StringTable.put(PMYBS_SDK_CONNECT_FAIL, "连接刻录服务失败");
			StringTable.put(PMYBS_SDK_PUBLISHER_RUN_FAIL, "初始化接收服务数据推送模块失败");
			StringTable.put(PMYBS_SDK_EXCEPTION, "异常错误");
			StringTable.put(PMYBS_SDK_INVALID_PARAMETER, "无效参数");
			StringTable.put(PMYBS_SDK_PARSE_RESPONSE_BODY_FAIL, "解析返回包体失败");
			StringTable.put(PMYBS_SDK_INVALID_SYSCONFIG_KEY, "无效的系统配置项");
			StringTable.put(PMYBS_SDK_OUTPUT_BUFFER_SMALL, "输出Buffer长度太小");
			StringTable.put(PMYBS_SDK_TASK_TYPE_NOT_SUPPORT, "不支持该类型的任务");
			StringTable.put(PMYBS_SDK_TASK_NOT_FOUND, "未找到任务");
			StringTable.put(PMYBS_SDK_FILE_NOT_EXIST, "文件不存在");
			StringTable.put(PMYBS_SDK_FILE_OPEN_FAIL, "文件打开失败");
			StringTable.put(PMYBS_SDK_ALREADY_LOGIN, "已经登录");
			StringTable.put(PMYBS_SDK_NOT_LOGIN, "未登录");
			StringTable.put(PMYBS_SDK_UPLOAD_NET_INIT_FAIL, "文件传输网络初始化失败");
			StringTable.put(PMYBS_SDK_NO_BURNING_FILES_IN_THE_TASK, "任务中未添加刻录文件");
			StringTable.put(PMYBS_SDK_TASK_JSON_TOO_LONG, "任务数据 Buffer 太长（一般是添加的文件太多导致）");
			StringTable.put(PMYBS_SDK_TASK_CLOSE_FAIL_UPLOADING, "任务关闭失败，刻录源传输中");
			StringTable.put(PMYBS_SDK_LOAD_FAIL, "底层动态库加载失败");

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static String GetString(int id) {
		if (StringTable.containsKey(id)) {
			return StringTable.get(id);
		}
		else {
			return "ErrorCode: " + id;
		}
	}
}
