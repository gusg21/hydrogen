/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_internal.h"

#if defined(SDL_VIDEO_DRIVER_WINDOWS)

#include "SDL_windowsvideo.h"

#if !defined(SDL_PLATFORM_XBOXONE) && !defined(SDL_PLATFORM_XBOXSERIES)

#include "SDL_windowsevents.h"

#include "../../joystick/usb_ids.h"

#define ENABLE_RAW_MOUSE_INPUT      0x01
#define ENABLE_RAW_KEYBOARD_INPUT   0x02

typedef struct
{
    SDL_bool done;
    Uint32 flags;
    HANDLE ready_event;
    HANDLE done_event;
    HANDLE thread;
} RawInputThreadData;

static RawInputThreadData thread_data = {
    SDL_FALSE,
    0,
    INVALID_HANDLE_VALUE,
    INVALID_HANDLE_VALUE,
    INVALID_HANDLE_VALUE
};

static DWORD WINAPI WIN_RawInputThread(LPVOID param)
{
    SDL_VideoDevice *_this = SDL_GetVideoDevice();
    RawInputThreadData *data = (RawInputThreadData *)param;
    RAWINPUTDEVICE devices[2];
    HWND window;
    UINT count = 0;

    window = CreateWindowEx(0, TEXT("Message"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
    if (!window) {
        return 0;
    }

    if (data->flags & ENABLE_RAW_MOUSE_INPUT) {
        devices[count].usUsagePage = USB_USAGEPAGE_GENERIC_DESKTOP;
        devices[count].usUsage = USB_USAGE_GENERIC_MOUSE;
        devices[count].dwFlags = 0;
        devices[count].hwndTarget = window;
        ++count;
    }

    if (data->flags & ENABLE_RAW_KEYBOARD_INPUT) {
        devices[count].usUsagePage = USB_USAGEPAGE_GENERIC_DESKTOP;
        devices[count].usUsage = USB_USAGE_GENERIC_KEYBOARD;
        devices[count].dwFlags = 0;
        devices[count].hwndTarget = window;
        ++count;
    }

    if (!RegisterRawInputDevices(devices, count, sizeof(devices[0]))) {
        DestroyWindow(window);
        return 0;
    }

    /* Make sure we get events as soon as possible */
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    /* Tell the parent we're ready to go! */
    SetEvent(data->ready_event);

    while (!data->done) {
        if (MsgWaitForMultipleObjects(1, &data->done_event, FALSE, INFINITE, QS_RAWINPUT) != (WAIT_OBJECT_0 + 1)) {
            break;
        }

        /* Clear the queue status so MsgWaitForMultipleObjects() will wait again */
        (void)GetQueueStatus(QS_RAWINPUT);

        WIN_PollRawInput(_this);
    }

    devices[0].dwFlags |= RIDEV_REMOVE;
    devices[1].dwFlags |= RIDEV_REMOVE;
    RegisterRawInputDevices(devices, count, sizeof(devices[0]));

    DestroyWindow(window);

    return 0;
}

static void CleanupRawInputThreadData(RawInputThreadData *data)
{
    if (data->thread != INVALID_HANDLE_VALUE) {
        data->done = SDL_TRUE;
        SetEvent(data->done_event);
        WaitForSingleObject(data->thread, 3000);
        CloseHandle(data->thread);
        data->thread = INVALID_HANDLE_VALUE;
    }

    if (data->ready_event != INVALID_HANDLE_VALUE) {
        CloseHandle(data->ready_event);
        data->ready_event = INVALID_HANDLE_VALUE;
    }

    if (data->done_event != INVALID_HANDLE_VALUE) {
        CloseHandle(data->done_event);
        data->done_event = INVALID_HANDLE_VALUE;
    }
}

static int WIN_SetRawInputEnabled(SDL_VideoDevice *_this, Uint32 flags)
{
    int result = -1;

    CleanupRawInputThreadData(&thread_data);

    if (flags) {
        HANDLE handles[2];

        thread_data.flags = flags;
        thread_data.ready_event = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (thread_data.ready_event == INVALID_HANDLE_VALUE) {
            WIN_SetError("CreateEvent");
            goto done;
        }

        thread_data.done = SDL_FALSE;
        thread_data.done_event = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (thread_data.done_event == INVALID_HANDLE_VALUE) {
            WIN_SetError("CreateEvent");
            goto done;
        }

        thread_data.thread = CreateThread(NULL, 0, WIN_RawInputThread, &thread_data, 0, NULL);
        if (thread_data.thread == INVALID_HANDLE_VALUE) {
            WIN_SetError("CreateThread");
            goto done;
        }

        /* Wait for the thread to signal ready or exit */
        handles[0] = thread_data.ready_event;
        handles[1] = thread_data.thread;
        if (WaitForMultipleObjects(2, handles, FALSE, INFINITE) != WAIT_OBJECT_0) {
            SDL_SetError("Couldn't set up raw input handling");
            goto done;
        }
        result = 0;
    } else {
        result = 0;
    }

done:
    if (result < 0) {
        CleanupRawInputThreadData(&thread_data);
    }
    return result;
}

static int WIN_UpdateRawInputEnabled(SDL_VideoDevice *_this)
{
    SDL_VideoData *data = _this->internal;
    Uint32 flags = 0;
    if (data->raw_mouse_enabled) {
        flags |= ENABLE_RAW_MOUSE_INPUT;
    }
    if (data->raw_keyboard_enabled) {
        flags |= ENABLE_RAW_KEYBOARD_INPUT;
    }
    if (flags != data->raw_input_enabled) {
        if (WIN_SetRawInputEnabled(_this, flags) == 0) {
            data->raw_input_enabled = flags;
        } else {
            return -1;
        }
    }
    return 0;
}

int WIN_SetRawMouseEnabled(SDL_VideoDevice *_this, SDL_bool enabled)
{
    SDL_VideoData *data = _this->internal;
    data->raw_mouse_enabled = enabled;
    if (data->gameinput_context) {
        if (WIN_UpdateGameInputEnabled(_this) < 0) {
            data->raw_mouse_enabled = !enabled;
            return -1;
        }
    } else {
        if (WIN_UpdateRawInputEnabled(_this) < 0) {
            data->raw_mouse_enabled = !enabled;
            return -1;
        }
    }
    return 0;
}

int WIN_SetRawKeyboardEnabled(SDL_VideoDevice *_this, SDL_bool enabled)
{
    SDL_VideoData *data = _this->internal;
    data->raw_keyboard_enabled = enabled;
    if (data->gameinput_context) {
        if (WIN_UpdateGameInputEnabled(_this) < 0) {
            data->raw_keyboard_enabled = !enabled;
            return -1;
        }
    } else {
        if (WIN_UpdateRawInputEnabled(_this) < 0) {
            data->raw_keyboard_enabled = !enabled;
            return -1;
        }
    }
    return 0;
}

#else

int WIN_SetRawMouseEnabled(SDL_VideoDevice *_this, SDL_bool enabled)
{
    return SDL_Unsupported();
}

int WIN_SetRawKeyboardEnabled(SDL_VideoDevice *_this, SDL_bool enabled)
{
    return SDL_Unsupported();
}

#endif /* !SDL_PLATFORM_XBOXONE && !SDL_PLATFORM_XBOXSERIES */

#endif /* SDL_VIDEO_DRIVER_WINDOWS */
