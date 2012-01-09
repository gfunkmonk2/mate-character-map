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

#if !defined (__MCHARMAP_MCHARMAP_H_INSIDE__) && !defined (MCHARMAP_COMPILATION)
#error "Only <mcharmap/mcharmap.h> can be included directly."
#endif

#ifndef MCHARMAP_CHARTABLE_H
#define MCHARMAP_CHARTABLE_H

#include <gtk/gtk.h>

#include <mcharmap/mcharmap-codepoint-list.h>

G_BEGIN_DECLS

#define MCHARMAP_TYPE_CHARTABLE             (mcharmap_chartable_get_type ())
#define MCHARMAP_CHARTABLE(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_CHARTABLE, McharmapChartable))
#define MCHARMAP_CHARTABLE_CLASS(k)         (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_CHARTABLE, McharmapChartableClass))
#define MCHARMAP_IS_CHARTABLE(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_CHARTABLE))
#define MCHARMAP_IS_CHARTABLE_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_CHARTABLE))
#define MCHARMAP_CHARTABLE_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_CHARTABLE, McharmapChartableClass))

typedef struct _McharmapChartable        McharmapChartable;
typedef struct _McharmapChartablePrivate McharmapChartablePrivate;
typedef struct _McharmapChartableClass   McharmapChartableClass;

struct _McharmapChartable
{
  GtkDrawingArea parent_instance;

  /*< private >*/
  McharmapChartablePrivate *priv;
};

struct _McharmapChartableClass
{
  GtkDrawingAreaClass parent_class;

  void    (* set_scroll_adjustments) (McharmapChartable *chartable,
                                      GtkAdjustment      *hadjustment,
                                      GtkAdjustment      *vadjustment);
  gboolean (* move_cursor)           (McharmapChartable *chartable,
                                      GtkMovementStep     step,
                                      gint                count);
  void (* activate) (McharmapChartable *chartable);
  void (* copy_clipboard) (McharmapChartable *chartable);
  void (* paste_clipboard) (McharmapChartable *chartable);

  void (* set_active_char) (McharmapChartable *chartable, guint ch);
  void (* status_message) (McharmapChartable *chartable, const gchar *message);
};

GType mcharmap_chartable_get_type (void);
GtkWidget * mcharmap_chartable_new (void);
void mcharmap_chartable_set_font_desc (McharmapChartable *chartable,
                                        PangoFontDescription *font_desc);
PangoFontDescription * mcharmap_chartable_get_font_desc (McharmapChartable *chartable);
gunichar mcharmap_chartable_get_active_character (McharmapChartable *chartable);
void mcharmap_chartable_set_active_character (McharmapChartable *chartable, 
                                               gunichar uc);
void mcharmap_chartable_set_zoom_enabled (McharmapChartable *chartable,
                                           gboolean enabled);
gboolean mcharmap_chartable_get_zoom_enabled (McharmapChartable *chartable);
void mcharmap_chartable_set_snap_pow2 (McharmapChartable *chartable,
                                        gboolean snap);
gboolean mcharmap_chartable_get_snap_pow2 (McharmapChartable *chartable);
void mcharmap_chartable_set_codepoint_list (McharmapChartable         *chartable,
                                             McharmapCodepointList *list);
McharmapCodepointList * mcharmap_chartable_get_codepoint_list (McharmapChartable *chartable);

G_END_DECLS

#endif  /* #ifndef MCHARMAP_CHARTABLE_H */
