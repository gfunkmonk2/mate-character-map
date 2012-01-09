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
 
#ifndef MCHARMAP_WINDOW_H
#define MCHARMAP_WINDOW_H

#include <gtk/gtk.h>
#include <mcharmap/mcharmap.h>
#include "mcharmap-mini-fontsel.h"

G_BEGIN_DECLS

#define MCHARMAP_TYPE_WINDOW             (mcharmap_window_get_type ())
#define MCHARMAP_WINDOW(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_WINDOW, McharmapWindow))
#define MCHARMAP_WINDOW_CLASS(k)         (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_WINDOW, McharmapWindowClass))
#define MCHARMAP_IS_WINDOW(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_WINDOW))
#define MCHARMAP_IS_WINDOW_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_WINDOW))
#define MCHARMAP_WINDOW_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_WINDOW, McharmapWindowClass))

typedef struct _McharmapWindow McharmapWindow;
typedef struct _McharmapWindowClass McharmapWindowClass;

struct _McharmapWindow
{
  GtkWindow parent;

  McharmapCharmap *charmap;
  GtkWidget *status;

  GtkWidget *fontsel;
  GtkWidget *text_to_copy_entry;

  GtkUIManager *uimanager;

  GtkActionGroup *action_group;

  GtkWidget *search_dialog; /* takes care of all aspects of searching */

  GtkWidget *progress;

  guint save_last_char_idle_id;

  GtkPageSetup *page_setup;
  GtkPrintSettings *print_settings;

  guint in_notification : 1;
};

struct _McharmapWindowClass
{
  GtkWindowClass parent_class;
};

#define MCHARMAP_ICON_NAME "accessories-character-map"

GType       mcharmap_window_get_type  (void);

GtkWidget * mcharmap_window_new       (void);

void        mcharmap_window_set_font  (McharmapWindow *guw,
                                        const char *font);

GdkCursor *_mcharmap_window_progress_cursor (void);

G_END_DECLS

#endif /* #ifndef MCHARMAP_WINDOW_H */
