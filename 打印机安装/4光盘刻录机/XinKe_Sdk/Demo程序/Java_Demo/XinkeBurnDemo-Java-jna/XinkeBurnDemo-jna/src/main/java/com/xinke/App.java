package com.xinke;
import java.math.BigInteger;
import java.util.Map;

import org.json.JSONArray;
import org.json.JSONObject;

import com.xinke.sdk.service.*;
import com.xinke.sdk.utils.Constants;

public class App 
{
	static XinKeBurnSdk sdk;
    public static void main( String[] args )
    {
    	//add callback listener
    	BurnStatusCallBackListener listener = new BurnStatusCallBackListener();
    	sdk = XinKeBurnSdkFactory.PMYBS_CreateSdk(listener);
    	Map<String, Object> ret;
    	int errorCode = 0;
    	String errorMsg = "";
    			
    	ret = sdk.PMYBS_Init();
    	errorCode = Integer.parseInt(ret.get("errorCode").toString());
    	errorMsg = ret.get("errorMsg").toString();
    	if (errorCode != 0) {
    		System.out.println( "init sdk fail, error: [" + errorCode + "] " + errorMsg );
    		return;
    	}
    	
    	ret = sdk.PMYBS_Login("192.168.1.10", 6000);
    	errorCode = Integer.parseInt(ret.get("errorCode").toString());
    	errorMsg = ret.get("errorMsg").toString();
    	if (errorCode != 0) {
    		System.out.println( "login burn svc fail, error: [" + errorCode + "] " + errorMsg );
    		return;
    	}
    	
    	ret = sdk.PMYBS_GetRobotStatus();
    	errorCode = Integer.parseInt(ret.get("errorCode").toString());
    	errorMsg = ret.get("errorMsg").toString();
    	if (errorCode != 0) {
    		System.out.println( "get robot status fail, error: [" + errorCode + "] " + errorMsg );
    		return;
    	}
    	String status = ret.get("status").toString();
        System.out.println( "robot status: " );
        System.out.println( status );

        //create task
        XinKeBurnTask task = sdk.PMYBS_CreateTask(Constants.TASK_TYPE_FILE_BURN, "TEST BURN TASK");
        if(task==null){
        	return;
        }
        
        //set burn files
        task.PMYBS_AddFile("E:\\XinKe_burn_sdk-1.0.jar", "/122.jpg");
        //task.PMYBS_AddFile("D:\\testFile\\Print_fail.std", "");
        //task.PMYBS_AddFile("D:\\testFile\\SDK guide.docx", "\\SDK guide.docx");
        //task.PMYBS_AddTree("D:\\test", "\\test");
        //task.PMYBS_AddDirectory("/new-dir1");
        //task.PMYBS_AddFile("D:\\testFile\\SDK guide.docx", "\\new-dir\\test\\SDK guide.docx");
        //task.PMYBS_AddDirectory("\\new-dir2\\444");
        //task.PMYBS_AddTree("D:\\testFile", "\\new-dir3\\test");
        //iso burn
        //task.PMYBS_AddTree("E:\\1.iso", "\\1.iso");
        
        //set print info
        task.PMYBS_SetPrintFile("E:\\ok.PTF", "E:\\fail.PTF");
        task.PMYBS_SetPrintField(1, "XINKE Burn Test");
        task.PMYBS_SetPrintField(2, Constants.PRINT_VALUE_TASK_NAME);
        task.PMYBS_SetPrintField(3, Constants.PRINT_VALUE_DISC_INDEX);
        task.PMYBS_SetPrintField(4, Constants.PRINT_VALUE_DISC_COUNT);
        task.PMYBS_SetPrintField(5, Constants.PRINT_VALUE_DISC_SN);
        task.PMYBS_SetPrintField(6, Constants.PRINT_VALUE_BURN_START_TIME);
        
        //other set
        task.PMYBS_SetTaskConfig(Constants.PMYBS_SDK_TASK_CFG_CLOSE_DISC, "1");//1-是；0-否
        task.PMYBS_SetTaskConfig(Constants.PMYBS_SDK_TASK_CFG_NOT_BLANK, "0");
        task.PMYBS_SetTaskConfig(Constants.PMYBS_SDK_TASK_CFG_DISC_LABEL, "XINKEBurn");
        task.PMYBS_SetTaskConfig(Constants.PMYBS_SDK_TASK_CFG_DISC_TYPE, String.valueOf(Constants.PMY_DISC_CD_700M));
        
        //iso burn
        //task.PMYBS_SetTaskConfig(Constants.PMYBS_SDK_TASK_CFG_BURN_IMAGE, "1");
        
        //submit task
        ret = task.PMYBS_SubmitTask();
        errorCode = Integer.parseInt(ret.get("errorCode").toString());
    	errorMsg = ret.get("errorMsg").toString();
    	if (errorCode != 0) {
    		System.out.println( "submit task fail, " + errorCode + "] " + errorMsg );
    		return;
    	}
    	String taskId = ret.get("taskId").toString();
    	
    	while(true) {
    		
    		ret = getTaskStatus(taskId);
        	errorCode = Integer.parseInt(ret.get("errorCode").toString());
        	errorMsg = ret.get("errorMsg").toString();
        	if (errorCode != 0) {
        		System.out.println( "get robot status fail, error: [" + errorCode + "] " + errorMsg );
        		break;
        	}
        	
        	int task_count = Integer.parseInt(ret.get("task_count").toString());
        	if (task_count == 0){
        		break;
        	}
        	
        	 try {
                 Thread.sleep(5000);
             } catch (InterruptedException e) {
                 // TODO Auto-generated catch block
                 e.printStackTrace();
             }
    	}
    	
        System.out.println( "Test finish!" );
    }
    
