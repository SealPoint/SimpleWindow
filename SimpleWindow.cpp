// SimpleWindow.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SimpleWindow.h"

#define MAX_LOADSTRING 100
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_CORNER_RADIUS 8

// Global Variables:
HWND hWnd;										// window handle
HRGN hRgn;										// Window's clipping region
HBRUSH hBackgroundBrush;							// Background color brush
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void UpdateClippingRegion (int width, int height);

int APIENTRY _tWinMain (HINSTANCE hInstance,
                        HINSTANCE hPrevInstance,
                        LPTSTR    lpCmdLine,
                        int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SIMPLEWINDOW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMPLEWINDOW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		static std::stringstream str;
		str << msg.message << std::endl;
		::OutputDebugStringA(str.str().c_str());

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass (HINSTANCE hInstance)
{
	hBackgroundBrush = ::CreateSolidBrush(RGB(118, 182, 252));

	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpszClassName	= szWindowClass;
	wcex.hbrBackground =  (HBRUSH)::GetStockObject(NULL_BRUSH); // transparent
	wcex.lpfnWndProc	= WndProc;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance (HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass,
	                   0, // no title
					   WS_POPUP,
					   200, // x
					   200, // y
					   WINDOW_WIDTH, // width
					   WINDOW_HEIGHT, // height
					   NULL, // no parent window
					   NULL, // no menu
					   hInstance,
					   0);

   if (!hWnd)
   {
      return FALSE;
   }

   UpdateClippingRegion(WINDOW_WIDTH, WINDOW_HEIGHT);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void UpdateClippingRegion (int width, int height)
{
    HDC hdc = ::GetDC(hWnd);

	// Bracket begin a path 
    BeginPath(hdc);

    ::ArcTo(hdc,
            0, // enclosing rect.left
            0, // enclosing rect.top
            WINDOW_CORNER_RADIUS * 2 - 1, // enclosing rect.right
            WINDOW_CORNER_RADIUS * 2 - 1, // enclosing rect.bottom
            WINDOW_CORNER_RADIUS - 1, // start point.x
            0, // start point.y
            0, // end point.x
            WINDOW_CORNER_RADIUS - 1); // end point.y
    ::LineTo(hdc,
             0, // x
             height - 1); // y
    ::LineTo(hdc,
             width - 1, // x
             height - 1); // y
    ::LineTo(hdc,
             width - 1, // x
             WINDOW_CORNER_RADIUS - 1); // y

    ::ArcTo(hdc,
            width - (WINDOW_CORNER_RADIUS * 2 - 1), // enclosing rect.left
            0, // enclosing rect.top
            width - 1, // enclosing rect.right
            WINDOW_CORNER_RADIUS * 2 - 1, // enclosing rect.bottom
            width - 1, // start point.x
            WINDOW_CORNER_RADIUS - 1, // start point.y
            width - (WINDOW_CORNER_RADIUS - 1), // end point.x
            0); // end point.height
    ::LineTo(hdc,
             WINDOW_CORNER_RADIUS - 1, // x
             0); // y

    // Bracket end a path 
    EndPath(hdc);

    hRgn = ::PathToRegion(hdc);
    ::SetWindowRgn(hWnd, hRgn, FALSE); // Do not repaint
    ::ReleaseDC(hWnd, hdc);
}

LRESULT OnCreate (WPARAM wParam, LPARAM lParam)
{
	CREATESTRUCTA* createParams = (CREATESTRUCTA*)lParam;
	hWnd = createParams->hwndParent;
	
	return 0;
}

void OnPaint (HDC hdc)
{
	::SetBkMode(hdc, TRANSPARENT);

	RECT rect;
	::GetClientRect(hWnd, &rect);
    ::FillRect(hdc, &rect, hBackgroundBrush);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		return OnCreate(wParam, lParam);
		
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

