
#include "precomp.h"
#include "mouse_movement.h"

using namespace clan;

MouseMovement::MouseMovement()
{
	stop_event = CreateEvent(0, TRUE, FALSE, 0);
	if (stop_event == 0)
		throw Exception("CreateEvent failed");
	thread.start(this, &MouseMovement::worker_main);
}

MouseMovement::~MouseMovement()
{
	SetEvent(stop_event);
	thread.join();
	CloseHandle(stop_event);
}

void MouseMovement::worker_main()
{
	WNDCLASSEX class_desc = { 0 };
	class_desc.cbSize = sizeof(WNDCLASSEX);
	class_desc.style = 0;
	class_desc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	class_desc.lpfnWndProc = &MouseMovement::static_window_proc;
	class_desc.lpszClassName = L"MouseMovement";
	ATOM class_atom = RegisterClassEx(&class_desc);

	DWORD ex_style = 0;
	DWORD style = WS_POPUPWINDOW;
	HWND window_handle = CreateWindowEx(ex_style, L"MouseMovement", L"MouseMovement", style, CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, 0, 0, (HINSTANCE) GetModuleHandle(0), this);
	if (window_handle == 0)
		throw Exception("CreateWindowEx failed");

	// See http://msdn.microsoft.com/en-us/library/ms789918.aspx

	#ifndef HID_USAGE_PAGE_GENERIC
	#define HID_USAGE_PAGE_GENERIC		((USHORT) 0x01)
	#endif

	#ifndef HID_USAGE_GENERIC_MOUSE
	#define HID_USAGE_GENERIC_MOUSE	((USHORT) 0x02)
	#endif

	#ifndef HID_USAGE_GENERIC_JOYSTICK
	#define HID_USAGE_GENERIC_JOYSTICK	((USHORT) 0x04)
	#endif

	#ifndef HID_USAGE_GENERIC_GAMEPAD
	#define HID_USAGE_GENERIC_GAMEPAD	((USHORT) 0x05)
	#endif

	#ifndef RIDEV_INPUTSINK
	#define RIDEV_INPUTSINK	(0x100)
	#endif

	RAWINPUTDEVICE rid;
	rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid.usUsage = HID_USAGE_GENERIC_MOUSE; 
	rid.dwFlags = RIDEV_INPUTSINK;   
	rid.hwndTarget = window_handle;
	BOOL result = RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));
	if (result == FALSE)
	{
		DestroyWindow(window_handle);
		throw Exception("RegisterRawInputDevices failed");
	}

	while (true)
	{
		DWORD wakeup_reason = MsgWaitForMultipleObjects(1, &stop_event, FALSE, INFINITE, QS_RAWINPUT);
		bool event_arrived = (wakeup_reason == WAIT_OBJECT_0 + 1);
		if (!event_arrived)
			break;

		MSG msg;
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	DestroyWindow(window_handle);
}

LRESULT MouseMovement::window_proc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	if (message_id == WM_INPUT)
	{
		HRAWINPUT handle = (HRAWINPUT)lparam;
		UINT size = 0;
		UINT result = GetRawInputData(handle, RID_INPUT, 0, &size, sizeof(RAWINPUTHEADER));
		if (result == 0 && size > 0)
		{
			DataBuffer buffer(size);
			result = GetRawInputData(handle, RID_INPUT, buffer.get_data(), &size, sizeof(RAWINPUTHEADER));
			if (result >= 0)
			{
				RAWINPUT *rawinput = (RAWINPUT*)buffer.get_data();
				if (rawinput->header.dwType == RIM_TYPEMOUSE)
				{
					_x.set(_x.get() + rawinput->data.mouse.lLastX);
					_y.set(_y.get() + rawinput->data.mouse.lLastY);
				}
			}
		}
		return DefWindowProc(window_handle, message_id, wparam, lparam);
	}
	else
	{
		return DefWindowProc(window_handle, message_id, wparam, lparam);
	}
}

LRESULT MouseMovement::static_window_proc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	MouseMovement *self = 0;
	if (message_id == WM_CREATE)
	{
		LPCREATESTRUCT create_struct = (LPCREATESTRUCT) lparam;
		self = (MouseMovement *)create_struct->lpCreateParams;
		SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR) self);
	}
	else
	{
		self = (MouseMovement *)GetWindowLongPtr(window_handle, GWLP_USERDATA);
	}
	return self->window_proc(window_handle, message_id, wparam, lparam);
}