    static Map<String, Object> getTaskStatus(String taskId){
    	
    	Map<String, Object> ret = sdk.PMYBS_GetTaskStatus(taskId);
    	int errorCode = Integer.parseInt(ret.get("errorCode").toString());
    	String errorMsg = ret.get("errorMsg").toString();
    	if (errorCode != 0) {
    		System.out.println( "get task status fail, error: [" + errorCode + "] " + errorMsg );
    		return ret;
    	}
    	String status = ret.get("status").toString();
    	if (status.isEmpty()) {
    		return ret;
    	}
    	    	
		JSONObject jsonObject = new JSONObject(status);
    	//解析task
		int count = 0;
    	JSONArray jsonArray = jsonObject.getJSONArray("tasks");
    	if(jsonArray != null && jsonArray.length() > 0){
    		//可能会有多个任务
    		for(int i=0; i<jsonArray.length(); i++){
    			JSONObject taskObject = jsonArray.getJSONObject(i);
    			
    	         String name = taskObject.get("name").toString();
    	         Boolean is_finish = taskObject.getBoolean("is_finish");
    	         String status_string = taskObject.get("status_string").toString();
    	         
    	         String msg = String.format("Task: Name(%s) isFinish(%b) Status(%s)", name, is_finish, status_string);
    	         System.out.println( msg ); 	
    	         
    	         if (is_finish == false) {
    	        	 count++;
    	         }
    
    			//光盘 burn-discs
    	         if (taskObject.has("burn")) {
    	        	 JSONObject burnObject = taskObject.getJSONObject("burn");
    	        	 JSONArray discsArray = burnObject.getJSONArray("discs");
    	        	 if(discsArray!=null && discsArray.length()>0){
    	    				//可能有多张光盘
    	        		 for(int j=0;j<discsArray.length();j++){
    	    					displayDisc(discsArray.getJSONObject(j));
    	    				}
    	    			}	 
    	         }

    	         if (taskObject.has("file_upload")) {
    	        	 JSONObject uploadObject = taskObject.getJSONObject("file_upload");
    	        	 if(uploadObject != null){
    	        		 displayUpload(uploadObject);
    	    		}	 
    	         }
    		}
    	}
    	
    	ret.put("task_count", count);
    	return ret;	
    }
    
    static void displayDisc(JSONObject discObject) {
    	try {
   		 int disc_index = discObject.getInt("disc_index");
   		 int copy_index = discObject.getInt("copy_index");
        String id = discObject.getString("id");
        String sn = discObject.getString("sn");
        String drive = discObject.getString("drive");
        String label = discObject.getString("label");
        int status_code = discObject.getInt("status_code");
        String status_string = discObject.getString("status_string");
        //int progress = discObject.getInt("progress");
        String msg = String.format("  Disc[#%d]: SN(%s) Drive(%s) Status(%s)", disc_index, sn, drive, status_string);
        System.out.println( msg ); 
		} catch (Exception e) {
			// TODO: handle exception
		}
    }
  
    static void displayUpload(JSONObject uploadObject) {
    	try {
            int total_count = uploadObject.getInt("total_count");
            BigInteger total_size = uploadObject.getBigInteger("total_size");

            int status_code = uploadObject.getInt("status_code");
            String status_string = uploadObject.getString("status_string");
            
            String msg = String.format("  Upload Files: Count(%d) Size(%d) Status(%s)", total_count, total_size, status_string);
            System.out.println( msg ); 		
		} catch (Exception e) {
			// TODO: handle exception
		}
   }

    public static class BurnStatusCallBackListener implements XinKeBurnCallBackListener
    {
        public void taskCallBack( String data)
        {
            //解析task
            System.out.println(data);
        }

        public void robotCallBack(String data)
        {
        	 //解析robot
            System.out.println(data);
        }
    }
}

