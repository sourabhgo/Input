// winmain.cpp

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

// Function prototypes
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int); 
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM); 

// Global variables
HINSTANCE hinst;
HDC    hdc;                 // handle to device context
TCHAR ch = ' ';             // character entered
RECT rect;                  // rectangle structure
PAINTSTRUCT ps;             // used in WM_PAINT

// Constants
const char CLASS_NAME[]  = "Keyboard";
const char APP_TITLE[]   = "Character Input";
const int  WINDOW_WIDTH  = 400;     // width of window
const int  WINDOW_HEIGHT = 300;     // height of window

//=============================================================================
// Starting point for a Windows application
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
    MSG  msg;

    // Create the window
    if (!CreateMainWindow(hInstance, nCmdShow))
        return false;

    // main message loop
    int done = 0;
    while (!done)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        {
            //look for quit message
            if (msg.message == WM_QUIT)
                done = 1;

            //decode and pass messages on to WinProc
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

//=============================================================================
// window event callback function
//=============================================================================
LRESULT WINAPI WinProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

    switch( msg )
    {
        case WM_DESTROY:
            //tell Windows to kill this program
            PostQuitMessage(0);
            return 0;

        case WM_CHAR:               // a character was entered by the keyboard
            switch (wParam)         // the character is in wParam
            {
                case 0x08:          // backspace
                case 0x09:          // tab
                case 0x0A:          // linefeed
                case 0x0D:          // carriage return
                case 0x1B:          // escape
                    MessageBeep((UINT) -1);    // beep but do not display
                    return 0;
                default:            // displayable character
                    ch = (TCHAR) wParam;    // get the character
                    InvalidateRect(hwnd, NULL, TRUE);    // force WM_PAINT
                    return 0;
            }

        case WM_PAINT:              // the window needs to be redrawn
            hdc = BeginPaint(hwnd, &ps);    // get handle to device context
            GetClientRect(hwnd, &rect);     // get the window rectangle
            // Display the character
            TextOut(hdc, rect.right/2, rect.bottom/2, &ch, 1);
            EndPaint(hwnd, &ps);
            return 0;

        default:
            return DefWindowProc( hwnd, msg, wParam, lParam );
    }
}

//=============================================================================
// Create the window
// Returns: false on error
//=============================================================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow) 
{ 
    WNDCLASSEX wcx; 
    HWND hwnd;
 
    // Fill in the window class structure with parameters 
    // that describe the main window. 
    wcx.cbSize = sizeof(wcx);           // size of structure 
    wcx.style = CS_HREDRAW | CS_VREDRAW;    // redraw if size changes 
    wcx.lpfnWndProc = WinProc;          // points to window procedure 
    wcx.cbClsExtra = 0;                 // no extra class memory 
    wcx.cbWndExtra = 0;                 // no extra window memory 
    wcx.hInstance = hInstance;          // handle to instance 
    wcx.hIcon = NULL; 
    wcx.hCursor = LoadCursor(NULL,IDC_ARROW);   // predefined arrow 
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);    // black background brush 
    wcx.lpszMenuName =  NULL;           // name of menu resource 
    wcx.lpszClassName = CLASS_NAME;	    // name of window class 
    wcx.hIconSm = NULL;                 // small class icon 
 
    // Register the window class. 
    // RegisterClassEx returns 0 on error.
    if (RegisterClassEx(&wcx) == 0)    // if error
        return false;

    // Create window
    hwnd = CreateWindow(
        CLASS_NAME,             // name of the window class
        APP_TITLE,              // title bar text
        WS_OVERLAPPEDWINDOW,    // window style
        CW_USEDEFAULT,          // default horizontal position of window
        CW_USEDEFAULT,          // default vertical position of window
        WINDOW_WIDTH,           // width of window
        WINDOW_HEIGHT,          // height of the window
        (HWND) NULL,            // no parent window
        (HMENU) NULL,           // no menu
        hInstance,              // handle to application instance
        (LPVOID) NULL);         // no window parameters

    // if there was an error creating the window
    if (!hwnd)
        return false;

    // Show the window
    ShowWindow(hwnd, nCmdShow);

    // Send a WM_PAINT message to the window procedure
    UpdateWindow(hwnd);
    return true;
}
