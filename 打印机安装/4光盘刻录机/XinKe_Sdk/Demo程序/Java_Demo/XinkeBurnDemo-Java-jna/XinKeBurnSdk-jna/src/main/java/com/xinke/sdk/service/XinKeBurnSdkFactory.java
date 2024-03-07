package com.xinke.sdk.service;
import com.xinke.sdk.service.impl.XinKeBurnSdkImpl;

public class XinKeBurnSdkFactory {
	
	/**
	 * @return
	 */
	public static XinKeBurnSdk PMYBS_CreateSdk(XinKeBurnCallBackListener listener) {
		return new XinKeBurnSdkImpl(listener);
	}
	
}
