/**
* <p>Title: XinKeBurnTask.java</p>  
* <p>Description: 刻录任务接口类</p>  
* <p>Copyright: Copyright (c) 2020</p>  
* <p>Company: http://www.cdprinter.com.cn</p>  
* @author: bing
* @date: 2020-8-13 
* @version: 1.0
*/ 
package com.xinke.sdk.service;
import java.util.Map;

/** 
* <p>Title: XinKeBurnTask</p>  
* <p>Description: 刻录任务接口类</p>  
* @author bing
* @date 2020年8月13日  
*/ 
public interface XinKeBurnTask {
	/**
	 * <p>Title: PMYBS_AddFile</p>
	 * <p>Description: 添加刻录文件</p>
	 * @param:  filePath 待刻录文件的物理全路径
	 * @param:  pathInDisc 文件刻录到光盘中的相对路径
	 * @param:  nFlag
	 * @param: @return true: 成功 false: 失败
	 * @return: boolean true: 成功 false: 失败
	 * @comment: pathInDisc参数必须以”/”开头，该参数也可设为空字符串（“”）空即认为将文件刻录到光盘根目录
	 *  例如：
	 *  		filePath       			pathInDisc
	 * 			C:/1.txt		->			 /1.txt（此时pathInDisc可以为“”）;
	 * 			C:/2.txt		->			 /1/2.txt
	 */
	public boolean PMYBS_AddFileEx( String filePath,  String pathInDisc,Integer nFlag);

	/**  
	 * <p>Title: PMYBS_AddFile</p>  
	 * <p>Description: 添加刻录文件</p>  
	 * @param:  filePath 待刻录文件的物理全路径
	 * @param:  pathInDisc 文件刻录到光盘中的相对路径
	 * @param: @return true: 成功 false: 失败     
	 * @return: boolean true: 成功 false: 失败
	 * @comment: pathInDisc参数必须以”/”开头，该参数也可设为空字符串（“”）空即认为将文件刻录到光盘根目录
	 *  例如：
	 *  		filePath       			pathInDisc     
	 * 			C:/1.txt		->			 /1.txt（此时pathInDisc可以为“”）;
	 * 			C:/2.txt		->			 /1/2.txt
	 */
	public boolean  PMYBS_AddFile(String filePath, String pathInDisc);
	/**  
	 * <p>Title: PMYBS_AddTree</p>  
	 * <p>Description: 添加文件夹目录树，将该文件夹及其所有子文件全部刻录到光盘</p>  
	 * @param:  dirPath 待刻文件夹的物理全路径
	 * @param:  pathInDisc 文件夹刻录到光盘中的相对路径
	 * @param: @return true: 成功 false: 失败     
	 * @return: boolean
	 * @comment: pathInDisc 参数必须以”/”开头，该参数可不填，不填即认为将文件夹刻录到光盘根目录，
	 * 例如:
	 * 			dirPath       				pathInDisc     
	 * 			C:/test			->		/test（此时pPathInDisc可以为“”）
	 * 			C:/test1		->		 /1/test1
	 */
	public boolean  PMYBS_AddTree(String dirPath, String pathInDisc);

	/**  
	 * <p>Title: PMYBS_AddDirectory</p>  
	 * <p>Description: 添加空文件夹，即在光盘中刻录一个空的文件夹</p>  
	 * @param:  pathInDisc 文件夹在光盘中的相对路径
	 * @param: @return true: 成功 false: 失败     
	 * @return: boolean
	 * @comment: pathInDisc 参数必须以”/”开头，例如：“/test”或者“/1/test1”
	 */
	public boolean  PMYBS_AddDirectory(String pathInDisc);

	/**
	 * <p>Title: PMYBS_SetPrintFile</p>  
	 * <p>Description: 设置任务打印封面文件</p>  
	 * @param:  susPrintFilePath 刻录成功封面文件
	 * @param:  failPrintFilePath 刻录失败封面文件
	 * @param: @return true: 成功 false: 失败     
	 * @return: boolean 
	 * @comment: windows and linux: .bmp .jpg .jpeg
	 * 						 only windows: .std
	 * 						 only linux: .ptf
	 */
	public boolean  PMYBS_SetPrintFile(String susPrintFilePath, String failPrintFilePath);

	/**
	 * <p>Title: PMYBS_SetPrintFile2</p>
	 * <p>Description: 设置任务打印封面文件</p>
	 * @param:  susPrintFilePath 刻录成功封面文件
	 * @param:  failPrintFilePath 刻录失败封面文件
	 * @param:  copyIndex 打印份数
	 * @param: @return true: 成功 false: 失败
	 * @return: boolean
	 * @comment: windows and linux: .bmp .jpg .jpeg
	 * 						 only windows: .std
	 * 						 only linux: .ptf
	 */
	public boolean PMYBS_SetPrintFile2(String susPrintFilePath,String failPrintFilePath,Integer copyIndex);

	/**
	 * <p>Title: PMYBS_SetPrintField</p>
	 * <p>Description: 设置打印字段值</p>
	 * @param:  index 字段编号，从1开始，字段编号同模板文件中的变量字段编号一一对应
	 * @param:  value 字段内容：自定义字段内容，固定字段内容
	 * @param: @return true: 成功 false: 失败
	 * @return: boolean
	 * @comment:
	 */
	public boolean  PMYBS_SetPrintField(int index, String value);

	/**
	 * <p>Title: PMYBS_SetTaskConfig</p>  
	 * <p>Description: 设置任务配置</p>  
	 * @param:  key 配置属性名称
	 * @param:  value 配置属性值
	 * @param: @return true: 成功 false: 失败     
	 * @return: boolean
	 * @comment: 
	 */
	public boolean  PMYBS_SetTaskConfig(String key, String value);	

	/**  
	 * <p>Title: PMYBS_SubmitTask</p>  
	 * <p>Description: 提交任务</p>  
	 * @param: @return      
	 * @return: Map<String,Object>
	 * 					key					value
	 * 					errorCode		错误码			0表示成功，其他值为失败
	 * 					errorMsg			错误信息
	 * 					taskId				任务ID
	 * @comment: 
	 */
	public Map<String, Object> PMYBS_SubmitTask();

	/**
	 * 关闭任务
	 * @return
	 */
	public boolean PMYBS_CloseTask();

}
