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

#ifndef MCHARMAP_CHAPTERS_MODEL_H
#define MCHARMAP_CHAPTERS_MODEL_H

#include <gtk/gtk.h>

#include <mcharmap/mcharmap-codepoint-list.h>

G_BEGIN_DECLS

#define MCHARMAP_TYPE_CHAPTERS_MODEL (mcharmap_chapters_model_get_type ())

#define MCHARMAP_TYPE_CHAPTERS_MODEL             (mcharmap_chapters_model_get_type ())
#define MCHARMAP_CHAPTERS_MODEL(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_CHAPTERS_MODEL, McharmapChaptersModel))
#define MCHARMAP_CHAPTERS_MODEL_CLASS(k)         (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_CHAPTERS_MODEL, McharmapChaptersModelClass))
#define MCHARMAP_IS_CHAPTERS_MODEL(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_CHAPTERS_MODEL))
#define MCHARMAP_IS_CHAPTERS_MODEL_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_CHAPTERS_MODEL))
#define MCHARMAP_CHAPTERS_MODEL_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_CHAPTERS_MODEL, McharmapChaptersModelClass))

typedef struct _McharmapChaptersModel        McharmapChaptersModel;
typedef struct _McharmapChaptersModelPrivate McharmapChaptersModelPrivate;
typedef struct _McharmapChaptersModelClass   McharmapChaptersModelClass;

struct _McharmapChaptersModel
{
  GtkListStore parent_instance;

  /*< private >*/
  McharmapChaptersModelPrivate *priv;
};

struct _McharmapChaptersModelClass
{
  GtkListStoreClass parent_class;

  const char *title;
  gboolean (* character_to_iter) (McharmapChaptersModel *chapters,
                                  gunichar wc,
                                  GtkTreeIter *iter);
  McharmapCodepointList * (* get_codepoint_list) (McharmapChaptersModel *chapters,
                                                   GtkTreeIter *iter);
  McharmapCodepointList * (* get_book_codepoint_list) (McharmapChaptersModel *chapters);
};


enum {
  MCHARMAP_CHAPTERS_MODEL_COLUMN_ID    = 0,
  MCHARMAP_CHAPTERS_MODEL_COLUMN_LABEL = 1
};

GType                    mcharmap_chapters_model_get_type                (void);
McharmapCodepointList * mcharmap_chapters_model_get_codepoint_list      (McharmapChaptersModel *chapters,
                                                                                          GtkTreeIter            *iter);
const char *             mcharmap_chapters_model_get_title               (McharmapChaptersModel *chapters);
McharmapCodepointList * mcharmap_chapters_model_get_book_codepoint_list (McharmapChaptersModel *chapters);
gboolean                 mcharmap_chapters_model_character_to_iter       (McharmapChaptersModel *chapters,
                                                                           gunichar                wc,
                                                                           GtkTreeIter            *iter);
gboolean                 mcharmap_chapters_model_id_to_iter              (McharmapChaptersModel *model,
                                                                           const char             *id,
                                                                           GtkTreeIter            *iter);

G_END_DECLS

#endif /* #ifndef MCHARMAP_CHAPTERS_MODEL_H */
