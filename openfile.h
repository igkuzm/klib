/**
 * File              : openfile.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.09.2021
 * Last Modified Date: 07.11.2025
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * open file at path with default program
 */

#ifndef k_lib_openfile_h__
#define k_lib_openfile_h__

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32
#include <windows.h>
#elif defined __APPLE__
#include <TargetConditionals.h>
#ifndef TARGET_OS_MAC
#include <Availability.h>
#include <objc/objc.h>
#include <objc/objc-runtime.h>
#endif // ifndef TARGET_OS_MAC
#else
#include <stdlib.h>
#include <string.h>
#endif

#include <stdio.h>

static int openfile(const char *path) {
#if defined _WIN32
  ShellExecute(NULL, NULL, path, NULL, NULL, SW_SHOWDEFAULT);

#elif defined __APPLE__
#ifdef TARGET_OS_MAC
	char open_file_command[BUFSIZ];
	sprintf(open_file_command, "open %s", path);
	system(open_file_command);	
#else //TARGET_OS_MAC
  id str = ((id (*)(Class, SEL, const char *))objc_msgSend)(
      objc_getClass("NSString"), sel_registerName("stringWithUTF8String:"),
      path);
  id url = ((id (*)(Class, SEL, id))objc_msgSend)(
      objc_getClass("NSURL"), sel_registerName("fileURLWithPath:"), str);
  id app = ((id (*)(Class, SEL))objc_msgSend)(
	  objc_getClass("UIApplication"), sel_registerName("sharedApplication"));
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
  id opt = ((id (*)(Class, SEL))objc_msgSend)(objc_getClass("NSDictionary"),
                                              sel_registerName("dictionary"));
			((void (*)(id, SEL, id, id, id))objc_msgSend)(
				app, sel_registerName("openURL:options:completionHandler:"), url, opt,
				NULL);
#else //__IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
			((void (*)(id, SEL, id))objc_msgSend)(app, sel_registerName("openURL:"), url);			
#endif // __IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
#endif // TARGET_OS_MAC
#else // defined _WIN32_
  char open_file_command[BUFSIZ];
  sprintf(open_file_command, "xdg-open %s", path);
  system(open_file_command);
#endif // defined _WIN32_

  return 0;
}

static int openurl(const char *uri) {
#ifdef __APPLE__ 
#ifndef TARGET_OS_MAC
  id str = ((id (*)(Class, SEL, const char *))objc_msgSend)(
      objc_getClass("NSString"), sel_registerName("stringWithUTF8String:"),
      uri);
  id url = ((id (*)(Class, SEL, id))objc_msgSend)(
      objc_getClass("NSURL"), sel_registerName("URLWithString:"), str);
		id app = ((id (*)(Class, SEL))objc_msgSend)(
			objc_getClass("UIApplication"), sel_registerName("sharedApplication"));
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
			id opt = ((id (*)(Class, SEL))objc_msgSend)(objc_getClass("NSDictionary"),
                                              sel_registerName("dictionary"));
			((void (*)(id, SEL, id, id, id))objc_msgSend)(
				app, sel_registerName("openURL:options:completionHandler:"), url, opt,
				NULL);
#else //__IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
			((void (*)(id, SEL, id))objc_msgSend)(app, sel_registerName("openURL:"), url);			
#endif // __IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
#else // ifndef TARGET_OS_MAC
	openfile(uri);
#endif // TARGET_OS_MAC
#else // __APPLE__
	openfile(uri);
#endif // __APPLE__

  return 0;
}


#ifdef __cplusplus
}
#endif

#endif // k_lib_openfile_h__
