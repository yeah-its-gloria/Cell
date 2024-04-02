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

#ifndef TABLET_UNSTABLE_V2_CLIENT_PROTOCOL_H
#define TABLET_UNSTABLE_V2_CLIENT_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

struct wl_client;
struct wl_resource;

struct zwp_tablet_manager_v2;
struct zwp_tablet_seat_v2;
struct zwp_tablet_tool_v2;
struct zwp_tablet_v2;
struct zwp_tablet_pad_ring_v2;
struct zwp_tablet_pad_strip_v2;
struct zwp_tablet_pad_group_v2;
struct zwp_tablet_pad_v2;

extern const struct wl_interface zwp_tablet_manager_v2_interface;
extern const struct wl_interface zwp_tablet_seat_v2_interface;
extern const struct wl_interface zwp_tablet_tool_v2_interface;
extern const struct wl_interface zwp_tablet_v2_interface;
extern const struct wl_interface zwp_tablet_pad_ring_v2_interface;
extern const struct wl_interface zwp_tablet_pad_strip_v2_interface;
extern const struct wl_interface zwp_tablet_pad_group_v2_interface;
extern const struct wl_interface zwp_tablet_pad_v2_interface;

#define ZWP_TABLET_MANAGER_V2_GET_TABLET_SEAT	0
#define ZWP_TABLET_MANAGER_V2_DESTROY	1

static inline void
zwp_tablet_manager_v2_set_user_data(struct zwp_tablet_manager_v2 *zwp_tablet_manager_v2, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_tablet_manager_v2, user_data);
}

static inline void *
zwp_tablet_manager_v2_get_user_data(struct zwp_tablet_manager_v2 *zwp_tablet_manager_v2)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_tablet_manager_v2);
}

static inline struct zwp_tablet_seat_v2 *
zwp_tablet_manager_v2_get_tablet_seat(struct zwp_tablet_manager_v2 *zwp_tablet_manager_v2, struct wl_seat *seat)
{
	struct wl_proxy *tablet_seat;

	tablet_seat = wl_proxy_create((struct wl_proxy *) zwp_tablet_manager_v2,
			     &zwp_tablet_seat_v2_interface);
	if (!tablet_seat)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_manager_v2,
			 ZWP_TABLET_MANAGER_V2_GET_TABLET_SEAT, tablet_seat, seat);

	return (struct zwp_tablet_seat_v2 *) tablet_seat;
}

static inline void
zwp_tablet_manager_v2_destroy(struct zwp_tablet_manager_v2 *zwp_tablet_manager_v2)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_manager_v2,
			 ZWP_TABLET_MANAGER_V2_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_tablet_manager_v2);
}

/**
 * zwp_tablet_seat_v2 - controller object for graphic tablet devices of a
 *	seat
 * @tablet_added: new device notification
 * @tool_added: a new tool has been used with a tablet
 * @pad_added: new pad notification
 *
 * An object that provides access to the graphics tablets available on
 * this seat. After binding to this interface, the compositor sends a set
 * of wp_tablet_seat.tablet_added and wp_tablet_seat.tool_added events.
 */
struct zwp_tablet_seat_v2_listener {
	/**
	 * tablet_added - new device notification
	 * @id: the newly added graphics tablet
	 *
	 * This event is sent whenever a new tablet becomes available on
	 * this seat. This event only provides the object id of the tablet,
	 * any static information about the tablet (device name, vid/pid,
	 * etc.) is sent through the wp_tablet interface.
	 */
	void (*tablet_added)(void *data,
			     struct zwp_tablet_seat_v2 *zwp_tablet_seat_v2,
			     struct zwp_tablet_v2 *id);
	/**
	 * tool_added - a new tool has been used with a tablet
	 * @id: the newly added tablet tool
	 *
	 * This event is sent whenever a tool that has not previously
	 * been used with a tablet comes into use. This event only provides
	 * the object id of the tool; any static information about the tool
	 * (capabilities, type, etc.) is sent through the wp_tablet_tool
	 * interface.
	 */
	void (*tool_added)(void *data,
			   struct zwp_tablet_seat_v2 *zwp_tablet_seat_v2,
			   struct zwp_tablet_tool_v2 *id);
	/**
	 * pad_added - new pad notification
	 * @id: the newly added pad
	 *
	 * This event is sent whenever a new pad is known to the system.
	 * Typically, pads are physically attached to tablets and a
	 * pad_added event is sent immediately after the
	 * wp_tablet_seat.tablet_added. However, some standalone pad
	 * devices logically attach to tablets at runtime, and the client
	 * must wait for wp_tablet_pad.enter to know the tablet a pad is
	 * attached to.
	 *
	 * This event only provides the object id of the pad. All further
	 * features (buttons, strips, rings) are sent through the
	 * wp_tablet_pad interface.
	 */
	void (*pad_added)(void *data,
			  struct zwp_tablet_seat_v2 *zwp_tablet_seat_v2,
			  struct zwp_tablet_pad_v2 *id);
};

static inline int
zwp_tablet_seat_v2_add_listener(struct zwp_tablet_seat_v2 *zwp_tablet_seat_v2,
				const struct zwp_tablet_seat_v2_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) zwp_tablet_seat_v2,
				     (void (**)(void)) listener, data);
}

#define ZWP_TABLET_SEAT_V2_DESTROY	0

static inline void
zwp_tablet_seat_v2_set_user_data(struct zwp_tablet_seat_v2 *zwp_tablet_seat_v2, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_tablet_seat_v2, user_data);
}

static inline void *
zwp_tablet_seat_v2_get_user_data(struct zwp_tablet_seat_v2 *zwp_tablet_seat_v2)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_tablet_seat_v2);
}

static inline void
zwp_tablet_seat_v2_destroy(struct zwp_tablet_seat_v2 *zwp_tablet_seat_v2)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_seat_v2,
			 ZWP_TABLET_SEAT_V2_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_tablet_seat_v2);
}

#ifndef ZWP_TABLET_TOOL_V2_TYPE_ENUM
#define ZWP_TABLET_TOOL_V2_TYPE_ENUM
/**
 * zwp_tablet_tool_v2_type - a physical tool type
 * @ZWP_TABLET_TOOL_V2_TYPE_PEN: Pen
 * @ZWP_TABLET_TOOL_V2_TYPE_ERASER: Eraser
 * @ZWP_TABLET_TOOL_V2_TYPE_BRUSH: Brush
 * @ZWP_TABLET_TOOL_V2_TYPE_PENCIL: Pencil
 * @ZWP_TABLET_TOOL_V2_TYPE_AIRBRUSH: Airbrush
 * @ZWP_TABLET_TOOL_V2_TYPE_FINGER: Finger
 * @ZWP_TABLET_TOOL_V2_TYPE_MOUSE: Mouse
 * @ZWP_TABLET_TOOL_V2_TYPE_LENS: Lens
 *
 * Describes the physical type of a tool. The physical type of a tool
 * generally defines its base usage.
 *
 * The mouse tool represents a mouse-shaped tool that is not a relative
 * device but bound to the tablet's surface, providing absolute
 * coordinates.
 *
 * The lens tool is a mouse-shaped tool with an attached lens to provide
 * precision focus.
 */
