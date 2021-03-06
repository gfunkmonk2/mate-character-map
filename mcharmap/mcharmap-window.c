/*
 * Copyright © 2004 Noah Levitt
 * Copyright © 2007, 2008 Christian Persch
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02110-1301  USA
 */

#include <config.h>

#include <stdlib.h>
#include <string.h>

#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "mcharmap-print-operation.h"
#include "mcharmap-search-dialog.h"
#include "mcharmap-settings.h"
#include "mcharmap-window.h"

#define FONT_CHANGE_FACTOR (1.189207115f) /* 2^(0.25) */

/* #define ENABLE_PRINTING */

static void mcharmap_window_class_init (McharmapWindowClass *klass);
static void mcharmap_window_init       (McharmapWindow *window);

G_DEFINE_TYPE (McharmapWindow, mcharmap_window, GTK_TYPE_WINDOW)

static void
show_error_dialog (GtkWindow *parent,
                   GError *error)
{
  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (parent,
                                   GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                   GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                   "%s", error->message);
  g_signal_connect (dialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
  gtk_window_present (GTK_WINDOW (dialog));
}

#ifdef ENABLE_PRINTING

static void
ensure_print_data (McharmapWindow *guw)
{
  if (!guw->page_setup) {
    guw->page_setup = gtk_page_setup_new ();
  }

  if (!guw->print_settings) {
    guw->print_settings = gtk_print_settings_new ();
  }
}

static void
print_operation_done_cb (GtkPrintOperation *operation,
                         GtkPrintOperationResult result,
                         McharmapWindow *guw)
{
  if (result == GTK_PRINT_OPERATION_RESULT_ERROR) {
    GError *error = NULL;

    gtk_print_operation_get_error (operation, &error);
    show_error_dialog (GTK_WINDOW (guw), error);
    g_error_free (error);
  } else if (result == GTK_PRINT_OPERATION_RESULT_APPLY) {
    if (guw->print_settings)
      g_object_unref (guw->print_settings);
    guw->print_settings = g_object_ref (gtk_print_operation_get_print_settings (operation));
  }
}

static void
mcharmap_window_print (McharmapWindow *guw,
                        GtkPrintOperationAction action)
{
  GtkPrintOperation *op;
  PangoFontDescription *font_desc;
  McharmapCodepointList *codepoint_list;
  McharmapChartable *chartable;
  char *chapter, *filename;
  GtkPrintOperationResult rv;
  GError *error = NULL;

  g_object_get (guw->charmap,
                "active-codepoint-list", &codepoint_list,
                "font-desc", &font_desc,
                NULL);

  op = mcharmap_print_operation_new (codepoint_list, font_desc);
  if (codepoint_list)
    g_object_unref (codepoint_list);
  if (font_desc)
    pango_font_description_free (font_desc);

  ensure_print_data (guw);
  if (guw->page_setup)
    gtk_print_operation_set_default_page_setup (op, guw->page_setup);
  if (guw->print_settings)
    gtk_print_operation_set_print_settings (op, guw->print_settings);

  chapter = mcharmap_charmap_get_active_chapter (guw->charmap);
  if (chapter) {
    filename = g_strconcat (chapter, ".pdf", NULL);
    gtk_print_operation_set_export_filename (op, filename);
    g_free (filename);
    g_free (chapter);
  }

  gtk_print_operation_set_allow_async (op, TRUE);
  gtk_print_operation_set_show_progress (op, TRUE);

  g_signal_connect (op, "done",
                    G_CALLBACK (print_operation_done_cb), guw);

  rv = gtk_print_operation_run (op, action, GTK_WINDOW (guw), &error);
  if (rv == GTK_PRINT_OPERATION_RESULT_ERROR) {
    show_error_dialog (GTK_WINDOW (guw), error);
    g_error_free (error);
  }

  g_object_unref (op);
}

#endif /* ENABLE_PRINTING */

static void
status_message (GtkWidget       *widget, 
                const gchar     *message, 
                McharmapWindow *guw)
{
  gtk_statusbar_pop (GTK_STATUSBAR (guw->status), 0);

  if (message)
    gtk_statusbar_push (GTK_STATUSBAR (guw->status), 0, message);
}

static gboolean
update_progress_bar (McharmapWindow *guw)
{
  gdouble fraction_completed;

  fraction_completed = mcharmap_search_dialog_get_completed (MCHARMAP_SEARCH_DIALOG (guw->search_dialog));

  if (fraction_completed < 0 || fraction_completed > 1)
    {
      gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (guw->progress), 0);
      gtk_progress_bar_set_text (GTK_PROGRESS_BAR (guw->progress), NULL);
      return FALSE;
    }
  else
    {
      gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (guw->progress), fraction_completed);
      return TRUE;
    }
}

