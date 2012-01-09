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

#include <config.h>
#include <glib.h>
#include <string.h>

#include "mcharmap.h"
#include "mcharmap-private.h"

#include "unicode-scripts.h"

typedef struct
{
  gunichar start;
  gunichar end;
  gint index;   /* index of @start in the codepoint list */
}
UnicodeRange;

struct _McharmapScriptCodepointListPrivate
{
  GPtrArray *ranges;
};

static void mcharmap_script_codepoint_list_class_init (McharmapScriptCodepointListClass *klass);
static void mcharmap_script_codepoint_list_init       (McharmapScriptCodepointList      *list);

G_DEFINE_TYPE (McharmapScriptCodepointList, mcharmap_script_codepoint_list, MCHARMAP_TYPE_CODEPOINT_LIST)

static gint
find_script (const gchar *script)
{
  gint min, mid, max;

  min = 0;
  max = G_N_ELEMENTS (unicode_script_list_offsets) - 1;

  while (max >= min) 
    {
      mid = (min + max) / 2;

      if (strcmp (script, unicode_script_list_strings + unicode_script_list_offsets[mid]) > 0)
        min = mid + 1;
      else if (strcmp (script, unicode_script_list_strings + unicode_script_list_offsets[mid]) < 0)
        max = mid - 1;
      else
        return mid;
    }

  return -1;
}

/* *ranges should be freed by caller */
/* adds unlisted characters to the "Common" script */
static gboolean
get_chars_for_script (const gchar            *script,
                      UnicodeRange          **ranges,
                      gint                   *size)
{
  gint i, j, index;
  gint script_index, common_script_index;
  gint prev_end;

  script_index = find_script (script);
  common_script_index = find_script ("Common");
  if (script_index == -1)
    return FALSE;

  j = 0;

  if (script_index == common_script_index)
    {
      prev_end = -1;
      for (i = 0;  i < G_N_ELEMENTS (unicode_scripts);  i++)
	{
	  if (unicode_scripts[i].start > prev_end + 1)
	    j++;
	  prev_end = unicode_scripts[i].end;
	}
      if (unicode_scripts[i-1].end < UNICHAR_MAX)
	j++;
    }

  for (i = 0;  i < G_N_ELEMENTS (unicode_scripts);  i++)
    if (unicode_scripts[i].script_index == script_index)
      j++;

  *size = j;
  *ranges = g_new (UnicodeRange, *size);

  j = 0, index = 0, prev_end = -1;

  for (i = 0;  i < G_N_ELEMENTS (unicode_scripts);  i++)
    {
      if (script_index == common_script_index)
	{
	  if (unicode_scripts[i].start > prev_end + 1)
	    {
	      (*ranges)[j].start = prev_end + 1;
	      (*ranges)[j].end = unicode_scripts[i].start - 1;
	      (*ranges)[j].index = index;
      
	      index += (*ranges)[j].end - (*ranges)[j].start + 1;
	      j++;
	    }

	  prev_end = unicode_scripts[i].end;
	}

      if (unicode_scripts[i].script_index == script_index)
	{
	  (*ranges)[j].start = unicode_scripts[i].start;
	  (*ranges)[j].end = unicode_scripts[i].end;
	  (*ranges)[j].index = index;

	  index += (*ranges)[j].end - (*ranges)[j].start + 1;
	  j++;
	}
    }

  if (script_index == common_script_index)
    {
      if (unicode_scripts[i-1].end < UNICHAR_MAX)
	{
	  (*ranges)[j].start = unicode_scripts[i-1].end + 1;
	  (*ranges)[j].end = UNICHAR_MAX;
	  (*ranges)[j].index = index;
	  j++;
	}
    }


  g_assert (j == *size);

  return TRUE;
}

static void
ensure_initialized (McharmapScriptCodepointList *guscl)
{
  McharmapScriptCodepointListPrivate *priv = guscl->priv;
  gboolean success;

  if (priv->ranges != NULL)
    return;

  success = mcharmap_script_codepoint_list_set_script (guscl, "Latin");

  g_assert (success);
}

