/*
 * Copyright © 2003  Sun Microsystems Inc.
 * Copyright © 2007  Christian Persch
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef MCHARMAP_CHARTABLE_ACCESSIBLE_H
#define MCHARMAP_CHARTABLE_ACCESSIBLE_H

#include <gtk/gtk.h>

#include "mcharmap-chartable.h"

G_BEGIN_DECLS

#define MCHARMAP_TYPE_CHARTABLE_ACCESSIBLE             (mcharmap_chartable_accessible_get_type ())
#define MCHARMAP_CHARTABLE_ACCESSIBLE(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_CHARTABLE_ACCESSIBLE, McharmapChartableAccessible))
#define MCHARMAP_CHARTABLE_ACCESSIBLE_CLASS(k) (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_CHARTABLE_ACCESSIBLE, McharmapChartableAccessibleClass))
#define MCHARMAP_IS_CHARTABLE_ACCESSIBLE(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_CHARTABLE_ACCESSIBLE))
#define MCHARMAP_IS_CHARTABLE_ACCESSIBLE_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_CHARTABLE_ACCESSIBLE))
#define MCHARMAP_CHARTABLE_ACCESSIBLE_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_CHARTABLE_ACCESSIBLE, McharmapChartableAccessibleClass))

typedef struct _McharmapChartableAccessible      McharmapChartableAccessible;
typedef struct _McharmapChartableAccessibleClass McharmapChartableAccessibleClass;

GType mcharmap_chartable_accessible_get_type (void);

AtkObject *mcharmap_chartable_accessible_new (McharmapChartable *chartable);

G_END_DECLS

#endif /* !MCHARMAP_CHARTABLE_ACCESSIBLE_H */