/* "progress" aka "busy-interactive" cursor (pointer + watch)
 * from mozilla 
 * caller should gdk_cursor_unref */
GdkCursor *
_mcharmap_window_progress_cursor (void)
{
  /* MOZ_CURSOR_SPINNING */
  static const char moz_spinning_bits[] = 
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
      0x00, 0x0c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3c, 0x00,
      0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0xfc,
      0x01, 0x00, 0x00, 0xfc, 0x3b, 0x00, 0x00, 0x7c, 0x38, 0x00, 0x00,
      0x6c, 0x54, 0x00, 0x00, 0xc4, 0xdc, 0x00, 0x00, 0xc0, 0x44, 0x00,
      0x00, 0x80, 0x39, 0x00, 0x00, 0x80, 0x39, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
    };
  static const char moz_spinning_mask_bits[] = 
    {
      0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00,
      0x00, 0x1e, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x7e, 0x00,
      0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0xfe,
      0x3b, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00,
      0xfe, 0xfe, 0x00, 0x00, 0xee, 0xff, 0x01, 0x00, 0xe4, 0xff, 0x00,
      0x00, 0xc0, 0x7f, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0x80, 0x39,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
    };

  GdkPixmap *cursor, *mask;
  GdkCursor *gdkcursor;
  GdkColor fg = { 0, 0, 0, 0 };             /* black */
  GdkColor bg = { 0, 65535, 65535, 65535 }; /* white */

  cursor = gdk_bitmap_create_from_data (NULL, moz_spinning_bits, 32, 32);
  mask = gdk_bitmap_create_from_data (NULL, moz_spinning_mask_bits, 32, 32);

  gdkcursor = gdk_cursor_new_from_pixmap (cursor, mask, &fg, &bg, 2, 2);

  g_object_unref (cursor);
  g_object_unref (mask);

  return gdkcursor;
}

static void
search_start (McharmapSearchDialog *search_dialog,
              McharmapWindow       *guw)
{
  GdkCursor *cursor;
  GtkAction *action;

  cursor = _mcharmap_window_progress_cursor ();
  gdk_window_set_cursor (gtk_widget_get_window (GTK_WIDGET (guw)), cursor);
  gdk_cursor_unref (cursor);

  action = gtk_action_group_get_action (guw->action_group, "Find");
  gtk_action_set_sensitive (action, FALSE);
  action = gtk_action_group_get_action (guw->action_group, "FindNext");
  gtk_action_set_sensitive (action, FALSE);
  action = gtk_action_group_get_action (guw->action_group, "FindPrevious");
  gtk_action_set_sensitive (action, FALSE);

  gtk_progress_bar_set_text (GTK_PROGRESS_BAR (guw->progress), _("Searching…"));
  g_timeout_add (100, (GSourceFunc) update_progress_bar, guw);
}

static void
search_finish (McharmapSearchDialog *search_dialog,
               gunichar               found_char,
               McharmapWindow       *guw)
{
  GtkAction *action;

  gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (guw->progress), 0);
  gtk_progress_bar_set_text (GTK_PROGRESS_BAR (guw->progress), NULL);

  if (found_char != (gunichar)(-1))
    mcharmap_charmap_set_active_character (guw->charmap, found_char);
  /* not-found dialog handled by McharmapSearchDialog */

  gdk_window_set_cursor (gtk_widget_get_window (GTK_WIDGET (guw)), NULL);

  action = gtk_action_group_get_action (guw->action_group, "Find");
  gtk_action_set_sensitive (action, TRUE);
  action = gtk_action_group_get_action (guw->action_group, "FindNext");
  gtk_action_set_sensitive (action, TRUE);
  action = gtk_action_group_get_action (guw->action_group, "FindPrevious");
  gtk_action_set_sensitive (action, TRUE);
}

static void
search_find (GtkAction       *action, 
             McharmapWindow *guw)
{
  g_assert (MCHARMAP_IS_WINDOW (guw));

  if (guw->search_dialog == NULL)
    {
      guw->search_dialog = mcharmap_search_dialog_new (guw);
      g_signal_connect (guw->search_dialog, "search-start", G_CALLBACK (search_start), guw);
      g_signal_connect (guw->search_dialog, "search-finish", G_CALLBACK (search_finish), guw);
    }

  mcharmap_search_dialog_present (MCHARMAP_SEARCH_DIALOG (guw->search_dialog));
}

static void
search_find_next (GtkAction       *action, 
                  McharmapWindow *guw)
{
  if (guw->search_dialog)
    mcharmap_search_dialog_start_search (MCHARMAP_SEARCH_DIALOG (guw->search_dialog), MCHARMAP_DIRECTION_FORWARD);
  else
    search_find (action, guw);
}