enum zwp_tablet_tool_v2_type {
	ZWP_TABLET_TOOL_V2_TYPE_PEN = 0x140,
	ZWP_TABLET_TOOL_V2_TYPE_ERASER = 0x141,
	ZWP_TABLET_TOOL_V2_TYPE_BRUSH = 0x142,
	ZWP_TABLET_TOOL_V2_TYPE_PENCIL = 0x143,
	ZWP_TABLET_TOOL_V2_TYPE_AIRBRUSH = 0x144,
	ZWP_TABLET_TOOL_V2_TYPE_FINGER = 0x145,
	ZWP_TABLET_TOOL_V2_TYPE_MOUSE = 0x146,
	ZWP_TABLET_TOOL_V2_TYPE_LENS = 0x147,
};
#endif /* ZWP_TABLET_TOOL_V2_TYPE_ENUM */

#ifndef ZWP_TABLET_TOOL_V2_CAPABILITY_ENUM
#define ZWP_TABLET_TOOL_V2_CAPABILITY_ENUM
/**
 * zwp_tablet_tool_v2_capability - capability flags for a tool
 * @ZWP_TABLET_TOOL_V2_CAPABILITY_TILT: Tilt axes
 * @ZWP_TABLET_TOOL_V2_CAPABILITY_PRESSURE: Pressure axis
 * @ZWP_TABLET_TOOL_V2_CAPABILITY_DISTANCE: Distance axis
 * @ZWP_TABLET_TOOL_V2_CAPABILITY_ROTATION: Z-rotation axis
 * @ZWP_TABLET_TOOL_V2_CAPABILITY_SLIDER: Slider axis
 * @ZWP_TABLET_TOOL_V2_CAPABILITY_WHEEL: Wheel axis
 *
 * Describes extra capabilities on a tablet.
 *
 * Any tool must provide x and y values, extra axes are device-specific.
 */
enum zwp_tablet_tool_v2_capability {
	ZWP_TABLET_TOOL_V2_CAPABILITY_TILT = 1,
	ZWP_TABLET_TOOL_V2_CAPABILITY_PRESSURE = 2,
	ZWP_TABLET_TOOL_V2_CAPABILITY_DISTANCE = 3,
	ZWP_TABLET_TOOL_V2_CAPABILITY_ROTATION = 4,
	ZWP_TABLET_TOOL_V2_CAPABILITY_SLIDER = 5,
	ZWP_TABLET_TOOL_V2_CAPABILITY_WHEEL = 6,
};
#endif /* ZWP_TABLET_TOOL_V2_CAPABILITY_ENUM */

#ifndef ZWP_TABLET_TOOL_V2_BUTTON_STATE_ENUM
#define ZWP_TABLET_TOOL_V2_BUTTON_STATE_ENUM
/**
 * zwp_tablet_tool_v2_button_state - physical button state
 * @ZWP_TABLET_TOOL_V2_BUTTON_STATE_RELEASED: button is not pressed
 * @ZWP_TABLET_TOOL_V2_BUTTON_STATE_PRESSED: button is pressed
 *
 * Describes the physical state of a button that produced the button
 * event.
 */
enum zwp_tablet_tool_v2_button_state {
	ZWP_TABLET_TOOL_V2_BUTTON_STATE_RELEASED = 0,
	ZWP_TABLET_TOOL_V2_BUTTON_STATE_PRESSED = 1,
};
#endif /* ZWP_TABLET_TOOL_V2_BUTTON_STATE_ENUM */

#ifndef ZWP_TABLET_TOOL_V2_ERROR_ENUM
#define ZWP_TABLET_TOOL_V2_ERROR_ENUM
enum zwp_tablet_tool_v2_error {
	ZWP_TABLET_TOOL_V2_ERROR_ROLE = 0,
};
#endif /* ZWP_TABLET_TOOL_V2_ERROR_ENUM */

/**
 * zwp_tablet_tool_v2 - a physical tablet tool
 * @type: tool type
 * @hardware_serial: unique hardware serial number of the tool
 * @hardware_id_wacom: hardware id notification in Wacom's format
 * @capability: tool capability notification
 * @done: tool description events sequence complete
 * @removed: tool removed
 * @proximity_in: proximity in event
 * @proximity_out: proximity out event
 * @down: tablet tool is making contact
 * @up: tablet tool is no longer making contact
 * @motion: motion event
 * @pressure: pressure change event
 * @distance: distance change event
 * @tilt: tilt change event
 * @rotation: z-rotation change event
 * @slider: Slider position change event
 * @wheel: Wheel delta event
 * @button: button event
 * @frame: frame event
 *
 * An object that represents a physical tool that has been, or is
 * currently in use with a tablet in this seat. Each wp_tablet_tool object
 * stays valid until the client destroys it; the compositor reuses the
 * wp_tablet_tool object to indicate that the object's respective physical
 * tool has come into proximity of a tablet again.
 *
 * A wp_tablet_tool object's relation to a physical tool depends on the
 * tablet's ability to report serial numbers. If the tablet supports this
 * capability, then the object represents a specific physical tool and can
 * be identified even when used on multiple tablets.
 *
 * A tablet tool has a number of static characteristics, e.g. tool type,
 * hardware_serial and capabilities. These capabilities are sent in an
 * event sequence after the wp_tablet_seat.tool_added event before any
 * actual events from this tool. This initial event sequence is terminated
 * by a wp_tablet_tool.done event.
 *
 * Tablet tool events are grouped by wp_tablet_tool.frame events. Any
 * events received before a wp_tablet_tool.frame event should be considered
 * part of the same hardware state change.
 */
