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

#include <config.h>
#include "mnote-samsung-entry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libexif/exif-format.h>
#include <libexif/exif-utils.h>
#include <libexif/exif-entry.h>
#include <libexif/i18n.h>

#define CF(format,target,v,maxlen)                              \
{                                                               \
        if (format != target) {                                 \
                snprintf (v, maxlen,	                        \
                        _("Invalid format '%s', "               \
                        "expected '%s'."),                      \
                        exif_format_get_name (format),          \
                        exif_format_get_name (target));         \
                break;                                          \
        }                                                       \
}

#define CF2(format,target1,target2,v,maxlen)                    \
{                                                               \
        if ((format != target1) && (format != target2)) {       \
                snprintf (v, maxlen,	                        \
                        _("Invalid format '%s', "               \
                        "expected '%s' or '%s'."),              \
                        exif_format_get_name (format),          \
                        exif_format_get_name (target1),         \
                        exif_format_get_name (target2));        \
                break;                                          \
        }                                                       \
}

#define CC(number,target,v,maxlen)                                      \
{                                                                       \
        if (number != target) {                                         \
                snprintf (v, maxlen,                                    \
                        _("Invalid number of components (%i, "          \
                        "expected %i)."), (int) number, (int) target);  \
                break;                                                  \
        }                                                               \
}

#define CC2(number,t1,t2,v,maxlen)                                      \
{                                                                       \
	if ((number < t1) || (number > t2)) {                           \
		snprintf (v, maxlen,                                    \
			_("Invalid number of components (%i, "          \
			"expected %i or %i)."), (int) number,		\
			(int) t1, (int) t2);  				\
		break;                                                  \
	}                                                               \
}

static const struct {
	ExifTag tag;
	ExifFormat fmt;
	struct {
		int index;
		const char *string;
	} elem[20];
} items[] = {
#ifndef NO_VERBOSE_TAG_DATA
  { MNOTE_SAMSUNG_TAG_MNOTE_VERSION, EXIF_FORMAT_UNDEFINED,
    { {0x30313030, N_("MakerNote Version")},
      {0, NULL}}},
  { MNOTE_SAMSUNG_TAG_DEVICE_ID, EXIF_FORMAT_LONG,
    { {0x00001000, N_("Compact")},
      {0x00002000, N_("DSLR")},
      {0x00003000, N_("Camcorder")},
      {0, NULL}}},
  { MNOTE_SAMSUNG_TAG_SERIAL_NUM, EXIF_FORMAT_ASCII,
    { {0, NULL}}},
  { MNOTE_SAMSUNG_TAG_FAVOR_TAGGING, EXIF_FORMAT_LONG,
    { {0x00000000, N_("Bad")},
      {0x00000001, N_("Good")},
      {0, NULL}}},
  { MNOTE_SAMSUNG_TAG_SRW_COMPRESS, EXIF_FORMAT_LONG,
    { {0x00000000, N_("Uncompressed Mode")},
      {0x00000001, N_("Lossless Compression Mode (Type 1)")},
      {0x00000002, N_("Lossless Compression Mode (Type 2)")},
      {0, NULL}}},
  { MNOTE_SAMSUNG_TAG_COLOR_SPACE, EXIF_FORMAT_LONG,
    { {0x00000001, N_("sRGB (Standard RGB)")},
      {0x0000FFFF, N_("Adobe RGB (Adobe RGB)")},
      {0, NULL}}},
  { MNOTE_SAMSUNG_TAG_SCENE_RESULT, EXIF_FORMAT_LONG,
    { {0x00000000, N_("Smart")},
      {0x00000001, N_("Portrait")},
      {0x00000002, N_("Night")},
      {0x00000003, N_("Night Portrait")},
      {0x00000004, N_("Backlight")},
      {0x00000005, N_("Backlight Portrait")},
      {0x00000006, N_("Macro")},
      {0x00000007, N_("White")},
      {0x00000008, N_("Landscape")},
      {0x00000009, N_("Macro Text")},
      {0x0000000A, N_("BLSunset")},
      {0x0000000B, N_("Bluesky")},
      {0x0000000C, N_("Macro Portrait")},
      {0x0000000D, N_("Natural Green")},
      {0x0000000E, N_("Color")},
      {0x0000000F, N_("Color Hidden")},
      {0x00000010, N_("Tripod")},
      {0x00000011, N_("Action")},
      {0, NULL}}},
  { MNOTE_SAMSUNG_TAG_FACE_DETECTION, EXIF_FORMAT_LONG,
    { {0x00000000, N_("FALSE")},
      {0x00000001, N_("TRUE")},
      {0, NULL}}},
  { MNOTE_SAMSUNG_TAG_FACE_RECOG, EXIF_FORMAT_LONG,
    { {0x00000000, N_("FALSE")},
      {0x00000001, N_("TRUE")},
      {0, NULL}}},

#endif
  { 0, 0, { { 0, NULL } } }
};

