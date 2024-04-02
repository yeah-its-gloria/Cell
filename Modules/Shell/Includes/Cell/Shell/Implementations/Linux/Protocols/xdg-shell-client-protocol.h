/* 
 * Copyright © 2008-2013 Kristian Høgsberg
 * Copyright © 2013      Rafael Antognolli
 * Copyright © 2013      Jasper St. Pierre
 * Copyright © 2010-2013 Intel Corporation
 * Copyright © 2015-2017 Samsung Electronics Co., Ltd
 * Copyright © 2015-2017 Red Hat Inc.
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

#ifndef XDG_SHELL_CLIENT_PROTOCOL_H
#define XDG_SHELL_CLIENT_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

struct wl_client;
struct wl_resource;

struct xdg_wm_base;
struct xdg_positioner;
struct xdg_surface;
struct xdg_toplevel;
struct xdg_popup;

extern const struct wl_interface xdg_wm_base_interface;
extern const struct wl_interface xdg_positioner_interface;
extern const struct wl_interface xdg_surface_interface;
extern const struct wl_interface xdg_toplevel_interface;
extern const struct wl_interface xdg_popup_interface;

#ifndef XDG_WM_BASE_ERROR_ENUM
#define XDG_WM_BASE_ERROR_ENUM
enum xdg_wm_base_error {
	XDG_WM_BASE_ERROR_ROLE = 0,
	XDG_WM_BASE_ERROR_DEFUNCT_SURFACES = 1,
	XDG_WM_BASE_ERROR_NOT_THE_TOPMOST_POPUP = 2,
	XDG_WM_BASE_ERROR_INVALID_POPUP_PARENT = 3,
	XDG_WM_BASE_ERROR_INVALID_SURFACE_STATE = 4,
	XDG_WM_BASE_ERROR_INVALID_POSITIONER = 5,
	XDG_WM_BASE_ERROR_UNRESPONSIVE = 6,
};
#endif /* XDG_WM_BASE_ERROR_ENUM */

/**
 * xdg_wm_base - create desktop-style surfaces
 * @ping: check if the client is alive
 *
 * The xdg_wm_base interface is exposed as a global object enabling
 * clients to turn their wl_surfaces into windows in a desktop environment.
 * It defines the basic functionality needed for clients and the compositor
 * to create windows that can be dragged, resized, maximized, etc, as well
 * as creating transient windows such as popup menus.
 */
struct xdg_wm_base_listener {
	/**
	 * ping - check if the client is alive
	 * @serial: pass this to the pong request
	 *
	 * The ping event asks the client if it's still alive. Pass the
	 * serial specified in the event back to the compositor by sending
	 * a "pong" request back with the specified serial. See
	 * xdg_wm_base.pong.
	 *
	 * Compositors can use this to determine if the client is still
	 * alive. It's unspecified what will happen if the client doesn't
	 * respond to the ping request, or in what timeframe. Clients
	 * should try to respond in a reasonable amount of time. The
	 * “unresponsive” error is provided for compositors that wish
	 * to disconnect unresponsive clients.
	 *
	 * A compositor is free to ping in any way it wants, but a client
	 * must always respond to any xdg_wm_base object it created.
	 */
	void (*ping)(void *data,
		     struct xdg_wm_base *xdg_wm_base,
		     uint32_t serial);
};

static inline int
xdg_wm_base_add_listener(struct xdg_wm_base *xdg_wm_base,
			 const struct xdg_wm_base_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) xdg_wm_base,
				     (void (**)(void)) listener, data);
}

#define XDG_WM_BASE_DESTROY	0
#define XDG_WM_BASE_CREATE_POSITIONER	1
#define XDG_WM_BASE_GET_XDG_SURFACE	2
#define XDG_WM_BASE_PONG	3

static inline void
xdg_wm_base_set_user_data(struct xdg_wm_base *xdg_wm_base, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) xdg_wm_base, user_data);
}

static inline void *
xdg_wm_base_get_user_data(struct xdg_wm_base *xdg_wm_base)
{
	return wl_proxy_get_user_data((struct wl_proxy *) xdg_wm_base);
}

static inline void
xdg_wm_base_destroy(struct xdg_wm_base *xdg_wm_base)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_wm_base,
			 XDG_WM_BASE_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) xdg_wm_base);
}

static inline struct xdg_positioner *
xdg_wm_base_create_positioner(struct xdg_wm_base *xdg_wm_base)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) xdg_wm_base,
			     &xdg_positioner_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) xdg_wm_base,
			 XDG_WM_BASE_CREATE_POSITIONER, id);

	return (struct xdg_positioner *) id;
}