struct zwp_tablet_tool_v2_listener {
	/**
	 * type - tool type
	 * @tool_type: the physical tool type
	 *
	 * The tool type is the high-level type of the tool and usually
	 * decides the interaction expected from this tool.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet_tool.done event.
	 */
	void (*type)(void *data,
		     struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
		     uint32_t tool_type);
	/**
	 * hardware_serial - unique hardware serial number of the tool
	 * @hardware_serial_hi: the unique serial number of the tool,
	 *	most significant bits
	 * @hardware_serial_lo: the unique serial number of the tool,
	 *	least significant bits
	 *
	 * If the physical tool can be identified by a unique 64-bit
	 * serial number, this event notifies the client of this serial
	 * number.
	 *
	 * If multiple tablets are available in the same seat and the tool
	 * is uniquely identifiable by the serial number, that tool may
	 * move between tablets.
	 *
	 * Otherwise, if the tool has no serial number and this event is
	 * missing, the tool is tied to the tablet it first comes into
	 * proximity with. Even if the physical tool is used on multiple
	 * tablets, separate wp_tablet_tool objects will be created, one
	 * per tablet.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet_tool.done event.
	 */
	void (*hardware_serial)(void *data,
				struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
				uint32_t hardware_serial_hi,
				uint32_t hardware_serial_lo);
	/**
	 * hardware_id_wacom - hardware id notification in Wacom's format
	 * @hardware_id_hi: the hardware id, most significant bits
	 * @hardware_id_lo: the hardware id, least significant bits
	 *
	 * This event notifies the client of a hardware id available on
	 * this tool.
	 *
	 * The hardware id is a device-specific 64-bit id that provides
	 * extra information about the tool in use, beyond the wl_tool.type
	 * enumeration. The format of the id is specific to tablets made by
	 * Wacom Inc. For example, the hardware id of a Wacom Grip Pen (a
	 * stylus) is 0x802.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet_tool.done event.
	 */
	void (*hardware_id_wacom)(void *data,
				  struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
				  uint32_t hardware_id_hi,
				  uint32_t hardware_id_lo);
	/**
	 * capability - tool capability notification
	 * @capability: the capability
	 *
	 * This event notifies the client of any capabilities of this
	 * tool, beyond the main set of x/y axes and tip up/down detection.
	 *
	 * One event is sent for each extra capability available on this
	 * tool.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet_tool.done event.
	 */
	void (*capability)(void *data,
			   struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
			   uint32_t capability);
	/**
	 * done - tool description events sequence complete
	 *
	 * This event signals the end of the initial burst of descriptive
	 * events. A client may consider the static description of the tool
	 * to be complete and finalize initialization of the tool.
	 */
	void (*done)(void *data,
		     struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2);
	/**
	 * removed - tool removed
	 *
	 * This event is sent when the tool is removed from the system
	 * and will send no further events. Should the physical tool come
	 * back into proximity later, a new wp_tablet_tool object will be
	 * created.
	 *
	 * It is compositor-dependent when a tool is removed. A compositor
	 * may remove a tool on proximity out, tablet removal or any other
	 * reason. A compositor may also keep a tool alive until shutdown.
	 *
	 * If the tool is currently in proximity, a proximity_out event
	 * will be sent before the removed event. See
	 * wp_tablet_tool.proximity_out for the handling of any buttons
	 * logically down.
	 *
	 * When this event is received, the client must
	 * wp_tablet_tool.destroy the object.
	 */
	void (*removed)(void *data,
			struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2);
	/**
	 * proximity_in - proximity in event
	 * @serial: (none)
	 * @tablet: The tablet the tool is in proximity of
	 * @surface: The current surface the tablet tool is over
	 *
	 * Notification that this tool is focused on a certain surface.
	 *
	 * This event can be received when the tool has moved from one
	 * surface to another, or when the tool has come back into
	 * proximity above the surface.
	 *
	 * If any button is logically down when the tool comes into
	 * proximity, the respective button event is sent after the
	 * proximity_in event but within the same frame as the proximity_in
	 * event.
	 */
	void (*proximity_in)(void *data,
			     struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
			     uint32_t serial,
			     struct zwp_tablet_v2 *tablet,
			     struct wl_surface *surface);
	/**
	 * proximity_out - proximity out event
	 *
	 * Notification that this tool has either left proximity, or is
	 * no longer focused on a certain surface.
	 *
	 * When the tablet tool leaves proximity of the tablet, button
	 * release events are sent for each button that was held down at
	 * the time of leaving proximity. These events are sent before the
	 * proximity_out event but within the same wp_tablet.frame.
	 *
	 * If the tool stays within proximity of the tablet, but the focus
	 * changes from one surface to another, a button release event may
	 * not be sent until the button is actually released or the tool
	 * leaves the proximity of the tablet.
	 */
	void (*proximity_out)(void *data,
			      struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2);
	/**
	 * down - tablet tool is making contact
	 * @serial: (none)
	 *
	 * Sent whenever the tablet tool comes in contact with the
	 * surface of the tablet.
	 *
	 * If the tool is already in contact with the tablet when entering
	 * the input region, the client owning said region will receive a
	 * wp_tablet.proximity_in event, followed by a wp_tablet.down event
	 * and a wp_tablet.frame event.
	 *
	 * Note that this event describes logical contact, not physical
	 * contact. On some devices, a compositor may not consider a tool
	 * in logical contact until a minimum physical pressure threshold
	 * is exceeded.
	 */
	void (*down)(void *data,
		     struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
		     uint32_t serial);
	/**
	 * up - tablet tool is no longer making contact
	 *
	 * Sent whenever the tablet tool stops making contact with the
	 * surface of the tablet, or when the tablet tool moves out of the
	 * input region and the compositor grab (if any) is dismissed.
	 *
	 * If the tablet tool moves out of the input region while in
	 * contact with the surface of the tablet and the compositor does
	 * not have an ongoing grab on the surface, the client owning said
	 * region will receive a wp_tablet.up event, followed by a
	 * wp_tablet.proximity_out event and a wp_tablet.frame event. If
	 * the compositor has an ongoing grab on this device, this event
	 * sequence is sent whenever the grab is dismissed in the future.
	 *
	 * Note that this event describes logical contact, not physical
	 * contact. On some devices, a compositor may not consider a tool
	 * out of logical contact until physical pressure falls below a
	 * specific threshold.
	 */
	void (*up)(void *data,
		   struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2);
	/**
	 * motion - motion event
	 * @x: surface-local x coordinate
	 * @y: surface-local y coordinate
	 *
	 * Sent whenever a tablet tool moves.
	 */
	void (*motion)(void *data,
		       struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
		       wl_fixed_t x,
		       wl_fixed_t y);
	/**
	 * pressure - pressure change event
	 * @pressure: The current pressure value
	 *
	 * Sent whenever the pressure axis on a tool changes. The value
	 * of this event is normalized to a value between 0 and 65535.
	 *
	 * Note that pressure may be nonzero even when a tool is not in
	 * logical contact. See the down and up events for more details.
	 */
	void (*pressure)(void *data,
			 struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
			 uint32_t pressure);
	/**
	 * distance - distance change event
	 * @distance: The current distance value
	 *
	 * Sent whenever the distance axis on a tool changes. The value
	 * of this event is normalized to a value between 0 and 65535.
	 *
	 * Note that distance may be nonzero even when a tool is not in
	 * logical contact. See the down and up events for more details.
	 */
	void (*distance)(void *data,
			 struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
			 uint32_t distance);
	/**
	 * tilt - tilt change event
	 * @tilt_x: The current value of the X tilt axis
	 * @tilt_y: The current value of the Y tilt axis
	 *
	 * Sent whenever one or both of the tilt axes on a tool change.
	 * Each tilt value is in degrees, relative to the z-axis of the
	 * tablet. The angle is positive when the top of a tool tilts along
	 * the positive x or y axis.
	 */
	void (*tilt)(void *data,
		     struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
		     wl_fixed_t tilt_x,
		     wl_fixed_t tilt_y);
	/**
	 * rotation - z-rotation change event
	 * @degrees: The current rotation of the Z axis
	 *
	 * Sent whenever the z-rotation axis on the tool changes. The
	 * rotation value is in degrees clockwise from the tool's logical
	 * neutral position.
	 */
	void (*rotation)(void *data,
			 struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
			 wl_fixed_t degrees);
	/**
	 * slider - Slider position change event
	 * @position: The current position of slider
	 *
	 * Sent whenever the slider position on the tool changes. The
	 * value is normalized between -65535 and 65535, with 0 as the
	 * logical neutral position of the slider.
	 *
	 * The slider is available on e.g. the Wacom Airbrush tool.
	 */
	void (*slider)(void *data,
		       struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
		       int32_t position);
	/**
	 * wheel - Wheel delta event
	 * @degrees: The wheel delta in degrees
	 * @clicks: The wheel delta in discrete clicks
	 *
	 * Sent whenever the wheel on the tool emits an event. This event
	 * contains two values for the same axis change. The degrees value
	 * is in the same orientation as the wl_pointer.vertical_scroll
	 * axis. The clicks value is in discrete logical clicks of the
	 * mouse wheel. This value may be zero if the movement of the wheel
	 * was less than one logical click.
	 *
	 * Clients should choose either value and avoid mixing degrees and
	 * clicks. The compositor may accumulate values smaller than a
	 * logical click and emulate click events when a certain threshold
	 * is met. Thus, wl_tablet_tool.wheel events with non-zero clicks
	 * values may have different degrees values.
	 */
	void (*wheel)(void *data,
		      struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
		      wl_fixed_t degrees,
		      int32_t clicks);
	/**
	 * button - button event
	 * @serial: (none)
	 * @button: The button whose state has changed
	 * @state: Whether the button was pressed or released
	 *
	 * Sent whenever a button on the tool is pressed or released.
	 *
	 * If a button is held down when the tool moves in or out of
	 * proximity, button events are generated by the compositor. See
	 * wp_tablet_tool.proximity_in and wp_tablet_tool.proximity_out for
	 * details.
	 */
	void (*button)(void *data,
		       struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
		       uint32_t serial,
		       uint32_t button,
		       uint32_t state);
	/**
	 * frame - frame event
	 * @time: The time of the event with millisecond granularity
	 *
	 * Marks the end of a series of axis and/or button updates from
	 * the tablet. The Wayland protocol requires axis updates to be
	 * sent sequentially, however all events within a frame should be
	 * considered one hardware event.
	 */
	void (*frame)(void *data,
		      struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
		      uint32_t time);
};

