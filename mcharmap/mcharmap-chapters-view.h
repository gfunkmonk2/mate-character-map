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

/* block means unicode block */

#if !defined (__MCHARMAP_MCHARMAP_H_INSIDE__) && !defined (MCHARMAP_COMPILATION)
#error "Only <mcharmap/mcharmap.h> can be included directly."
#endif

#ifndef MCHARMAP_CHAPTERS_VIEW_H
#define MCHARMAP_CHAPTERS_VIEW_H

#include <gtk/gtk.h>

#include <mcharmap/mcharmap-chapters-model.h>

G_BEGIN_DECLS

#define MCHARMAP_TYPE_CHAPTERS_VIEW             (mcharmap_chapters_view_get_type ())
#define MCHARMAP_CHAPTERS_VIEW(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_CHAPTERS_VIEW, McharmapChaptersView))
#define MCHARMAP_CHAPTERS_VIEW_CLASS(k)         (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_CHAPTERS_VIEW, McharmapChaptersViewClass))
#define MCHARMAP_IS_CHAPTERS_VIEW(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_CHAPTERS_VIEW))
#define MCHARMAP_IS_CHAPTERS_VIEW_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_CHAPTERS_VIEW))
#define MCHARMAP_CHAPTERS_VIEW_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_CHAPTERS_VIEW, McharmapChaptersViewClass))

typedef struct _McharmapChaptersView         McharmapChaptersView;
typedef struct _McharmapChaptersViewPrivate  McharmapChaptersViewPrivate;
typedef struct _McharmapChaptersViewClass    McharmapChaptersViewClass;

struct _McharmapChaptersView
{
  GtkTreeView parent_instance;

  /*< private >*/
  McharmapChaptersViewPrivate *priv;
};

struct _McharmapChaptersViewClass
{
  GtkTreeViewClass parent_class;
};

GType       mcharmap_chapters_view_get_type (void);

GtkWidget * mcharmap_chapters_view_new      (void);

void                    mcharmap_chapters_view_set_model (McharmapChaptersView *view,
                                                           McharmapChaptersModel *model);
McharmapChaptersModel *mcharmap_chapters_view_get_model (McharmapChaptersView *view);

gboolean           mcharmap_chapters_view_select_character (McharmapChaptersView *view,
                                                             gunichar           wc);
McharmapCodepointList * mcharmap_chapters_view_get_codepoint_list      (McharmapChaptersView *view);
McharmapCodepointList * mcharmap_chapters_view_get_book_codepoint_list (McharmapChaptersView *view);

void               mcharmap_chapters_view_next         (McharmapChaptersView *view);
void               mcharmap_chapters_view_previous     (McharmapChaptersView *view);

gchar *            mcharmap_chapters_view_get_selected  (McharmapChaptersView *view);
gboolean           mcharmap_chapters_view_set_selected  (McharmapChaptersView *view,
                                                          const gchar       *name);

gboolean           mcharmap_chapters_view_select_locale (McharmapChaptersView *view);

G_END_DECLS

#endif /* #ifndef MCHARMAP_CHAPTERS_VIEW_H */
