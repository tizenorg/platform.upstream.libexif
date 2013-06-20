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


#include "config.h"
#include "exif-mnote-data-samsung.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <libexif/exif-byte-order.h>
#include <libexif/exif-utils.h>

#define DEBUG

static void exif_mnote_data_samsung_clear (ExifMnoteDataSamsung *mds)
{
	ExifMnoteData *md = (ExifMnoteData *) mds;
	unsigned int i;

	if (!mds) return;

	if (mds->entries) {
		for (i = 0; i < mds->count; i++)
			if (mds->entries[i].data) {
				exif_mem_free (md->mem, mds->entries[i].data);
				mds->entries[i].data = NULL;
			}
		exif_mem_free (md->mem, mds->entries);
		mds->entries = NULL;
		mds->count = 0;
	}
}

static void exif_mnote_data_samsung_free (ExifMnoteData *md)
{
	if (!md) return;

	exif_mnote_data_samsung_clear ((ExifMnoteDataSamsung *) md);
}

static char *exif_mnote_data_samsung_get_value (ExifMnoteData *md, unsigned int i, char *val, unsigned int maxlen)
{
	ExifMnoteDataSamsung *mds = (ExifMnoteDataSamsung *) md;

	if (!md || !val) return NULL;
	if (i > mds->count -1) return NULL;
	exif_log (md->log, EXIF_LOG_CODE_DEBUG, "ExifMnoteDataSamsung",
		  "Querying value for tag '%s'...",
		  mnote_samsung_tag_get_name (mds->entries[i].tag));
	return mnote_samsung_entry_get_value (&mds->entries[i], val, maxlen);
}


/**
 * @brief save the MnoteData from md to buf
 *
 * @param md extract the data from this structure
 * @param *buf write the mnoteData to this buffer (buffer will be allocated)
 * @param buf_size the size of the buffer
 */
static void exif_mnote_data_samsung_save (ExifMnoteData *md,
		unsigned char **buf, unsigned int *buf_size)
{
	ExifMnoteDataSamsung *mds = (ExifMnoteDataSamsung *) md;
	size_t i, off, val_size, doff;
	unsigned char *new_buf;
	size_t new_buf_size;

	if (!mds || !buf || !buf_size) return;

	/*
	 * Allocate enough memory for all entries and the number of entries.
	 */
	*buf_size = 2 + mds->count * 12 + 4;
	*buf = exif_mem_alloc (md->mem, *buf_size);
	if (!*buf) {
		EXIF_LOG_NO_MEMORY(md->log, "ExifMnoteDataSamsung", *buf_size);
		return;
	}

	/* Save the number of entries */
	exif_set_short(*buf, mds->order, (ExifShort) mds->count);

	/* Save entries */
	for (i = 0; i < mds->count; i++) {
		off = 2 + i * 12;

		exif_set_short (*buf + off + 0, mds->order,	(ExifShort) mds->entries[i].tag);
		exif_set_short (*buf + off + 2, mds->order, (ExifShort) mds->entries[i].format);
		exif_set_long  (*buf + off + 4, mds->order,	mds->entries[i].components);
		off += 8;
		val_size = exif_format_get_size (mds->entries[i].format) * mds->entries[i].components;

		if (val_size > 65536) {
			/* Corrupt data: EXIF data size is limited to the
			 * maximum size of a JPEG segment (64 kb).
			 */
			continue;
		}
		if (val_size > 4) {
			new_buf_size = *buf_size + val_size;
			if(val_size & 1)
				new_buf_size += 1;

			new_buf = exif_mem_realloc (md->mem, *buf, sizeof (char) * new_buf_size);
			if (!new_buf) {
				EXIF_LOG_NO_MEMORY(md->log, "ExifMnoteDataSamsung", new_buf_size);
				return;
			}
			doff = *buf_size;
			*buf = new_buf;
			*buf_size = new_buf_size;

			if(val_size & 1)
				*(*buf + *buf_size - 1) = '\0';

			//exif_set_long (*buf + off, mds->order, mds->offset + doff);
			exif_set_long (*buf + off, mds->order, doff);
		} else
			doff = off;

		/* Write the data. */
		if (mds->entries[i].data) {
			memcpy (*buf + doff, mds->entries[i].data, val_size);
			//fprintf(stdout, "in exif_mnote_data_samsung_save (write the data to buffer)\n");
			//fprintf(stdout, "		mds->entries[%d].data size : %d \n",i, val_size );
			//fprintf(stdout, "		mds->entries[%d].data : %s \n",i, mds->entries[i].data );
			//fprintf(stdout, "		buff + %d : %s \n",doff, buf+doff );
		} else {
			/* Most certainly damaged input file */
			memset (*buf + doff, 0, val_size);
		}
	}
}