static const struct {
	ExifTag tag;
	ExifFormat fmt;
	struct {
		short index;
		const char *string;
	} elem[20];
} subitems[] = {
#ifndef NO_VERBOSE_TAG_DATA
  { MNOTE_SAMSUNG_SUBTAG_MODEL_ID_CLASS, EXIF_FORMAT_UNDEFINED,
    { {0x1, N_("Essential")},
      {0x2, N_("PlusOnel")},
      {0x3, N_("Intelligent")},
      {0x4, N_("Style")},
      {0x5, N_("Wannabe")},
      {0x6, N_("Expertise")},
      {0x7, N_("NX")},
      {0x8, N_("GX")},
      {0x9, N_("HD")},
      {0xA, N_("SD")},
      {0, NULL}}},
  { MNOTE_SAMSUNG_SUBTAG_MODEL_ID_DEVEL, EXIF_FORMAT_UNDEFINED,
    { {0x0, N_("OWN")},
      {0x7, N_("TSOE")},
      {0xA, N_("ODM")},
      {0, NULL}}},
  { MNOTE_SAMSUNG_SUBTAG_COLOR_ID, EXIF_FORMAT_SHORT,
    { {0x0000, N_("Red")},
      {0x0100, N_("Yellow")},
      {0x0200, N_("Green")},
      {0x0300, N_("Blue")},
      {0x0400, N_("Magenta")},
      {0x0500, N_("Black")},
      {0x0600, N_("Gray")},
      {0x0700, N_("Un-classification")},
      {0, NULL}}},
#endif
  { 0, 0, { { 0, NULL } } }
};


/*for setting data in an entry*/
void mnote_samsung_entry_set_value_by_index (MnoteSamsungEntry *entry, int index)
{
	int i;
	unsigned char* v;

	v = entry->data;
	for (i = 0; (items[i].tag != entry->tag); i++);
	exif_set_long(v, entry->order,(ExifLong)items[i].elem[index].index);
}

void mnote_samsung_entry_set_value_by_subtag (MnoteSamsungEntry *entry, MnoteSamsungSubTag stag1, int sindex1, MnoteSamsungSubTag stag2, int sindex2, ExifShort sval)
{

	unsigned char* v;
	ExifLong result = 0;
	int i;

	switch(entry->tag){
		case MNOTE_SAMSUNG_TAG_MODEL_ID:
			v = entry->data;
			for(i = 0 ; (subitems[i].tag != stag1) ; i++);
			result |= subitems[i].elem[sindex1].index;
			result <<= 4;
			for(i = 0 ; (subitems[i].tag != stag2) ; i++);
			result |= subitems[i].elem[sindex2].index;
			result <<= 20;
			result |= sval;
			exif_set_long(v, entry->order, result);
			break;

		case MNOTE_SAMSUNG_TAG_COLOR_INFO:
			v = entry->data;
			for(i = 0 ; (subitems[i].tag != stag1) ; i++);
			result |= subitems[i].elem[sindex1].index;
			result <<= 16;
			result |= sval;
			exif_set_long(v, entry->order, result);
			break;

		default:
			fprintf (stdout,_("inappropriate using mnote_samsung_entry_set_value_by_subtag - %i bytes"), entry->size);
			break;

	}

}

void mnote_samsung_entry_set_value_by_string (MnoteSamsungEntry *entry, unsigned char* string, unsigned int len)
{
	/* for MNOTE_TAG_SERIAL_NUM */
	int i;
	unsigned char* v;

	v = entry->data;
	for (i = 0; (items[i].tag != entry->tag); i++);
	if(items[i].elem[0].string == NULL)
		strncpy(v, string, len);
}