static inline struct xdg_surface *
xdg_wm_base_get_xdg_surface(struct xdg_wm_base *xdg_wm_base, struct wl_surface *surface)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) xdg_wm_base,
			     &xdg_surface_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) xdg_wm_base,
			 XDG_WM_BASE_GET_XDG_SURFACE, id, surface);

	return (struct xdg_surface *) id;
}

static inline void
xdg_wm_base_pong(struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_wm_base,
			 XDG_WM_BASE_PONG, serial);
}

#ifndef XDG_POSITIONER_ERROR_ENUM
#define XDG_POSITIONER_ERROR_ENUM
enum xdg_positioner_error {
	XDG_POSITIONER_ERROR_INVALID_INPUT = 0,
};
#endif /* XDG_POSITIONER_ERROR_ENUM */

#ifndef XDG_POSITIONER_ANCHOR_ENUM
#define XDG_POSITIONER_ANCHOR_ENUM
enum xdg_positioner_anchor {
	XDG_POSITIONER_ANCHOR_NONE = 0,
	XDG_POSITIONER_ANCHOR_TOP = 1,
	XDG_POSITIONER_ANCHOR_BOTTOM = 2,
	XDG_POSITIONER_ANCHOR_LEFT = 3,
	XDG_POSITIONER_ANCHOR_RIGHT = 4,
	XDG_POSITIONER_ANCHOR_TOP_LEFT = 5,
	XDG_POSITIONER_ANCHOR_BOTTOM_LEFT = 6,
	XDG_POSITIONER_ANCHOR_TOP_RIGHT = 7,
	XDG_POSITIONER_ANCHOR_BOTTOM_RIGHT = 8,
};
#endif /* XDG_POSITIONER_ANCHOR_ENUM */

#ifndef XDG_POSITIONER_GRAVITY_ENUM
#define XDG_POSITIONER_GRAVITY_ENUM
enum xdg_positioner_gravity {
	XDG_POSITIONER_GRAVITY_NONE = 0,
	XDG_POSITIONER_GRAVITY_TOP = 1,
	XDG_POSITIONER_GRAVITY_BOTTOM = 2,
	XDG_POSITIONER_GRAVITY_LEFT = 3,
	XDG_POSITIONER_GRAVITY_RIGHT = 4,
	XDG_POSITIONER_GRAVITY_TOP_LEFT = 5,
	XDG_POSITIONER_GRAVITY_BOTTOM_LEFT = 6,
	XDG_POSITIONER_GRAVITY_TOP_RIGHT = 7,
	XDG_POSITIONER_GRAVITY_BOTTOM_RIGHT = 8,
};
#endif /* XDG_POSITIONER_GRAVITY_ENUM */

#ifndef XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_ENUM
#define XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_ENUM
/**
 * xdg_positioner_constraint_adjustment - vertically resize the surface
 * @XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_NONE: (none)
 * @XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_X: (none)
 * @XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_Y: (none)
 * @XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_X: (none)
 * @XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_Y: (none)
 * @XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_RESIZE_X: (none)
 * @XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_RESIZE_Y: (none)
 *
 * Resize the surface vertically so that it is completely unconstrained.
 */
enum xdg_positioner_constraint_adjustment {
	XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_NONE = 0,
	XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_X = 1,
	XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_Y = 2,
	XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_X = 4,
	XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_Y = 8,
	XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_RESIZE_X = 16,
	XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_RESIZE_Y = 32,
};
#endif /* XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_ENUM */

#define XDG_POSITIONER_DESTROY	0
#define XDG_POSITIONER_SET_SIZE	1
#define XDG_POSITIONER_SET_ANCHOR_RECT	2
#define XDG_POSITIONER_SET_ANCHOR	3
#define XDG_POSITIONER_SET_GRAVITY	4
#define XDG_POSITIONER_SET_CONSTRAINT_ADJUSTMENT	5
#define XDG_POSITIONER_SET_OFFSET	6
#define XDG_POSITIONER_SET_REACTIVE	7
#define XDG_POSITIONER_SET_PARENT_SIZE	8
#define XDG_POSITIONER_SET_PARENT_CONFIGURE	9

static inline void
xdg_positioner_set_user_data(struct xdg_positioner *xdg_positioner, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) xdg_positioner, user_data);
}

static inline void *
xdg_positioner_get_user_data(struct xdg_positioner *xdg_positioner)
{
	return wl_proxy_get_user_data((struct wl_proxy *) xdg_positioner);
}

