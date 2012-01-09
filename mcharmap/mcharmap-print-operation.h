/*
*  Copyright © 2007, 2008 Christian Persch
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3, or (at your option)
*  any later version.
*
*  This program is distributed in the hope print_operation it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef MCHARMAP_PRINT_OPERATION_H
#define MCHARMAP_PRINT_OPERATION_H

#include <gtk/gtk.h>

#include <mcharmap/mcharmap.h>

G_BEGIN_DECLS

#define MCHARMAP_TYPE_PRINT_OPERATION          (mcharmap_print_operation_get_type ())
#define MCHARMAP_PRINT_OPERATION(o)            (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_PRINT_OPERATION, McharmapPrintOperation))
#define MCHARMAP_PRINT_OPERATION_CLASS(k)      (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_PRINT_OPERATION, McharmapPrintOperationClass))
#define MCHARMAP_IS_PRINT_OPERATION(o)         (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_PRINT_OPERATION))
#define MCHARMAP_IS_PRINT_OPERATION_CLASS(k)   (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_PRINT_OPERATION))
#define MCHARMAP_PRINT_OPERATION_GET_CLASS(o)  (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_PRINT_OPERATION, McharmapPrintOperationClass))

typedef struct _McharmapPrintOperation         McharmapPrintOperation;
typedef struct _McharmapPrintOperationClass    McharmapPrintOperationClass;
typedef struct _McharmapPrintOperationPrivate  McharmapPrintOperationPrivate;

struct _McharmapPrintOperation
{
  GtkPrintOperation parent_instance;

  /*< private >*/
  McharmapPrintOperationPrivate *priv;
};

struct _McharmapPrintOperationClass
{
  GtkPrintOperationClass parent_class;
};

GType               mcharmap_print_operation_get_type (void);

GtkPrintOperation * mcharmap_print_operation_new      (McharmapCodepointList *codepoint_list,
                                                        PangoFontDescription *font_desc);

G_END_DECLS

#endif /* !MCHARMAP_PRINT_OPERATION_H */