static gunichar 
get_char (McharmapCodepointList *list, 
          gint                    index)
{
  McharmapScriptCodepointList *guscl = MCHARMAP_SCRIPT_CODEPOINT_LIST (list);
  McharmapScriptCodepointListPrivate *priv = guscl->priv;
  gint min, mid, max;

  ensure_initialized (guscl);

  min = 0;
  max = priv->ranges->len - 1;

  while (max >= min) 
    {
      UnicodeRange *range;

      mid = (min + max) / 2;
      range = (UnicodeRange *) (priv->ranges->pdata[mid]);

      if (index > range->index + range->end - range->start)
        min = mid + 1;
      else if (index < range->index)
        max = mid - 1;
      else
        return range->start + index - range->index;
    }

  return (gunichar)(-1);
}

/* XXX: linear search */
static gint
get_index (McharmapCodepointList *list, 
           gunichar                wc)
{
  McharmapScriptCodepointList *guscl = MCHARMAP_SCRIPT_CODEPOINT_LIST (list);
  McharmapScriptCodepointListPrivate *priv = guscl->priv;
  gint i;

  ensure_initialized (guscl);

  for (i = 0;  i < priv->ranges->len;  i++)
    {
      UnicodeRange *range = (UnicodeRange *) priv->ranges->pdata[i];
      if (wc >= range->start && wc <= range->end)
        return range->index + wc - range->start;
    }

  return -1;
}

static gint
get_last_index (McharmapCodepointList *list)
{
  McharmapScriptCodepointList *guscl = MCHARMAP_SCRIPT_CODEPOINT_LIST (list);
  McharmapScriptCodepointListPrivate *priv = guscl->priv;
  UnicodeRange *last_range;

  ensure_initialized (guscl);

  last_range = (UnicodeRange *) (priv->ranges->pdata[priv->ranges->len-1]);

  return last_range->index + last_range->end - last_range->start;
}

static void
clear_ranges (GPtrArray *ranges)
{
  guint i, n;

  n = ranges->len;
  for (i = 0; i < n; ++i)
    g_free (g_ptr_array_index (ranges, i));

  g_ptr_array_set_size (ranges, 0);
}

static void
mcharmap_script_codepoint_list_finalize (GObject *object)
{
  McharmapScriptCodepointList *guscl = MCHARMAP_SCRIPT_CODEPOINT_LIST (object);
  McharmapScriptCodepointListPrivate *priv = guscl->priv;

  if (priv->ranges)
    {
      clear_ranges (priv->ranges);
      g_ptr_array_free (priv->ranges, TRUE);
    }

  G_OBJECT_CLASS (mcharmap_script_codepoint_list_parent_class)->finalize (object);
}

static void
mcharmap_script_codepoint_list_class_init (McharmapScriptCodepointListClass *clazz)
{
  McharmapCodepointListClass *codepoint_list_class = MCHARMAP_CODEPOINT_LIST_CLASS (clazz);
  GObjectClass *gobject_class = G_OBJECT_CLASS (clazz);

  _mcharmap_intl_ensure_initialized ();

  g_type_class_add_private (codepoint_list_class, sizeof (McharmapScriptCodepointListPrivate));

  codepoint_list_class->get_char = get_char;
  codepoint_list_class->get_index = get_index;
  codepoint_list_class->get_last_index = get_last_index;
  
  gobject_class->finalize = mcharmap_script_codepoint_list_finalize;
}

static void 
mcharmap_script_codepoint_list_init (McharmapScriptCodepointList *guscl)
{
  guscl->priv = G_TYPE_INSTANCE_GET_PRIVATE (guscl, MCHARMAP_TYPE_SCRIPT_CODEPOINT_LIST, McharmapScriptCodepointListPrivate);
}

/**
 * mcharmap_script_codepoint_list_new:
 *
 * Creates a new script codepoint list. The default script is Latin.
 *
 * Return value: the newly-created #McharmapCodepointList. Use
 * g_object_unref() to free the result.
 **/
McharmapCodepointList * 
mcharmap_script_codepoint_list_new (void)
{
  return MCHARMAP_CODEPOINT_LIST (g_object_new (mcharmap_script_codepoint_list_get_type (), NULL));
}

/**
 * mcharmap_script_codepoint_list_set_script:
 * @list: a McharmapScriptCodepointList
 * @script: the script name
 *
 * Sets the script for the codepoint list. 
 *
 * Return value: %TRUE on success, %FALSE if there is no such script, in
 * which case the script is not changed.
 **/
gboolean 
mcharmap_script_codepoint_list_set_script (McharmapScriptCodepointList *list,
                                            const gchar                  *script)
{
  const gchar *scripts[2];

  scripts[0] = script;
  scripts[1] = NULL;

  return mcharmap_script_codepoint_list_set_scripts (list, scripts);
}