static inline void
xdg_positioner_destroy(struct xdg_positioner *xdg_positioner)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_positioner,
			 XDG_POSITIONER_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) xdg_positioner);
}

static inline void
xdg_positioner_set_size(struct xdg_positioner *xdg_positioner, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_positioner,
			 XDG_POSITIONER_SET_SIZE, width, height);
}

static inline void
xdg_positioner_set_anchor_rect(struct xdg_positioner *xdg_positioner, int32_t x, int32_t y, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_positioner,
			 XDG_POSITIONER_SET_ANCHOR_RECT, x, y, width, height);
}

static inline void
xdg_positioner_set_anchor(struct xdg_positioner *xdg_positioner, uint32_t anchor)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_positioner,
			 XDG_POSITIONER_SET_ANCHOR, anchor);
}

static inline void
xdg_positioner_set_gravity(struct xdg_positioner *xdg_positioner, uint32_t gravity)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_positioner,
			 XDG_POSITIONER_SET_GRAVITY, gravity);
}

static inline void
xdg_positioner_set_constraint_adjustment(struct xdg_positioner *xdg_positioner, uint32_t constraint_adjustment)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_positioner,
			 XDG_POSITIONER_SET_CONSTRAINT_ADJUSTMENT, constraint_adjustment);
}

static inline void
xdg_positioner_set_offset(struct xdg_positioner *xdg_positioner, int32_t x, int32_t y)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_positioner,
			 XDG_POSITIONER_SET_OFFSET, x, y);
}

static inline void
xdg_positioner_set_reactive(struct xdg_positioner *xdg_positioner)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_positioner,
			 XDG_POSITIONER_SET_REACTIVE);
}

static inline void
xdg_positioner_set_parent_size(struct xdg_positioner *xdg_positioner, int32_t parent_width, int32_t parent_height)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_positioner,
			 XDG_POSITIONER_SET_PARENT_SIZE, parent_width, parent_height);
}

static inline void
xdg_positioner_set_parent_configure(struct xdg_positioner *xdg_positioner, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_positioner,
			 XDG_POSITIONER_SET_PARENT_CONFIGURE, serial);
}

#ifndef XDG_SURFACE_ERROR_ENUM
#define XDG_SURFACE_ERROR_ENUM
enum xdg_surface_error {
	XDG_SURFACE_ERROR_NOT_CONSTRUCTED = 1,
	XDG_SURFACE_ERROR_ALREADY_CONSTRUCTED = 2,
	XDG_SURFACE_ERROR_UNCONFIGURED_BUFFER = 3,
	XDG_SURFACE_ERROR_INVALID_SERIAL = 4,
	XDG_SURFACE_ERROR_INVALID_SIZE = 5,
	XDG_SURFACE_ERROR_DEFUNCT_ROLE_OBJECT = 6,
};
#endif /* XDG_SURFACE_ERROR_ENUM */

/**
 * xdg_surface - desktop user interface surface base interface
 * @configure: suggest a surface change
 *
 * An interface that may be implemented by a wl_surface, for
 * implementations that provide a desktop-style user interface.
 *
 * It provides a base set of functionality required to construct user
 * interface elements requiring management by the compositor, such as
 * toplevel windows, menus, etc. The types of functionality are split into
 * xdg_surface roles.
 *
 * Creating an xdg_surface does not set the role for a wl_surface. In order
 * to map an xdg_surface, the client must create a role-specific object
 * using, e.g., get_toplevel, get_popup. The wl_surface for any given
 * xdg_surface can have at most one role, and may not be assigned any role
 * not based on xdg_surface.
 *
 * A role must be assigned before any other requests are made to the
 * xdg_surface object.
 *
 * The client must call wl_surface.commit on the corresponding wl_surface
 * for the xdg_surface state to take effect.
 *
 * Creating an xdg_surface from a wl_surface which has a buffer attached or
 * committed is a client error, and any attempts by a client to attach or
 * manipulate a buffer prior to the first xdg_surface.configure call must
 * also be treated as errors.
 *
 * After creating a role-specific object and setting it up, the client must
 * perform an initial commit without any buffer attached. The compositor
 * will reply with initial wl_surface state such as
 * wl_surface.preferred_buffer_scale followed by an xdg_surface.configure
 * event. The client must acknowledge it and is then allowed to attach a
 * buffer to map the surface.
 *
 * Mapping an xdg_surface-based role surface is defined as making it
 * possible for the surface to be shown by the compositor. Note that a
 * mapped surface is not guaranteed to be visible once it is mapped.
 *
 * For an xdg_surface to be mapped by the compositor, the following
 * conditions must be met: (1) the client has assigned an xdg_surface-based
 * role to the surface (2) the client has set and committed the xdg_surface
 * state and the role-dependent state to the surface (3) the client has
 * committed a buffer to the surface
 *
 * A newly-unmapped surface is considered to have met condition (1) out of
 * the 3 required conditions for mapping a surface if its role surface has
 * not been destroyed, i.e. the client must perform the initial commit
 * again before attaching a buffer.
 */
