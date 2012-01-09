/*
 * Copyright © 2004 Noah Levitt
 * Copyright © 2007 Christian Persch
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

#include <gtk/gtk.h>

#define I_(string) g_intern_static_string (string)

/* The last unicode character we support */
#define UNICHAR_MAX (0x0010FFFFUL)

G_GNUC_INTERNAL void _mcharmap_intl_ensure_initialized (void);

G_GNUC_INTERNAL gboolean _mcharmap_unicode_has_nameslist_entry (gunichar uc);


struct _McharmapChaptersModelPrivate {
  McharmapCodepointList *book_list;
};

struct _McharmapChartablePrivate {
  /* scrollable implementation */
  GtkAdjustment *vadjustment;
  gulong vadjustment_changed_handler_id;

  /* Font */
  PangoFontDescription *font_desc;

  /* Geometry */
  int minimal_column_width;      /* depends on font_desc and size allocation */
  int minimal_row_height;        /* depends on font_desc and size allocation */
  int n_padded_columns;          /* columns 0..n-1 will be 1px wider than minimal_column_width */
  int n_padded_rows;             /* rows 0..n-1 will be 1px taller than minimal_row_height */
  int rows;
  int cols;
  int page_size;       /* rows * cols */

  int page_first_cell; /* the cell index of the top left corner */
  int active_cell;     /* the active cell index */
  int old_page_first_cell;
  int old_active_cell;

  /* Drawing */
  GdkPixmap *pixmap;
  PangoLayout *pango_layout;

  /* Zoom popup */
  GtkWidget *zoom_window;
  GtkWidget *zoom_image;

  /* for dragging (#114534) */
  gdouble click_x, click_y; 

  GtkTargetList *target_list;

  McharmapCodepointList *codepoint_list;
  int last_cell; /* from mcharmap_codepoint_list_get_last_index */
  gboolean codepoint_list_changed;

  /* Settings */
  guint snap_pow2_enabled : 1;
  guint zoom_mode_enabled : 1;
};

gint _mcharmap_chartable_cell_column	(McharmapChartable *chartable,
					 guint cell);
gint _mcharmap_chartable_column_width	(McharmapChartable *chartable,
					 gint col);
gint _mcharmap_chartable_x_offset	(McharmapChartable *chartable,
					 gint col);
gint _mcharmap_chartable_row_height	(McharmapChartable *chartable,
		 			 gint row);
gint _mcharmap_chartable_y_offset	(McharmapChartable *chartable,
					 gint row);
void _mcharmap_chartable_redraw	(McharmapChartable *chartable,
					 gboolean move_zoom);
