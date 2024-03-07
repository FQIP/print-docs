package com.xinke.sdk.utils;

import com.xinke.sdk.service.PMYBurnSdk;
import com.sun.jna.Native;
import org.apache.log4j.Logger;

public class Constants {
	public static int PMYBS_SDK_CUR_VERSION = 1;
	public static int PMYBS_SDK_LOGIN_PORT = 6000;

	/*CreateTask TaskType*/
	public static int TASK_TYPE_UNKNOW = 0;
	public static int TASK_TYPE_FILE_BURN = 1;
	public static int TASK_TYPE_DISC_PRINT = 2;
	public static int TASK_TYPE_REAL_FILE_BURN =3;

	//Task Config Key
	public static String PMYBS_SDK_TASK_CFG_DISC_TYPE 				= "disc_type";
	public static String PMYBS_SDK_TASK_CFG_BURN_NUM			 	= "copy_count";
	public static String PMYBS_SDK_TASK_CFG_BURN_SPEED			 	= "burn_speed";
	public static String PMYBS_SDK_TASK_CFG_DISC_LABEL 				= "disc_label";
	public static String PMYBS_SDK_TASK_CFG_DISC_OUTPUT_MODE 		= "disc_output_mode";
	public static String PMYBS_SDK_TASK_CFG_DELETE_CACHE 			= "delete_files";
	public static String PMYBS_SDK_TASK_CFG_CLOSE_DISC 				= "close_disc";// 刻录完成后关闭光盘1-是；0-否
	public static String PMYBS_SDK_TASK_CFG_VERIFY_DISC			 	= "verify_disc";
	public static String PMYBS_SDK_TASK_CFG_ANALOG_BURN	 			= "test_burn";//模拟刻录
	public static String PMYBS_SDK_TASK_CFG_BURN_PROOF				="burn_proof";
	public static String PMYBS_SDK_TASK_CFG_BDR						= "vnr_write";//是否开启BD-R校验，默认："0"
	public static String PMYBS_SDK_TASK_CFG_NOT_BLANK				= "reject_if_not_blank";
	public static String PMYBS_SDK_TASK_CFG_SPLIT_DISC_ACCORD		= "split_disc_accord";//分盘依据
	public static String PMYBS_SDK_TASK_CFG_SPLITE_DISC_UNIT		="split_disc_unit";
	public static String PMYBS_SDK_TASK_CFG_MIN_FILE_LAYER			="min_file_layer";
	public static String PMYBS_SDK_TASK_CFG_BURN_IMAGE				= "burn_image";//镜像文件
	public static String PMYBS_SDK_TASK_CFG_BURN_ERROR_RETRY		="error_retry";
	public static String PMYBS_SDK_TASK_CFG_BURN_REAL_FINISH_DISC	="real_finish_disc";
	public static String PMYBS_SDK_TASK_CFG_BURN_REAL_FINISH_TASK	="real_finish_task";
	public static String PMYBS_SDK_TASK_CFG_BURN_REAL_BURN_STREAM	="burn_stream";

	//file type
	public static int  PMYBS_BURN_FILE_FILE			=0x01;
	public static int  PMYBS_BURN_FILE_TREE			=0x02;
	public static int  PMYBS_BURN_FILE_DIRECTORY	=0x04;
	public static int  PMYBS_BURN_FILE_IMAGE		=0x08;
	public static int  PMYBS_BURN_FILE_ALLDISC		=0x10;
	public static int  PMYBS_BURN_FILE_REAL			=0x20;
	public static int  PMYBS_BURN_FILE_REAL_WRITING	=0x40;
	public static int  PMYBS_BURN_FILE_REAL_FINISH	=0x80;

	//task status
	public static int PMYBS_SDK_TASK_STATUS_CODE_FREE=0;//空闲
	public static int PMYBS_SDK_TASK_STATUS_CODE_WAITING=1;//等待运行
	public static int PMYBS_SDK_TASK_STATUS_CODE_RUNNING=2;//正在运行
	public static int PMYBS_SDK_TASK_STATUS_CODE_SUCCESS=3;//运行成功
	public static int PMYBS_SDK_TASK_STATUS_CODE_CANCEL=4;//任务取消
	public static int PMYBS_SDK_TASK_STATUS_CODE_FAIL=5;//运行失败

	//光盘输出模式 disc_output_mode
	public static int DISC_OUTPUT_MODE_DEFAULT = 0;//默认输出
	public static int DISC_OUTPUT_MODE_FRONT = 200;//前置输出

	//分盘依据 split_disc_accord
	public static int ASSIGN_ACCORD_FILE_SIZE           = 0;
	public static int ASSIGN_ACCORD_FILE_NAME           = 1;
	public static int ASSIGN_ACCORD_FILE_MODIFY_TIME    = 2;

	//分盘单位 split_disc_unit
	public static int ASSIGN_UNIT_SINGLE_FILE           = 0;
	public static int ASSIGN_UNIT_FIRST_LEVEL_SUBDIR    = 1;

	//print，sdkdemo用
	public static String PRINT_VALUE_TASK_NAME		 = "$(TASK_NAME)";
	public static String PRINT_VALUE_BURN_START_TIME = "$(BURN_START_TIME)";
	public static String PRINT_VALUE_BURN_END_TIME	 = "$(BURN_END_TIME)";
	public static String PRINT_VALUE_DISC_INDEX		 = "$(DISC_INDEX)";
	public static String PRINT_VALUE_DISC_COUNT		 = "$(DISC_COUNT)";
	public static String PRINT_VALUE_DISC_LABEL		 = "$(DISC_LABEL)";
	public static String PRINT_VALUE_DISC_SN		 = "$(DISC_SN)";
	public static String PRINT_VALUE_BURN_SIZE		 = "$(BURN_SIZE)";

