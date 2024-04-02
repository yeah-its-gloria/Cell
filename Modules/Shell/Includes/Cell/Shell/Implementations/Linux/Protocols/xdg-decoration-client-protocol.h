/* 
 * Copyright © 2018 Simon Ser
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

#ifndef XDG_DECORATION_UNSTABLE_V1_CLIENT_PROTOCOL_H
#define XDG_DECORATION_UNSTABLE_V1_CLIENT_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

struct wl_client;
struct wl_resource;

struct zxdg_decoration_manager_v1;
struct zxdg_toplevel_decoration_v1;

extern const struct wl_interface zxdg_decoration_manager_v1_interface;
extern const struct wl_interface zxdg_toplevel_decoration_v1_interface;

#define ZXDG_DECORATION_MANAGER_V1_DESTROY	0
#define ZXDG_DECORATION_MANAGER_V1_GET_TOPLEVEL_DECORATION	1

static inline void
zxdg_decoration_manager_v1_set_user_data(struct zxdg_decoration_manager_v1 *zxdg_decoration_manager_v1, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zxdg_decoration_manager_v1, user_data);
}

static inline void *
zxdg_decoration_manager_v1_get_user_data(struct zxdg_decoration_manager_v1 *zxdg_decoration_manager_v1)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zxdg_decoration_manager_v1);
}

static inline void
zxdg_decoration_manager_v1_destroy(struct zxdg_decoration_manager_v1 *zxdg_decoration_manager_v1)
{
	wl_proxy_marshal((struct wl_proxy *) zxdg_decoration_manager_v1,
			 ZXDG_DECORATION_MANAGER_V1_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zxdg_decoration_manager_v1);
}

static inline struct zxdg_toplevel_decoration_v1 *
zxdg_decoration_manager_v1_get_toplevel_decoration(struct zxdg_decoration_manager_v1 *zxdg_decoration_manager_v1, struct xdg_toplevel *toplevel)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) zxdg_decoration_manager_v1,
			     &zxdg_toplevel_decoration_v1_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) zxdg_decoration_manager_v1,
			 ZXDG_DECORATION_MANAGER_V1_GET_TOPLEVEL_DECORATION, id, toplevel);

	return (struct zxdg_toplevel_decoration_v1 *) id;
}

#ifndef ZXDG_TOPLEVEL_DECORATION_V1_ERROR_ENUM
#define ZXDG_TOPLEVEL_DECORATION_V1_ERROR_ENUM
enum zxdg_toplevel_decoration_v1_error {
	ZXDG_TOPLEVEL_DECORATION_V1_ERROR_UNCONFIGURED_BUFFER = 0,
	ZXDG_TOPLEVEL_DECORATION_V1_ERROR_ALREADY_CONSTRUCTED = 1,
	ZXDG_TOPLEVEL_DECORATION_V1_ERROR_ORPHANED = 2,
};
#endif /* ZXDG_TOPLEVEL_DECORATION_V1_ERROR_ENUM */

#ifndef ZXDG_TOPLEVEL_DECORATION_V1_MODE_ENUM
#define ZXDG_TOPLEVEL_DECORATION_V1_MODE_ENUM
/**
 * zxdg_toplevel_decoration_v1_mode - window decoration modes
 * @ZXDG_TOPLEVEL_DECORATION_V1_MODE_CLIENT_SIDE: no server-side window
 *	decoration
 * @ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE: server-side window
 *	decoration
 *
 * These values describe window decoration modes.
 */
enum zxdg_toplevel_decoration_v1_mode {
	ZXDG_TOPLEVEL_DECORATION_V1_MODE_CLIENT_SIDE = 1,
	ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE = 2,
};
#endif /* ZXDG_TOPLEVEL_DECORATION_V1_MODE_ENUM */

/**
 * zxdg_toplevel_decoration_v1 - decoration object for a toplevel surface
 * @configure: notify a decoration mode change
 *
 * The decoration object allows the compositor to toggle server-side
 * window decorations for a toplevel surface. The client can request to
 * switch to another mode.
 *
 * The xdg_toplevel_decoration object must be destroyed before its
 * xdg_toplevel.
 */
struct zxdg_toplevel_decoration_v1_listener {
	/**
	 * configure - notify a decoration mode change
	 * @mode: the decoration mode
	 *
	 * The configure event configures the effective decoration mode.
	 * The configured state should not be applied immediately. Clients
	 * must send an ack_configure in response to this event. See
	 * xdg_surface.configure and xdg_surface.ack_configure for details.
	 *
	 * A configure event can be sent at any time. The specified mode
	 * must be obeyed by the client.
	 */
	void (*configure)(void *data,
			  struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1,
			  uint32_t mode);
};

static inline int
zxdg_toplevel_decoration_v1_add_listener(struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1,
					 const struct zxdg_toplevel_decoration_v1_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) zxdg_toplevel_decoration_v1,
				     (void (**)(void)) listener, data);
}

#define ZXDG_TOPLEVEL_DECORATION_V1_DESTROY	0
#define ZXDG_TOPLEVEL_DECORATION_V1_SET_MODE	1
#define ZXDG_TOPLEVEL_DECORATION_V1_UNSET_MODE	2

static inline void
zxdg_toplevel_decoration_v1_set_user_data(struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zxdg_toplevel_decoration_v1, user_data);
}

static inline void *
zxdg_toplevel_decoration_v1_get_user_data(struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zxdg_toplevel_decoration_v1);
}

static inline void
zxdg_toplevel_decoration_v1_destroy(struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1)
{
	wl_proxy_marshal((struct wl_proxy *) zxdg_toplevel_decoration_v1,
			 ZXDG_TOPLEVEL_DECORATION_V1_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zxdg_toplevel_decoration_v1);
}

static inline void
zxdg_toplevel_decoration_v1_set_mode(struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1, uint32_t mode)
{
	wl_proxy_marshal((struct wl_proxy *) zxdg_toplevel_decoration_v1,
			 ZXDG_TOPLEVEL_DECORATION_V1_SET_MODE, mode);
}

static inline void
zxdg_toplevel_decoration_v1_unset_mode(struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1)
{
	wl_proxy_marshal((struct wl_proxy *) zxdg_toplevel_decoration_v1,
			 ZXDG_TOPLEVEL_DECORATION_V1_UNSET_MODE);
}

#ifdef  __cplusplus
}
#endif

#endif