static void exif_mnote_data_samsung_load (ExifMnoteData *md,
			      const unsigned char *buf, unsigned int buf_size)
{
	ExifMnoteDataSamsung *mds = (ExifMnoteDataSamsung *) md;
	ExifShort entry_count;
	size_t i, tcount, off, datao;
	size_t val_size;

	if (!mds || !buf || !buf_size) {
		exif_log (md->log, EXIF_LOG_CODE_CORRUPT_DATA,
			  "ExifMnoteDataSamsung", "Short MakerNote");
		return;
	}
	datao = mds->offset; /* Start of interesting data */
	datao += 6;

	if ((datao + 2 < datao) || (datao + 2 < 2) || (datao + 2 > buf_size)) {
		exif_log (md->log, EXIF_LOG_CODE_CORRUPT_DATA,
			  "ExifMnoteDataSamsung", "Short MakerNote");
		return;
	}

	/* Read the number of Makernote Entries */
	entry_count = exif_get_short (buf + datao, mds->order);

	datao += 2;

	/* Remove any old entries */
	exif_mnote_data_samsung_clear (mds);

	/* Reserve enough space for all the possible MakerNote tags */
	mds->entries = exif_mem_alloc (md->mem, sizeof (MnoteSamsungEntry) * entry_count);
	if (!mds->entries) {
		EXIF_LOG_NO_MEMORY(md->log, "ExifMnoteSamsung", sizeof (MnoteSamsungEntry) * entry_count);
		return;
	}

	/* Parse all entries */
	tcount = 0;

	for (i = entry_count, off = datao; i; --i, off += 12) {

		if ((off + 12 < off) || (off + 12 < 12) || (off + 12 > buf_size)) {
			exif_log (md->log, EXIF_LOG_CODE_CORRUPT_DATA,
				  "ExifMnoteSamsung", "Short MakerNote");
			break;
		}

		mds->entries[tcount].tag        = exif_get_short (buf + off, mds->order);
	   	mds->entries[tcount].format     = exif_get_short (buf + off + 2, mds->order);
	   	mds->entries[tcount].components = exif_get_long (buf + off + 4, mds->order);
	   	mds->entries[tcount].order      = mds->order;

	 	exif_log (md->log, EXIF_LOG_CODE_DEBUG, "ExifMnoteSamsung","Loading entry 0x%x ('%s')...", mds->entries[tcount].tag, mnote_samsung_tag_get_name (mds->entries[tcount].tag));

		val_size = exif_format_get_size (mds->entries[tcount].format) * mds->entries[tcount].components;
		mds->entries[tcount].size = val_size;


		if (val_size) {
			size_t dataofs = off + 8;
			if (val_size > 4)
				dataofs = exif_get_long (buf + dataofs, mds->order) + datao - 2; // 2bytes for No. of IFD Entry field size

			if ((dataofs + val_size < dataofs) || (dataofs + val_size < val_size) || (dataofs + val_size > buf_size)) {
				exif_log (md->log, EXIF_LOG_CODE_DEBUG, "ExifMnoteSamsung", "Tag data past end of buffer (%u > %u)", dataofs + val_size, buf_size);
				continue;
			}

			mds->entries[tcount].data = exif_mem_alloc (md->mem, val_size);
			if (!mds->entries[tcount].data) {
				EXIF_LOG_NO_MEMORY(md->log, "ExifMnoteSamsung", val_size);
				continue;
			}
			memcpy (mds->entries[tcount].data, buf + dataofs, val_size);

		}else{
			exif_log (md->log, EXIF_LOG_CODE_CORRUPT_DATA, "ExifMnoteSamsung", "Invalid zero-length tag size");
			continue;
		}

		/* Tag was successfully parsed */
		++tcount;
	}
	/* Store the count of successfully parsed tags */
	mds->count = tcount;
}

