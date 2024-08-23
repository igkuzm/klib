/**
 * File              : openfile.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.09.2021
 * Last Modified Date: 23.08.2024
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

#if defined _WIN32_
#include <windows.h>
#elif defined __APPLE__
#include <objc/objc.h>
#include <objc/objc-runtime.h>
#include <TargetConditionals.h>
#else
#include <stdlib.h>
#include <strings.h>
#endif

#include <stdio.h>

static int openfile(const char *path) {
#if defined _WIN32_
  ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);

#elif defined __APPLE__
  id str = ((id (*)(Class, SEL, const char *))objc_msgSend)(
      objc_getClass("NSString"), sel_registerName("stringWithUTF8String:"),
      path);
  id url = ((id (*)(Class, SEL, id))objc_msgSend)(
      objc_getClass("NSURL"), sel_registerName("fileURLWithPath:"), str);
	#if TARGET_OS_MAC
		id ws = ((id (*)(Class, SEL))objc_msgSend)(
			objc_getClass("NSWorkspace"), sel_registerName("sharedWorkspace"));
		((void (*)(id, SEL, id))objc_msgSend)(ws, sel_registerName("openURL:"), url);
	#else
	#include <Availability.h>
		id app = ((id (*)(Class, SEL))objc_msgSend)(
			objc_getClass("UIApplication"), sel_registerName("sharedApplication"));
		#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
			id opt = ((id (*)(Class, SEL))objc_msgSend)(objc_getClass("NSDictionary"),
                                              sel_registerName("dictionary"));
			((void (*)(id, SEL, id, id, id))objc_msgSend)(
				app, sel_registerName("openURL:options:completionHandler:"), url, opt,
				NULL);
		#else
			((void (*)(id, SEL, id))objc_msgSend)(app, sel_registerName("openURL:"), url);			
		#endif
	#endif
#else
  char open_file_command[BUFSIZ];
  sprintf(open_file_command, "xdg-open %s", path);
  system(open_file_command);
#endif

  return 0;
}

static int openurl(const char *uri) {
#ifdef __APPLE__
  id str = ((id (*)(Class, SEL, const char *))objc_msgSend)(
      objc_getClass("NSString"), sel_registerName("stringWithUTF8String:"),
      uri);
  id url = ((id (*)(Class, SEL, id))objc_msgSend)(
      objc_getClass("NSURL"), sel_registerName("URLWithString:"), str);
	#if TARGET_OS_MAC
		id ws = ((id (*)(Class, SEL))objc_msgSend)(
			objc_getClass("NSWorkspace"), sel_registerName("sharedWorkspace"));
		((void (*)(id, SEL, id))objc_msgSend)(ws, sel_registerName("openURL:"), url);
	#else
	#include <Availability.h>
		id app = ((id (*)(Class, SEL))objc_msgSend)(
			objc_getClass("UIApplication"), sel_registerName("sharedApplication"));
		#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
			id opt = ((id (*)(Class, SEL))objc_msgSend)(objc_getClass("NSDictionary"),
                                              sel_registerName("dictionary"));
			((void (*)(id, SEL, id, id, id))objc_msgSend)(
				app, sel_registerName("openURL:options:completionHandler:"), url, opt,
				NULL);
		#else
			((void (*)(id, SEL, id))objc_msgSend)(app, sel_registerName("openURL:"), url);			
		#endif
	#endif
#else
	openfile(uri);
#endif

  return 0;
}


#ifdef __cplusplus
}
#endif

#endif // k_lib_openfile_h__