/**
 * mcharmap_script_codepoint_list_set_scripts:
 * @list: a McharmapScriptCodepointList
 * @scripts: NULL-terminated array of script names
 *
 * Sets multiple scripts for the codepoint list. Codepoints are sorted
 * according to their order in @scripts.
 *
 * Return value: %TRUE on success, %FALSE if any of the scripts don’t
 * exist, in which case the script is not changed.
 **/
gboolean
mcharmap_script_codepoint_list_set_scripts (McharmapScriptCodepointList  *list,
	                                     const gchar                  **scripts)
{
  McharmapScriptCodepointListPrivate *priv = list->priv;
  UnicodeRange *ranges;
  gint i, j, size;

  if (priv->ranges)
    clear_ranges (priv->ranges);
  else
    priv->ranges = g_ptr_array_new ();

  for (i = 0;  scripts[i];  i++)
    if (get_chars_for_script (scripts[i], &ranges, &size))
      {
        for (j = 0;  j < size;  j++)
          g_ptr_array_add (priv->ranges, g_memdup (ranges + j, sizeof (ranges[j])));
        g_free (ranges);
      }
    else
      {
        g_ptr_array_free (priv->ranges, TRUE);
        return FALSE;
      }

  return TRUE;
}

/**
 * mcharmap_script_codepoint_list_append_script:
 * @list: a McharmapScriptCodepointList
 * @script: the script name
 *
 * Appends the characters in @script to the codepoint list.
 *
 * Return value: %TRUE on success, %FALSE if there is no such script, in
 * which case the codepoint list is not changed.
 **/
gboolean
mcharmap_script_codepoint_list_append_script (McharmapScriptCodepointList  *list,
                                               const gchar                   *script)
{
  McharmapScriptCodepointListPrivate *priv = list->priv;
  UnicodeRange *ranges;
  gint j, size, index0;

  if (priv->ranges == NULL)
    priv->ranges = g_ptr_array_new ();

  if (priv->ranges->len > 0)
    {
      UnicodeRange *last_range = (UnicodeRange *) priv->ranges->pdata[priv->ranges->len - 1];
      index0 = last_range->index  + last_range->end - last_range->start + 1;
    }
  else
    index0 = 0;

  if (get_chars_for_script (script, &ranges, &size))
    {
      for (j = 0;  j < size;  j++)
        {
          UnicodeRange *range = g_memdup (ranges + j, sizeof (ranges[j]));
          range->index += index0;
          g_ptr_array_add (priv->ranges, range);
        }
      g_free (ranges);

      return TRUE;
    }

  return FALSE;
}

/**
 * mcharmap_unicode_list_scripts:
 *
 * Return value: %NULL-terminated array of script names. These have been
 * marked for translation with N_().
 * The strings in the array are owned by mcharmap and should not be
 * modified or free; the array itself however is allocated and should
 * be freed with g_free().
 *
 * Returns: a newly allocated %NULL-terminated array of strings
 **/
const gchar **
mcharmap_unicode_list_scripts (void)
{
  const char **scripts;
  guint i;

  scripts = (const char **) g_new (char*, G_N_ELEMENTS (unicode_script_list_offsets) + 1);
  for (i = 0; i < G_N_ELEMENTS (unicode_script_list_offsets); ++i)
    {
      scripts[i] = unicode_script_list_strings + unicode_script_list_offsets[i];
    }
  scripts[i] = NULL;

  return scripts;
}

/**
 * mcharmap_unicode_get_script_for_char:
 * @wc: a character
 *
 * Return value: The English (untranslated) name of the script to which the
 * character belongs. Characters that don't belong to an actual script
 * return %"Common".
 **/
const gchar* mcharmap_unicode_get_script_for_char(gunichar wc)
{
	gint min = 0;
	gint mid;
	gint max = sizeof(unicode_scripts) / sizeof(UnicodeScript) - 1;

	if (wc > UNICHAR_MAX)
	{
		return NULL;
    }

	while (max >= min)
	{
		mid = (min + max) / 2;

		if (wc > unicode_scripts[mid].end)
		{
			min = mid + 1;
		}
		else if (wc < unicode_scripts[mid].start)
		{
			max = mid - 1;
		}
		else
		{
			return unicode_script_list_strings + unicode_script_list_offsets[unicode_scripts[mid].script_index];
		}
	}

	/* Unicode assigns "Common" as the script name for any character not
	 * specifically listed in Scripts.txt */
	return N_("Common");
}
