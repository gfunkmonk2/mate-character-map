/*
 * Copyright © 2003  Sun Microsystems Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef MCHARMAP_CHARTABLE_CELL_ACCESSIBLE_H
#define MCHARMAP_CHARTABLE_CELL_ACCESSIBLE_H

#include <atk/atk.h>

#include "mcharmap-chartable.h"

G_BEGIN_DECLS

#define MCHARMAP_TYPE_CHARTABLE_CELL_ACCESSIBLE             (mcharmap_chartable_cell_accessible_get_type ())
#define MCHARMAP_CHARTABLE_CELL_ACCESSIBLE(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_CHARTABLE_CELL_ACCESSIBLE, McharmapChartableCellAccessible))
#define MCHARMAP_CHARTABLE_CELL_ACCESSIBLE_CLASS(k) (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_CHARTABLE_CELL_ACCESSIBLE, McharmapChartableCellAccessibleClass))
#define MCHARMAP_IS_CHARTABLE_CELL_ACCESSIBLE(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_CHARTABLE_CELL_ACCESSIBLE))
#define MCHARMAP_IS_CHARTABLE_CELL_ACCESSIBLE_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_CHARTABLE_CELL_ACCESSIBLE))
#define MCHARMAP_CHARTABLE_CELL_ACCESSIBLE_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_CHARTABLE_CELL_ACCESSIBLE, McharmapChartableCellAccessibleClass))

typedef struct _McharmapChartableCellAccessible      McharmapChartableCellAccessible;
typedef struct _McharmapChartableCellAccessibleClass McharmapChartableCellAccessibleClass;

struct _McharmapChartableCellAccessible
{
  AtkObject parent;

  GtkWidget    *widget;
  int           index;
  AtkStateSet  *state_set;
  gchar        *activate_description;
  guint         action_idle_handler;
};

struct _McharmapChartableCellAccessibleClass
{
  AtkObjectClass parent_class;
};

GType mcharmap_chartable_cell_accessible_get_type (void);

AtkObject* mcharmap_chartable_cell_accessible_new (void);

void mcharmap_chartable_cell_accessible_initialise (McharmapChartableCellAccessible *cell,
                                                     GtkWidget          *widget,
                                                     AtkObject          *parent,
                                                     gint                index);

gboolean mcharmap_chartable_cell_accessible_add_state (McharmapChartableCellAccessible *cell,
                                                        AtkStateType        state_type,
                                                        gboolean            emit_signal);

gboolean mcharmap_chartable_cell_accessible_remove_state (McharmapChartableCellAccessible *cell,
                                                           AtkStateType        state_type,
                                                           gboolean            emit_signal);

G_END_DECLS

#endif /* MCHARMAP_CHARTABLE_CELL_ACCESSIBLE_H */
