#ifndef _WEBVIEW_PRIVATE_H_
#define _WEBVIEW_PRIVATE_H_

#include <stdio.h>
#ifdef _cplusplus
extern "C" {
#endif



#include "shell.h"
#include <webkit2/webkit2.h>
#include <JavaScriptCore/JavaScript.h>

#include <pthread.h>

#define MAX_WEB_VIEWS_COUNT (1024)

typedef void * webview_callbacks_table_ptr;
typedef struct webview_data webview_data_t;

typedef struct webview_list
{
	shell_ctx_t * shell;
	char * home_uri;
	
	pthread_mutex_t mutex;
	WebKitWebContext * web_context;
	WebKitSettings * web_settings;
	JSGlobalContextRef js_context;

	webview_data_t * items[MAX_WEB_VIEWS_COUNT];
	ssize_t count;
	
	int (* add)(struct webview_list * list, GtkWidget * webview);
	int (* remove)(struct webview_list * list, GtkWidget * webview);
	void (* clear)(struct webview_list * list);
	
}webview_list_t;
webview_list_t * webview_list_init(webview_list_t * list, void * shell);
void webview_list_cleanup(webview_list_t * list);

struct webview_data
{
	webview_list_t * list;
	webview_callbacks_table_ptr callbacks;  
	long id;
	GtkWidget * webview;
	char * uri;
};

webview_data_t * webview_data_new(GtkWidget * widget, webview_list_t * list);
void webview_data_destroy(webview_data_t * data);


enum webview_signal_type
{
	webview_signal_type_authenticate, 
	webview_signal_type_close, 
	webview_signal_type_context_menu, 
	webview_signal_type_context_menu_dismissed, 
	webview_signal_type_create, 
	webview_signal_type_decide_policy, 
	webview_signal_type_enter_fullscreen, 
	webview_signal_type_insecure_content_detected, 
	webview_signal_type_leave_fullscreen, 
	webview_signal_type_load_changed, 
	webview_signal_type_load_failed, 
	webview_signal_type_load_failed_with_tls_errors, 
	webview_signal_type_mouse_target_changed, 
	webview_signal_type_permission_request, 
	webview_signal_type_print, 
	webview_signal_type_ready_to_show, 
	webview_signal_type_resource_load_started, 
	webview_signal_type_run_as_modal, 
	webview_signal_type_run_color_chooser, 
	webview_signal_type_run_file_chooser, 
	webview_signal_type_script_dialog, 
	webview_signal_type_show_notification, 
	webview_signal_type_show_option_menu, 
	webview_signal_type_submit_form, 
	webview_signal_type_web_process_crashed, 
	webview_signals_count
};

typedef struct webview_signal_param
{
	char name[100];
	void * callback;
	void * user_data;
	int swapped;
}webview_signal_param_t;

#ifdef _cplusplus
}
#endif
#endif
