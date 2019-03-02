/*
 * shell.c
 * 
 * Copyright 2019 chehw <htc.chehw@gmail.com>
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
#include "shell.h"
#include "shell_private.h"

#include "common.h"


static void init_windows(shell_private_data_t * priv);
static int shell_init_default(shell_ctx_t * shell, void * init_data)
{
	init_windows(shell->priv_data);
	return 0;
}

static int shell_run_default(shell_ctx_t * shell, int flags)
{
	shell_private_data_t * priv = shell->priv_data;
	g_signal_connect_swapped(priv->window, "destroy", G_CALLBACK(shell->stop), shell);
	gtk_widget_show_all(priv->window);
	
	
	gtk_main();
	return 0;
}

static void shell_stop_default(shell_ctx_t * shell)
{
	shell_private_data_t * priv = shell->priv_data;
	if(priv && !priv->quit)
	{
		gtk_main_quit();
		priv->quit = 1;
	}
	return ;
}

static shell_private_data_t s_priv_data[1];
static shell_ctx_t g_shell[1] = {{
	.priv_data = s_priv_data,
	
	.init = shell_init_default,
	.run = shell_run_default,
	.stop = shell_stop_default,
}};

shell_ctx_t * shell_ctx_new(int * p_argc, char *** p_argv, void * user_data)
{
	if(p_argc && p_argv) gtk_init(p_argc, p_argv);
	
	shell_ctx_t * shell = &g_shell[0];
	
	shell->priv_data = s_priv_data;
	s_priv_data->shell = shell;
	
	shell->user_data = user_data;

	webview_list_t * list = webview_list_init(s_priv_data->list, shell);
	assert(list);
	list->home_uri = "https://www.google.com";

	return shell;
}

void shell_ctx_cleanup(shell_ctx_t * shell)
{
	shell_private_data_t * priv = shell->priv_data;
	
	if(!priv->quit)
	{
		shell->stop(shell);
	}
	
	webview_list_cleanup(priv->list);
	return;
}


static void go_prev(GtkWidget * button, shell_private_data_t * priv)
{
	debug_printf("(%p,%p)", button, priv);
	GtkNotebook * notebook = GTK_NOTEBOOK(priv->notebook);
	int cur_page = gtk_notebook_get_current_page(notebook);

	GtkWidget * webview = gtk_notebook_get_nth_page(notebook, cur_page);
	if(webview && webkit_web_view_can_go_back(WEBKIT_WEB_VIEW(webview)))
	{
		webkit_web_view_go_back(WEBKIT_WEB_VIEW(webview));
	}
	
	return;
}


static void go_next(GtkWidget * button, shell_private_data_t * priv)
{
	debug_printf("(%p,%p)", button, priv);
	GtkNotebook * notebook = GTK_NOTEBOOK(priv->notebook);
	int cur_page = gtk_notebook_get_current_page(notebook);

	GtkWidget * webview = gtk_notebook_get_nth_page(notebook, cur_page);
	if(webview && webkit_web_view_can_go_forward(WEBKIT_WEB_VIEW(webview)))
	{
		webkit_web_view_go_forward(WEBKIT_WEB_VIEW(webview));
	}
	return;
}

static void view_refresh(GtkWidget * button, shell_private_data_t * priv)
{
	debug_printf("(%p,%p)", button, priv);
	GtkNotebook * notebook = GTK_NOTEBOOK(priv->notebook);
	int cur_page = gtk_notebook_get_current_page(notebook);

	GtkWidget * webview = gtk_notebook_get_nth_page(notebook, cur_page);
	if(webview)
	{
		webkit_web_view_reload(WEBKIT_WEB_VIEW(webview));
	}
	
	return;
}

static void go_home(GtkWidget * button, shell_private_data_t * priv)
{
	debug_printf("(%p,%p)", button, priv);
	webview_list_t * list = priv->list;
	
	GtkNotebook * notebook = GTK_NOTEBOOK(priv->notebook);
	int cur_page = gtk_notebook_get_current_page(notebook);

	GtkWidget * webview = gtk_notebook_get_nth_page(notebook, cur_page);
	if(webview)
	{
		webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), list->home_uri);
	}
	
	return;
}

static void on_uri_entry_activate(GtkEntry * entry, shell_private_data_t * priv)
{
	debug_printf("(%p,%p)", entry, priv);
	const char * uri = gtk_entry_get_text(entry);
	if(NULL == uri || '\0' == uri[0]) return;
	
	char cooked_uri[4096] = "";
	char * location = strstr(uri, "://");
	
	
	if(NULL == location)
	{
		snprintf(cooked_uri, sizeof(cooked_uri), "http://%s", uri);
		uri = cooked_uri;
	}
	
	GtkNotebook * notebook = GTK_NOTEBOOK(priv->notebook);
	int cur_page = gtk_notebook_get_current_page(notebook);
	
	GtkWidget * webview = gtk_notebook_get_nth_page(notebook, cur_page);
	if(webview)
	{
		webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), uri);
	}
	
	return;
}

static void on_options(GtkWidget * button, shell_private_data_t * priv)
{
	debug_printf("(%p,%p)", button, priv);
	return;
}


static void on_show_inspector(GtkWidget * button, shell_private_data_t * priv)
{
	debug_printf("(%p,%p)", button, priv);
	
	
	GtkNotebook * notebook = GTK_NOTEBOOK(priv->notebook);
	int cur_page = gtk_notebook_get_current_page(notebook);
	
	GtkWidget * webview = gtk_notebook_get_nth_page(notebook, cur_page);
	if(webview)
	{
		WebKitSettings * settings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(webview));
		if(settings)
		{
			webkit_settings_set_enable_developer_extras(settings, TRUE);
			WebKitWebInspector * inspector = webkit_web_view_get_inspector(WEBKIT_WEB_VIEW(webview));
			webkit_web_inspector_show(inspector);
		}
	}
	
	return;
}


static int s_page_id = 1;

static void on_destroy_notify(GtkWidget * webview, shell_private_data_t * priv)
{
	debug_printf("(%p,%p)", webview, priv);
	
	
	//~ int id = GPOINTER_TO_INT(user_data);
	//~ printf("view %d is destroyed\n", id);
	webview_list_t * list = priv->list;
	
	list->remove(list, webview);
	
	return;
}

int shell_private_add_new_page(shell_private_data_t * priv, GtkWidget * webview)
{
	webview_list_t * list = priv->list;
	
	if(NULL == webview) webview = webkit_web_view_new();
	
	char label_name[100];
	snprintf(label_name, sizeof(label_name), "view_%d", ++s_page_id);
	GtkNotebook * notebook = GTK_NOTEBOOK(priv->notebook);
	list->add(list, webview);
	
	GtkWidget * label = gtk_label_new(label_name);
	gtk_notebook_append_page(notebook, webview, label);
	gtk_widget_show(webview);
	
	g_signal_connect(webview, "destroy", G_CALLBACK(on_destroy_notify), priv);
	
	gtk_notebook_set_current_page(notebook, -1);
	return 0;
}

static void on_add_page(GtkWidget * button, shell_private_data_t * priv)
{
	debug_printf("(%p,%p)", button, priv);
	
	
	
	shell_private_add_new_page(priv, NULL);
	
	return;
}



static void on_remove_page(GtkWidget * button, shell_private_data_t * priv)
{
	debug_printf("(%p,%p)", button, priv);
	
	
	webview_list_t * list = priv->list;
	GtkNotebook * notebook = GTK_NOTEBOOK(priv->notebook);
	
	int cur_page = gtk_notebook_get_current_page(notebook);
	GtkWidget * webview = gtk_notebook_get_nth_page(notebook, cur_page);
	
	list->remove(list, webview);
	//gtk_widget_destroy(webview);
	
	gtk_notebook_remove_page(notebook, cur_page);
	
	return;
}


static void init_navigation_bar(GtkWidget * hbox, shell_private_data_t * priv)
{
	GtkWidget * prev_button = gtk_button_new_from_icon_name("go-previous", GTK_ICON_SIZE_SMALL_TOOLBAR);
	GtkWidget * next_button = gtk_button_new_from_icon_name("go-next", GTK_ICON_SIZE_SMALL_TOOLBAR);
	GtkWidget * refresh_button = gtk_button_new_from_icon_name("view-refresh", GTK_ICON_SIZE_SMALL_TOOLBAR);
	GtkWidget * home_button = gtk_button_new_from_icon_name("go-home", GTK_ICON_SIZE_SMALL_TOOLBAR);
	GtkWidget * uri_entry = gtk_entry_new();

	GtkWidget * options = gtk_button_new_from_icon_name("preferences-system", GTK_ICON_SIZE_SMALL_TOOLBAR);
	GtkWidget * show_inspector = gtk_button_new_from_icon_name("help-contents", GTK_ICON_SIZE_SMALL_TOOLBAR);
	
	gtk_box_pack_start(GTK_BOX(hbox), show_inspector, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), prev_button, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), next_button, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), refresh_button, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), home_button, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), uri_entry, TRUE, TRUE, 0);
	
	gtk_box_pack_start(GTK_BOX(hbox), options, FALSE, TRUE, 0);
	
	
	gtk_widget_set_size_request(uri_entry, 400, -1);
	gtk_widget_set_hexpand(uri_entry, TRUE);
	
	g_signal_connect(prev_button, "clicked", G_CALLBACK(go_prev), priv);
	g_signal_connect(next_button, "clicked", G_CALLBACK(go_next), priv);
	g_signal_connect(refresh_button, "clicked", G_CALLBACK(view_refresh), priv);
	g_signal_connect(home_button, "clicked", G_CALLBACK(go_home), priv);
	g_signal_connect(uri_entry, "activate", G_CALLBACK(on_uri_entry_activate), priv);
	g_signal_connect(options, "clicked", G_CALLBACK(on_options), priv);
	g_signal_connect(show_inspector, "clicked", G_CALLBACK(on_show_inspector), priv);
	
	
	priv->uri_entry = uri_entry;
	return;
}



static void on_switch_page(GtkNotebook *notebook, GtkWidget * page, guint page_num, shell_private_data_t * priv)
{
	WebKitWebView * webview = WEBKIT_WEB_VIEW(page);
	const char * uri = webkit_web_view_get_uri(webview);
	if(NULL == uri) uri = "";
	gtk_entry_set_text(GTK_ENTRY(priv->uri_entry), uri);
	return;
}
            
static void init_windows(shell_private_data_t * priv)
{
	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget * grid = gtk_grid_new();
	GtkWidget * header_bar = gtk_header_bar_new();
	GtkWidget * hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget * switcher = gtk_stack_switcher_new();
	GtkWidget * stack = gtk_stack_new();
	GtkWidget * statusbar = gtk_statusbar_new();
	
	GtkWidget * notebook = gtk_notebook_new();
	GtkWidget * scrolled_win;
	GtkWidget * textview;
	
	gtk_widget_set_margin_top(statusbar, 0);
	gtk_widget_set_margin_bottom(statusbar, 0);
	
	GtkWidget * webview = webkit_web_view_new();
	gtk_widget_set_hexpand(webview, TRUE);
	gtk_widget_set_vexpand(webview, TRUE);
	
	webview_list_t * list  = priv->list;
	list->add(list, webview);
	
	gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
	
	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_grid_attach(GTK_GRID(grid), hbox, 0, 0, 1, 1);
	
	//~ gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), switcher);
	gtk_box_pack_end(GTK_BOX(statusbar), switcher, FALSE, TRUE, 1);
	
	gtk_grid_attach(GTK_GRID(grid), stack, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), statusbar ,0, 2, 1, 1);
	
	gtk_widget_set_halign(switcher, GTK_ALIGN_END);
	//gtk_widget_set_hexpand(switcher, TRUE);
	
	gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(stack));
	
	gtk_stack_add_titled(GTK_STACK(stack), notebook, "notebook", "Viewer");
	gtk_widget_set_hexpand(notebook, TRUE);
	gtk_widget_set_vexpand(notebook, TRUE);
	
	gtk_widget_set_hexpand(stack, TRUE);
	gtk_widget_set_vexpand(stack, TRUE);
	
	scrolled_win = gtk_scrolled_window_new(NULL, NULL);
	textview = gtk_text_view_new();
	gtk_container_add(GTK_CONTAINER(scrolled_win), textview);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_win), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
	
	gtk_stack_add_titled(GTK_STACK(stack), scrolled_win, "textview", "HTML"); 	
	
	// add webview to notebook
	GtkWidget * label = gtk_label_new("view_1");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), webview, label);	
	gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TRUE);
	
	GtkWidget * button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	GtkWidget * add = gtk_button_new_from_icon_name("window-new", GTK_ICON_SIZE_SMALL_TOOLBAR);
	GtkWidget * remove = gtk_button_new_from_icon_name("window-close", GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_box_pack_start(GTK_BOX(button_box), add, FALSE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(button_box), remove, FALSE, TRUE, 1);
	gtk_widget_show_all(button_box);
	
	gtk_notebook_set_action_widget(GTK_NOTEBOOK(notebook), button_box, GTK_PACK_START);
	//~ gtk_notebook_set_action_widget(GTK_NOTEBOOK(notebook), remove, GTK_PACK_START);
	gtk_widget_show(add);
	gtk_widget_show(remove);
	
	gtk_widget_set_margin_top(add, 1); gtk_widget_set_margin_bottom(add, 1);
	gtk_widget_set_margin_top(remove, 1); gtk_widget_set_margin_bottom(remove, 1);
	g_signal_connect(add, "clicked", G_CALLBACK(on_add_page), priv);
	g_signal_connect(remove, "clicked", G_CALLBACK(on_remove_page), priv);
	/* add navigation bar */
	
	gtk_widget_set_margin_top(switcher, 1);
	gtk_widget_set_margin_bottom(switcher, 1);
	
	init_navigation_bar(hbox, priv);
	
	
//	list->add(list, webview);
	
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	
	priv->window = window;
	priv->content_area = grid;
	priv->header_bar = header_bar;
	priv->switcher = switcher;
	priv->stack = stack;
	priv->statusbar = statusbar;
	
	priv->notebook = notebook;
	priv->textview = textview;
	

	
	g_signal_connect(notebook, "switch-page", G_CALLBACK(on_switch_page), priv);
}
