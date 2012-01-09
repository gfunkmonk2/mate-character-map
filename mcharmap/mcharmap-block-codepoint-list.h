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

#if !defined (__MCHARMAP_MCHARMAP_H_INSIDE__) && !defined (MCHARMAP_COMPILATION)
#error "Only <mcharmap/mcharmap.h> can be included directly."
#endif

#ifndef MCHARMAP_BLOCK_CODEPOINT_LIST_H
#define MCHARMAP_BLOCK_CODEPOINT_LIST_H

#include <glib-object.h>

#include <mcharmap/mcharmap-codepoint-list.h>

G_BEGIN_DECLS

#define MCHARMAP_TYPE_BLOCK_CODEPOINT_LIST             (mcharmap_block_codepoint_list_get_type ())
#define MCHARMAP_BLOCK_CODEPOINT_LIST(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_BLOCK_CODEPOINT_LIST, McharmapBlockCodepointList))
#define MCHARMAP_BLOCK_CODEPOINT_LIST_CLASS(k)         (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_BLOCK_CODEPOINT_LIST, McharmapBlockCodepointListClass))
#define MCHARMAP_IS_BLOCK_CODEPOINT_LIST(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_BLOCK_CODEPOINT_LIST))
#define MCHARMAP_IS_BLOCK_CODEPOINT_LIST_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_BLOCK_CODEPOINT_LIST))
#define MCHARMAP_BLOCK_CODEPOINT_LIST_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_BLOCK_CODEPOINT_LIST, McharmapBlockCodepointListClass))

typedef struct _McharmapBlockCodepointList         McharmapBlockCodepointList;
typedef struct _McharmapBlockCodepointListPrivate  McharmapBlockCodepointListPrivate;
typedef struct _McharmapBlockCodepointListClass    McharmapBlockCodepointListClass;

struct _McharmapBlockCodepointList
{
  McharmapCodepointList parent_instance;

  /*< private >*/
  McharmapBlockCodepointListPrivate *priv;
};

struct _McharmapBlockCodepointListClass
{
  McharmapCodepointListClass parent_class;
};

GType                    mcharmap_block_codepoint_list_get_type (void);

McharmapCodepointList * mcharmap_block_codepoint_list_new      (gunichar start,
                                                                  gunichar end);

G_END_DECLS

#endif /* #ifndef MCHARMAP_BLOCK_CODEPOINT_LIST_H */