struct xdg_surface_listener {
	/**
	 * configure - suggest a surface change
	 * @serial: serial of the configure event
	 *
	 * The configure event marks the end of a configure sequence. A
	 * configure sequence is a set of one or more events configuring
	 * the state of the xdg_surface, including the final
	 * xdg_surface.configure event.
	 *
	 * Where applicable, xdg_surface surface roles will during a
	 * configure sequence extend this event as a latched state sent as
	 * events before the xdg_surface.configure event. Such events
	 * should be considered to make up a set of atomically applied
	 * configuration states, where the xdg_surface.configure commits
	 * the accumulated state.
	 *
	 * Clients should arrange their surface for the new states, and
	 * then send an ack_configure request with the serial sent in this
	 * configure event at some point before committing the new surface.
	 *
	 * If the client receives multiple configure events before it can
	 * respond to one, it is free to discard all but the last event it
	 * received.
	 */
	void (*configure)(void *data,
			  struct xdg_surface *xdg_surface,
			  uint32_t serial);
};

static inline int
xdg_surface_add_listener(struct xdg_surface *xdg_surface,
			 const struct xdg_surface_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) xdg_surface,
				     (void (**)(void)) listener, data);
}

#define XDG_SURFACE_DESTROY	0
#define XDG_SURFACE_GET_TOPLEVEL	1
#define XDG_SURFACE_GET_POPUP	2
#define XDG_SURFACE_SET_WINDOW_GEOMETRY	3
#define XDG_SURFACE_ACK_CONFIGURE	4

static inline void
xdg_surface_set_user_data(struct xdg_surface *xdg_surface, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) xdg_surface, user_data);
}

static inline void *
xdg_surface_get_user_data(struct xdg_surface *xdg_surface)
{
	return wl_proxy_get_user_data((struct wl_proxy *) xdg_surface);
}

static inline void
xdg_surface_destroy(struct xdg_surface *xdg_surface)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_surface,
			 XDG_SURFACE_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) xdg_surface);
}

static inline struct xdg_toplevel *
xdg_surface_get_toplevel(struct xdg_surface *xdg_surface)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) xdg_surface,
			     &xdg_toplevel_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) xdg_surface,
			 XDG_SURFACE_GET_TOPLEVEL, id);

	return (struct xdg_toplevel *) id;
}

static inline struct xdg_popup *
xdg_surface_get_popup(struct xdg_surface *xdg_surface, struct xdg_surface *parent, struct xdg_positioner *positioner)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) xdg_surface,
			     &xdg_popup_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) xdg_surface,
			 XDG_SURFACE_GET_POPUP, id, parent, positioner);

	return (struct xdg_popup *) id;
}

static inline void
xdg_surface_set_window_geometry(struct xdg_surface *xdg_surface, int32_t x, int32_t y, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_surface,
			 XDG_SURFACE_SET_WINDOW_GEOMETRY, x, y, width, height);
}

static inline void
xdg_surface_ack_configure(struct xdg_surface *xdg_surface, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_surface,
			 XDG_SURFACE_ACK_CONFIGURE, serial);
}

#ifndef XDG_TOPLEVEL_ERROR_ENUM
#define XDG_TOPLEVEL_ERROR_ENUM
enum xdg_toplevel_error {
	XDG_TOPLEVEL_ERROR_INVALID_RESIZE_EDGE = 0,
	XDG_TOPLEVEL_ERROR_INVALID_PARENT = 1,
	XDG_TOPLEVEL_ERROR_INVALID_SIZE = 2,
};
#endif /* XDG_TOPLEVEL_ERROR_ENUM */

