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

#include <config.h>

#include <string.h>

#include "mcharmap.h"
#include "mcharmap-private.h"
#include "mcharmap-marshal.h"

G_DEFINE_TYPE (McharmapChaptersModel, mcharmap_chapters_model, GTK_TYPE_LIST_STORE)

static McharmapCodepointList * 
default_get_codepoint_list (McharmapChaptersModel *chapters,
                            GtkTreeIter *iter)
{
  return mcharmap_block_codepoint_list_new (0, UNICHAR_MAX);
}

static void
mcharmap_chapters_model_init (McharmapChaptersModel *model)
{
  model->priv = G_TYPE_INSTANCE_GET_PRIVATE (model, MCHARMAP_TYPE_CHAPTERS_MODEL, McharmapChaptersModelPrivate);
}

static void
mcharmap_chapters_model_finalize (GObject *object)
{
  McharmapChaptersModel *model = MCHARMAP_CHAPTERS_MODEL (object);
  McharmapChaptersModelPrivate *priv = model->priv;

  if (priv->book_list)
    g_object_unref (priv->book_list);

  G_OBJECT_CLASS (mcharmap_chapters_model_parent_class)->finalize (object);
}

static void
mcharmap_chapters_model_class_init (McharmapChaptersModelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (object_class, sizeof (McharmapChaptersModelPrivate));

  object_class->finalize = mcharmap_chapters_model_finalize;

  klass->get_codepoint_list = default_get_codepoint_list;
}

/**
 * mcharmap_chapters_model_get_codepoint_list:
 * @chapters: a #McharmapChaptersModel
 * @iter: a #GtkTreeIter
 *
 * Creates a new #McharmapCodepointList representing the characters in the
 * current chapter.
 *
 * Return value: the newly-created #McharmapCodepointList, or NULL if
 * there is no chapter selected. The caller should release the result with
 * g_object_unref() when finished.
 **/
McharmapCodepointList * 
mcharmap_chapters_model_get_codepoint_list (McharmapChaptersModel *chapters,
                                       GtkTreeIter       *iter)
{
  g_return_val_if_fail (MCHARMAP_IS_CHAPTERS_MODEL (chapters), NULL);

  return MCHARMAP_CHAPTERS_MODEL_GET_CLASS (chapters)->get_codepoint_list (chapters, iter);
}

/**
 * mcharmap_chapters_model_get_codepoint_list:
 * @chapters: a #McharmapChaptersModel
 *
 * Return value: a reference to a #McharmapCodepointList representing all the characters
 * in all the chapters. It should not be modified, but must be g_object_unref()'d after use.
 **/
McharmapCodepointList *
mcharmap_chapters_model_get_book_codepoint_list (McharmapChaptersModel *chapters)
{
  g_return_val_if_fail (MCHARMAP_IS_CHAPTERS_MODEL (chapters), NULL);

  return MCHARMAP_CHAPTERS_MODEL_GET_CLASS (chapters)->get_book_codepoint_list (chapters);
}

/**
 * mcharmap_chapters_model_character_to_iter:
 * @chapters: a #McharmapChaptersModel
 * @wc: a character
 * @iter: a #GtkTreeIter
 *
 * Return value: %TRUE on success, %FALSE on failure.
 **/
gboolean
mcharmap_chapters_model_character_to_iter (McharmapChaptersModel *chapters,
                                            gunichar           wc,
                                            GtkTreeIter       *iter)
{
  g_return_val_if_fail (MCHARMAP_IS_CHAPTERS_MODEL (chapters), FALSE);

  return MCHARMAP_CHAPTERS_MODEL_GET_CLASS (chapters)->character_to_iter (chapters, wc, iter);
}

const char *
mcharmap_chapters_model_get_title (McharmapChaptersModel *chapters)
{
  g_return_val_if_fail (MCHARMAP_IS_CHAPTERS_MODEL (chapters), NULL);

  return MCHARMAP_CHAPTERS_MODEL_GET_CLASS (chapters)->title;
}

gboolean
mcharmap_chapters_model_id_to_iter (McharmapChaptersModel *chapters_model,
                                     const char *id,
                                     GtkTreeIter *_iter)
{
  GtkTreeModel *model = GTK_TREE_MODEL (chapters_model);
  GtkTreeIter iter;
  char *str;
  int match;

  g_return_val_if_fail (MCHARMAP_IS_CHAPTERS_MODEL (model), FALSE);

  if (!id)
    return FALSE;

  if (!gtk_tree_model_get_iter_first (model, &iter))
    return FALSE;

  do {
    gtk_tree_model_get(model, &iter, MCHARMAP_CHAPTERS_MODEL_COLUMN_ID, &str, -1);
    match = strcmp (id, str);
    g_free(str);
    if (0 == match) {
      *_iter = iter;
      break;
    }
  } while (gtk_tree_model_iter_next (model, &iter));

  return 0 == match;
}
