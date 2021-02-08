#pragma once

#include <Windows.h>

/** GDI double-buffering */
class ScreenBuffer {
	HWND hwnd;
	HDC hdcBackBuffer;// handle to the back-buffer device context
	HBITMAP mbmp;    // memory bitmap image (the back-buffer image)
	HBITMAP moldbmp; // keep track of the old bitmap used by this window

	PAINTSTRUCT ps;
	RECT r;
	HDC hdc;

	bool useDoubleBuffer; // disables double-buffering, to test behavior
public:
	void setUseDoubleBuffer(bool use){ useDoubleBuffer = use; }
	void init(HWND a_hwnd){
		hwnd = a_hwnd;
		useDoubleBuffer = true;
		// create the backbuffer so the screen does not flicker
		HDC hdc = GetDC(hwnd);
		RECT r;
		GetClientRect(hwnd, &r);
		hdcBackBuffer = CreateCompatibleDC(NULL);
		mbmp = CreateCompatibleBitmap(hdc, r.right, r.bottom);
		moldbmp = (HBITMAP)SelectObject(hdcBackBuffer,mbmp);
		ReleaseDC(hwnd, hdc);
	}

	void release(){
		// cleanup GDI double-buffer
		SelectObject(hdcBackBuffer,moldbmp);
		DeleteObject(mbmp);
		DeleteDC(hdcBackBuffer);
	}

	void resize() {
		// release the old back-buffer
		SelectObject(hdcBackBuffer,moldbmp);
		DeleteObject(mbmp);
		DeleteDC(hdcBackBuffer);

		// recreate the backbuffer to be the current window size
		RECT r;
		GetClientRect(hwnd, &r);
		HDC hdc = GetDC(hwnd);
		hdcBackBuffer = CreateCompatibleDC(NULL);
		mbmp = CreateCompatibleBitmap(hdc, r.right, r.bottom);
		moldbmp = (HBITMAP)SelectObject(hdcBackBuffer,mbmp);
		ReleaseDC(hwnd, hdc);
	}

	void drawBegin() {
		hdc = BeginPaint (hwnd, &ps) ;
	}

	HDC getHdc() { return useDoubleBuffer?hdcBackBuffer:hdc; }

	void drawEnd() {
		if(useDoubleBuffer) {
			// print the back buffer onto the current device context
			GetClientRect(hwnd, &r);
			BitBlt(hdc, r.left, r.top, r.right, r.bottom, 
				hdcBackBuffer, 0, 0, SRCCOPY);
		}
		EndPaint(hwnd, &ps);
	}
};
