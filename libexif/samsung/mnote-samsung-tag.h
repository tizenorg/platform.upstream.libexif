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

#ifndef __MNOTE_SAMSUNG_TAG_H__
#define __MNOTE_SAMSUNG_TAG_H__

#include <libexif/exif-data.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum _MnoteSamsungTag {
	MNOTE_SAMSUNG_TAG_MNOTE_VERSION	= 0x01,
	MNOTE_SAMSUNG_TAG_DEVICE_ID	= 0x02,
	MNOTE_SAMSUNG_TAG_MODEL_ID	= 0x03,
	MNOTE_SAMSUNG_TAG_COLOR_INFO	= 0x20,
	MNOTE_SAMSUNG_TAG_SERIAL_NUM	= 0x23,
	MNOTE_SAMSUNG_TAG_IMAGE_COUNT	= 0x25,
	MNOTE_SAMSUNG_TAG_GPS_INFO01	= 0x30,
	MNOTE_SAMSUNG_TAG_GPS_INFO02	= 0x31,
	MNOTE_SAMSUNG_TAG_PREVIEW_IMAGE	= 0x35,
	MNOTE_SAMSUNG_TAG_FAVOR_TAGGING	= 0x40,
	MNOTE_SAMSUNG_TAG_SRW_COMPRESS	= 0x45,
	MNOTE_SAMSUNG_TAG_COLOR_SPACE	= 0x50,
	MNOTE_SAMSUNG_TAG_AE		= 0x60,
	MNOTE_SAMSUNG_TAG_AF		= 0x80,
	MNOTE_SAMSUNG_TAG_AWB01		= 0xa0,
	MNOTE_SAMSUNG_TAG_AWB02		= 0xa1,
	MNOTE_SAMSUNG_TAG_IPC		= 0xc0,
	MNOTE_SAMSUNG_TAG_SCENE_RESULT	= 0xe0,
	MNOTE_SAMSUNG_TAG_SADEBUG_INFO01= 0xe1,
	MNOTE_SAMSUNG_TAG_SADEBUG_INFO02= 0xe2,
	MNOTE_SAMSUNG_TAG_FACE_DETECTION= 0x100,
	MNOTE_SAMSUNG_TAG_FACE_FEAT01	= 0x101,
	MNOTE_SAMSUNG_TAG_FACE_FEAT02	= 0x102,
	MNOTE_SAMSUNG_TAG_FACE_RECOG	= 0x120,
	MNOTE_SAMSUNG_TAG_LENS_INFO	= 0x140,
	MNOTE_SAMSUNG_TAG_THIRDPARTY	= 0xa000
};
typedef enum _MnoteSamsungTag MnoteSamsungTag;

enum _MnoteSamsungSubTag {
	MNOTE_SAMSUNG_SUBTAG_MODEL_ID_CLASS	= 0x201,
	MNOTE_SAMSUNG_SUBTAG_MODEL_ID_DEVEL	= 0x202,
	MNOTE_SAMSUNG_SUBTAG_COLOR_ID		= 0x211
};
typedef enum _MnoteSamsungSubTag MnoteSamsungSubTag;



const char *mnote_samsung_tag_get_name        (MnoteSamsungTag);

const char *mnote_samsung_tag_get_title       (MnoteSamsungTag);

const char *mnote_samsung_tag_get_description (MnoteSamsungTag);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MNOTE_SAMSUNG_TAG_H__ */
