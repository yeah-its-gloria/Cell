/* 
 * Copyright 2018 The Chromium Authors
 * Copyright 2023 Simon Ser
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef CURSOR_SHAPE_V1_CLIENT_PROTOCOL_H
#define CURSOR_SHAPE_V1_CLIENT_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

struct wl_client;
struct wl_resource;

struct wp_cursor_shape_manager_v1;
struct wp_cursor_shape_device_v1;

extern const struct wl_interface wp_cursor_shape_manager_v1_interface;
extern const struct wl_interface wp_cursor_shape_device_v1_interface;

#define WP_CURSOR_SHAPE_MANAGER_V1_DESTROY	0
#define WP_CURSOR_SHAPE_MANAGER_V1_GET_POINTER	1
#define WP_CURSOR_SHAPE_MANAGER_V1_GET_TABLET_TOOL_V2	2

static inline void
wp_cursor_shape_manager_v1_set_user_data(struct wp_cursor_shape_manager_v1 *wp_cursor_shape_manager_v1, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wp_cursor_shape_manager_v1, user_data);
}

static inline void *
wp_cursor_shape_manager_v1_get_user_data(struct wp_cursor_shape_manager_v1 *wp_cursor_shape_manager_v1)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wp_cursor_shape_manager_v1);
}

static inline void
wp_cursor_shape_manager_v1_destroy(struct wp_cursor_shape_manager_v1 *wp_cursor_shape_manager_v1)
{
	wl_proxy_marshal((struct wl_proxy *) wp_cursor_shape_manager_v1,
			 WP_CURSOR_SHAPE_MANAGER_V1_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wp_cursor_shape_manager_v1);
}

static inline struct wp_cursor_shape_device_v1 *
wp_cursor_shape_manager_v1_get_pointer(struct wp_cursor_shape_manager_v1 *wp_cursor_shape_manager_v1, struct wl_pointer *pointer)
{
	struct wl_proxy *cursor_shape_device;

	cursor_shape_device = wl_proxy_create((struct wl_proxy *) wp_cursor_shape_manager_v1,
			     &wp_cursor_shape_device_v1_interface);
	if (!cursor_shape_device)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wp_cursor_shape_manager_v1,
			 WP_CURSOR_SHAPE_MANAGER_V1_GET_POINTER, cursor_shape_device, pointer);

	return (struct wp_cursor_shape_device_v1 *) cursor_shape_device;
}

static inline struct wp_cursor_shape_device_v1 *
wp_cursor_shape_manager_v1_get_tablet_tool_v2(struct wp_cursor_shape_manager_v1 *wp_cursor_shape_manager_v1, struct zwp_tablet_tool_v2 *tablet_tool)
{
	struct wl_proxy *cursor_shape_device;

	cursor_shape_device = wl_proxy_create((struct wl_proxy *) wp_cursor_shape_manager_v1,
			     &wp_cursor_shape_device_v1_interface);
	if (!cursor_shape_device)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wp_cursor_shape_manager_v1,
			 WP_CURSOR_SHAPE_MANAGER_V1_GET_TABLET_TOOL_V2, cursor_shape_device, tablet_tool);

	return (struct wp_cursor_shape_device_v1 *) cursor_shape_device;
}

#ifndef WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ENUM
#define WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ENUM
/**
 * wp_cursor_shape_device_v1_shape - cursor shapes
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_DEFAULT: default cursor
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_CONTEXT_MENU: a context menu is
 *	available for the object under the cursor
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_HELP: help is available for the
 *	object under the cursor
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_POINTER: pointer that indicates a
 *	link or another interactive element
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_PROGRESS: progress indicator
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_WAIT: program is busy, user should
 *	wait
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_CELL: a cell or set of cells may be
 *	selected
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_CROSSHAIR: simple crosshair
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_TEXT: text may be selected
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_VERTICAL_TEXT: vertical text may be
 *	selected
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ALIAS: drag-and-drop: alias
 *	of/shortcut to something is to be created
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_COPY: drag-and-drop: something is to
 *	be copied
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_MOVE: drag-and-drop: something is to
 *	be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NO_DROP: drag-and-drop: the dragged
 *	item cannot be dropped at the current cursor location
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NOT_ALLOWED: drag-and-drop: the
 *	requested action will not be carried out
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_GRAB: drag-and-drop: something can be
 *	grabbed
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_GRABBING: drag-and-drop: something is
 *	being grabbed
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_E_RESIZE: resizing: the east border
 *	is to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_N_RESIZE: resizing: the north border
 *	is to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NE_RESIZE: resizing: the north-east
 *	corner is to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NW_RESIZE: resizing: the north-west
 *	corner is to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_S_RESIZE: resizing: the south border
 *	is to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_SE_RESIZE: resizing: the south-east
 *	corner is to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_SW_RESIZE: resizing: the south-west
 *	corner is to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_W_RESIZE: resizing: the west border
 *	is to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_EW_RESIZE: resizing: the east and
 *	west borders are to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NS_RESIZE: resizing: the north and
 *	south borders are to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NESW_RESIZE: resizing: the north-east
 *	and south-west corners are to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NWSE_RESIZE: resizing: the north-west
 *	and south-east corners are to be moved
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_COL_RESIZE: resizing: that the
 *	item/column can be resized horizontally
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ROW_RESIZE: resizing: that the
 *	item/row can be resized vertically
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ALL_SCROLL: something can be scrolled
 *	in any direction
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ZOOM_IN: something can be zoomed in
 * @WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ZOOM_OUT: something can be zoomed out
 *
 * This enum describes cursor shapes.
 *
 * The names are taken from the CSS W3C specification:
 * https://w3c.github.io/csswg-drafts/css-ui/#cursor
 */
