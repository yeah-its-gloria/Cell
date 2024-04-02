/* 
 * Copyright 2014 © Stephen "Lyude" Chandler Paul
 * Copyright 2015-2016 © Red Hat, Inc.
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <stdint.h>
#include "wayland-util.h"

extern const struct wl_interface zwp_tablet_seat_v2_interface;
extern const struct wl_interface wl_seat_interface;
extern const struct wl_interface zwp_tablet_v2_interface;
extern const struct wl_interface zwp_tablet_tool_v2_interface;
extern const struct wl_interface zwp_tablet_pad_v2_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface zwp_tablet_v2_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface zwp_tablet_pad_ring_v2_interface;
extern const struct wl_interface zwp_tablet_pad_strip_v2_interface;
extern const struct wl_interface zwp_tablet_pad_group_v2_interface;
extern const struct wl_interface zwp_tablet_v2_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_surface_interface;

static const struct wl_interface *types[] = {
	NULL,
	NULL,
	NULL,
	&zwp_tablet_seat_v2_interface,
	&wl_seat_interface,
	&zwp_tablet_v2_interface,
	&zwp_tablet_tool_v2_interface,
	&zwp_tablet_pad_v2_interface,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	NULL,
	&zwp_tablet_v2_interface,
	&wl_surface_interface,
	&zwp_tablet_pad_ring_v2_interface,
	&zwp_tablet_pad_strip_v2_interface,
	&zwp_tablet_pad_group_v2_interface,
	NULL,
	&zwp_tablet_v2_interface,
	&wl_surface_interface,
	NULL,
	&wl_surface_interface,
};

static const struct wl_message zwp_tablet_manager_v2_requests[] = {
	{ "get_tablet_seat", "no", types + 3 },
	{ "destroy", "", types + 0 },
};

WL_EXPORT const struct wl_interface zwp_tablet_manager_v2_interface = {
	"zwp_tablet_manager_v2", 1,
	2, zwp_tablet_manager_v2_requests,
	0, NULL,
};

static const struct wl_message zwp_tablet_seat_v2_requests[] = {
	{ "destroy", "", types + 0 },
};

static const struct wl_message zwp_tablet_seat_v2_events[] = {
	{ "tablet_added", "n", types + 5 },
	{ "tool_added", "n", types + 6 },
	{ "pad_added", "n", types + 7 },
};

WL_EXPORT const struct wl_interface zwp_tablet_seat_v2_interface = {
	"zwp_tablet_seat_v2", 1,
	1, zwp_tablet_seat_v2_requests,
	3, zwp_tablet_seat_v2_events,
};

static const struct wl_message zwp_tablet_tool_v2_requests[] = {
	{ "set_cursor", "u?oii", types + 8 },
	{ "destroy", "", types + 0 },
};

static const struct wl_message zwp_tablet_tool_v2_events[] = {
	{ "type", "u", types + 0 },
	{ "hardware_serial", "uu", types + 0 },
	{ "hardware_id_wacom", "uu", types + 0 },
	{ "capability", "u", types + 0 },
	{ "done", "", types + 0 },
	{ "removed", "", types + 0 },
	{ "proximity_in", "uoo", types + 12 },
	{ "proximity_out", "", types + 0 },
	{ "down", "u", types + 0 },
	{ "up", "", types + 0 },
	{ "motion", "ff", types + 0 },
	{ "pressure", "u", types + 0 },
	{ "distance", "u", types + 0 },
	{ "tilt", "ff", types + 0 },
	{ "rotation", "f", types + 0 },
	{ "slider", "i", types + 0 },
	{ "wheel", "fi", types + 0 },
	{ "button", "uuu", types + 0 },
	{ "frame", "u", types + 0 },
};

WL_EXPORT const struct wl_interface zwp_tablet_tool_v2_interface = {
	"zwp_tablet_tool_v2", 1,
	2, zwp_tablet_tool_v2_requests,
	19, zwp_tablet_tool_v2_events,
};

static const struct wl_message zwp_tablet_v2_requests[] = {
	{ "destroy", "", types + 0 },
};

static const struct wl_message zwp_tablet_v2_events[] = {
	{ "name", "s", types + 0 },
	{ "id", "uu", types + 0 },
	{ "path", "s", types + 0 },
	{ "done", "", types + 0 },
	{ "removed", "", types + 0 },
};

WL_EXPORT const struct wl_interface zwp_tablet_v2_interface = {
	"zwp_tablet_v2", 1,
	1, zwp_tablet_v2_requests,
	5, zwp_tablet_v2_events,
};

static const struct wl_message zwp_tablet_pad_ring_v2_requests[] = {
	{ "set_feedback", "su", types + 0 },
	{ "destroy", "", types + 0 },
};

static const struct wl_message zwp_tablet_pad_ring_v2_events[] = {
	{ "source", "u", types + 0 },
	{ "angle", "f", types + 0 },
	{ "stop", "", types + 0 },
	{ "frame", "u", types + 0 },
};

WL_EXPORT const struct wl_interface zwp_tablet_pad_ring_v2_interface = {
	"zwp_tablet_pad_ring_v2", 1,
	2, zwp_tablet_pad_ring_v2_requests,
	4, zwp_tablet_pad_ring_v2_events,
};

static const struct wl_message zwp_tablet_pad_strip_v2_requests[] = {
	{ "set_feedback", "su", types + 0 },
	{ "destroy", "", types + 0 },
};

static const struct wl_message zwp_tablet_pad_strip_v2_events[] = {
	{ "source", "u", types + 0 },
	{ "position", "u", types + 0 },
	{ "stop", "", types + 0 },
	{ "frame", "u", types + 0 },
};

WL_EXPORT const struct wl_interface zwp_tablet_pad_strip_v2_interface = {
	"zwp_tablet_pad_strip_v2", 1,
	2, zwp_tablet_pad_strip_v2_requests,
	4, zwp_tablet_pad_strip_v2_events,
};

static const struct wl_message zwp_tablet_pad_group_v2_requests[] = {
	{ "destroy", "", types + 0 },
};

static const struct wl_message zwp_tablet_pad_group_v2_events[] = {
	{ "buttons", "a", types + 0 },
	{ "ring", "n", types + 15 },
	{ "strip", "n", types + 16 },
	{ "modes", "u", types + 0 },
	{ "done", "", types + 0 },
	{ "mode_switch", "uuu", types + 0 },
};

WL_EXPORT const struct wl_interface zwp_tablet_pad_group_v2_interface = {
	"zwp_tablet_pad_group_v2", 1,
	1, zwp_tablet_pad_group_v2_requests,
	6, zwp_tablet_pad_group_v2_events,
};

static const struct wl_message zwp_tablet_pad_v2_requests[] = {
	{ "set_feedback", "usu", types + 0 },
	{ "destroy", "", types + 0 },
};

static const struct wl_message zwp_tablet_pad_v2_events[] = {
	{ "group", "n", types + 17 },
	{ "path", "s", types + 0 },
	{ "buttons", "u", types + 0 },
	{ "done", "", types + 0 },
	{ "button", "uuu", types + 0 },
	{ "enter", "uoo", types + 18 },
	{ "leave", "uo", types + 21 },
	{ "removed", "", types + 0 },
};

WL_EXPORT const struct wl_interface zwp_tablet_pad_v2_interface = {
	"zwp_tablet_pad_v2", 1,
	2, zwp_tablet_pad_v2_requests,
	8, zwp_tablet_pad_v2_events,
};

