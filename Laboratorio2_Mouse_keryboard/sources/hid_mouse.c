/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_hid.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "composite.h"
#include "hid_mouse.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static usb_status_t USB_DeviceHidMouseAction(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_MouseBuffer[USB_HID_MOUSE_REPORT_LENGTH];
static usb_device_composite_struct_t *s_UsbDeviceComposite;
static usb_device_hid_mouse_struct_t s_UsbDeviceHidMouse;
extern uint8_t FlagLMouseSelect;
extern uint8_t FlagMSpaint;
uint8_t FlagMSpaintready = pdFALSE;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Update mouse pointer location. Draw a rectangular rotation*/
static usb_status_t USB_DeviceHidMouseAction(void)
{
	static int8_t x = 0U;
	static int8_t y = 0U;
	enum
	{
		RIGHT,
		DOWN,
		LEFT,
		UP,
		WAIT,
		DRAW,
		SELECT
	};
	static uint8_t dir = RIGHT;
	static uint8_t dir_draw = 0U;
	s_UsbDeviceHidMouse.buffer[ 0 ] = 0U;

    //state machine
	switch (dir)
	{
	case RIGHT:

		/* Move right. Increase X value. */
		s_UsbDeviceHidMouse.buffer[ 1 ] = 15U;
		s_UsbDeviceHidMouse.buffer[ 2 ] = 0U;
		x++;
		if ( x > 99U )
		{
			dir = UP;
		}
		break;

	case DOWN:

		/* Move down. Increase Y value. */
		s_UsbDeviceHidMouse.buffer[ 1 ] = 0U;
		s_UsbDeviceHidMouse.buffer[ 2 ] = 5U;
		y++;
		if ( y > 99U )
		{
			dir = WAIT;
		}
		break;

	case LEFT:

		/* Move left. Discrease X value. */
		s_UsbDeviceHidMouse.buffer[ 1 ] = (uint8_t)( -8 );
		s_UsbDeviceHidMouse.buffer[ 2 ] = 0U;
		x--;
		if ( x < 2U )
		{
			dir = DOWN;
		}
		break;

	case UP:

		/* Move up. Discrease Y value. */
		s_UsbDeviceHidMouse.buffer[ 1 ] = 0U;
		s_UsbDeviceHidMouse.buffer[ 2 ] = (uint8_t)( -10 );
		y--;
		if ( y < 2U )
		{
			dir = LEFT;
		}
		break;

	case WAIT:
		s_UsbDeviceHidMouse.buffer[ 1 ] = 0U;
		s_UsbDeviceHidMouse.buffer[ 2 ] = 0U;
		if ( FlagLMouseSelect )
		{
			dir = SELECT;
			x = 0U;
			y = 0U;
		}
		if ( FlagMSpaint )
		{
			dir = DRAW;
			x = 0U;
			y = 0U;
		}
		break;

	case DRAW:
		switch ( dir_draw )
		{
		case 0:
			/* Move right. Increase X value. */
			s_UsbDeviceHidMouse.buffer[ 0 ] = 1U;
			s_UsbDeviceHidMouse.buffer[ 1 ] = 2U;
			s_UsbDeviceHidMouse.buffer[ 2 ] = 0U;
			x++;

			if ( x > 99U )
			{
				dir_draw++;
			}
			break;

		case 1:
			/* Move down. Increase Y value. */
			s_UsbDeviceHidMouse.buffer[ 0 ] = 1U;
			s_UsbDeviceHidMouse.buffer[ 1 ] = 0U;
			s_UsbDeviceHidMouse.buffer[ 2 ] = 2U;
			y++;

			if ( y > 99U )
			{
				dir_draw++;
			}
			break;

		case 2:
			/* Move left. Discrease X value. */
			s_UsbDeviceHidMouse.buffer[ 0 ] = 1U;
			s_UsbDeviceHidMouse.buffer[ 1 ] = (uint8_t)( -2 );
			s_UsbDeviceHidMouse.buffer[ 2 ] = 0U;
			x--;

			if ( x < 2U )
			{
				dir_draw++;
			}
			break;

		case 3:
			/* Move up. Discrease Y value. */
			s_UsbDeviceHidMouse.buffer[ 0 ] = 1U;
			s_UsbDeviceHidMouse.buffer[ 1 ] = 0U;
			s_UsbDeviceHidMouse.buffer[ 2 ] = (uint8_t)( -2 );
			y--;

			if ( y < 2U )
			{
				dir = WAIT;
				FlagMSpaint = pdFALSE;
				FlagMSpaintready = pdTRUE;
			}
			break;
		}
		break;

		case SELECT:
			x++;
			s_UsbDeviceHidMouse.buffer[ 1 ] = (uint8_t)( -2 );
			s_UsbDeviceHidMouse.buffer[ 2 ] = 0U;

			if ( x > 2 )
			{
				s_UsbDeviceHidMouse.buffer[ 0 ] = 4U;
				dir = WAIT;
			}

			break;
		default:
			break;
	}
	return USB_DeviceHidSend(s_UsbDeviceComposite->hidMouseHandle,
			USB_HID_MOUSE_ENDPOINT_IN,s_UsbDeviceHidMouse.buffer,
			USB_HID_MOUSE_REPORT_LENGTH);
}

/* The device HID class callback */
usb_status_t USB_DeviceHidMouseCallback(class_handle_t handle, uint32_t event, void *param)
{
	usb_status_t error = kStatus_USB_Error;

	switch (event)
	{
	case kUSB_DeviceHidEventSendResponse:
		if (s_UsbDeviceComposite->attach)
		{
			return USB_DeviceHidMouseAction();
		}
		break;
	case kUSB_DeviceHidEventGetReport:
	case kUSB_DeviceHidEventSetReport:
	case kUSB_DeviceHidEventRequestReportBuffer:
		error = kStatus_USB_InvalidRequest;
		break;
	case kUSB_DeviceHidEventGetIdle:
	case kUSB_DeviceHidEventGetProtocol:
	case kUSB_DeviceHidEventSetIdle:
	case kUSB_DeviceHidEventSetProtocol:
		break;
	default:
		break;
	}

	return error;
}

/* The device callback */
usb_status_t USB_DeviceHidMouseSetConfigure(class_handle_t handle, uint8_t configure)
{
	if (USB_COMPOSITE_CONFIGURE_INDEX == configure)
	{
		return USB_DeviceHidMouseAction(); /* run the cursor movement code */
	}
	return kStatus_USB_Error;
}

/* Set interface */
usb_status_t USB_DeviceHidMouseSetInterface(class_handle_t handle, uint8_t interface,
		uint8_t alternateSetting)
{
	if (USB_HID_KEYBOARD_INTERFACE_INDEX == interface)
	{
		return USB_DeviceHidMouseAction(); /* run the cursor movement code */
	}
	return kStatus_USB_Error;
}

/* Initialize the HID mouse */
usb_status_t USB_DeviceHidMouseInit(usb_device_composite_struct_t *deviceComposite)
{
	s_UsbDeviceComposite = deviceComposite;
	s_UsbDeviceHidMouse.buffer = s_MouseBuffer;
	return kStatus_USB_Success;
}