static inline int
zwp_tablet_tool_v2_add_listener(struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2,
				const struct zwp_tablet_tool_v2_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) zwp_tablet_tool_v2,
				     (void (**)(void)) listener, data);
}

#define ZWP_TABLET_TOOL_V2_SET_CURSOR	0
#define ZWP_TABLET_TOOL_V2_DESTROY	1

static inline void
zwp_tablet_tool_v2_set_user_data(struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_tablet_tool_v2, user_data);
}

static inline void *
zwp_tablet_tool_v2_get_user_data(struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_tablet_tool_v2);
}

static inline void
zwp_tablet_tool_v2_set_cursor(struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2, uint32_t serial, struct wl_surface *surface, int32_t hotspot_x, int32_t hotspot_y)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_tool_v2,
			 ZWP_TABLET_TOOL_V2_SET_CURSOR, serial, surface, hotspot_x, hotspot_y);
}

static inline void
zwp_tablet_tool_v2_destroy(struct zwp_tablet_tool_v2 *zwp_tablet_tool_v2)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_tool_v2,
			 ZWP_TABLET_TOOL_V2_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_tablet_tool_v2);
}

/**
 * zwp_tablet_v2 - graphics tablet device
 * @name: tablet device name
 * @id: tablet device USB vendor/product id
 * @path: path to the device
 * @done: tablet description events sequence complete
 * @removed: tablet removed event
 *
 * The wp_tablet interface represents one graphics tablet device. The
 * tablet interface itself does not generate events; all events are
 * generated by wp_tablet_tool objects when in proximity above a tablet.
 *
 * A tablet has a number of static characteristics, e.g. device name and
 * pid/vid. These capabilities are sent in an event sequence after the
 * wp_tablet_seat.tablet_added event. This initial event sequence is
 * terminated by a wp_tablet.done event.
 */
struct zwp_tablet_v2_listener {
	/**
	 * name - tablet device name
	 * @name: the device name
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet.done event.
	 */
	void (*name)(void *data,
		     struct zwp_tablet_v2 *zwp_tablet_v2,
		     const char *name);
	/**
	 * id - tablet device USB vendor/product id
	 * @vid: USB vendor id
	 * @pid: USB product id
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet.done event.
	 */
	void (*id)(void *data,
		   struct zwp_tablet_v2 *zwp_tablet_v2,
		   uint32_t vid,
		   uint32_t pid);
	/**
	 * path - path to the device
	 * @path: path to local device
	 *
	 * A system-specific device path that indicates which device is
	 * behind this wp_tablet. This information may be used to gather
	 * additional information about the device, e.g. through libwacom.
	 *
	 * A device may have more than one device path. If so, multiple
	 * wp_tablet.path events are sent. A device may be emulated and not
	 * have a device path, and in that case this event will not be
	 * sent.
	 *
	 * The format of the path is unspecified, it may be a device node,
	 * a sysfs path, or some other identifier. It is up to the client
	 * to identify the string provided.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet.done event.
	 */
	void (*path)(void *data,
		     struct zwp_tablet_v2 *zwp_tablet_v2,
		     const char *path);
	/**
	 * done - tablet description events sequence complete
	 *
	 * This event is sent immediately to signal the end of the
	 * initial burst of descriptive events. A client may consider the
	 * static description of the tablet to be complete and finalize
	 * initialization of the tablet.
	 */
	void (*done)(void *data,
		     struct zwp_tablet_v2 *zwp_tablet_v2);
	/**
	 * removed - tablet removed event
	 *
	 * Sent when the tablet has been removed from the system. When a
	 * tablet is removed, some tools may be removed.
	 *
	 * When this event is received, the client must wp_tablet.destroy
	 * the object.
	 */
	void (*removed)(void *data,
			struct zwp_tablet_v2 *zwp_tablet_v2);
};

