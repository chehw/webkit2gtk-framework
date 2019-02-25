/*
 * webview.c
 * 
 * Copyright 2019 chehw <chehw@debian9x64>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "webview_private.h"
#include "shell_private.h"

#include "common.h"

static gboolean on_webview_authenticate(WebKitWebView * webview,
	WebKitAuthenticationRequest * request,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static void on_webview_close(WebKitWebView * webview,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return;
}

static gboolean on_webview_context_menu(WebKitWebView * webview,
	WebKitContextMenu * context_menu,
	GdkEvent * event,
	WebKitHitTestResult * hit_test_result,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static void on_webview_context_menu_dismissed(WebKitWebView * webview,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return;
}

static GtkWidget * on_webview_create(WebKitWebView * webview,
	WebKitNavigationAction * navigation_action,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return NULL;
}

static gboolean on_webview_decide_policy(WebKitWebView * webview,
	WebKitPolicyDecision * decision,
	WebKitPolicyDecisionType decision_type,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static gboolean on_webview_enter_fullscreen(WebKitWebView * webview,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static void on_webview_insecure_content_detected(WebKitWebView * webview,
	WebKitInsecureContentEvent event,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return;
}

static gboolean on_webview_leave_fullscreen(WebKitWebView * webview,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}
	
static void on_webview_load_changed(WebKitWebView * webview,
	WebKitLoadEvent load_event,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	
	webview_data_t * data = user_data;
	
	switch(load_event)
	{
	case WEBKIT_LOAD_STARTED:
	case WEBKIT_LOAD_REDIRECTED:
		break;
	case WEBKIT_LOAD_COMMITTED:
		data->uri = (char *)webkit_web_view_get_uri(webview);
		break;
	case WEBKIT_LOAD_FINISHED:
		// TODO: call shell->on_page_loaded(...);
		{
			shell_ctx_t * shell = data->list->shell;
			assert(shell);
			shell_private_data_t * priv = shell->priv_data;
			gtk_entry_set_text(GTK_ENTRY(priv->uri_entry), data->uri);
		}
		
		break;
	}
	
	return;
}

static gboolean on_webview_load_failed(WebKitWebView * webview,
	WebKitLoadEvent load_event,
	gchar * failing_uri,
	GError * gerr,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static gboolean on_webview_load_failed_with_tls_errors(WebKitWebView * webview,
	gchar * failing_uri,
	GTlsCertificate * certificate,
	GTlsCertificateFlags errors,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static void on_webview_mouse_target_changed(WebKitWebView * webview,
	WebKitHitTestResult * hit_test_result,
	guint modifiers,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return;
}

static gboolean on_webview_permission_request(WebKitWebView * webview,
	WebKitPermissionRequest * request,
	GtkWidget * parent_window)
{
	debug_printf("(%p, %p)", webview, parent_window);
	return FALSE;
}

static gboolean on_webview_print(WebKitWebView * webview,
	WebKitPrintOperation * print_operation,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static void on_webview_ready_to_show(WebKitWebView * webview,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return;
}

static void on_webview_resource_load_started(WebKitWebView * webview,
	WebKitWebResource * resource,
	WebKitURIRequest * request,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return;
}

static void on_webview_run_as_modal(WebKitWebView * webview,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return;
}

static gboolean on_webview_run_color_chooser(WebKitWebView * webview,
	WebKitColorChooserRequest * request,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static gboolean on_webview_run_file_chooser(WebKitWebView * webview,
	WebKitFileChooserRequest * request,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}


static gboolean on_webview_script_dialog(WebKitWebView * webview,
	WebKitScriptDialog * dialog,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static gboolean on_webview_show_notification(WebKitWebView * webview,
	WebKitNotification * notification,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static gboolean on_webview_show_option_menu(WebKitWebView * webview,
	WebKitOptionMenu * menu,
	GdkEvent * event,
	GdkRectangle * rectangle,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}

static void on_webview_submit_form(WebKitWebView * webview,
	WebKitFormSubmissionRequest * request,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return;
}


static gboolean on_webview_web_process_crashed(WebKitWebView * webview,
	gpointer user_data)
{
	debug_printf("(%p, %p)", webview, user_data);
	return FALSE;
}


static webview_signal_param_t s_signal_cakkbacks[webview_signals_count] = {
	[webview_signal_type_authenticate] = { .name="authenticate", .callback=on_webview_authenticate, .user_data=NULL, }, 
	[webview_signal_type_close] = { .name="close", .callback=on_webview_close, .user_data=NULL, }, 
	[webview_signal_type_context_menu] = { .name="context-menu", .callback=on_webview_context_menu, .user_data=NULL, }, 
	[webview_signal_type_context_menu_dismissed] = { .name="context-menu-dismissed", .callback=on_webview_context_menu_dismissed, .user_data=NULL, }, 
	[webview_signal_type_create] = { .name="create", .callback=on_webview_create, .user_data=NULL, }, 
	[webview_signal_type_decide_policy] = { .name="decide-policy", .callback=on_webview_decide_policy, .user_data=NULL, }, 
	[webview_signal_type_enter_fullscreen] = { .name="enter-fullscreen", .callback=on_webview_enter_fullscreen, .user_data=NULL, }, 
	[webview_signal_type_insecure_content_detected] = { .name="insecure-content-detected", .callback=on_webview_insecure_content_detected, .user_data=NULL, }, 
	[webview_signal_type_leave_fullscreen] = { .name="leave-fullscreen", .callback=on_webview_leave_fullscreen, .user_data=NULL, }, 
	[webview_signal_type_load_changed] = { .name="load-changed", .callback=on_webview_load_changed, .user_data=NULL, }, 
	[webview_signal_type_load_failed] = { .name="load-failed", .callback=on_webview_load_failed, .user_data=NULL, }, 
	[webview_signal_type_load_failed_with_tls_errors] = { .name="load-failed-with-tls-errors", .callback=on_webview_load_failed_with_tls_errors, .user_data=NULL, }, 
	[webview_signal_type_mouse_target_changed] = { .name="mouse-target-changed", .callback=on_webview_mouse_target_changed, .user_data=NULL, }, 
	[webview_signal_type_permission_request] = { .name="permission-request", .callback=on_webview_permission_request, .user_data=NULL, }, 
	[webview_signal_type_print] = { .name="print", .callback=on_webview_print, .user_data=NULL, }, 
	[webview_signal_type_ready_to_show] = { .name="ready-to-show", .callback=on_webview_ready_to_show, .user_data=NULL, }, 
	[webview_signal_type_resource_load_started] = { .name="resource-load-started", .callback=on_webview_resource_load_started, .user_data=NULL, }, 
	[webview_signal_type_run_as_modal] = { .name="run-as-modal", .callback=on_webview_run_as_modal, .user_data=NULL, }, 
	[webview_signal_type_run_color_chooser] = { .name="run-color-chooser", .callback=on_webview_run_color_chooser, .user_data=NULL, }, 
	[webview_signal_type_run_file_chooser] = { .name="run-file-chooser", .callback=on_webview_run_file_chooser, .user_data=NULL, }, 
	[webview_signal_type_script_dialog] = { .name="script-dialog", .callback=on_webview_script_dialog, .user_data=NULL, }, 
	[webview_signal_type_show_notification] = { .name="show-notification", .callback=on_webview_show_notification, .user_data=NULL, }, 
	[webview_signal_type_show_option_menu] = { .name="show-option-menu", .callback=on_webview_show_option_menu, .user_data=NULL, }, 
	[webview_signal_type_submit_form] = { .name="submit-form", .callback=on_webview_submit_form, .user_data=NULL, }, 
	[webview_signal_type_web_process_crashed] = { .name="web-process-crashed", .callback=on_webview_web_process_crashed, .user_data=NULL, }, 
};



static int webview_list_add(struct webview_list * list, GtkWidget * webview)
{
	debug_printf("(%p,%p)", list, webview);
	pthread_mutex_lock(&list->mutex);
	assert(list->count < (MAX_WEB_VIEWS_COUNT - 1));
	
	webview_data_t * data = webview_data_new(webview, list);
	assert(data);
	
	list->items[list->count++] = data;
	pthread_mutex_unlock(&list->mutex);
	return 0;
}
static int webview_list_remove(struct webview_list * list, GtkWidget * webview)
{
	debug_printf("(%p,%p)", list, webview);
	pthread_mutex_lock(&list->mutex);
	for(ssize_t i = 0; i < list->count; ++i)
	{
		webview_data_t * data = list->items[i];
		if(data && data->webview == webview)
		{
			webview_data_destroy(data);
			list->items[i] = list->items[--list->count];
			break;
		}
	}
	list->items[list->count] = NULL;
	pthread_mutex_unlock(&list->mutex);
	return 0;
}

static void webview_list_clear(struct webview_list * list)
{
	pthread_mutex_lock(&list->mutex);
	for(ssize_t i = 0; i < list->count; ++i)
	{
		webview_data_destroy(list->items[i]);
		list->items[i] = NULL;
	}
	list->count = 0;
	pthread_mutex_unlock(&list->mutex);
	return;
}


webview_list_t * webview_list_init(webview_list_t * list, void * shell)
{
	if(NULL == list) list = calloc(1, sizeof(*list));
	assert(list);
	
	list->shell = shell;
	
	list->add = webview_list_add;
	list->remove = webview_list_remove;
	list->clear = webview_list_clear;
	
	pthread_mutex_init(&list->mutex, NULL);
	
	return list;
}

void webview_list_cleanup(webview_list_t * list)
{
	if(NULL == list) return;
	list->clear(list);
	pthread_mutex_destroy(&list->mutex);
}

webview_data_t * webview_data_new(GtkWidget * webview, webview_list_t * list)
{
	webview_data_t * data = calloc(1, sizeof(*data));
	assert(data);
	
	data->list = list;
	data->webview = webview;
	if(NULL == data->callbacks) data->callbacks = s_signal_cakkbacks;
	
	webview_signal_param_t * callbacks = data->callbacks;
	if(callbacks)
	{
		for(int i = 0; i < webview_signals_count; ++i)
		{
			webview_signal_param_t * param = &callbacks[i];
			if(param->callback)
			{
				g_signal_connect(webview, param->name, 
					G_CALLBACK(param->callback), 
					param->user_data?param->user_data:data);
			}
		}
	}
	return data;
}

void webview_data_destroy(webview_data_t * data)
{
	debug_printf("%p", data);
	if(data->callbacks != s_signal_cakkbacks)
	{
		free(data->callbacks);
		data->callbacks = NULL;
	}
	
	if(data->webview)
	{
	//	gtk_widget_destroy(data->webview);
	}
	
	free(data);
	return;
}