#ifndef XDG_TOPLEVEL_RESIZE_EDGE_ENUM
#define XDG_TOPLEVEL_RESIZE_EDGE_ENUM
/**
 * xdg_toplevel_resize_edge - edge values for resizing
 * @XDG_TOPLEVEL_RESIZE_EDGE_NONE: (none)
 * @XDG_TOPLEVEL_RESIZE_EDGE_TOP: (none)
 * @XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM: (none)
 * @XDG_TOPLEVEL_RESIZE_EDGE_LEFT: (none)
 * @XDG_TOPLEVEL_RESIZE_EDGE_TOP_LEFT: (none)
 * @XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_LEFT: (none)
 * @XDG_TOPLEVEL_RESIZE_EDGE_RIGHT: (none)
 * @XDG_TOPLEVEL_RESIZE_EDGE_TOP_RIGHT: (none)
 * @XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_RIGHT: (none)
 *
 * These values are used to indicate which edge of a surface is being
 * dragged in a resize operation.
 */
enum xdg_toplevel_resize_edge {
	XDG_TOPLEVEL_RESIZE_EDGE_NONE = 0,
	XDG_TOPLEVEL_RESIZE_EDGE_TOP = 1,
	XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM = 2,
	XDG_TOPLEVEL_RESIZE_EDGE_LEFT = 4,
	XDG_TOPLEVEL_RESIZE_EDGE_TOP_LEFT = 5,
	XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_LEFT = 6,
	XDG_TOPLEVEL_RESIZE_EDGE_RIGHT = 8,
	XDG_TOPLEVEL_RESIZE_EDGE_TOP_RIGHT = 9,
	XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_RIGHT = 10,
};
#endif /* XDG_TOPLEVEL_RESIZE_EDGE_ENUM */

#ifndef XDG_TOPLEVEL_STATE_ENUM
#define XDG_TOPLEVEL_STATE_ENUM
/**
 * xdg_toplevel_state - surface repaint is suspended
 * @XDG_TOPLEVEL_STATE_MAXIMIZED: the surface is maximized
 * @XDG_TOPLEVEL_STATE_FULLSCREEN: the surface is fullscreen
 * @XDG_TOPLEVEL_STATE_RESIZING: the surface is being resized
 * @XDG_TOPLEVEL_STATE_ACTIVATED: the surface is now activated
 * @XDG_TOPLEVEL_STATE_TILED_LEFT: (none)
 * @XDG_TOPLEVEL_STATE_TILED_RIGHT: (none)
 * @XDG_TOPLEVEL_STATE_TILED_TOP: (none)
 * @XDG_TOPLEVEL_STATE_TILED_BOTTOM: (none)
 * @XDG_TOPLEVEL_STATE_SUSPENDED: (none)
 *
 * The surface is currently not ordinarily being repainted; for example
 * because its content is occluded by another window, or its outputs are
 * switched off due to screen locking.
 */
enum xdg_toplevel_state {
	XDG_TOPLEVEL_STATE_MAXIMIZED = 1,
	XDG_TOPLEVEL_STATE_FULLSCREEN = 2,
	XDG_TOPLEVEL_STATE_RESIZING = 3,
	XDG_TOPLEVEL_STATE_ACTIVATED = 4,
	XDG_TOPLEVEL_STATE_TILED_LEFT = 5,
	XDG_TOPLEVEL_STATE_TILED_RIGHT = 6,
	XDG_TOPLEVEL_STATE_TILED_TOP = 7,
	XDG_TOPLEVEL_STATE_TILED_BOTTOM = 8,
	XDG_TOPLEVEL_STATE_SUSPENDED = 9,
};
#endif /* XDG_TOPLEVEL_STATE_ENUM */

#ifndef XDG_TOPLEVEL_WM_CAPABILITIES_ENUM
#define XDG_TOPLEVEL_WM_CAPABILITIES_ENUM
enum xdg_toplevel_wm_capabilities {
	XDG_TOPLEVEL_WM_CAPABILITIES_WINDOW_MENU = 1,
	XDG_TOPLEVEL_WM_CAPABILITIES_MAXIMIZE = 2,
	XDG_TOPLEVEL_WM_CAPABILITIES_FULLSCREEN = 3,
	XDG_TOPLEVEL_WM_CAPABILITIES_MINIMIZE = 4,
};
#endif /* XDG_TOPLEVEL_WM_CAPABILITIES_ENUM */