static void
search_find_prev (GtkAction       *action, 
                  McharmapWindow *guw)
{
  if (guw->search_dialog)
    mcharmap_search_dialog_start_search (MCHARMAP_SEARCH_DIALOG (guw->search_dialog), MCHARMAP_DIRECTION_BACKWARD);
  else
    search_find (action, guw);
}

#ifdef ENABLE_PRINTING

static void
page_setup_done_cb (GtkPageSetup *page_setup,
                    McharmapWindow *guw)
{
  if (page_setup) {
    g_object_unref (guw->page_setup);
    guw->page_setup = page_setup;
  }
}

static void
file_page_setup (GtkAction *action,
                 McharmapWindow *guw)
{
  ensure_print_data (guw);

  gtk_print_run_page_setup_dialog_async (GTK_WINDOW (guw),
                                         guw->page_setup,
                                         guw->print_settings,
                                         (GtkPageSetupDoneFunc) page_setup_done_cb,
                                         guw);
}

#if 0
static void
file_print_preview (GtkAction *action,
                    McharmapWindow *guw)
{
  mcharmap_window_print (guw, GTK_PRINT_OPERATION_ACTION_PREVIEW);
}
#endif

static void
file_print (GtkAction *action,
            McharmapWindow *guw)
{
  mcharmap_window_print (guw, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG);
}

#endif /* ENABLE_PRINTING */

static void
close_window (GtkAction *action,
              GtkWidget *widget)
{
  gtk_widget_destroy (widget);
}

static void
font_bigger (GtkAction       *action, 
             McharmapWindow *guw)
{
  mcharmap_mini_font_selection_change_font_size (MCHARMAP_MINI_FONT_SELECTION (guw->fontsel),
                                                  FONT_CHANGE_FACTOR);
}

static void
font_smaller (GtkAction       *action, 
              McharmapWindow *guw)
{
  mcharmap_mini_font_selection_change_font_size (MCHARMAP_MINI_FONT_SELECTION (guw->fontsel),
                                                  1.0f / FONT_CHANGE_FACTOR);
}

static void
font_default (GtkAction       *action, 
              McharmapWindow *guw)
{
  mcharmap_mini_font_selection_reset_font_size (MCHARMAP_MINI_FONT_SELECTION (guw->fontsel));
}

static void
snap_cols_pow2 (GtkAction        *action, 
                McharmapWindow  *guw)
{
  gboolean is_active = gtk_toggle_action_get_active (GTK_TOGGLE_ACTION (action));
  mcharmap_charmap_set_snap_pow2 (guw->charmap, is_active);
  mcharmap_settings_set_snap_pow2 (is_active);
}

static void
open_url (GtkWindow *parent,
          const char *uri,
          guint32 user_time)
{
  GdkScreen *screen;
  GError *error = NULL;

  if (parent)
    screen = gtk_widget_get_screen (GTK_WIDGET (parent));
  else
    screen = gdk_screen_get_default ();

  if (!gtk_show_uri (screen, uri, user_time, &error)) {
    show_error_dialog (parent, error);
    g_error_free (error);
  }
}

static void
help_contents (GtkAction *action,
               McharmapWindow *window)
{
  const char *lang;
  char *uri = NULL, *url;
  guint i;
 
  const char * const * langs = g_get_language_names ();
  for (i = 0; langs[i]; i++) {
    lang = langs[i];
    if (strchr (lang, '.')) {
      continue;
    }
 
    uri = g_build_filename (HELPDIR,
                            "mcharmap", /* DOC_MODULE */
                            lang,
                            "mcharmap.xml",
                            NULL);
					
    if (g_file_test (uri, G_FILE_TEST_EXISTS)) {
      break;
    }

    g_free (uri);
    uri = NULL;
  }

  if (!uri)
    return;

  url = g_strconcat ("ghelp://", uri, NULL);
  open_url (GTK_WINDOW (window), url, gtk_get_current_event_time ());
  g_free (uri);
  g_free (url);
}

#if !GTK_CHECK_VERSION(3, 0, 0)

	static void about_open_url(GtkAboutDialog* about, const char* link, gpointer data)
	{
		open_url(GTK_WINDOW(about), link, gtk_get_current_event_time());
	}

	static void about_email_hook(GtkAboutDialog* about, const char* email_address, gpointer user_data)
	{
		char* escaped;
		char* uri;

		escaped = g_uri_escape_string(email_address, NULL, FALSE);
		uri = g_strdup_printf("mailto:%s", escaped);
		g_free(escaped);

		open_url(GTK_WINDOW(about), uri, gtk_get_current_event_time());
		g_free(uri);
	}

#endif