enum wp_cursor_shape_device_v1_shape {
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_DEFAULT = 1,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_CONTEXT_MENU = 2,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_HELP = 3,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_POINTER = 4,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_PROGRESS = 5,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_WAIT = 6,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_CELL = 7,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_CROSSHAIR = 8,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_TEXT = 9,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_VERTICAL_TEXT = 10,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ALIAS = 11,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_COPY = 12,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_MOVE = 13,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NO_DROP = 14,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NOT_ALLOWED = 15,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_GRAB = 16,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_GRABBING = 17,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_E_RESIZE = 18,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_N_RESIZE = 19,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NE_RESIZE = 20,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NW_RESIZE = 21,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_S_RESIZE = 22,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_SE_RESIZE = 23,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_SW_RESIZE = 24,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_W_RESIZE = 25,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_EW_RESIZE = 26,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NS_RESIZE = 27,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NESW_RESIZE = 28,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_NWSE_RESIZE = 29,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_COL_RESIZE = 30,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ROW_RESIZE = 31,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ALL_SCROLL = 32,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ZOOM_IN = 33,
	WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ZOOM_OUT = 34,
};
#endif /* WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_ENUM */

#ifndef WP_CURSOR_SHAPE_DEVICE_V1_ERROR_ENUM
#define WP_CURSOR_SHAPE_DEVICE_V1_ERROR_ENUM
enum wp_cursor_shape_device_v1_error {
	WP_CURSOR_SHAPE_DEVICE_V1_ERROR_INVALID_SHAPE = 1,
};
#endif /* WP_CURSOR_SHAPE_DEVICE_V1_ERROR_ENUM */

#define WP_CURSOR_SHAPE_DEVICE_V1_DESTROY	0
#define WP_CURSOR_SHAPE_DEVICE_V1_SET_SHAPE	1

static inline void
wp_cursor_shape_device_v1_set_user_data(struct wp_cursor_shape_device_v1 *wp_cursor_shape_device_v1, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wp_cursor_shape_device_v1, user_data);
}

static inline void *
wp_cursor_shape_device_v1_get_user_data(struct wp_cursor_shape_device_v1 *wp_cursor_shape_device_v1)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wp_cursor_shape_device_v1);
}

static inline void
wp_cursor_shape_device_v1_destroy(struct wp_cursor_shape_device_v1 *wp_cursor_shape_device_v1)
{
	wl_proxy_marshal((struct wl_proxy *) wp_cursor_shape_device_v1,
			 WP_CURSOR_SHAPE_DEVICE_V1_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wp_cursor_shape_device_v1);
}

static inline void
wp_cursor_shape_device_v1_set_shape(struct wp_cursor_shape_device_v1 *wp_cursor_shape_device_v1, uint32_t serial, uint32_t shape)
{
	wl_proxy_marshal((struct wl_proxy *) wp_cursor_shape_device_v1,
			 WP_CURSOR_SHAPE_DEVICE_V1_SET_SHAPE, serial, shape);
}

#ifdef  __cplusplus
}
#endif

#endif
