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

#ifndef __EXIF_MNOTE_DATA_SAMSUNG_H__
#define __EXIF_MNOTE_DATA_SAMSUNG_H__

#include <libexif/exif-byte-order.h>
#include <libexif/exif-mnote-data.h>
#include <libexif/exif-mnote-data-priv.h>
#include <libexif/samsung/mnote-samsung-entry.h>
#include <libexif/exif-mem.h>
#include <libexif/exif-data.h>

typedef struct _ExifMnoteDataSamsung ExifMnoteDataSamsung;

struct _ExifMnoteDataSamsung {
	ExifMnoteData parent;

	MnoteSamsungEntry *entries;
	unsigned int count;

	ExifByteOrder order;
	unsigned int offset;

    ExifDataOption options;
};

int exif_mnote_data_samsung_identify (const ExifData *ed, const ExifEntry *e);
ExifMnoteData *exif_mnote_data_samsung_new (ExifMem *mem);

#endif /* __EXIF_MNOTE_DATA_SAMSUNG_H__ */