static inline int
zwp_tablet_v2_add_listener(struct zwp_tablet_v2 *zwp_tablet_v2,
			   const struct zwp_tablet_v2_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) zwp_tablet_v2,
				     (void (**)(void)) listener, data);
}

#define ZWP_TABLET_V2_DESTROY	0

static inline void
zwp_tablet_v2_set_user_data(struct zwp_tablet_v2 *zwp_tablet_v2, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_tablet_v2, user_data);
}

static inline void *
zwp_tablet_v2_get_user_data(struct zwp_tablet_v2 *zwp_tablet_v2)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_tablet_v2);
}

static inline void
zwp_tablet_v2_destroy(struct zwp_tablet_v2 *zwp_tablet_v2)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_v2,
			 ZWP_TABLET_V2_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_tablet_v2);
}

#ifndef ZWP_TABLET_PAD_RING_V2_SOURCE_ENUM
#define ZWP_TABLET_PAD_RING_V2_SOURCE_ENUM
/**
 * zwp_tablet_pad_ring_v2_source - ring axis source
 * @ZWP_TABLET_PAD_RING_V2_SOURCE_FINGER: finger
 *
 * Describes the source types for ring events. This indicates to the
 * client how a ring event was physically generated; a client may adjust
 * the user interface accordingly. For example, events from a "finger"
 * source may trigger kinetic scrolling.
 */
enum zwp_tablet_pad_ring_v2_source {
	ZWP_TABLET_PAD_RING_V2_SOURCE_FINGER = 1,
};
#endif /* ZWP_TABLET_PAD_RING_V2_SOURCE_ENUM */

/**
 * zwp_tablet_pad_ring_v2 - pad ring
 * @source: ring event source
 * @angle: angle changed
 * @stop: interaction stopped
 * @frame: end of a ring event sequence
 *
 * A circular interaction area, such as the touch ring on the Wacom
 * Intuos Pro series tablets.
 *
 * Events on a ring are logically grouped by the wl_tablet_pad_ring.frame
 * event.
 */
struct zwp_tablet_pad_ring_v2_listener {
	/**
	 * source - ring event source
	 * @source: the event source
	 *
	 * Source information for ring events.
	 *
	 * This event does not occur on its own. It is sent before a
	 * wp_tablet_pad_ring.frame event and carries the source
	 * information for all events within that frame.
	 *
	 * The source specifies how this event was generated. If the source
	 * is wp_tablet_pad_ring.source.finger, a wp_tablet_pad_ring.stop
	 * event will be sent when the user lifts the finger off the
	 * device.
	 *
	 * This event is optional. If the source is unknown for an
	 * interaction, no event is sent.
	 */
	void (*source)(void *data,
		       struct zwp_tablet_pad_ring_v2 *zwp_tablet_pad_ring_v2,
		       uint32_t source);
	/**
	 * angle - angle changed
	 * @degrees: the current angle in degrees
	 *
	 * Sent whenever the angle on a ring changes.
	 *
	 * The angle is provided in degrees clockwise from the logical
	 * north of the ring in the pad's current rotation.
	 */
	void (*angle)(void *data,
		      struct zwp_tablet_pad_ring_v2 *zwp_tablet_pad_ring_v2,
		      wl_fixed_t degrees);
	/**
	 * stop - interaction stopped
	 *
	 * Stop notification for ring events.
	 *
	 * For some wp_tablet_pad_ring.source types, a
	 * wp_tablet_pad_ring.stop event is sent to notify a client that
	 * the interaction with the ring has terminated. This enables the
	 * client to implement kinetic scrolling. See the
	 * wp_tablet_pad_ring.source documentation for information on when
	 * this event may be generated.
	 *
	 * Any wp_tablet_pad_ring.angle events with the same source after
	 * this event should be considered as the start of a new
	 * interaction.
	 */
	void (*stop)(void *data,
		     struct zwp_tablet_pad_ring_v2 *zwp_tablet_pad_ring_v2);
	/**
	 * frame - end of a ring event sequence
	 * @time: timestamp with millisecond granularity
	 *
	 * Indicates the end of a set of ring events that logically
	 * belong together. A client is expected to accumulate the data in
	 * all events within the frame before proceeding.
	 *
	 * All wp_tablet_pad_ring events before a wp_tablet_pad_ring.frame
	 * event belong logically together. For example, on termination of
	 * a finger interaction on a ring the compositor will send a
	 * wp_tablet_pad_ring.source event, a wp_tablet_pad_ring.stop event
	 * and a wp_tablet_pad_ring.frame event.
	 *
	 * A wp_tablet_pad_ring.frame event is sent for every logical event
	 * group, even if the group only contains a single
	 * wp_tablet_pad_ring event. Specifically, a client may get a
	 * sequence: angle, frame, angle, frame, etc.
	 */
	void (*frame)(void *data,
		      struct zwp_tablet_pad_ring_v2 *zwp_tablet_pad_ring_v2,
		      uint32_t time);
};

static inline int
zwp_tablet_pad_ring_v2_add_listener(struct zwp_tablet_pad_ring_v2 *zwp_tablet_pad_ring_v2,
				    const struct zwp_tablet_pad_ring_v2_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) zwp_tablet_pad_ring_v2,
				     (void (**)(void)) listener, data);
}

#define ZWP_TABLET_PAD_RING_V2_SET_FEEDBACK	0
#define ZWP_TABLET_PAD_RING_V2_DESTROY	1

static inline void
zwp_tablet_pad_ring_v2_set_user_data(struct zwp_tablet_pad_ring_v2 *zwp_tablet_pad_ring_v2, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_tablet_pad_ring_v2, user_data);
}

static inline void *
zwp_tablet_pad_ring_v2_get_user_data(struct zwp_tablet_pad_ring_v2 *zwp_tablet_pad_ring_v2)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_tablet_pad_ring_v2);
}

