/*
 * libexif
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Sangchul Lee <sc11.lee@samsung.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#ifndef __MNOTE_SAMSUNG_ENTRY_H__
#define __MNOTE_SAMSUNG_ENTRY_H__

#include <libexif/exif-format.h>
#include <libexif/exif-byte-order.h>
#include <libexif/samsung/mnote-samsung-tag.h>

typedef struct _MnoteSamsungEntry        MnoteSamsungEntry;

struct _MnoteSamsungEntry {
	MnoteSamsungTag tag;
	ExifFormat format;
	unsigned long components;

	unsigned char *data;
	unsigned int size;

	ExifByteOrder order;
};

void	mnote_samsung_entry_set_value_by_index (MnoteSamsungEntry *entry, int index);

void	mnote_samsung_entry_set_value_by_string (MnoteSamsungEntry *entry, unsigned char* string, unsigned int len);

void	mnote_samsung_entry_set_value_by_subtag (MnoteSamsungEntry *entry, MnoteSamsungSubTag stag1, int sindex1, MnoteSamsungSubTag stag2, int sindex2, ExifShort sval);

char	*mnote_samsung_entry_get_value (const MnoteSamsungEntry *, char *v, unsigned int maxlen);

#endif /* __MNOTE_SAMSUNG_ENTRY_H__ */
