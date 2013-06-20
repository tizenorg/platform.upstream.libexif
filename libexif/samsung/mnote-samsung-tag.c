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
#include "mnote-samsung-tag.h"

#include <stdlib.h>

#include <libexif/i18n.h>

static const struct {
	MnoteSamsungTag tag;
	const char *name;
	const char *title;
	const char *description;
} table[] = {
#ifndef NO_VERBOSE_TAG_STRINGS
	{MNOTE_SAMSUNG_TAG_MNOTE_VERSION, "MakerNoteVersion", N_("Maker Note Version"), "1.0.0"},
	{MNOTE_SAMSUNG_TAG_DEVICE_ID, "SamsungDeviceID", N_("Samsung Device ID"), ""},
	{MNOTE_SAMSUNG_TAG_MODEL_ID, "SamsungModelID", N_("Samsung Model ID"), ""},
	{MNOTE_SAMSUNG_TAG_COLOR_INFO, "ColorInfo", N_("Color Info"), ""},
	{MNOTE_SAMSUNG_TAG_SERIAL_NUM, "ModelSerialNumber", N_("Model Serial Number"), ""},
	{MNOTE_SAMSUNG_TAG_IMAGE_COUNT, "ImageCount", N_("Image Count"), ""},
	{MNOTE_SAMSUNG_TAG_GPS_INFO01, "GPSInfo01", N_("GPS Infomation 01"), ""},
	{MNOTE_SAMSUNG_TAG_GPS_INFO02, "GPSInfo02", N_("GPS Infomation 02"), ""},
	{MNOTE_SAMSUNG_TAG_PREVIEW_IMAGE, "PreviewImageInfo", N_("Preview Image IFD Offset"), ""},
	{MNOTE_SAMSUNG_TAG_FAVOR_TAGGING, "FavoriteTagging", N_("Favorite Tagging"), ""},
	{MNOTE_SAMSUNG_TAG_SRW_COMPRESS, "SRWCompressionMode", N_("SRW Compression Mode"), ""},
	{MNOTE_SAMSUNG_TAG_COLOR_SPACE, "ColorSpace", N_("Color Space"), ""},
	{MNOTE_SAMSUNG_TAG_AE, "AE", N_("Auto Exposure"), ""},
	{MNOTE_SAMSUNG_TAG_AF, "AF", N_("Auto Focus"), ""},
	{MNOTE_SAMSUNG_TAG_AWB01, "AWB01", N_("Auto White Balance 01(Capture)"), ""},
	{MNOTE_SAMSUNG_TAG_AWB02, "AWB02", N_("Auto White Balance 02(Preview)"), ""},
	{MNOTE_SAMSUNG_TAG_IPC, "IPC", N_("Image Processing Chain"), ""},
	{MNOTE_SAMSUNG_TAG_SCENE_RESULT, "SceneResult", N_("Scene Recognition"), ""},
	{MNOTE_SAMSUNG_TAG_SADEBUG_INFO01, "SADebugInfo01", N_("Scene Recognition Debug Info 01"), ""},
	{MNOTE_SAMSUNG_TAG_SADEBUG_INFO02, "SADebugInfo02", N_("Scene Recognition Debug Info 02"), ""},
	{MNOTE_SAMSUNG_TAG_FACE_DETECTION, "FaceDetection", N_("Face Detection"), ""},
	{MNOTE_SAMSUNG_TAG_FACE_FEAT01, "FaceFeature01", N_("Face Feature 01"), ""},
	{MNOTE_SAMSUNG_TAG_FACE_FEAT02, "FaceFeature02", N_("Face Feature 02"), ""},
	{MNOTE_SAMSUNG_TAG_FACE_RECOG, "FaceRecognition", N_("Face Recognition"), ""},
	{MNOTE_SAMSUNG_TAG_LENS_INFO, "LensInfo", N_("Lens Infomation"), ""},
	{MNOTE_SAMSUNG_TAG_THIRDPARTY, "ThirdParty", N_("Ichikawa S/W"), ""},

#endif
	{0, NULL, NULL, NULL}
};


const char *mnote_samsung_tag_get_name (MnoteSamsungTag t)
{
	unsigned int i;

	for (i = 0; i < sizeof (table) / sizeof (table[0]); i++)
		if (table[i].tag == t) return (table[i].name);
	return NULL;
}

const char *mnote_samsung_tag_get_title (MnoteSamsungTag t)
{
	unsigned int i;

	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
	for (i = 0; i < sizeof (table) / sizeof (table[0]); i++)
		if (table[i].tag == t) return (_(table[i].title));
	return NULL;
}

const char *mnote_samsung_tag_get_description (MnoteSamsungTag t)
{
	unsigned int i;

	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
	for (i = 0; i < sizeof (table) / sizeof (table[0]); i++)
		if (table[i].tag == t) {
			if (!*table[i].description)
				return "";
			return (_(table[i].description));
		}
	return NULL;
}

