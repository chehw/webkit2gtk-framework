#ifndef _SHELL_PRIVATE_H_
#define _SHELL_PRIVATE_H_

#include <stdio.h>
#ifdef _cplusplus
extern "C" {
#endif


#include "shell.h"
#include <gtk/gtk.h>

#include "webview_private.h"

#define MAX_WEB_VIEWS_COUNT (1024)
typedef struct shell_private_data
{
	shell_ctx_t * shell;
	GtkWidget * window;
	GtkWidget * content_area;
	GtkWidget * header_bar;
	GtkWidget * statusbar;
	
	GtkWidget * switcher;
	GtkWidget * stack;
	
	GtkWidget * uri_entry;
	
	GtkWidget * notebook;
	GtkWidget * textview;
	
	webview_list_t list[1];
	
	int quit;
	
}shell_private_data_t;


int shell_private_add_new_page(shell_private_data_t * priv, GtkWidget * webview);

#ifdef _cplusplus
}
#endif
#endif
