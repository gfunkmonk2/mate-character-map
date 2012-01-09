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

#ifndef MCHARMAP_CHARMAP_H
#define MCHARMAP_CHARMAP_H

#include <gtk/gtk.h>

#include <mcharmap/mcharmap-chapters-model.h>
#include <mcharmap/mcharmap-chapters-view.h>
#include <mcharmap/mcharmap-chartable.h>

G_BEGIN_DECLS

#define MCHARMAP_TYPE_CHARMAP             (mcharmap_charmap_get_type ())
#define MCHARMAP_CHARMAP(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), MCHARMAP_TYPE_CHARMAP, McharmapCharmap))
#define MCHARMAP_CHARMAP_CLASS(k)         (G_TYPE_CHECK_CLASS_CAST((k), MCHARMAP_TYPE_CHARMAP, McharmapCharmapClass))
#define MCHARMAP_IS_CHARMAP(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), MCHARMAP_TYPE_CHARMAP))
#define MCHARMAP_IS_CHARMAP_CLASS(k)      (G_TYPE_CHECK_CLASS_TYPE ((k), MCHARMAP_TYPE_CHARMAP))
#define MCHARMAP_CHARMAP_GET_CLASS(o)     (G_TYPE_INSTANCE_GET_CLASS ((o), MCHARMAP_TYPE_CHARMAP, McharmapCharmapClass))

typedef struct _McharmapCharmap        McharmapCharmap;
typedef struct _McharmapCharmapPrivate McharmapCharmapPrivate;
typedef struct _McharmapCharmapClass   McharmapCharmapClass;

struct _McharmapCharmap
{
  GtkPaned parent;

  /*< private >*/
  McharmapCharmapPrivate *priv;
};

struct _McharmapCharmapClass
{
  GtkPanedClass parent_class;

  void (* status_message) (McharmapCharmap *charmap, const gchar *message);
  void (* link_clicked) (McharmapCharmap *charmap, 
                         gunichar old_character,
                         gunichar new_character);
  void (* _mcharmap_reserved0) (void);
  void (* _mcharmap_reserved1) (void);
  void (* _mcharmap_reserved2) (void);
  void (* _mcharmap_reserved3) (void);
};

GType                 mcharmap_charmap_get_type           (void);

GtkWidget *           mcharmap_charmap_new                (void);

#ifndef MCHARMAP_DISABLE_DEPRECATED
void           mcharmap_charmap_set_orientation (McharmapCharmap *charmap,
                                                  GtkOrientation orientation);
GtkOrientation mcharmap_charmap_get_orientation (McharmapCharmap *charmap);
#endif

void      mcharmap_charmap_set_active_character (McharmapCharmap *charmap,
                                                  gunichar           uc);
gunichar  mcharmap_charmap_get_active_character (McharmapCharmap *charmap);

void      mcharmap_charmap_set_active_chapter (McharmapCharmap *charmap,
                                                const gchar *chapter);
char *    mcharmap_charmap_get_active_chapter (McharmapCharmap *charmap);

void mcharmap_charmap_next_chapter     (McharmapCharmap *charmap);
void mcharmap_charmap_previous_chapter (McharmapCharmap *charmap);

void                     mcharmap_charmap_set_font_desc      (McharmapCharmap  *charmap,
                                                               PangoFontDescription *font_desc);

PangoFontDescription *   mcharmap_charmap_get_font_desc      (McharmapCharmap  *charmap);

McharmapChaptersView *  mcharmap_charmap_get_chapters_view  (McharmapCharmap       *charmap);

void                     mcharmap_charmap_set_chapters_model (McharmapCharmap       *charmap,
                                                               McharmapChaptersModel *model);

McharmapChaptersModel * mcharmap_charmap_get_chapters_model (McharmapCharmap       *charmap);

McharmapCodepointList * mcharmap_charmap_get_active_codepoint_list (McharmapCharmap *charmap);

McharmapCodepointList * mcharmap_charmap_get_book_codepoint_list (McharmapCharmap *charmap);

void     mcharmap_charmap_set_chapters_visible (McharmapCharmap *charmap,
                                                 gboolean visible);

gboolean mcharmap_charmap_get_chapters_visible (McharmapCharmap *charmap);

typedef enum {
  MCHARMAP_CHARMAP_PAGE_CHARTABLE,
  MCHARMAP_CHARMAP_PAGE_DETAILS
} McharmapCharmapPageType;

void     mcharmap_charmap_set_page_visible (McharmapCharmap *charmap,
                                             int page,
                                             gboolean visible);

gboolean mcharmap_charmap_get_page_visible (McharmapCharmap *charmap,
                                             int page);

void mcharmap_charmap_set_active_page (McharmapCharmap *charmap,
                                        int page);

int  mcharmap_charmap_get_active_page (McharmapCharmap *charmap);

void mcharmap_charmap_set_snap_pow2 (McharmapCharmap *charmap,
                                      gboolean snap);
gboolean mcharmap_charmap_get_snap_pow2 (McharmapCharmap *charmap);

/* private; FIXMEchpe remove */
McharmapChartable *     mcharmap_charmap_get_chartable      (McharmapCharmap  *charmap);

G_END_DECLS

#endif  /* #ifndef MCHARMAP_CHARMAP_H */
