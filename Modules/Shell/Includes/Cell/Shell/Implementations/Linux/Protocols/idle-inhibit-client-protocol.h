/* 
 * Copyright © 2015 Samsung Electronics Co., Ltd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef IDLE_INHIBIT_UNSTABLE_V1_CLIENT_PROTOCOL_H
#define IDLE_INHIBIT_UNSTABLE_V1_CLIENT_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

struct wl_client;
struct wl_resource;

struct zwp_idle_inhibit_manager_v1;
struct zwp_idle_inhibitor_v1;

extern const struct wl_interface zwp_idle_inhibit_manager_v1_interface;
extern const struct wl_interface zwp_idle_inhibitor_v1_interface;

#define ZWP_IDLE_INHIBIT_MANAGER_V1_DESTROY	0
#define ZWP_IDLE_INHIBIT_MANAGER_V1_CREATE_INHIBITOR	1

static inline void
zwp_idle_inhibit_manager_v1_set_user_data(struct zwp_idle_inhibit_manager_v1 *zwp_idle_inhibit_manager_v1, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_idle_inhibit_manager_v1, user_data);
}

static inline void *
zwp_idle_inhibit_manager_v1_get_user_data(struct zwp_idle_inhibit_manager_v1 *zwp_idle_inhibit_manager_v1)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_idle_inhibit_manager_v1);
}

static inline void
zwp_idle_inhibit_manager_v1_destroy(struct zwp_idle_inhibit_manager_v1 *zwp_idle_inhibit_manager_v1)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_idle_inhibit_manager_v1,
			 ZWP_IDLE_INHIBIT_MANAGER_V1_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_idle_inhibit_manager_v1);
}

static inline struct zwp_idle_inhibitor_v1 *
zwp_idle_inhibit_manager_v1_create_inhibitor(struct zwp_idle_inhibit_manager_v1 *zwp_idle_inhibit_manager_v1, struct wl_surface *surface)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) zwp_idle_inhibit_manager_v1,
			     &zwp_idle_inhibitor_v1_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) zwp_idle_inhibit_manager_v1,
			 ZWP_IDLE_INHIBIT_MANAGER_V1_CREATE_INHIBITOR, id, surface);

	return (struct zwp_idle_inhibitor_v1 *) id;
}

#define ZWP_IDLE_INHIBITOR_V1_DESTROY	0

static inline void
zwp_idle_inhibitor_v1_set_user_data(struct zwp_idle_inhibitor_v1 *zwp_idle_inhibitor_v1, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_idle_inhibitor_v1, user_data);
}

static inline void *
zwp_idle_inhibitor_v1_get_user_data(struct zwp_idle_inhibitor_v1 *zwp_idle_inhibitor_v1)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_idle_inhibitor_v1);
}

static inline void
zwp_idle_inhibitor_v1_destroy(struct zwp_idle_inhibitor_v1 *zwp_idle_inhibitor_v1)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_idle_inhibitor_v1,
			 ZWP_IDLE_INHIBITOR_V1_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_idle_inhibitor_v1);
}

#ifdef  __cplusplus
}
#endif

#endif