/**
 * xdg_toplevel - toplevel surface
 * @configure: suggest a surface change
 * @close: surface wants to be closed
 * @configure_bounds: recommended window geometry bounds
 * @wm_capabilities: compositor capabilities
 *
 * This interface defines an xdg_surface role which allows a surface to,
 * among other things, set window-like properties such as maximize,
 * fullscreen, and minimize, set application-specific metadata like title
 * and id, and well as trigger user interactive operations such as
 * interactive resize and move.
 *
 * A xdg_toplevel by default is responsible for providing the full intended
 * visual representation of the toplevel, which depending on the window
 * state, may mean things like a title bar, window controls and drop
 * shadow.
 *
 * Unmapping an xdg_toplevel means that the surface cannot be shown by the
 * compositor until it is explicitly mapped again. All active operations
 * (e.g., move, resize) are canceled and all attributes (e.g. title, state,
 * stacking, ...) are discarded for an xdg_toplevel surface when it is
 * unmapped. The xdg_toplevel returns to the state it had right after
 * xdg_surface.get_toplevel. The client can re-map the toplevel by
 * perfoming a commit without any buffer attached, waiting for a configure
 * event and handling it as usual (see xdg_surface description).
 *
 * Attaching a null buffer to a toplevel unmaps the surface.
 */
struct xdg_toplevel_listener {
	/**
	 * configure - suggest a surface change
	 * @width: (none)
	 * @height: (none)
	 * @states: (none)
	 *
	 * This configure event asks the client to resize its toplevel
	 * surface or to change its state. The configured state should not
	 * be applied immediately. See xdg_surface.configure for details.
	 *
	 * The width and height arguments specify a hint to the window
	 * about how its surface should be resized in window geometry
	 * coordinates. See set_window_geometry.
	 *
	 * If the width or height arguments are zero, it means the client
	 * should decide its own window dimension. This may happen when the
	 * compositor needs to configure the state of the surface but
	 * doesn't have any information about any previous or expected
	 * dimension.
	 *
	 * The states listed in the event specify how the width/height
	 * arguments should be interpreted, and possibly how it should be
	 * drawn.
	 *
	 * Clients must send an ack_configure in response to this event.
	 * See xdg_surface.configure and xdg_surface.ack_configure for
	 * details.
	 */
	void (*configure)(void *data,
			  struct xdg_toplevel *xdg_toplevel,
			  int32_t width,
			  int32_t height,
			  struct wl_array *states);
	/**
	 * close - surface wants to be closed
	 *
	 * The close event is sent by the compositor when the user wants
	 * the surface to be closed. This should be equivalent to the user
	 * clicking the close button in client-side decorations, if your
	 * application has any.
	 *
	 * This is only a request that the user intends to close the
	 * window. The client may choose to ignore this request, or show a
	 * dialog to ask the user to save their data, etc.
	 */
	void (*close)(void *data,
		      struct xdg_toplevel *xdg_toplevel);
	/**
	 * configure_bounds - recommended window geometry bounds
	 * @width: (none)
	 * @height: (none)
	 *
	 * The configure_bounds event may be sent prior to a
	 * xdg_toplevel.configure event to communicate the bounds a window
	 * geometry size is recommended to constrain to.
	 *
	 * The passed width and height are in surface coordinate space. If
	 * width and height are 0, it means bounds is unknown and
	 * equivalent to as if no configure_bounds event was ever sent for
	 * this surface.
	 *
	 * The bounds can for example correspond to the size of a monitor
	 * excluding any panels or other shell components, so that a
	 * surface isn't created in a way that it cannot fit.
	 *
	 * The bounds may change at any point, and in such a case, a new
	 * xdg_toplevel.configure_bounds will be sent, followed by
	 * xdg_toplevel.configure and xdg_surface.configure.
	 * @since: 4
	 */
	void (*configure_bounds)(void *data,
				 struct xdg_toplevel *xdg_toplevel,
				 int32_t width,
				 int32_t height);
	/**
	 * wm_capabilities - compositor capabilities
	 * @capabilities: array of 32-bit capabilities
	 *
	 * This event advertises the capabilities supported by the
	 * compositor. If a capability isn't supported, clients should hide
	 * or disable the UI elements that expose this functionality. For
	 * instance, if the compositor doesn't advertise support for
	 * minimized toplevels, a button triggering the set_minimized
	 * request should not be displayed.
	 *
	 * The compositor will ignore requests it doesn't support. For
	 * instance, a compositor which doesn't advertise support for
	 * minimized will ignore set_minimized requests.
	 *
	 * Compositors must send this event once before the first
	 * xdg_surface.configure event. When the capabilities change,
	 * compositors must send this event again and then send an
	 * xdg_surface.configure event.
	 *
	 * The configured state should not be applied immediately. See
	 * xdg_surface.configure for details.
	 *
	 * The capabilities are sent as an array of 32-bit unsigned
	 * integers in native endianness.
	 * @since: 5
	 */
	void (*wm_capabilities)(void *data,
				struct xdg_toplevel *xdg_toplevel,
				struct wl_array *capabilities);
};

