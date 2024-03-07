package com.xinke.sdk.service.impl;
import java.io.File;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.lang.StringUtils;

import com.xinke.sdk.service.PMYBurnSdk;
import com.xinke.sdk.service.PMYBurnSdk.TASK_ID;
import com.xinke.sdk.service.XinKeBurnTask;
import com.xinke.sdk.utils.Constants;
import com.xinke.sdk.utils.ErrorCode;
import com.sun.jna.Pointer;

public class XinKeBurnTaskImpl implements XinKeBurnTask {
	static PMYBurnSdk hPMYBurnSDK = Constants.INSTANCE;
	private Pointer taskHandle;

	
	public XinKeBurnTaskImpl(Pointer TaskHandle) 
	{
		taskHandle=TaskHandle;
	}
	
	TASK_ID taskId = new TASK_ID();

	public boolean PMYBS_AddFileEx( String filePath,  String pathInDisc,Integer nFlag) {
		int nRet =1;
		try {
			//添加文件到实时流刻录任务中
			nRet = hPMYBurnSDK.PMYBS_AddFileEx(taskHandle , filePath,nFlag,
					pathInDisc);
			if (nRet == 0) {
				System.out.println("添加文件到实时流刻录任务成功");
				return true;
			} else {
				System.out.println("添加文件到实时流刻录任务失败");
				return false;
			}

		} catch (Exception e) {
			System.out.println("PMYBS_AddFileEx exception: " + e.toString());
			return false;
		}
	}

	public boolean PMYBS_AddFile(String filePath, String pathInDisc) {
		// TODO Auto-generated method stub
		int nRet = 1;
		try {
			if (StringUtils.isNotEmpty(filePath)) {
				File fTempFile = new File(filePath);
				if (!fTempFile.exists()) {
					return false;
				}
				// 为任务运行添加待刻录文件
				nRet = hPMYBurnSDK.PMYBS_AddFile(taskHandle, filePath,
						pathInDisc);
				if (nRet == 0) {
					System.out.println("添加文件" + filePath + "成功");
					return true;
				} else {
					System.out.println("添加文件" + filePath + "失败");
					return false;
				}
			}
		} catch (Exception e) {
			System.out.println("PMYBS_AddFile exception: " + e.toString());
			return false;
		}
		return true;
	}

	public boolean PMYBS_AddTree(String dirPath, String pathInDisc) {
		// TODO Auto-generated method stub
		int nRet =1;
		try {
			if (StringUtils.isNotEmpty(dirPath)) {
				File fTempFile = new File(dirPath);
				if (!fTempFile.exists()) {
					return false;
				}

				// 为任务运行添加待刻录文件夹
				nRet = hPMYBurnSDK.PMYBS_AddTree(taskHandle, dirPath,
						pathInDisc);
				if (nRet == 0) {
					System.out.println("添加文件夹" + dirPath + "成功");
					return true;
				} else {
					System.out.println("添加文件夹" + dirPath + "失败");
					return false;
				}
			
			}
		} catch (Exception e) {
			System.out.println("PMYBS_AddTree exception: " + e.toString());
			return false;
		}
		return true;
	}

	public boolean PMYBS_AddDirectory(String pathInDisc) {
		// TODO Auto-generated method stub
	int nRet =1;
		try {

			// 为任务运行添加待刻录文件夹
			nRet = hPMYBurnSDK.PMYBS_AddDirectory(taskHandle, pathInDisc);
			if (nRet == 0) {
				System.out.println("添加空文件夹成功");
				return true;
			} else {
				System.out.println("添加文件夹失败");
				return false;
			}

		} catch (Exception e) {
			System.out.println("PMYBS_AddTree exception: " + e.toString());
			return false;
		}
		
	}

	public boolean PMYBS_SetPrintFile(String susPrintFilePath,
			String failPrintFilePath) {
		int nret = hPMYBurnSDK.PMYBS_SetPrintFile(taskHandle, susPrintFilePath,
				failPrintFilePath);
		if (nret == 0) {
			System.out.println("PMYBS_SetPrintFile success!");
			return true;
		}
		System.out.println("PMYBS_SetPrintFile fail!nret=" + nret);
		return false;
	}

	public boolean PMYBS_SetPrintFile2(String susPrintFilePath,
									   String failPrintFilePath,Integer copyIndex) {
		//刻录多份时可设置不同的打印模板 nIndex 为份数索引 从0开始
		int nret = hPMYBurnSDK.PMYBS_SetPrintFile2(taskHandle, susPrintFilePath,
				failPrintFilePath,copyIndex);
		if (nret == 0) {
			System.out.println("PMYBS_SetPrintFile2 success!");
			return true;
		}
		System.out.println("PMYBS_SetPrintFile2 fail!nret=" + nret);
		return false;
	}

	public boolean PMYBS_SetPrintField(int index, String value) {
		int nret=hPMYBurnSDK.PMYBS_SetPrintField(taskHandle, index, value);
		
		if(nret==0){
			System.out.println("PMYBS_SetPrintField success!");
			return true;
		}
		System.out.println("PMYBS_SetPrintField fail!nret="+nret);
		return false;
	}

	public boolean PMYBS_SetTaskConfig(String key, String value) {
		int nret=hPMYBurnSDK.PMYBS_SetTaskConfig(taskHandle, key,value);
		if(nret==0){
			System.out.println("PMYBS_SetTaskConfig success!");
			return true;
		}
		System.out.println("PMYBS_SetTaskConfig fail!nret="+nret+";key="+key+";value="+value);
		return false;
	}
	
	public Map<String, Object> PMYBS_SubmitTask() {
		Map<String, Object> map = new HashMap<String, Object>();
		try {
			int nret = hPMYBurnSDK.PMYBS_SubmitTask(taskHandle, taskId);
			if(nret==0){
				String strTaskId = new String(taskId.strTaskId, "UTF-8");//任务ID
	    		map.put("taskId", strTaskId);	
	    		map.put("errorCode", nret);
	    		map.put("errorMsg", "");
	    		System.out.println("PMYBS_SubmitTask success");
			}else{
				map.put("errorCode", nret);
	    		map.put("errorMsg", ErrorCode.PMYBS_SDK_CONNECT_FAIL);
				System.out.println("PMYBS_SubmitTask fail!nret="+nret);
			}
			
            return map;
		}
		catch (Exception e) {
			e.printStackTrace();
			System.out.println("PMYBS_SubmitTask exception: " + e.toString());
			map.put("errorCode", ErrorCode.PMYBS_SDK_CONNECT_FAIL);
    		map.put("errorMsg", hPMYBurnSDK.PMYBS_GetErrorString(ErrorCode.PMYBS_SDK_CONNECT_FAIL));
			return map;
		}
	}

	public boolean PMYBS_CloseTask() {
		int nret=hPMYBurnSDK.PMYBS_CloseTask(taskHandle);
		if(nret==0){
			System.out.println("PMYBS_CloseTask success!");
			return true;
		}
		return false;
	}
}