static void
help_about (GtkAction       *action, 
            McharmapWindow *guw)
{
  const gchar *authors[] =
    {
      "Noah Levitt <nlevitt@columbia.edu>",
      "Daniel Elstner <daniel.elstner@gmx.net>",
      "Padraig O'Briain <Padraig.Obriain@sun.com>",
      "Christian Persch <" "chpe" "\100" "mate" "." "org" ">",
      NULL
    };

  const gchar *documenters[] =
    {
      "Chee Bin HOH <cbhoh@gnome.org>",
      "Sun Microsystems",
      NULL
    };	  

  const gchar *license[] = {
    N_("Mcharmap is free software; you can redistribute it and/or modify "
       "it under the terms of the GNU General Public License as published by "
       "the Free Software Foundation; either version 3 of the License, or "
       "(at your option) any later version."),
    N_("Permission is hereby granted, free of charge, to any person obtaining "
       "a copy of the Unicode data files to deal in them without restriction, "
       "including without limitation the rights to use, copy, modify, merge, "
       "publish, distribute, and/or sell copies."),
    N_("Mcharmap and the Unicode data files are distributed in the hope that "
       "they will be useful, but WITHOUT ANY WARRANTY; without even the implied "
       "warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See "
       "the GNU General Public License and Unicode Copyright for more details."),
    N_("You should have received a copy of the GNU General Public License "
       "along with Mcharmap; if not, write to the Free Software Foundation, Inc., "
       "59 Temple Place, Suite 330, Boston, MA  02110-1301  USA"),
    N_("Also you should have received a copy of the Unicode Copyright along "
       "with Mcharmap; you can always find it at Unicode's website: "
       "http://www.unicode.org/copyright.html")
  };
  gchar *license_trans;
  license_trans = g_strconcat (_(license[0]), "\n\n", _(license[1]), "\n\n",
			       _(license[2]), "\n\n", _(license[3]), "\n\n",
			       _(license[4]), "\n\n", NULL);

	#if !GTK_CHECK_VERSION(3, 0, 0)
		gtk_about_dialog_set_url_hook(about_open_url, NULL, NULL);
		gtk_about_dialog_set_email_hook(about_email_hook, NULL, NULL);
	#endif

  gtk_show_about_dialog (GTK_WINDOW (guw),
			 "program-name", _("MATE Character Map"),
			 "version", VERSION,
			 "comments", _("Based on the Unicode Character Database 5.2"),
			 "copyright", "Copyright © 2004 Noah Levitt\n"
				      "Copyright © 1991–2009 Unicode, Inc.\n"
				      "Copyright © 2007–2010 Christian Persch",
			 "documenters", documenters,
			 "license", license_trans,
			 "wrap-license", TRUE,
			 "logo-icon-name", MCHARMAP_ICON_NAME,
  			 "authors", authors,
			 "translator-credits", _("translator-credits"),
			 "website", "http://live.gnome.org/Mcharmap",
			 NULL);

  g_free (license_trans);
}

static void
next_or_prev_character (GtkAction       *action,
                        McharmapWindow *guw)
{
  McharmapChartable *chartable;
  McharmapChartableClass *klass;
  GtkBindingSet *binding_set;
  const char *name;
  guint keyval = 0;

  name = gtk_action_get_name (action);
  if (strcmp (name, "NextCharacter") == 0) {
    keyval = GDK_Right;
  } else if (strcmp (name, "PreviousCharacter") == 0) {
    keyval = GDK_Left;
  }

  chartable = mcharmap_charmap_get_chartable (guw->charmap);
  klass = MCHARMAP_CHARTABLE_GET_CLASS (chartable);
  binding_set = gtk_binding_set_by_class (klass);
  gtk_binding_set_activate (gtk_binding_set_by_class (klass),
                            keyval,
                            0,
                            GTK_OBJECT (chartable));
}

static void
next_chapter (GtkAction       *action,
              McharmapWindow *guw)
{
  mcharmap_charmap_next_chapter (guw->charmap);
}

static void
prev_chapter (GtkAction       *action,
              McharmapWindow *guw)
{
  mcharmap_charmap_previous_chapter (guw->charmap);
}

static void
chapters_set_labels (const gchar     *labelnext,
		     const gchar     *labelprev,
		     McharmapWindow *guw)
{
  GtkAction *action;

  action = gtk_action_group_get_action (guw->action_group, "NextChapter");
  g_object_set ( G_OBJECT (action), "label", labelnext, NULL);
  action = gtk_action_group_get_action (guw->action_group, "PreviousChapter");
  g_object_set ( G_OBJECT (action), "label", labelprev, NULL);
}

enum {
  VIEW_BY_SCRIPT,
  VIEW_BY_BLOCK
};