static inline int
xdg_toplevel_add_listener(struct xdg_toplevel *xdg_toplevel,
			  const struct xdg_toplevel_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) xdg_toplevel,
				     (void (**)(void)) listener, data);
}

#define XDG_TOPLEVEL_DESTROY	0
#define XDG_TOPLEVEL_SET_PARENT	1
#define XDG_TOPLEVEL_SET_TITLE	2
#define XDG_TOPLEVEL_SET_APP_ID	3
#define XDG_TOPLEVEL_SHOW_WINDOW_MENU	4
#define XDG_TOPLEVEL_MOVE	5
#define XDG_TOPLEVEL_RESIZE	6
#define XDG_TOPLEVEL_SET_MAX_SIZE	7
#define XDG_TOPLEVEL_SET_MIN_SIZE	8
#define XDG_TOPLEVEL_SET_MAXIMIZED	9
#define XDG_TOPLEVEL_UNSET_MAXIMIZED	10
#define XDG_TOPLEVEL_SET_FULLSCREEN	11
#define XDG_TOPLEVEL_UNSET_FULLSCREEN	12
#define XDG_TOPLEVEL_SET_MINIMIZED	13

static inline void
xdg_toplevel_set_user_data(struct xdg_toplevel *xdg_toplevel, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) xdg_toplevel, user_data);
}

static inline void *
xdg_toplevel_get_user_data(struct xdg_toplevel *xdg_toplevel)
{
	return wl_proxy_get_user_data((struct wl_proxy *) xdg_toplevel);
}

static inline void
xdg_toplevel_destroy(struct xdg_toplevel *xdg_toplevel)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) xdg_toplevel);
}

static inline void
xdg_toplevel_set_parent(struct xdg_toplevel *xdg_toplevel, struct xdg_toplevel *parent)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_SET_PARENT, parent);
}

static inline void
xdg_toplevel_set_title(struct xdg_toplevel *xdg_toplevel, const char *title)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_SET_TITLE, title);
}

static inline void
xdg_toplevel_set_app_id(struct xdg_toplevel *xdg_toplevel, const char *app_id)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_SET_APP_ID, app_id);
}

static inline void
xdg_toplevel_show_window_menu(struct xdg_toplevel *xdg_toplevel, struct wl_seat *seat, uint32_t serial, int32_t x, int32_t y)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_SHOW_WINDOW_MENU, seat, serial, x, y);
}

static inline void
xdg_toplevel_move(struct xdg_toplevel *xdg_toplevel, struct wl_seat *seat, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_MOVE, seat, serial);
}

static inline void
xdg_toplevel_resize(struct xdg_toplevel *xdg_toplevel, struct wl_seat *seat, uint32_t serial, uint32_t edges)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_RESIZE, seat, serial, edges);
}

static inline void
xdg_toplevel_set_max_size(struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_SET_MAX_SIZE, width, height);
}

static inline void
xdg_toplevel_set_min_size(struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_SET_MIN_SIZE, width, height);
}

static inline void
xdg_toplevel_set_maximized(struct xdg_toplevel *xdg_toplevel)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_SET_MAXIMIZED);
}

static inline void
xdg_toplevel_unset_maximized(struct xdg_toplevel *xdg_toplevel)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_UNSET_MAXIMIZED);
}

static inline void
xdg_toplevel_set_fullscreen(struct xdg_toplevel *xdg_toplevel, struct wl_output *output)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_SET_FULLSCREEN, output);
}

static inline void
xdg_toplevel_unset_fullscreen(struct xdg_toplevel *xdg_toplevel)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_UNSET_FULLSCREEN);
}

static inline void
xdg_toplevel_set_minimized(struct xdg_toplevel *xdg_toplevel)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_SET_MINIMIZED);
}

#ifndef XDG_POPUP_ERROR_ENUM
#define XDG_POPUP_ERROR_ENUM
enum xdg_popup_error {
	XDG_POPUP_ERROR_INVALID_GRAB = 0,
};
#endif /* XDG_POPUP_ERROR_ENUM */