static inline void
zwp_tablet_pad_ring_v2_set_feedback(struct zwp_tablet_pad_ring_v2 *zwp_tablet_pad_ring_v2, const char *description, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_pad_ring_v2,
			 ZWP_TABLET_PAD_RING_V2_SET_FEEDBACK, description, serial);
}

static inline void
zwp_tablet_pad_ring_v2_destroy(struct zwp_tablet_pad_ring_v2 *zwp_tablet_pad_ring_v2)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_pad_ring_v2,
			 ZWP_TABLET_PAD_RING_V2_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_tablet_pad_ring_v2);
}

#ifndef ZWP_TABLET_PAD_STRIP_V2_SOURCE_ENUM
#define ZWP_TABLET_PAD_STRIP_V2_SOURCE_ENUM
/**
 * zwp_tablet_pad_strip_v2_source - strip axis source
 * @ZWP_TABLET_PAD_STRIP_V2_SOURCE_FINGER: finger
 *
 * Describes the source types for strip events. This indicates to the
 * client how a strip event was physically generated; a client may adjust
 * the user interface accordingly. For example, events from a "finger"
 * source may trigger kinetic scrolling.
 */
enum zwp_tablet_pad_strip_v2_source {
	ZWP_TABLET_PAD_STRIP_V2_SOURCE_FINGER = 1,
};
#endif /* ZWP_TABLET_PAD_STRIP_V2_SOURCE_ENUM */

/**
 * zwp_tablet_pad_strip_v2 - pad strip
 * @source: strip event source
 * @position: position changed
 * @stop: interaction stopped
 * @frame: end of a strip event sequence
 *
 * A linear interaction area, such as the strips found in Wacom Cintiq
 * models.
 *
 * Events on a strip are logically grouped by the wl_tablet_pad_strip.frame
 * event.
 */
struct zwp_tablet_pad_strip_v2_listener {
	/**
	 * source - strip event source
	 * @source: the event source
	 *
	 * Source information for strip events.
	 *
	 * This event does not occur on its own. It is sent before a
	 * wp_tablet_pad_strip.frame event and carries the source
	 * information for all events within that frame.
	 *
	 * The source specifies how this event was generated. If the source
	 * is wp_tablet_pad_strip.source.finger, a wp_tablet_pad_strip.stop
	 * event will be sent when the user lifts their finger off the
	 * device.
	 *
	 * This event is optional. If the source is unknown for an
	 * interaction, no event is sent.
	 */
	void (*source)(void *data,
		       struct zwp_tablet_pad_strip_v2 *zwp_tablet_pad_strip_v2,
		       uint32_t source);
	/**
	 * position - position changed
	 * @position: the current position
	 *
	 * Sent whenever the position on a strip changes.
	 *
	 * The position is normalized to a range of [0, 65535], the 0-value
	 * represents the top-most and/or left-most position of the strip
	 * in the pad's current rotation.
	 */
	void (*position)(void *data,
			 struct zwp_tablet_pad_strip_v2 *zwp_tablet_pad_strip_v2,
			 uint32_t position);
	/**
	 * stop - interaction stopped
	 *
	 * Stop notification for strip events.
	 *
	 * For some wp_tablet_pad_strip.source types, a
	 * wp_tablet_pad_strip.stop event is sent to notify a client that
	 * the interaction with the strip has terminated. This enables the
	 * client to implement kinetic scrolling. See the
	 * wp_tablet_pad_strip.source documentation for information on when
	 * this event may be generated.
	 *
	 * Any wp_tablet_pad_strip.position events with the same source
	 * after this event should be considered as the start of a new
	 * interaction.
	 */
	void (*stop)(void *data,
		     struct zwp_tablet_pad_strip_v2 *zwp_tablet_pad_strip_v2);
	/**
	 * frame - end of a strip event sequence
	 * @time: timestamp with millisecond granularity
	 *
	 * Indicates the end of a set of events that represent one
	 * logical hardware strip event. A client is expected to accumulate
	 * the data in all events within the frame before proceeding.
	 *
	 * All wp_tablet_pad_strip events before a
	 * wp_tablet_pad_strip.frame event belong logically together. For
	 * example, on termination of a finger interaction on a strip the
	 * compositor will send a wp_tablet_pad_strip.source event, a
	 * wp_tablet_pad_strip.stop event and a wp_tablet_pad_strip.frame
	 * event.
	 *
	 * A wp_tablet_pad_strip.frame event is sent for every logical
	 * event group, even if the group only contains a single
	 * wp_tablet_pad_strip event. Specifically, a client may get a
	 * sequence: position, frame, position, frame, etc.
	 */
	void (*frame)(void *data,
		      struct zwp_tablet_pad_strip_v2 *zwp_tablet_pad_strip_v2,
		      uint32_t time);
};

static inline int
zwp_tablet_pad_strip_v2_add_listener(struct zwp_tablet_pad_strip_v2 *zwp_tablet_pad_strip_v2,
				     const struct zwp_tablet_pad_strip_v2_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) zwp_tablet_pad_strip_v2,
				     (void (**)(void)) listener, data);
}

#define ZWP_TABLET_PAD_STRIP_V2_SET_FEEDBACK	0
#define ZWP_TABLET_PAD_STRIP_V2_DESTROY	1

static inline void
zwp_tablet_pad_strip_v2_set_user_data(struct zwp_tablet_pad_strip_v2 *zwp_tablet_pad_strip_v2, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_tablet_pad_strip_v2, user_data);
}

static inline void *
zwp_tablet_pad_strip_v2_get_user_data(struct zwp_tablet_pad_strip_v2 *zwp_tablet_pad_strip_v2)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_tablet_pad_strip_v2);
}

static inline void
zwp_tablet_pad_strip_v2_set_feedback(struct zwp_tablet_pad_strip_v2 *zwp_tablet_pad_strip_v2, const char *description, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_pad_strip_v2,
			 ZWP_TABLET_PAD_STRIP_V2_SET_FEEDBACK, description, serial);
}

static inline void
zwp_tablet_pad_strip_v2_destroy(struct zwp_tablet_pad_strip_v2 *zwp_tablet_pad_strip_v2)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_pad_strip_v2,
			 ZWP_TABLET_PAD_STRIP_V2_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_tablet_pad_strip_v2);
}