static void
mcharmap_window_set_chapters_model (McharmapWindow *guw,
                                     McharmapChaptersMode mode)
{
  McharmapChaptersModel *model = NULL;

  switch (mode)
    {
      case MCHARMAP_CHAPTERS_SCRIPT:
      	model = mcharmap_script_chapters_model_new ();
	chapters_set_labels (_("Next Script"), _("Previous Script"), guw);
	break;
      
      case MCHARMAP_CHAPTERS_BLOCK:
      	model = mcharmap_block_chapters_model_new ();
	chapters_set_labels (_("Next Block"), _("Previous Block"), guw);
	break;
      
      default:
        g_assert_not_reached ();
    }

  mcharmap_charmap_set_chapters_model (guw->charmap, model);
  g_object_unref (model);
}

static void
view_by (GtkAction        *action,
	 GtkRadioAction   *radioaction,
         McharmapWindow  *guw)
{
  McharmapChaptersMode mode;

  switch (gtk_radio_action_get_current_value (radioaction))
    {
      case VIEW_BY_SCRIPT:
        mode = MCHARMAP_CHAPTERS_SCRIPT;
	break;
      
      case VIEW_BY_BLOCK:
        mode = MCHARMAP_CHAPTERS_BLOCK;
	break;
      
      default:
        g_assert_not_reached ();
    }

  mcharmap_window_set_chapters_model (guw, mode);
  mcharmap_settings_set_chapters_mode (mode);
}

#ifdef DEBUG_chpe
static void
move_to_next_screen_cb (GtkAction *action,
                        GtkWidget *widget)
{
  GdkScreen *screen;
  GdkDisplay *display;
  int number_of_screens, screen_num;

  screen = gtk_widget_get_screen (widget);
  display = gdk_screen_get_display (screen);
  screen_num = gdk_screen_get_number (screen);
  number_of_screens =  gdk_display_get_n_screens (display);

  if ((screen_num + 1) < number_of_screens) {
    screen = gdk_display_get_screen (display, screen_num + 1);
  } else {
    screen = gdk_display_get_screen (display, 0);
  }

  gtk_window_set_screen (GTK_WINDOW (widget), screen);
}
#endif

/* create the menu entries */

static const char ui_info [] =
  "<menubar name='MenuBar'>"
    "<menu action='File'>"
#ifdef ENABLE_PRINTING
      "<menuitem action='PageSetup' />"
#if 0
      "<menuitem action='PrintPreview' />"
#endif
      "<menuitem action='Print' />"
      "<separator />"
#endif /* ENABLE_PRINTING */
      "<menuitem action='Close' />"
#ifdef DEBUG_chpe
      "<menuitem action='MoveNextScreen' />"
#endif
    "</menu>"
    "<menu action='View'>"
      "<menuitem action='ByScript' />"
      "<menuitem action='ByUnicodeBlock' />"
      "<separator />"
      "<menuitem action='SnapColumns' />"
      "<separator />"
      "<menuitem action='ZoomIn' />"
      "<menuitem action='ZoomOut' />"
      "<menuitem action='NormalSize' />"
    "</menu>"
    "<menu action='Search'>"
      "<menuitem action='Find' />"
      "<menuitem action='FindNext' />"
      "<menuitem action='FindPrevious' />"
    "</menu>"
    "<menu action='Go'>"
      "<menuitem action='NextCharacter' />"
      "<menuitem action='PreviousCharacter' />"
      "<separator />"
      "<menuitem action='NextChapter' />"
      "<menuitem action='PreviousChapter' />"
    "</menu>"
    "<menu action='Help'>"
      "<menuitem action='HelpContents' />"
      "<menuitem action='About' />"
    "</menu>"
  "</menubar>";

static void
insert_character_in_text_to_copy (McharmapChartable *chartable,
                                  McharmapWindow *guw)
{
  gchar ubuf[7];
  gint pos = -1;
  gunichar wc;

  wc = mcharmap_chartable_get_active_character (chartable);
  /* Can't copy values that are not valid unicode characters */
  if (!mcharmap_unichar_validate (wc))
    return;

  ubuf[g_unichar_to_utf8 (wc, ubuf)] = '\0';
  gtk_editable_delete_selection (GTK_EDITABLE (guw->text_to_copy_entry));
  pos = gtk_editable_get_position (GTK_EDITABLE (guw->text_to_copy_entry));
  gtk_editable_insert_text (GTK_EDITABLE (guw->text_to_copy_entry), ubuf, -1, &pos);
  gtk_editable_set_position (GTK_EDITABLE (guw->text_to_copy_entry), pos);
}