/**
 * xdg_popup - short-lived, popup surfaces for menus
 * @configure: configure the popup surface
 * @popup_done: popup interaction is done
 * @repositioned: signal the completion of a repositioned request
 *
 * A popup surface is a short-lived, temporary surface. It can be used to
 * implement for example menus, popovers, tooltips and other similar user
 * interface concepts.
 *
 * A popup can be made to take an explicit grab. See xdg_popup.grab for
 * details.
 *
 * When the popup is dismissed, a popup_done event will be sent out, and at
 * the same time the surface will be unmapped. See the xdg_popup.popup_done
 * event for details.
 *
 * Explicitly destroying the xdg_popup object will also dismiss the popup
 * and unmap the surface. Clients that want to dismiss the popup when
 * another surface of their own is clicked should dismiss the popup using
 * the destroy request.
 *
 * A newly created xdg_popup will be stacked on top of all previously
 * created xdg_popup surfaces associated with the same xdg_toplevel.
 *
 * The parent of an xdg_popup must be mapped (see the xdg_surface
 * description) before the xdg_popup itself.
 *
 * The client must call wl_surface.commit on the corresponding wl_surface
 * for the xdg_popup state to take effect.
 */
struct xdg_popup_listener {
	/**
	 * configure - configure the popup surface
	 * @x: x position relative to parent surface window geometry
	 * @y: y position relative to parent surface window geometry
	 * @width: window geometry width
	 * @height: window geometry height
	 *
	 * This event asks the popup surface to configure itself given
	 * the configuration. The configured state should not be applied
	 * immediately. See xdg_surface.configure for details.
	 *
	 * The x and y arguments represent the position the popup was
	 * placed at given the xdg_positioner rule, relative to the upper
	 * left corner of the window geometry of the parent surface.
	 *
	 * For version 2 or older, the configure event for an xdg_popup is
	 * only ever sent once for the initial configuration. Starting with
	 * version 3, it may be sent again if the popup is setup with an
	 * xdg_positioner with set_reactive requested, or in response to
	 * xdg_popup.reposition requests.
	 */
	void (*configure)(void *data,
			  struct xdg_popup *xdg_popup,
			  int32_t x,
			  int32_t y,
			  int32_t width,
			  int32_t height);
	/**
	 * popup_done - popup interaction is done
	 *
	 * The popup_done event is sent out when a popup is dismissed by
	 * the compositor. The client should destroy the xdg_popup object
	 * at this point.
	 */
	void (*popup_done)(void *data,
			   struct xdg_popup *xdg_popup);
	/**
	 * repositioned - signal the completion of a repositioned request
	 * @token: reposition request token
	 *
	 * The repositioned event is sent as part of a popup
	 * configuration sequence, together with xdg_popup.configure and
	 * lastly xdg_surface.configure to notify the completion of a
	 * reposition request.
	 *
	 * The repositioned event is to notify about the completion of a
	 * xdg_popup.reposition request. The token argument is the token
	 * passed in the xdg_popup.reposition request.
	 *
	 * Immediately after this event is emitted, xdg_popup.configure and
	 * xdg_surface.configure will be sent with the updated size and
	 * position, as well as a new configure serial.
	 *
	 * The client should optionally update the content of the popup,
	 * but must acknowledge the new popup configuration for the new
	 * position to take effect. See xdg_surface.ack_configure for
	 * details.
	 * @since: 3
	 */
	void (*repositioned)(void *data,
			     struct xdg_popup *xdg_popup,
			     uint32_t token);
};

static inline int
xdg_popup_add_listener(struct xdg_popup *xdg_popup,
		       const struct xdg_popup_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) xdg_popup,
				     (void (**)(void)) listener, data);
}

#define XDG_POPUP_DESTROY	0
#define XDG_POPUP_GRAB	1
#define XDG_POPUP_REPOSITION	2

static inline void
xdg_popup_set_user_data(struct xdg_popup *xdg_popup, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) xdg_popup, user_data);
}

static inline void *
xdg_popup_get_user_data(struct xdg_popup *xdg_popup)
{
	return wl_proxy_get_user_data((struct wl_proxy *) xdg_popup);
}

static inline void
xdg_popup_destroy(struct xdg_popup *xdg_popup)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_popup,
			 XDG_POPUP_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) xdg_popup);
}

static inline void
xdg_popup_grab(struct xdg_popup *xdg_popup, struct wl_seat *seat, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_popup,
			 XDG_POPUP_GRAB, seat, serial);
}

static inline void
xdg_popup_reposition(struct xdg_popup *xdg_popup, struct xdg_positioner *positioner, uint32_t token)
{
	wl_proxy_marshal((struct wl_proxy *) xdg_popup,
			 XDG_POPUP_REPOSITION, positioner, token);
}

#ifdef  __cplusplus
}
#endif

#endif