/**
 * zwp_tablet_pad_group_v2 - a set of buttons, rings and strips
 * @buttons: buttons announced
 * @ring: ring announced
 * @strip: strip announced
 * @modes: mode-switch ability announced
 * @done: tablet group description events sequence complete
 * @mode_switch: mode switch event
 *
 * A pad group describes a distinct (sub)set of buttons, rings and strips
 * present in the tablet. The criteria of this grouping is usually
 * positional, eg. if a tablet has buttons on the left and right side, 2
 * groups will be presented. The physical arrangement of groups is
 * undisclosed and may change on the fly.
 *
 * Pad groups will announce their features during pad initialization.
 * Between the corresponding wp_tablet_pad.group event and
 * wp_tablet_pad_group.done, the pad group will announce the buttons, rings
 * and strips contained in it, plus the number of supported modes.
 *
 * Modes are a mechanism to allow multiple groups of actions for every
 * element in the pad group. The number of groups and available modes in
 * each is persistent across device plugs. The current mode is
 * user-switchable, it will be announced through the
 * wp_tablet_pad_group.mode_switch event both whenever it is switched, and
 * after wp_tablet_pad.enter.
 *
 * The current mode logically applies to all elements in the pad group,
 * although it is at clients' discretion whether to actually perform
 * different actions, and/or issue the respective .set_feedback requests to
 * notify the compositor. See the wp_tablet_pad_group.mode_switch event for
 * more details.
 */
struct zwp_tablet_pad_group_v2_listener {
	/**
	 * buttons - buttons announced
	 * @buttons: buttons in this group
	 *
	 * Sent on wp_tablet_pad_group initialization to announce the
	 * available buttons in the group. Button indices start at 0, a
	 * button may only be in one group at a time.
	 *
	 * This event is first sent in the initial burst of events before
	 * the wp_tablet_pad_group.done event.
	 *
	 * Some buttons are reserved by the compositor. These buttons may
	 * not be assigned to any wp_tablet_pad_group. Compositors may
	 * broadcast this event in the case of changes to the mapping of
	 * these reserved buttons. If the compositor happens to reserve all
	 * buttons in a group, this event will be sent with an empty array.
	 */
	void (*buttons)(void *data,
			struct zwp_tablet_pad_group_v2 *zwp_tablet_pad_group_v2,
			struct wl_array *buttons);
	/**
	 * ring - ring announced
	 * @ring: (none)
	 *
	 * Sent on wp_tablet_pad_group initialization to announce
	 * available rings. One event is sent for each ring available on
	 * this pad group.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet_pad_group.done event.
	 */
	void (*ring)(void *data,
		     struct zwp_tablet_pad_group_v2 *zwp_tablet_pad_group_v2,
		     struct zwp_tablet_pad_ring_v2 *ring);
	/**
	 * strip - strip announced
	 * @strip: (none)
	 *
	 * Sent on wp_tablet_pad initialization to announce available
	 * strips. One event is sent for each strip available on this pad
	 * group.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet_pad_group.done event.
	 */
	void (*strip)(void *data,
		      struct zwp_tablet_pad_group_v2 *zwp_tablet_pad_group_v2,
		      struct zwp_tablet_pad_strip_v2 *strip);
	/**
	 * modes - mode-switch ability announced
	 * @modes: the number of modes
	 *
	 * Sent on wp_tablet_pad_group initialization to announce that
	 * the pad group may switch between modes. A client may use a mode
	 * to store a specific configuration for buttons, rings and strips
	 * and use the wl_tablet_pad_group.mode_switch event to toggle
	 * between these configurations. Mode indices start at 0.
	 *
	 * Switching modes is compositor-dependent. See the
	 * wp_tablet_pad_group.mode_switch event for more details.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet_pad_group.done event. This event is only sent when
	 * more than more than one mode is available.
	 */
	void (*modes)(void *data,
		      struct zwp_tablet_pad_group_v2 *zwp_tablet_pad_group_v2,
		      uint32_t modes);
	/**
	 * done - tablet group description events sequence complete
	 *
	 * This event is sent immediately to signal the end of the
	 * initial burst of descriptive events. A client may consider the
	 * static description of the tablet to be complete and finalize
	 * initialization of the tablet group.
	 */
	void (*done)(void *data,
		     struct zwp_tablet_pad_group_v2 *zwp_tablet_pad_group_v2);
	/**
	 * mode_switch - mode switch event
	 * @time: the time of the event with millisecond granularity
	 * @serial: (none)
	 * @mode: the new mode of the pad
	 *
	 * Notification that the mode was switched.
	 *
	 * A mode applies to all buttons, rings and strips in a group
	 * simultaneously, but a client is not required to assign different
	 * actions for each mode. For example, a client may have
	 * mode-specific button mappings but map the ring to vertical
	 * scrolling in all modes. Mode indices start at 0.
	 *
	 * Switching modes is compositor-dependent. The compositor may
	 * provide visual cues to the client about the mode, e.g. by
	 * toggling LEDs on the tablet device. Mode-switching may be
	 * software-controlled or controlled by one or more physical
	 * buttons. For example, on a Wacom Intuos Pro, the button inside
	 * the ring may be assigned to switch between modes.
	 *
	 * The compositor will also send this event after
	 * wp_tablet_pad.enter on each group in order to notify of the
	 * current mode. Groups that only feature one mode will use mode=0
	 * when emitting this event.
	 *
	 * If a button action in the new mode differs from the action in
	 * the previous mode, the client should immediately issue a
	 * wp_tablet_pad.set_feedback request for each changed button.
	 *
	 * If a ring or strip action in the new mode differs from the
	 * action in the previous mode, the client should immediately issue
	 * a wp_tablet_ring.set_feedback or wp_tablet_strip.set_feedback
	 * request for each changed ring or strip.
	 */
	void (*mode_switch)(void *data,
			    struct zwp_tablet_pad_group_v2 *zwp_tablet_pad_group_v2,
			    uint32_t time,
			    uint32_t serial,
			    uint32_t mode);
};

static inline int
zwp_tablet_pad_group_v2_add_listener(struct zwp_tablet_pad_group_v2 *zwp_tablet_pad_group_v2,
				     const struct zwp_tablet_pad_group_v2_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) zwp_tablet_pad_group_v2,
				     (void (**)(void)) listener, data);
}

#define ZWP_TABLET_PAD_GROUP_V2_DESTROY	0

static inline void
zwp_tablet_pad_group_v2_set_user_data(struct zwp_tablet_pad_group_v2 *zwp_tablet_pad_group_v2, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_tablet_pad_group_v2, user_data);
}

static inline void *
zwp_tablet_pad_group_v2_get_user_data(struct zwp_tablet_pad_group_v2 *zwp_tablet_pad_group_v2)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_tablet_pad_group_v2);
}

static inline void
zwp_tablet_pad_group_v2_destroy(struct zwp_tablet_pad_group_v2 *zwp_tablet_pad_group_v2)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_pad_group_v2,
			 ZWP_TABLET_PAD_GROUP_V2_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_tablet_pad_group_v2);
}