static void
edit_copy (GtkWidget *widget, McharmapWindow *guw)
{
  /* if nothing is selected, select the whole thing */
  if (! gtk_editable_get_selection_bounds (
              GTK_EDITABLE (guw->text_to_copy_entry), NULL, NULL))
    gtk_editable_select_region (GTK_EDITABLE (guw->text_to_copy_entry), 0, -1);

  gtk_editable_copy_clipboard (GTK_EDITABLE (guw->text_to_copy_entry));
}

static void
entry_changed_sensitize_button (GtkEditable *editable, GtkWidget *button)
{
  const gchar *entry_text = gtk_entry_get_text (GTK_ENTRY (editable));
  gtk_widget_set_sensitive (button, entry_text[0] != '\0');
}

static void
status_realize (GtkWidget       *status,
                McharmapWindow *guw)
{
  GtkAllocation *allocation;
#if GTK_CHECK_VERSION (2, 18, 0)
  GtkAllocation widget_allocation;

  gtk_widget_get_allocation (guw->status, &widget_allocation);
  allocation = &widget_allocation;
#else
  allocation = &guw->status->allocation;
#endif

  /* FIXMEchpe ewww... */
  /* increase the height a bit so it doesn't resize itself */
  gtk_widget_set_size_request (guw->status, -1, allocation->height + 9);
}

static gboolean
save_last_char_idle_cb (McharmapWindow *guw)
{
  gunichar wc;

  guw->save_last_char_idle_id = 0;

  wc = mcharmap_charmap_get_active_character (guw->charmap);
  mcharmap_settings_set_last_char (wc);

  return FALSE;
}

static void
fontsel_sync_font_desc (McharmapMiniFontSelection *fontsel,
                        GParamSpec *pspec,
                        McharmapWindow *guw)
{
  PangoFontDescription *font_desc;
  char *font;

  if (guw->in_notification)
    return;

  font_desc = mcharmap_mini_font_selection_get_font_desc (fontsel);

  guw->in_notification = TRUE;
  mcharmap_charmap_set_font_desc (guw->charmap, font_desc);
  guw->in_notification = FALSE;

  font = pango_font_description_to_string (font_desc);
  mcharmap_settings_set_font (font);
  g_free (font);
}

static void
charmap_sync_font_desc (McharmapCharmap *charmap,
                        GParamSpec *pspec,
                        McharmapWindow *guw)
{
  PangoFontDescription *font_desc;

  if (guw->in_notification)
    return;

  font_desc = mcharmap_charmap_get_font_desc (charmap);

  guw->in_notification = TRUE;
  mcharmap_mini_font_selection_set_font_desc (MCHARMAP_MINI_FONT_SELECTION (guw->fontsel),
                                               font_desc);
  guw->in_notification = FALSE;
}

static void
charmap_sync_active_character (GtkWidget *widget,
                               GParamSpec *pspec,
                               McharmapWindow *guw)
{
  if (guw->save_last_char_idle_id != 0)
    return;

  guw->save_last_char_idle_id = g_idle_add ((GSourceFunc) save_last_char_idle_cb, guw);
}

