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

#ifndef MCHARMAP_SCRIPT_CODEPOINT_LIST_H
#define MCHARMAP_SCRIPT_CODEPOINT_LIST_H

#include <glib-object.h>

#include <mcharmap/mcharmap-codepoint-list.h>

G_BEGIN_DECLS

#define MCHARMAP_TYPE_SCRIPT_CODEPOINT_LIST             (mcharmap_script_codepoint_list_get_type ())
#define MCHARMAP_SCRIPT_CODEPOINT_LIST(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_SCRIPT_CODEPOINT_LIST, McharmapScriptCodepointList))
#define MCHARMAP_SCRIPT_CODEPOINT_LIST_CLASS(k) (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_SCRIPT_CODEPOINT_LIST, McharmapScriptCodepointListClass))
#define MCHARMAP_IS_SCRIPT_CODEPOINT_LIST(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_SCRIPT_CODEPOINT_LIST))
#define MCHARMAP_IS_SCRIPT_CODEPOINT_LIST_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_SCRIPT_CODEPOINT_LIST))
#define MCHARMAP_SCRIPT_CODEPOINT_LIST_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_SCRIPT_CODEPOINT_LIST, McharmapScriptCodepointListClass))

typedef struct _McharmapScriptCodepointList        McharmapScriptCodepointList;
typedef struct _McharmapScriptCodepointListPrivate McharmapScriptCodepointListPrivate;
typedef struct _McharmapScriptCodepointListClass   McharmapScriptCodepointListClass;

struct _McharmapScriptCodepointList
{
  McharmapCodepointList parent;

  /*< private >*/
  McharmapScriptCodepointListPrivate *priv;
};

struct _McharmapScriptCodepointListClass
{
  McharmapCodepointListClass parent_class;
};

GType                    mcharmap_script_codepoint_list_get_type       (void);
McharmapCodepointList * mcharmap_script_codepoint_list_new            (void);
gboolean                 mcharmap_script_codepoint_list_set_script     (McharmapScriptCodepointList  *list,
	                                                                 const gchar                   *script);
gboolean                 mcharmap_script_codepoint_list_set_scripts    (McharmapScriptCodepointList  *list,
	                                                                 const gchar                  **scripts);
gboolean                 mcharmap_script_codepoint_list_append_script  (McharmapScriptCodepointList  *list,
                                                                         const gchar                   *script);
/* XXX: mcharmap_script_codepoint_list_get_script? seems unnecessary */

G_END_DECLS

#endif /* #ifndef MCHARMAP_SCRIPT_CODEPOINT_LIST_H */