static unsigned int exif_mnote_data_samsung_count (ExifMnoteData *md)
{
	return md ? ((ExifMnoteDataSamsung *) md)->count : 0;
}

static unsigned int exif_mnote_data_samsung_get_id (ExifMnoteData *md, unsigned int n)
{
	ExifMnoteDataSamsung *mds = (ExifMnoteDataSamsung *) md;

	if (!mds) return 0;
	if (mds->count <= n) return 0;
	return mds->entries[n].tag;
}

static const char *exif_mnote_data_samsung_get_name (ExifMnoteData *md, unsigned int i)
{
	ExifMnoteDataSamsung *mds = (ExifMnoteDataSamsung *) md;

	if (!mds) return NULL;
	if (i >= mds->count) return NULL;
	return mnote_samsung_tag_get_name (mds->entries[i].tag);
}

static const char *
exif_mnote_data_samsung_get_title (ExifMnoteData *note, unsigned int i)
{
	ExifMnoteDataSamsung *dc = (ExifMnoteDataSamsung *) note;
	unsigned int m, s;

	if (!dc) return NULL;
	exif_mnote_data_canon_get_tags (dc, i, &m, &s);
	if (m >= dc->count) return NULL;
	return mnote_samsung_tag_get_title_sub (dc->entries[m].tag, s, dc->options);
}

static const char *
exif_mnote_data_samsung_get_description (ExifMnoteData *note, unsigned int i)
{
	ExifMnoteDataSamsung *dc = (ExifMnoteDataSamsung *) note;
	unsigned int m;

	if (!dc) return NULL;
	exif_mnote_data_canon_get_tags (dc, i, &m, NULL);
	if (m >= dc->count) return NULL;
	return mnote_samsung_tag_get_description (dc->entries[m].tag);
}

int
exif_mnote_data_samsung_identify (const ExifData *ed, const ExifEntry *e)
{
	char value[8];
	ExifEntry *em = exif_data_get_entry (ed, EXIF_TAG_MAKE);
	if (!em) 
		return 0;
	return !strcmp (exif_entry_get_value (em, value, sizeof (value)), "samsung");
}

static void exif_mnote_data_samsung_set_byte_order (ExifMnoteData *md, ExifByteOrder o)
{
	ExifByteOrder o_orig;
	ExifMnoteDataSamsung *mds = (ExifMnoteDataSamsung *) md;
	unsigned int i;

	if (!mds) return;

	o_orig = mds->order;
	mds->order = o;
	for (i = 0; i < mds->count; i++) {
		mds->entries[i].order = o;
		exif_array_set_byte_order (mds->entries[i].format, mds->entries[i].data, mds->entries[i].components, o_orig, o);
	}
}

static void exif_mnote_data_samsung_set_offset (ExifMnoteData *md, unsigned int o)
{
	if (md) ((ExifMnoteDataSamsung *) md)->offset = o;
}

ExifMnoteData *exif_mnote_data_samsung_new (ExifMem *mem)
{
	ExifMnoteData *md;

	if (!mem) return NULL;

	md = exif_mem_alloc (mem, sizeof (ExifMnoteDataSamsung));
	if (!md) return NULL;

	exif_mnote_data_construct (md, mem);

	/* Set up function pointers */
	md->methods.free            = exif_mnote_data_samsung_free;
	md->methods.set_byte_order  = exif_mnote_data_samsung_set_byte_order;
	md->methods.set_offset      = exif_mnote_data_samsung_set_offset;
	md->methods.load            = exif_mnote_data_samsung_load;
	md->methods.save            = exif_mnote_data_samsung_save;
	md->methods.count           = exif_mnote_data_samsung_count;
	md->methods.get_id          = exif_mnote_data_samsung_get_id;
	md->methods.get_name        = exif_mnote_data_samsung_get_name;
	md->methods.get_value       = exif_mnote_data_samsung_get_value;

	return md;
}