static void
mcharmap_window_init (McharmapWindow *guw)
{
  GtkWidget *big_vbox, *hbox, *bbox, *button, *label;
  McharmapChartable *chartable;
  /* tooltips are NULL because they are never actually shown in the program */
  const GtkActionEntry menu_entries[] =
  {
    { "File", NULL, N_("_File"), NULL, NULL, NULL },
    { "View", NULL, N_("_View"), NULL, NULL, NULL },
    { "Search", NULL, N_("_Search"), NULL, NULL, NULL },
    { "Go", NULL, N_("_Go"), NULL, NULL, NULL },
    { "Help", NULL, N_("_Help"), NULL, NULL, NULL },

#ifdef ENABLE_PRINTING
    { "PageSetup", NULL, N_("Page _Setup"), NULL,
      NULL, G_CALLBACK (file_page_setup) },
#if 0
    { "PrintPreview", GTK_STOCK_PRINT_PREVIEW, NULL, NULL,
      NULL, G_CALLBACK (file_print_preview) },
#endif
    { "Print", GTK_STOCK_PRINT, NULL, NULL,
      NULL, G_CALLBACK (file_print) },
#endif /* ENABLE_PRINTING */
    { "Close", GTK_STOCK_CLOSE, NULL, NULL,
      NULL, G_CALLBACK (close_window) },

    { "ZoomIn", GTK_STOCK_ZOOM_IN, NULL, "<control>plus",
      NULL, G_CALLBACK (font_bigger) },
    { "ZoomOut", GTK_STOCK_ZOOM_OUT, NULL, "<control>minus",
      NULL, G_CALLBACK (font_smaller) },
    { "NormalSize", GTK_STOCK_ZOOM_100, NULL, "<control>0",
      NULL, G_CALLBACK (font_default) },

    { "Find", GTK_STOCK_FIND, NULL, NULL,
      NULL, G_CALLBACK (search_find) },
    { "FindNext", NULL, N_("Find _Next"), "<control>G",
      NULL, G_CALLBACK (search_find_next) },
    { "FindPrevious", NULL, N_("Find _Previous"), "<shift><control>G",
      NULL, G_CALLBACK (search_find_prev) },

    { "NextCharacter", NULL, N_("_Next Character"), "<control>N",
      NULL, G_CALLBACK (next_or_prev_character) },
    { "PreviousCharacter", NULL, N_("_Previous Character"), "<control>P",
      NULL, G_CALLBACK (next_or_prev_character) },
    { "NextChapter", NULL, N_("Next Script"), "<control>Page_Down",
      NULL, G_CALLBACK (next_chapter) },
    { "PreviousChapter", NULL, N_("Previous Script"), "<control>Page_Up",
      NULL, G_CALLBACK (prev_chapter) },

    { "HelpContents", GTK_STOCK_HELP, N_("_Contents"), "F1",
      NULL, G_CALLBACK (help_contents) },
    { "About", GTK_STOCK_ABOUT, N_("_About"), NULL,
      NULL, G_CALLBACK (help_about) },

  #ifdef DEBUG_chpe
    { "MoveNextScreen", NULL, "Move window to next screen", NULL,
      NULL, G_CALLBACK (move_to_next_screen_cb) },
  #endif
  };
  const GtkRadioActionEntry radio_menu_entries[] =
  {
    { "ByScript", NULL, N_("By _Script"), NULL,
      NULL, VIEW_BY_SCRIPT },
    { "ByUnicodeBlock", NULL, N_("By _Unicode Block"), NULL,
      NULL, VIEW_BY_BLOCK }
  };
  const GtkToggleActionEntry toggle_menu_entries[] =
  {
    { "SnapColumns", NULL, N_("Snap _Columns to Power of Two"), NULL,
      NULL,
      G_CALLBACK (snap_cols_pow2), FALSE },
  };
  GtkWidget *menubar;
  GtkAction *action;

  gtk_window_set_title (GTK_WINDOW (guw), _("Character Map"));
  gtk_window_set_icon_name (GTK_WINDOW (guw), MCHARMAP_ICON_NAME);

  /* UI manager setup */
  guw->uimanager = gtk_ui_manager_new();

  gtk_window_add_accel_group ( GTK_WINDOW (guw),
  			       gtk_ui_manager_get_accel_group (guw->uimanager) );
  
  guw->action_group = gtk_action_group_new ("mcharmap_actions");
  gtk_action_group_set_translation_domain (guw->action_group, GETTEXT_PACKAGE);

  gtk_action_group_add_actions (guw->action_group,
  				menu_entries,
				G_N_ELEMENTS (menu_entries),
				guw);
  gtk_action_group_add_radio_actions (guw->action_group,
  				      radio_menu_entries,
				      G_N_ELEMENTS (radio_menu_entries),
				      mcharmap_settings_get_chapters_mode(),
				      G_CALLBACK (view_by),
				      guw);
  gtk_action_group_add_toggle_actions (guw->action_group,
  				       toggle_menu_entries,
				       G_N_ELEMENTS (toggle_menu_entries),
				       guw);

  gtk_ui_manager_insert_action_group (guw->uimanager, guw->action_group, 0);
  g_object_unref (guw->action_group);
  
  gtk_ui_manager_add_ui_from_string (guw->uimanager, ui_info, strlen (ui_info), NULL);
  
  /* Now the widgets */
  big_vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (guw), big_vbox);

  /* First the menubar */
  menubar = gtk_ui_manager_get_widget (guw->uimanager, "/MenuBar");
  gtk_box_pack_start (GTK_BOX (big_vbox), menubar, FALSE, FALSE, 0);

  /* The font selector */
  guw->fontsel = mcharmap_mini_font_selection_new ();
  g_signal_connect (guw->fontsel, "notify::font-desc",
                    G_CALLBACK (fontsel_sync_font_desc), guw);
  gtk_box_pack_start (GTK_BOX (big_vbox), guw->fontsel, FALSE, FALSE, 0);
  gtk_widget_show (GTK_WIDGET (guw->fontsel));

  /* The charmap */
  guw->charmap = MCHARMAP_CHARMAP (mcharmap_charmap_new ());
  g_signal_connect (guw->charmap, "notify::active-character",
                    G_CALLBACK (charmap_sync_active_character), guw);
  g_signal_connect (guw->charmap, "notify::font-desc",
                    G_CALLBACK (charmap_sync_font_desc), guw);

  gtk_box_pack_start (GTK_BOX (big_vbox), GTK_WIDGET (guw->charmap),
                      TRUE, TRUE, 0);
  gtk_widget_show (GTK_WIDGET (guw->charmap));

  /* Text to copy entry + button */
  hbox = gtk_hbox_new (FALSE, 6);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 6);

  label = gtk_label_new_with_mnemonic (_("_Text to copy:"));
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  bbox = gtk_hbutton_box_new ();

  button = gtk_button_new_from_stock (GTK_STOCK_COPY);
  gtk_widget_set_tooltip_text (button, _("Copy to the clipboard."));
  g_signal_connect (G_OBJECT (button), "clicked",
                    G_CALLBACK (edit_copy), guw);
  gtk_box_pack_start (GTK_BOX (bbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  gtk_widget_set_sensitive (button, FALSE);
  guw->text_to_copy_entry = gtk_entry_new ();
  gtk_label_set_mnemonic_widget (GTK_LABEL (label), guw->text_to_copy_entry);
  g_signal_connect (G_OBJECT (guw->text_to_copy_entry), "changed",
                    G_CALLBACK (entry_changed_sensitize_button), button);

  gtk_box_pack_start (GTK_BOX (hbox), guw->text_to_copy_entry, TRUE, TRUE, 0);
  gtk_widget_show (guw->text_to_copy_entry);

  gtk_box_pack_end (GTK_BOX (hbox), bbox, FALSE, FALSE, 0);
  gtk_widget_show (bbox);

  gtk_box_pack_start (GTK_BOX (big_vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  /* FIXMEchpe!! */
  chartable = mcharmap_charmap_get_chartable (guw->charmap);
  g_signal_connect (chartable, "activate", G_CALLBACK (insert_character_in_text_to_copy), guw);

  /* Finally the statusbar */
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (big_vbox), hbox, FALSE, FALSE, 0);

  guw->status = gtk_statusbar_new ();
  gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (guw->status), FALSE);
  gtk_box_pack_start (GTK_BOX (hbox), guw->status, TRUE, TRUE, 0);
  gtk_widget_show (guw->status);
  g_signal_connect (guw->status, "realize", G_CALLBACK (status_realize), guw);

  guw->progress = gtk_progress_bar_new ();
  gtk_box_pack_start (GTK_BOX (hbox), guw->progress, FALSE, FALSE, 0);

#if 0
  grip = gtk_statusbar_new ();
  gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (grip), TRUE);
  gtk_box_pack_start (GTK_BOX (hbox), grip, FALSE, FALSE, 0);
#endif
  gtk_widget_show_all (hbox);

  g_signal_connect (guw->charmap, "status-message", G_CALLBACK (status_message), guw);

  gtk_widget_show (big_vbox);

  action = gtk_action_group_get_action (guw->action_group, "SnapColumns");
  gtk_toggle_action_set_active (GTK_TOGGLE_ACTION (action),
                                mcharmap_settings_get_snap_pow2 ());

  mcharmap_window_set_chapters_model (guw, mcharmap_settings_get_chapters_mode ());

  mcharmap_charmap_set_active_character (guw->charmap,
                                          mcharmap_settings_get_last_char ());

  gtk_widget_grab_focus (GTK_WIDGET (mcharmap_charmap_get_chartable (guw->charmap)));
}

