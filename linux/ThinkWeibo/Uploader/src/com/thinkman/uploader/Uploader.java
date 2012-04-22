package com.thinkman.uploader;

import java.io.File;
import java.net.URLEncoder;

import weibo4j.Status;
import weibo4j.Weibo;

public class Uploader {

	/**
	 * @param args
	 */
	 public static void main(String[] args) {
			try {
				if (args.length < 4) {
					System.out.println("[FAILED]");
					return;
		        }
				
				System.setProperty("weibo4j.oauth.consumerKey", "3551174478");
				System.setProperty("weibo4j.oauth.consumerSecret", "c53f4123bdb7b39a0ae03e400b839934");

				Weibo weibo = new Weibo();
				
				/*
				 * 此处需要填写AccessToken的key和Secret
	                        */
				weibo.setToken(args[0], args[1]);
				try {
					File file=new File(args[2]);
					String msg = URLEncoder.encode(args[3], "UTF-8");
					Status status = weibo.uploadStatus(msg, file);

					System.out.println("[SUCCESS]:"
							+ status.getId() + "");
					
				} catch (Exception e1) {
					System.out.println("[FAILED]");
				}
			} catch (Exception ioe) {
				System.out.println("[FAILED]");
			}
		}

}