	//Robot System State
	public static int  SYSSTATE_IDLE   = 0;
	public static int  SYSSTATE_BUSY   = 1;
	public static int  SYSSTATE_ERROR  = 2;

	//Robot Device Type
	public static int  ROBOT_DS20        = 11;
	public static int  ROBOT_DS4200      = 12;
	public static int  ROBOT_DS100       = 13;
	public static int  ROBOT_DS4052      = 14;

	//Robot Cmd
	public static int  ROBOT_CMD_PTACT_ALIGNPRINTER			=0x00000001;
	public static int  ROBOT_CMD_PTACT_IGNOREINKLOW			=0x00000002;
	public static int  ROBOT_CMD_PTACT_DISABLEPWRBUTTON		=0x00000004;
	public static int  ROBOT_CMD_PTACT_REINIT_DRIVES		=0x00000008;
	public static int  ROBOT_CMD_PTACT_IDENTIFY				=0x00000010;
	public static int  ROBOT_CMD_PTACT_CANCELCMD			=0x00000020;
	public static int  ROBOT_CMD_PTACT_ENABLEPWRBUTTON		=0x00000040;
	public static int  ROBOT_CMD_PTACT_RESETSYSTEM			=0x00000080;
	public static int  ROBOT_CMD_PTACT_CHECKDISCS			=0x00000100;
	public static int  ROBOT_CMD_PTACT_CLEANCARTRIDGES		=0x00000200;
	public static int  ROBOT_CMD_PTACT_CALIBRATE_ONE_DISC	=0x00000400;
	public static int  ROBOT_CMD_PTACT_CHANGE_CARTRIDGE		=0x00000800;
	public static int  ROBOT_CMD_PTACT_END_CARTRIDGE_CHANGE	=0x00001000;
	public static int  ROBOT_CMD_PTACT_SHIP_POSITION		=0x00002000;
	public static int  ROBOT_CMD_PTACT_ALLOW_NO_CARTRIDGES	=0x00010000;

	//Disc Type
	public static int PMY_DISC_CD_700M        = 1;
	public static int PMY_DISC_DVD_4_7G       = 2;
	public static int PMY_DISC_DVD_DL_9_4G    = 3;
	public static int PMY_DISC_BD_25G         = 4;
	public static int PMY_DISC_BD_50G         = 5;
	public static int PMY_DISC_BD_100G        = 6;
	public static int PMY_DISC_BD_128G        = 7;

	/*  Error Code Start  */
	public static int PMYBS_SDK_CONNECT_FAIL                	= 11000;//连接刻录服务失败，需确认刻录服务IP端口是否正确，并保证网络通畅
	public static int PMYBS_SDK_PUBLISHER_RUN_FAIL          	= 11001;//初始化接收服务数据推送模块失败
	public static int PMYBS_SDK_EXCEPTION                   	= 11002;//异常错误
	public static int PMYBS_SDK_INVALID_PARAMETER           	= 11003;//无效参数
	public static int PMYBS_SDK_PARSE_RESPONSE_BODY_FAIL    	= 11004;//解析返回包体失败
	public static int PMYBS_SDK_INVALID_SYSCONFIG_KEY       	= 11005;//无效的系统配置项
	public static int PMYBS_SDK_OUTPUT_BUFFER_SMALL         	= 11006;//输出Buffer长度太小，一般用于获取输出Buffer长度；
	public static int PMYBS_SDK_TASK_TYPE_NOT_SUPPORT       	= 11007;//不支持该类型的任务
	public static int PMYBS_SDK_TASK_NOT_FOUND              	= 11008;//未找到任务
	public static int PMYBS_SDK_FILE_NOT_EXIST              	= 11009;//文件不存在
	public static int PMYBS_SDK_FILE_OPEN_FAIL              	= 11010;//文件打开失败
	public static int PMYBS_SDK_ALREADY_LOGIN               	= 11011;//已登录
	public static int PMYBS_SDK_NOT_LOGIN                   	= 11012;//未登录
	public static int PMYBS_SDK_UPLOAD_NET_INIT_FAIL        	= 11013;//文件传输网络初始化失败
	public static int PMYBS_SDK_NO_BURNING_FILES_IN_THE_TASK	= 11014;//任务中未添加刻录文件
	public static int PMYBS_SDK_TASK_JSON_TOO_LONG              = 11015; //任务数据 Buffer 太长（一般是添加的文件太多导致）
	public static int PMYBS_SDK_TASK_CLOSE_FAIL_UPLOADING       = 11016; //任务关闭失败，刻录源传输中
	public static int PMYBS_SDK_LOAD_FAIL						= 11100; //底层动态库加载失败

	public static  String OS_TYPE= null ;  // 操作系统类型

	public static PMYBurnSdk INSTANCE =null;
	static{
		// 加载属性文件
		System.out.println("constants init begin!");
		try {
			String name = null;
			String arch = null;
			try {
				arch = System.getProperty("os.arch");
				name = System.getProperty("os.name");
				System.out.println("arch:"+arch+";name:"+name);
			} catch (Exception e) {
				System.out.println("get system exception"+e.getMessage());
				e.printStackTrace();
			}
			System.out.println("loadLibrary begin!");
			INSTANCE = Native.loadLibrary("PMYBurnSvcSdk", PMYBurnSdk.class);
			System.out.println("loadLibrary end!");
		} catch (Exception e) {
			System.out.println("loadLibrary exception!"+e.getMessage());
			e.printStackTrace();
		}
		System.out.println("load sdk success");
	}

	//判断操作系统是window还是linux
	static String getOSType() {
		String osName = System.getProperty("os.name");
		if (osName.toLowerCase().startsWith("linux")) {
			return "linux";
		}else {
			return "win";
		}
	}
}
