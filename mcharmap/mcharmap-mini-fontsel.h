/*
 * Copyright © 2004 Noah Levitt
 * Copyright © 2008 Christian Persch
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

#ifndef MCHARMAP_MINI_FONTSEL_H
#define MCHARMAP_MINI_FONTSEL_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MCHARMAP_TYPE_MINI_FONT_SELECTION             (mcharmap_mini_font_selection_get_type ())
#define MCHARMAP_MINI_FONT_SELECTION(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_MINI_FONT_SELECTION, McharmapMiniFontSelection))
#define MCHARMAP_MINI_FONT_SELECTION_CLASS(k)         (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_MINI_FONT_SELECTION, McharmapMiniFontSelectionClass))
#define MCHARMAP_IS_MINI_FONT_SELECTION(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_MINI_FONT_SELECTION))
#define MCHARMAP_IS_MINI_FONT_SELECTION_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_MINI_FONT_SELECTION))
#define MCHARMAP_MINI_FONT_SELECTION_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_MINI_FONT_SELECTION, McharmapMiniFontSelectionClass))

typedef struct _McharmapMiniFontSelection McharmapMiniFontSelection;
typedef struct _McharmapMiniFontSelectionClass McharmapMiniFontSelectionClass;

struct _McharmapMiniFontSelection
{
  GtkHBox parent;

  GtkListStore         *family_store;
  GtkWidget            *family; /* combo box */
  GtkWidget            *bold;   /* toggle button*/
  GtkWidget            *italic; /* toggle button*/

  GtkObject            *size_adj; 
  GtkWidget            *size;   /* spin button */
  
  PangoFontDescription *font_desc;

  gint                  default_size;
};

struct _McharmapMiniFontSelectionClass
{
  GtkHBoxClass parent_class;
};


GType                  mcharmap_mini_font_selection_get_type         (void);

GtkWidget *            mcharmap_mini_font_selection_new              (void);

void                   mcharmap_mini_font_selection_set_font_desc    (McharmapMiniFontSelection *fontsel,
                                                                       PangoFontDescription       *font_desc);

PangoFontDescription * mcharmap_mini_font_selection_get_font_desc    (McharmapMiniFontSelection *fontsel);

void                   mcharmap_mini_font_selection_change_font_size (McharmapMiniFontSelection *fontsel,
                                                                       float factor);

void                   mcharmap_mini_font_selection_reset_font_size  (McharmapMiniFontSelection *fontsel);

G_END_DECLS

#endif /* #ifndef MCHARMAP_MINI_FONTSEL_H */