static void
mcharmap_window_finalize (GObject *object)
{
  McharmapWindow *guw = MCHARMAP_WINDOW (object);

  if (guw->save_last_char_idle_id != 0)
    g_source_remove (guw->save_last_char_idle_id);

  if (guw->page_setup)
    g_object_unref (guw->page_setup);

  if (guw->print_settings)
    g_object_unref (guw->print_settings);

  G_OBJECT_CLASS (mcharmap_window_parent_class)->finalize (object);
}

static void
mcharmap_window_class_init (McharmapWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = mcharmap_window_finalize;
}

/* Public API */

GtkWidget *
mcharmap_window_new (void)
{
  return GTK_WIDGET (g_object_new (mcharmap_window_get_type (), NULL));
}

void
mcharmap_window_set_font (McharmapWindow *guw,
                           const char *font)
{
  PangoFontDescription *font_desc;

  g_return_if_fail (MCHARMAP_IS_WINDOW (guw));

#if GTK_CHECK_VERSION (2,20,0)
  g_assert (!gtk_widget_get_realized (GTK_WIDGET (guw)));
#else
  g_assert (!GTK_WIDGET_REALIZED (guw));
#endif

  if (!font)
    return;

  font_desc = pango_font_description_from_string (font);
  mcharmap_charmap_set_font_desc (guw->charmap, font_desc);
  pango_font_description_free (font_desc);
}