#ifndef ZWP_TABLET_PAD_V2_BUTTON_STATE_ENUM
#define ZWP_TABLET_PAD_V2_BUTTON_STATE_ENUM
/**
 * zwp_tablet_pad_v2_button_state - physical button state
 * @ZWP_TABLET_PAD_V2_BUTTON_STATE_RELEASED: the button is not pressed
 * @ZWP_TABLET_PAD_V2_BUTTON_STATE_PRESSED: the button is pressed
 *
 * Describes the physical state of a button that caused the button event.
 */
enum zwp_tablet_pad_v2_button_state {
	ZWP_TABLET_PAD_V2_BUTTON_STATE_RELEASED = 0,
	ZWP_TABLET_PAD_V2_BUTTON_STATE_PRESSED = 1,
};
#endif /* ZWP_TABLET_PAD_V2_BUTTON_STATE_ENUM */

/**
 * zwp_tablet_pad_v2 - a set of buttons, rings and strips
 * @group: group announced
 * @path: path to the device
 * @buttons: buttons announced
 * @done: pad description event sequence complete
 * @button: physical button state
 * @enter: enter event
 * @leave: leave event
 * @removed: pad removed event
 *
 * A pad device is a set of buttons, rings and strips usually physically
 * present on the tablet device itself. Some exceptions exist where the pad
 * device is physically detached, e.g. the Wacom ExpressKey Remote.
 *
 * Pad devices have no axes that control the cursor and are generally
 * auxiliary devices to the tool devices used on the tablet surface.
 *
 * A pad device has a number of static characteristics, e.g. the number of
 * rings. These capabilities are sent in an event sequence after the
 * wp_tablet_seat.pad_added event before any actual events from this pad.
 * This initial event sequence is terminated by a wp_tablet_pad.done event.
 *
 * All pad features (buttons, rings and strips) are logically divided into
 * groups and all pads have at least one group. The available groups are
 * notified through the wp_tablet_pad.group event; the compositor will emit
 * one event per group before emitting wp_tablet_pad.done.
 *
 * Groups may have multiple modes. Modes allow clients to map multiple
 * actions to a single pad feature. Only one mode can be active per group,
 * although different groups may have different active modes.
 */
struct zwp_tablet_pad_v2_listener {
	/**
	 * group - group announced
	 * @pad_group: (none)
	 *
	 * Sent on wp_tablet_pad initialization to announce available
	 * groups. One event is sent for each pad group available.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet_pad.done event. At least one group will be announced.
	 */
	void (*group)(void *data,
		      struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2,
		      struct zwp_tablet_pad_group_v2 *pad_group);
	/**
	 * path - path to the device
	 * @path: path to local device
	 *
	 * A system-specific device path that indicates which device is
	 * behind this wp_tablet_pad. This information may be used to
	 * gather additional information about the device, e.g. through
	 * libwacom.
	 *
	 * The format of the path is unspecified, it may be a device node,
	 * a sysfs path, or some other identifier. It is up to the client
	 * to identify the string provided.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet_pad.done event.
	 */
	void (*path)(void *data,
		     struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2,
		     const char *path);
	/**
	 * buttons - buttons announced
	 * @buttons: the number of buttons
	 *
	 * Sent on wp_tablet_pad initialization to announce the available
	 * buttons.
	 *
	 * This event is sent in the initial burst of events before the
	 * wp_tablet_pad.done event. This event is only sent when at least
	 * one button is available.
	 */
	void (*buttons)(void *data,
			struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2,
			uint32_t buttons);
	/**
	 * done - pad description event sequence complete
	 *
	 * This event signals the end of the initial burst of descriptive
	 * events. A client may consider the static description of the pad
	 * to be complete and finalize initialization of the pad.
	 */
	void (*done)(void *data,
		     struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2);
	/**
	 * button - physical button state
	 * @time: the time of the event with millisecond granularity
	 * @button: the index of the button that changed state
	 * @state: (none)
	 *
	 * Sent whenever the physical state of a button changes.
	 */
	void (*button)(void *data,
		       struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2,
		       uint32_t time,
		       uint32_t button,
		       uint32_t state);
	/**
	 * enter - enter event
	 * @serial: serial number of the enter event
	 * @tablet: the tablet the pad is attached to
	 * @surface: surface the pad is focused on
	 *
	 * Notification that this pad is focused on the specified
	 * surface.
	 */
	void (*enter)(void *data,
		      struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2,
		      uint32_t serial,
		      struct zwp_tablet_v2 *tablet,
		      struct wl_surface *surface);
	/**
	 * leave - leave event
	 * @serial: serial number of the leave event
	 * @surface: surface the pad is no longer focused on
	 *
	 * Notification that this pad is no longer focused on the
	 * specified surface.
	 */
	void (*leave)(void *data,
		      struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2,
		      uint32_t serial,
		      struct wl_surface *surface);
	/**
	 * removed - pad removed event
	 *
	 * Sent when the pad has been removed from the system. When a
	 * tablet is removed its pad(s) will be removed too.
	 *
	 * When this event is received, the client must destroy all rings,
	 * strips and groups that were offered by this pad, and issue
	 * wp_tablet_pad.destroy the pad itself.
	 */
	void (*removed)(void *data,
			struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2);
};

static inline int
zwp_tablet_pad_v2_add_listener(struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2,
			       const struct zwp_tablet_pad_v2_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) zwp_tablet_pad_v2,
				     (void (**)(void)) listener, data);
}

#define ZWP_TABLET_PAD_V2_SET_FEEDBACK	0
#define ZWP_TABLET_PAD_V2_DESTROY	1

static inline void
zwp_tablet_pad_v2_set_user_data(struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) zwp_tablet_pad_v2, user_data);
}

static inline void *
zwp_tablet_pad_v2_get_user_data(struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2)
{
	return wl_proxy_get_user_data((struct wl_proxy *) zwp_tablet_pad_v2);
}

static inline void
zwp_tablet_pad_v2_set_feedback(struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2, uint32_t button, const char *description, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_pad_v2,
			 ZWP_TABLET_PAD_V2_SET_FEEDBACK, button, description, serial);
}

static inline void
zwp_tablet_pad_v2_destroy(struct zwp_tablet_pad_v2 *zwp_tablet_pad_v2)
{
	wl_proxy_marshal((struct wl_proxy *) zwp_tablet_pad_v2,
			 ZWP_TABLET_PAD_V2_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) zwp_tablet_pad_v2);
}

#ifdef  __cplusplus
}
#endif

#endif
