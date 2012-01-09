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

/* block means unicode block */

#if !defined (__MCHARMAP_MCHARMAP_H_INSIDE__) && !defined (MCHARMAP_COMPILATION)
#error "Only <mcharmap/mcharmap.h> can be included directly."
#endif

#ifndef MCHARMAP_BLOCK_CHAPTERS_MODEL_H
#define MCHARMAP_BLOCK_CHAPTERS_MODEL_H

#include <mcharmap/mcharmap-chapters-model.h>

G_BEGIN_DECLS

#define MCHARMAP_TYPE_BLOCK_CHAPTERS_MODEL             (mcharmap_block_chapters_model_get_type ())
#define MCHARMAP_BLOCK_CHAPTERS_MODEL(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_BLOCK_CHAPTERS_MODEL, McharmapBlockChaptersModel))
#define MCHARMAP_BLOCK_CHAPTERS_MODEL_CLASS(k) (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_BLOCK_CHAPTERS_MODEL, McharmapBlockChaptersModelClass))
#define MCHARMAP_IS_BLOCK_CHAPTERS_MODEL(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_BLOCK_CHAPTERS_MODEL))
#define MCHARMAP_IS_BLOCK_CHAPTERS_MODEL_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_BLOCK_CHAPTERS_MODEL))
#define MCHARMAP_BLOCK_CHAPTERS_MODEL_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_BLOCK_CHAPTERS_MODEL, McharmapBlockChaptersModelClass))

typedef struct _McharmapBlockChaptersModel McharmapBlockChaptersModel;
typedef struct _McharmapBlockChaptersModelPrivate McharmapBlockChaptersModelPrivate;
typedef struct _McharmapBlockChaptersModelClass McharmapBlockChaptersModelClass;

struct _McharmapBlockChaptersModel
{
  McharmapChaptersModel parent;

  /*< private >*/
  McharmapBlockChaptersModelPrivate *priv;
};

struct _McharmapBlockChaptersModelClass
{
  McharmapChaptersModelClass parent_class;
};

GType                   mcharmap_block_chapters_model_get_type (void);
McharmapChaptersModel* mcharmap_block_chapters_model_new      (void);

G_END_DECLS

#endif /* #ifndef MCHARMAP_BLOCK_CHAPTERS_MODEL_H */
