/*
 * Copyright © 2004 Noah Levitt
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

/* McharmapSearchDialog handles all aspects of searching */

#ifndef MCHARMAP_SEARCH_DIALOG_H
#define MCHARMAP_SEARCH_DIALOG_H

#include <gtk/gtk.h>
#include <mcharmap/mcharmap.h>
#include "mcharmap-window.h"

G_BEGIN_DECLS

#define MCHARMAP_TYPE_SEARCH_DIALOG             (mcharmap_search_dialog_get_type ())
#define MCHARMAP_SEARCH_DIALOG(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_SEARCH_DIALOG, McharmapSearchDialog))
#define MCHARMAP_SEARCH_DIALOG_CLASS(k) (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_SEARCH_DIALOG, McharmapSearchDialogClass))
#define MCHARMAP_IS_SEARCH_DIALOG(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_SEARCH_DIALOG))
#define MCHARMAP_IS_SEARCH_DIALOG_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_SEARCH_DIALOG))
#define MCHARMAP_SEARCH_DIALOG_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_SEARCH_DIALOG, McharmapSearchDialogClass))

typedef struct _McharmapSearchDialog McharmapSearchDialog;
typedef struct _McharmapSearchDialogClass McharmapSearchDialogClass;

struct _McharmapSearchDialog
{
  GtkDialog parent;
};

struct _McharmapSearchDialogClass
{
  GtkDialogClass parent_class;

  /* signals */
  void (* search_start)  (void);
  void (* search_finish) (gunichar found_char);
};

typedef enum
{
  MCHARMAP_DIRECTION_BACKWARD = -1,
  MCHARMAP_DIRECTION_FORWARD = 1
}
McharmapDirection;

GType       mcharmap_search_dialog_get_type      (void);
GtkWidget * mcharmap_search_dialog_new           (McharmapWindow *parent);
void        mcharmap_search_dialog_present       (McharmapSearchDialog *search_dialog);
void        mcharmap_search_dialog_start_search  (McharmapSearchDialog *search_dialog,
                                                   McharmapDirection     direction);
gdouble     mcharmap_search_dialog_get_completed (McharmapSearchDialog *search_dialog); 

G_END_DECLS

#endif /* #ifndef MCHARMAP_SEARCH_DIALOG_H */