char *mnote_samsung_entry_get_value (const MnoteSamsungEntry *entry, char *v, unsigned int maxlen)
{
	char buf[30]={0};
	char v1[512]={0};
	size_t v1_maxlen= sizeof(v1);
	ExifLong     vl;
	ExifShort    vs = 0,vs1 = 0;
	ExifRational vr;
	ExifSRational vsr;
	int i, j;
	char c, c1;
	double r;

	if (!entry)
		return (NULL);

	memset (v, 0, maxlen);
	maxlen--;

	if ((!entry->data) && (entry->components > 0))
		return (v);

	switch (entry->tag) {

	case MNOTE_SAMSUNG_TAG_MNOTE_VERSION:
		CF (entry->format,  EXIF_FORMAT_UNDEFINED, v, maxlen);
		CC (entry->components, 4, v, maxlen);
		vl = exif_get_long (entry->data, entry->order);
		snprintf (v, maxlen, _("Samsung MakerNote Version: %li"), (long int)vl);
		break;

	case MNOTE_SAMSUNG_TAG_DEVICE_ID:
		CF (entry->format,  EXIF_FORMAT_LONG, v, maxlen);
		CC (entry->components, 1, v, maxlen);
		vl = exif_get_long (entry->data, entry->order);

		for (i = 0; (items[i].tag != entry->tag); i++);
		if (!items[i].tag) {
		  	snprintf (v, maxlen, _("Internal error (unknown value %li)"),(long int) vl);
		  	break;
		}
		CF (entry->format, items[i].fmt, v, maxlen);
		/* find the value */
		for (j = 0; items[i].elem[j].string && (items[i].elem[j].index < vl); j++);
		if (items[i].elem[j].index != vl) {
			snprintf (v, maxlen, _("Unknown value %li"), (long int)vl);
			break;
		}

		snprintf (v, maxlen, _("Device ID: %s"),  _(items[i].elem[j].string));
		break;

	case MNOTE_SAMSUNG_TAG_MODEL_ID:
		CF (entry->format,  EXIF_FORMAT_LONG, v, maxlen);
		CC (entry->components, 1, v, maxlen);
		vl = exif_get_long (entry->data, entry->order);
		c = vl >> 24;				/* Class info */
		c1 = vl >> 20;				/* Development info */
		c1 = ~(c << 4) & c1;
		vs = (ExifShort)(0x0000FFFF & vl);	/* PID */

		for (i = 0; (subitems[i].tag != MNOTE_SAMSUNG_SUBTAG_MODEL_ID_CLASS); i++);
		CF (EXIF_FORMAT_UNDEFINED, subitems[i].fmt, v, maxlen);
		/* find the value */
		for (j = 0; subitems[i].elem[j].string && (subitems[i].elem[j].index < c); j++);
		if (subitems[i].elem[j].index != c ) {
			snprintf (v, maxlen, _("Unknown Class info value %d in MNOTE_SAMSUNG_TAG_MODEL_ID"), c);
			break;
		}
		snprintf (v, maxlen, _("Model ID: ClassInfo=%s, "), _(subitems[i].elem[j].string));

		for (i = 0; (subitems[i].tag != MNOTE_SAMSUNG_SUBTAG_MODEL_ID_DEVEL); i++);
		CF (EXIF_FORMAT_UNDEFINED, subitems[i].fmt, v, maxlen);
		/* find the value */
		for (j = 0; subitems[i].elem[j].string && (subitems[i].elem[j].index < c1); j++);
		if (subitems[i].elem[j].index != c1 ) {
			snprintf (v, maxlen, _("Unknown Development info. value %d in MNOTE_SAMSUNG_TAG_MODEL_ID"), c1);
			break;
		}
		snprintf (v1, v1_maxlen, _("DevelopmentInfo=%s, PID=%d"), _(subitems[i].elem[j].string), vs);
		strncat (v, v1, strlen(v1));

		break;

	case MNOTE_SAMSUNG_TAG_COLOR_INFO:
		CF (entry->format,  EXIF_FORMAT_SHORT, v, maxlen);
		CC (entry->components, 2, v, maxlen);
		vl = exif_get_long (entry->data, entry->order);
		vs = vl >> 16;				/* Color ID */
		vs1 = (ExifShort)(0x0000FFFF & vl);	/* Color Score */

		for (i = 0; (subitems[i].tag != MNOTE_SAMSUNG_SUBTAG_COLOR_ID); i++);
		CF (EXIF_FORMAT_SHORT, subitems[i].fmt, v, maxlen);
		/* find the value */
		for (j = 0; subitems[i].elem[j].string && (subitems[i].elem[j].index < vs); j++);
		if (subitems[i].elem[j].index != vs ) {
			snprintf (v, maxlen, _("Unknown Color ID value %d in MNOTE_SAMSUNG_TAG_COLOR_INFO"), vs);
			break;
		}
		snprintf (v, maxlen, _("Color Info: ColorID=%s, ColorScore=0x%x"), _(subitems[i].elem[j].string), vs1);
		break;

	case MNOTE_SAMSUNG_TAG_SERIAL_NUM:
		CF (entry->format,  EXIF_FORMAT_ASCII, v, maxlen);
		strncpy (v1, (char *)entry->data, MIN(v1_maxlen, entry->size));
	    	snprintf(v, maxlen, _("Serial Number: %s"), _(v1));
		break;

	case MNOTE_SAMSUNG_TAG_IMAGE_COUNT:
	case MNOTE_SAMSUNG_TAG_GPS_INFO01:
	case MNOTE_SAMSUNG_TAG_GPS_INFO02:
	case MNOTE_SAMSUNG_TAG_PREVIEW_IMAGE:
	case MNOTE_SAMSUNG_TAG_FAVOR_TAGGING:
		CF (entry->format,  EXIF_FORMAT_LONG, v, maxlen);
		CC (entry->components, 1, v, maxlen);
		vl = exif_get_long (entry->data, entry->order);

		for (i = 0; (items[i].tag != entry->tag); i++);

		if (!items[i].tag) {
		  	snprintf (v, maxlen, _("Internal error (unknown value %li)"), (long int)vl);
		  	break;
		}
		CF (entry->format, items[i].fmt, v, maxlen);
		/* find the value */
		for (j = 0; items[i].elem[j].string && (items[i].elem[j].index < vl); j++);
		if (items[i].elem[j].index != vl) {
			snprintf (v, maxlen, _("Unknown value %li"),(long int) vl);
			break;
		}
		snprintf(v, maxlen, _("Favorite Tagging: %s"), _(items[i].elem[j].string));
		break;

	case MNOTE_SAMSUNG_TAG_SRW_COMPRESS:
		CF (entry->format,  EXIF_FORMAT_LONG, v, maxlen);
		CC (entry->components, 1, v, maxlen);
		vl = exif_get_long (entry->data, entry->order);

		for (i = 0; (items[i].tag != entry->tag); i++);

		if (!items[i].tag) {
		  	snprintf (v, maxlen, _("Internal error (unknown value %li)"), (long int)vl);
		  	break;
		}
		CF (entry->format, items[i].fmt, v, maxlen);
		/* find the value */
		for (j = 0; items[i].elem[j].string && (items[i].elem[j].index < vl); j++);
		if (items[i].elem[j].index != vl) {
			snprintf (v, maxlen, _("Unknown value %li"),(long int) vl);
			break;
		}
		snprintf(v, maxlen, _("SRW Compression: %s"), _(items[i].elem[j].string));
		break;

	case MNOTE_SAMSUNG_TAG_COLOR_SPACE:
		CF (entry->format,  EXIF_FORMAT_LONG, v, maxlen);
		CC (entry->components, 1, v, maxlen);
		vl = exif_get_long (entry->data, entry->order);

		for (i = 0; (items[i].tag != entry->tag); i++);

		if (!items[i].tag) {
		  	snprintf (v, maxlen, _("Internal error (unknown value %li)"),(long int) vl);
		  	break;
		}
		CF (entry->format, items[i].fmt, v, maxlen);
		/* find the value */
		for (j = 0; items[i].elem[j].string && (items[i].elem[j].index < vl); j++);
		if (items[i].elem[j].index != vl) {
			snprintf (v, maxlen, _("Unknown value %li"),(long int) vl);
			break;
		}
		snprintf(v, maxlen, _("Color Space: %s"), _(items[i].elem[j].string));
		break;

	case MNOTE_SAMSUNG_TAG_AE:
	case MNOTE_SAMSUNG_TAG_AF:
	case MNOTE_SAMSUNG_TAG_AWB01:
	case MNOTE_SAMSUNG_TAG_AWB02:
	case MNOTE_SAMSUNG_TAG_IPC:
	case MNOTE_SAMSUNG_TAG_SCENE_RESULT:
		CF (entry->format,  EXIF_FORMAT_LONG, v, maxlen);
		CC (entry->components, 1, v, maxlen);
		vl = exif_get_long (entry->data, entry->order);

		for (i = 0; (items[i].tag != entry->tag); i++);

		if (!items[i].tag) {
		  	snprintf (v, maxlen, _("Internal error (unknown value %li)"), (long int) vl);
		  	break;
		}
		CF (entry->format, items[i].fmt, v, maxlen);
		/* find the value */
		for (j = 0; items[i].elem[j].string && (items[i].elem[j].index < vl); j++);
		if (items[i].elem[j].index != vl) {
			snprintf (v, maxlen, _("Unknown value %li"), (long int) vl);
			break;
		}
		snprintf(v, maxlen, _("Scene Result: %s"), _(items[i].elem[j].string));
		break;

	case MNOTE_SAMSUNG_TAG_SADEBUG_INFO01:
	case MNOTE_SAMSUNG_TAG_SADEBUG_INFO02:
	case MNOTE_SAMSUNG_TAG_FACE_DETECTION:
		CF (entry->format,  EXIF_FORMAT_LONG, v, maxlen);
		CC (entry->components, 1, v, maxlen);
		vl = exif_get_long (entry->data, entry->order);

		for (i = 0; (items[i].tag != entry->tag); i++);

		if (!items[i].tag) {
		  	snprintf (v, maxlen, _("Internal error (unknown value %li)"), (long int) vl);
		  	break;
		}
		CF (entry->format, items[i].fmt, v, maxlen);
		/* find the value */
		for (j = 0; items[i].elem[j].string && (items[i].elem[j].index < vl); j++);
		if (items[i].elem[j].index != vl) {
			snprintf (v, maxlen, _("Unknown value %li"), (long int) vl);
			break;
		}
		snprintf(v, maxlen, _("Face Detection: %s"), _(items[i].elem[j].string));
		break;

	case MNOTE_SAMSUNG_TAG_FACE_FEAT01:
	case MNOTE_SAMSUNG_TAG_FACE_FEAT02:
	case MNOTE_SAMSUNG_TAG_FACE_RECOG:
		CF (entry->format,  EXIF_FORMAT_LONG, v, maxlen);
		CC (entry->components, 1, v, maxlen);
		vl = exif_get_long (entry->data, entry->order);

		for (i = 0; (items[i].tag != entry->tag); i++);

		if (!items[i].tag) {
		  	snprintf (v, maxlen, _("Internal error (unknown value %li)"),(long int) vl);
		  	break;
		}
		CF (entry->format, items[i].fmt, v, maxlen);
		/* find the value */
		for (j = 0; items[i].elem[j].string && (items[i].elem[j].index < vl); j++);
		if (items[i].elem[j].index != vl) {
			snprintf (v, maxlen, _("Unknown value %li"), (long int)vl);
			break;
		}
		snprintf(v, maxlen, _("Face Recognition: %s"), _(items[i].elem[j].string));

		break;

	case MNOTE_SAMSUNG_TAG_LENS_INFO:
	case MNOTE_SAMSUNG_TAG_THIRDPARTY:


	default:
		switch (entry->format) {
		case EXIF_FORMAT_ASCII:
			strncpy (v, (char *)entry->data, MIN (maxlen, entry->size));
			break;
		case EXIF_FORMAT_SHORT:
			CC (entry->components, 1, v, maxlen);
			vs = exif_get_short (entry->data, entry->order);
			snprintf (v, maxlen, "%hu", vs);
			break;
		case EXIF_FORMAT_LONG:
			CC (entry->components, 1, v, maxlen);
			vl = exif_get_long (entry->data, entry->order);
			snprintf (v, maxlen, "%li", (long int) vl);
			break;
		case EXIF_FORMAT_RATIONAL:
			CC (entry->components, 1, v, maxlen);
			vr = exif_get_rational (entry->data, entry->order);
			if (!vr.denominator) {
				strncpy (v, _("Infinite"), maxlen);
			} else {
				r = (double)vr.numerator / vr.denominator;
				snprintf (v, maxlen, "%2.3f", r);
			}
			break;
		case EXIF_FORMAT_SRATIONAL:
			CC (entry->components, 1, v, maxlen);
			vsr = exif_get_srational (entry->data, entry->order);
			if (!vsr.denominator) {
				strncpy (v, _("Infinite"), maxlen);
			} else {
				r = (double)vsr.numerator / vsr.denominator;
				snprintf (v, maxlen, "%2.3f", r);
			}
			break;
		case EXIF_FORMAT_UNDEFINED:
		default:
			snprintf (v, maxlen, _("%i bytes unknown data: "),
				  entry->size);
			for (i = 0; i < (int)entry->size; i++) {
				sprintf (buf, "%02x", entry->data[i]);
				strncat (v, buf, maxlen - strlen (v));
			}
			break;
		}
		break;
	}

	return (v);
}
