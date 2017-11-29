// Snake.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Snake_Course.h"
#include <ctime>
#include <Windows.h>
#define MAX_LOADSTRING 100
using namespace std;
enum MAP_INDEX {
     GRASS_IND, SNAKE_IND, APPLE_IND, WALL_IND,GAPPLE_IND,BOMB_IND
};
enum DIRECTION {
     up, down, left, right
};
struct POSITION {
     int x;
     int y;
};
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

bool bOne = true;
char map[13][13] = {
     { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
     { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
     { 3, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3 },
     { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
     { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
     { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
     { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
     { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
     { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
     { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
     { 3, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3 },
     { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
     { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
};
POSITION coords[150];
int bodySize = 3;
POSITION oldCoords[150];
int pts = 0; int level = 1; int Map_level = 1; char headDirection = 5;
void move(char direction, HWND hWnd);
void putApple(HWND hWnd);
void gameOver(HWND hWnd);
void gameOverWin(HWND hWnd);
void newLevel(HWND hWnd);
void putGApple(HWND hWnd);
void nextLevel(HWND hWnd);
void randomWall();
void randomBomb();
wchar_t text[10];
int game_speed = 150;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
     _In_opt_ HINSTANCE hPrevInstance,
     _In_ LPTSTR    lpCmdLine,
     _In_ int       nCmdShow)
{
     UNREFERENCED_PARAMETER(hPrevInstance);
     UNREFERENCED_PARAMETER(lpCmdLine);

     // TODO: Place code here.
     MSG msg;
     HACCEL hAccelTable;

     // Initialize global strings
     LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
     LoadString(hInstance, IDC_SNAKE_COURSE, szWindowClass, MAX_LOADSTRING);
     MyRegisterClass(hInstance);

     // Perform application initialization:
     if (!InitInstance(hInstance, nCmdShow))
     {
          return FALSE;
     }

     hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKE_COURSE));

     // Main message loop:
     while (GetMessage(&msg, NULL, 0, 0))
     {
          if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
          {
               TranslateMessage(&msg);
               DispatchMessage(&msg);
          }
     }

     return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
     WNDCLASSEX wcex;

     wcex.cbSize = sizeof(WNDCLASSEX);

     wcex.style = CS_HREDRAW | CS_VREDRAW;
     wcex.lpfnWndProc = WndProc;
     wcex.cbClsExtra = 0;
     wcex.cbWndExtra = 0;
     wcex.hInstance = hInstance;
     wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE_COURSE));
     wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
     wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
     wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SNAKE_COURSE);
     wcex.lpszClassName = szWindowClass;
     wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
     HWND hWnd;

     hInst = hInstance; // Store instance handle in our global variable

     hWnd = CreateWindow(szWindowClass, szTitle, WS_SYSMENU,
          CW_USEDEFAULT, 0, 1000, 800, NULL, NULL, hInstance, NULL);

     if (!hWnd)
     {
          return FALSE;
     }

     ShowWindow(hWnd, nCmdShow);
     UpdateWindow(hWnd);

     return TRUE;
}


bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC, int width, int height, int x, int y)
{
     // Load the bitmap image file
     HBITMAP hBitmap;
     hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, x, y,
          LR_LOADFROMFILE);
     int d = GetLastError();
     // Verify that the image was loaded
     if (hBitmap == NULL) {

          ::MessageBox(NULL, __T("LoadImage Failed"), __T("Error"), MB_OK);
          int a = GetLastError();
          return false;
     }


     // Create a device context that is compatible with the window
     HDC hLocalDC;
     hLocalDC = ::CreateCompatibleDC(hWinDC);
     // Verify that the device context was created
     if (hLocalDC == NULL) {
          ::MessageBox(NULL, __T("CreateCompatibleDC Failed"), __T("Error"), MB_OK);
          return false;
     }

     // Get the bitmap's parameters and verify the get
     BITMAP qBitmap;
     int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP),
          reinterpret_cast<LPVOID>(&qBitmap));
     if (!iReturn) {
          ::MessageBox(NULL, __T("GetObject Failed"), __T("Error"), MB_OK);
          return false;
     }

     // Select the loaded bitmap into the device context
     HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
     if (hOldBmp == NULL) {
          ::MessageBox(NULL, __T("SelectObject Failed"), __T("Error"), MB_OK);
          return false;
     }

     // Blit the dc which holds the bitmap onto the window's dc
     BOOL qRetBlit = ::BitBlt(hWinDC, width, height, qBitmap.bmWidth, qBitmap.bmHeight,
          hLocalDC, 0, 0, SRCCOPY);
     if (!qRetBlit) {
          ::MessageBox(NULL, __T("Blit Failed"), __T("Error"), MB_OK);
          return false;
     }

     // Unitialize and deallocate resources
     ::SelectObject(hLocalDC, hOldBmp);
     ::DeleteDC(hLocalDC);
     ::DeleteObject(hBitmap);
     return true;
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     if (bOne)
     {
          bOne = false;
          coords[0].x = 2;
          coords[0].y = 2;
          coords[1].x = 2;
          coords[1].y = 3;
          coords[2].x = 2;
          coords[2].y = 4;
          oldCoords[0].x = 2;
          oldCoords[0].y = 2;
          oldCoords[1].x = 2;
          oldCoords[1].y = 3;
          oldCoords[2].x = 2;
          oldCoords[2].y = 4;
     }
     int wmId, wmEvent;
     PAINTSTRUCT ps;
     HDC hdc;
     HDC hcompdc;
     HPEN hPen;
     HBRUSH hBr;
     HBITMAP hBmp;
     SetTimer(hWnd, 1, (game_speed / level * 2), NULL);
     switch (message)
     {
     case WM_TIMER:
          move(headDirection, hWnd);
          InvalidateRect(hWnd, 0, FALSE);
          break;
     case WM_KEYDOWN:
          switch (wParam)
          {
               move(headDirection, hWnd);
          case VK_UP:
               if (headDirection != down) headDirection = up;
               break;
          case VK_DOWN:
               if (headDirection != up) headDirection = down;
               break;
          case VK_LEFT:
               if (headDirection != right) headDirection = left;
               break;
          case VK_RIGHT:
               if (headDirection != left) headDirection = right;
               break;
          case VK_ESCAPE:
               DestroyWindow(hWnd);
               break;
          }
     case WM_COMMAND:
          wmId = LOWORD(wParam);
          wmEvent = HIWORD(wParam);
          // Parse the menu selections:
          switch (wmId)
          {
          case IDM_ABOUT:
               DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
               break;
          case IDM_EXIT:
               DestroyWindow(hWnd);
               break;
          case IDC_MAP1:
               Map_level = 1;
               newLevel(hWnd);
               putApple(hWnd);
               break;
          case IDC_MAP2:
               Map_level = 2;
               newLevel(hWnd);
               putApple(hWnd);
               break;
          case IDC_MAP3:
               Map_level = 3;
               newLevel(hWnd);
               putApple(hWnd);
               break;
          case IDC_MAP4:
               Map_level = 4;
               newLevel(hWnd);
               putApple(hWnd);
               break;
          case IDC_MAP5:
               Map_level = 5;
               newLevel(hWnd);
               putApple(hWnd);
               break;
          case IDC_SLOW:
               game_speed = 200;
               break;
          case IDC_NORMAL:
               game_speed = 150;
               break;
          case IDC_FAST:
               game_speed = 100;
               break;
          default:
               return DefWindowProc(hWnd, message, wParam, lParam);
          }
          break;
     case WM_PAINT:
          hdc = BeginPaint(hWnd, &ps);
          // TODO: Add any drawing code here...
          for (int i = 0; i < 13; i++)
          {
               for (int j = 0; j < 13; j++)
               {
                    if (map[i][j] == GRASS_IND)
                    {
                        /* hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP5);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         */
                         LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/gr.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                    }
                     else if (map[i][j] == SNAKE_IND)
                    {
                    if (i == coords[bodySize - 1].x && j == coords[bodySize - 1].y)
                    {
                    if (headDirection == up)
                    {
                      /*   hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP10);
                         hcompdc = CreateCompatibleDC(hdc);
                         SelectObject(hcompdc, hBmp);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         //Rectangle(hdc, 40 * j, 40 * i, 40 * (j + 1), 40 * (i + 1));
                         BitBlt(hdc, 50 * j, 50 * i, 50, 50, hcompdc, 0, 0, SRCCOPY);
                         */
                         LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/snake_headup.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                    }
                    if (headDirection == down)
                    {
                      /*   hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP9);
                         hcompdc = CreateCompatibleDC(hdc);
                         SelectObject(hcompdc, hBmp);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         //Rectangle(hdc, 40 * j, 40 * i, 40 * (j + 1), 40 * (i + 1));
                         BitBlt(hdc, 50 * j, 50 * i, 50, 50, hcompdc, 0, 0, SRCCOPY);
                         */
                         LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/snake_headdown.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                    }
                    if (headDirection == left)
                    {
                       /*  hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP8);
                         hcompdc = CreateCompatibleDC(hdc);
                         SelectObject(hcompdc, hBmp);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         //Rectangle(hdc, 40 * j, 40 * i, 40 * (j + 1), 40 * (i + 1));
                         BitBlt(hdc, 50 * j, 50 * i, 50, 50, hcompdc, 0, 0, SRCCOPY);
                         */
                         LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/snake_headleft.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                    }
                    if (headDirection == right || headDirection == 5)
                    {
                       /*  hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP7);
                         hcompdc = CreateCompatibleDC(hdc);
                         SelectObject(hcompdc, hBmp);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         //Rectangle(hdc, 40 * j, 40 * i, 40 * (j + 1), 40 * (i + 1));
                         BitBlt(hdc, 50 * j, 50 * i, 50, 50, hcompdc, 10 , 0 , SRCCOPY);
                         */
                         LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/snake_head.bmp"), hdc, 50 * j, 50 * i, 50, 50);

                    }
                    }
                    else if (i == coords[0].x && j == coords[0].y)
                    {
                    if (coords[0].x - 1 == coords[1].x)
                    {
                         /*hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP11);
                         hcompdc = CreateCompatibleDC(hdc);
                         SelectObject(hcompdc, hBmp);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         //Rectangle(hdc, 40 * j, 40 * i, 40 * (j + 1), 40 * (i + 1));
                         BitBlt(hdc, 50 * j, 50 * i, 50, 50, hcompdc, 0, 0, SRCCOPY);
                         */
                    LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/new_snake_tail_up.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                    }
                    if (coords[0].x + 1 == coords[1].x)
                    {
                        /* hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP13);
                         hcompdc = CreateCompatibleDC(hdc);
                         SelectObject(hcompdc, hBmp);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         //Rectangle(hdc, 40 * j, 40 * i, 40 * (j + 1), 40 * (i + 1));
                         BitBlt(hdc, 50 * j, 50 * i, 50, 50, hcompdc, 0, 0, SRCCOPY);
                         */
                    LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/new_snake_tail_down.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                    }
                    if (coords[0].y - 1 == coords[1].y)
                    {
                        /* hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP12);
                         hcompdc = CreateCompatibleDC(hdc);
                         SelectObject(hcompdc, hBmp);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         //Rectangle(hdc, 40 * j, 40 * i, 40 * (j + 1), 40 * (i + 1));
                         BitBlt(hdc, 50 * j, 50 * i, 50, 50, hcompdc, 0, 0, SRCCOPY);
                         */
                    LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/new_snake_tail_left.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                    }
                    if (coords[0].y + 1 == coords[1].y)
                    {
                         /*hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP14);
                         hcompdc = CreateCompatibleDC(hdc);
                         SelectObject(hcompdc, hBmp);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         //Rectangle(hdc, 40 * j, 40 * i, 40 * (j + 1), 40 * (i + 1));
                         BitBlt(hdc, 50 * j, 50 * i, 50, 50, hcompdc, 0, 0, SRCCOPY);
                         */
                         LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/new_snake_tail.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                    }
                    }
                    else
                    {
                         if (headDirection == right || headDirection == left||headDirection==5)
                         {

                              /*hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                              SelectObject(hdc, hPen);
                              hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP6);
                              Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                              hcompdc = CreateCompatibleDC(hdc);
                              SelectObject(hcompdc, hBmp);
                              hBr = CreatePatternBrush(hBmp);
                              SelectObject(hdc, hBr);
                              BitBlt(hdc, 50 * j, 50 * i, 60, 60, hcompdc, 0, 0, SRCCOPY);
                              */
                              LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/new_snake_body.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                         }
                         else if (headDirection == up || headDirection == down)
                         {
                              LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/new_snake_bodyvert.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                         }
                         
                         }
                    }
                    
                    else if (map[i][j] == APPLE_IND)
                    {
                        /* hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP2);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         */
                         LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/blue.bmp"), hdc, 50 * j, 50 * i, 50, 50);
                         
                    }
                    else if (map[i][j] == GAPPLE_IND)
                    {
                         /* hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP2);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         */
                         LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/golden.bmp"), hdc, 50 * j, 50 * i, 50, 50);

                    }
                    else if (map[i][j] == WALL_IND)
                    {
                        /* hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP4);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         */
                         LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/wall2.bmp") , hdc, 50 * j, 50 * i, 50, 50);
                         
                    }
                    else if (map[i][j] == BOMB_IND)
                    {
                         /* hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                         SelectObject(hdc, hPen);
                         hBmp = LoadBitmap(hInst, (LPCWSTR)IDB_BITMAP4);
                         hBr = CreatePatternBrush(hBmp);
                         SelectObject(hdc, hBr);
                         Rectangle(hdc, 50 * j, 50 * i, 50 * (j + 1), 50 * (i + 1));
                         */
                         LoadAndBlitBitmap(__T("C:/Users/Konstantin/documents/visual studio 2013/Projects/Snake_Course/Snake_Course/bomb.bmp"), hdc, 50 * j, 50 * i, 50, 50);

                    }

               }

          }
          wsprintf(text, __T("Score: %d"), pts);
          TextOut(hdc, 650, 50, text, 15);
          wsprintf(text, __T("Size: %i"), bodySize);
          TextOut(hdc, 650, 100, text, 8);
          wsprintf(text, __T("Level: %i"), level);
          TextOut(hdc, 650, 150, text, 8);
          wsprintf(text, __T("Map: %i"), Map_level);
          TextOut(hdc, 650, 200, text, 7);
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
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
     UNREFERENCED_PARAMETER(lParam);
     switch (message)
     {
     case WM_INITDIALOG:
          return (INT_PTR)TRUE;

     case WM_COMMAND:
          if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
          {
               EndDialog(hDlg, LOWORD(wParam));
               return (INT_PTR)TRUE;
          }
          break;
     }
     return (INT_PTR)FALSE;
}
void move(char direction, HWND hWnd)
{
     srand(unsigned int(time(0)));
     int r = rand() % 5;
     for (int i = 0; i < bodySize; i++)
     {
          oldCoords[i].x = coords[i].x;
          oldCoords[i].y = coords[i].y;
     }
     if (headDirection == up)
     {
          if (map[(coords[bodySize - 1].x) - 1][coords[bodySize - 1].y] == GRASS_IND)
          {
               map[(coords[bodySize - 1].x) - 1][coords[bodySize - 1].y] = SNAKE_IND;
               coords[bodySize - 1].x--;
               map[coords[0].x][coords[0].y] = GRASS_IND;
               for (int i = 0; i < bodySize - 1; i++)
               {
                    coords[i].x = oldCoords[i + 1].x;
                    coords[i].y = oldCoords[i + 1].y;
               }
          }
          else if (map[(coords[bodySize - 1].x) - 1][coords[bodySize - 1].y] == APPLE_IND || map[(coords[bodySize - 1].x) - 1][coords[bodySize - 1].y] == GAPPLE_IND)
          {
               map[(coords[bodySize - 1].x) - 1][coords[bodySize - 1].y] = SNAKE_IND;
               bodySize++;
               coords[bodySize - 1].x = coords[bodySize - 2].x - 1;
               coords[bodySize - 1].y = coords[bodySize - 2].y;
               if (r == 4) {
                    putGApple(hWnd);
               }
               else putApple(hWnd);
          }
          else
          {
               gameOver(hWnd);
          }
     }
     if (headDirection == down)
     {
          if (map[(coords[bodySize - 1].x) + 1][coords[bodySize - 1].y] == GRASS_IND)
          {
               map[(coords[bodySize - 1].x) + 1][coords[bodySize - 1].y] = SNAKE_IND;
               coords[bodySize - 1].x++;
               map[coords[0].x][coords[0].y] = GRASS_IND;
               for (int i = 0; i < bodySize - 1; i++)
               {
                    coords[i].x = oldCoords[i + 1].x;
                    coords[i].y = oldCoords[i + 1].y;
               }
          }
          else if (map[(coords[bodySize - 1].x) + 1][coords[bodySize - 1].y] == APPLE_IND || map[(coords[bodySize - 1].x) + 1][coords[bodySize - 1].y] == GAPPLE_IND)
          {
               map[(coords[bodySize - 1].x) + 1][coords[bodySize - 1].y] = SNAKE_IND;
               bodySize++;
               coords[bodySize - 1].x = coords[bodySize - 2].x + 1;
               coords[bodySize - 1].y = coords[bodySize - 2].y;
               if (r == 4) {
                    putGApple(hWnd);
               }
               else putApple(hWnd);
          }
          else
          {
               gameOver(hWnd);
          }
     }
     if (headDirection == left)
     {
          if (map[(coords[bodySize - 1].x)][coords[bodySize - 1].y-1] == GRASS_IND)
          {
               map[(coords[bodySize - 1].x)][coords[bodySize - 1].y-1] = SNAKE_IND;
               coords[bodySize - 1].y--;
               map[coords[0].x][coords[0].y] = GRASS_IND;
               for (int i = 0; i < bodySize - 1; i++)
               {
                    coords[i].x = oldCoords[i + 1].x;
                    coords[i].y = oldCoords[i + 1].y;
               }
          }
          else if (map[(coords[bodySize - 1].x)][coords[bodySize - 1].y - 1] == APPLE_IND || map[(coords[bodySize - 1].x)][coords[bodySize - 1].y-1] == GAPPLE_IND)
          {
               map[(coords[bodySize - 1].x)][coords[bodySize - 1].y-1] = SNAKE_IND;
               bodySize++;
               coords[bodySize - 1].x = coords[bodySize - 2].x;
               coords[bodySize - 1].y = coords[bodySize - 2].y-1;
               if (r == 4) {
                    putGApple(hWnd);
               }
               else putApple(hWnd);
          }
          else
          {
               gameOver(hWnd);
          }
     }
     if (headDirection == right)
     {
          if (map[(coords[bodySize - 1].x)][coords[bodySize - 1].y+1] == GRASS_IND)
          {
               map[(coords[bodySize - 1].x)][coords[bodySize - 1].y+1] = SNAKE_IND;
               coords[bodySize - 1].y++;
               map[coords[0].x][coords[0].y] = GRASS_IND;
               for (int i = 0; i < bodySize - 1; i++)
               {
                    coords[i].x = oldCoords[i + 1].x;
                    coords[i].y = oldCoords[i + 1].y;
               }
          }
          else if (map[(coords[bodySize - 1].x)][coords[bodySize - 1].y + 1] == APPLE_IND || map[(coords[bodySize - 1].x) ][coords[bodySize - 1].y+1] == GAPPLE_IND)
          {
               map[(coords[bodySize - 1].x)][coords[bodySize - 1].y+1] = SNAKE_IND;
               bodySize++;
               coords[bodySize - 1].x = coords[bodySize - 2].x;
               coords[bodySize - 1].y = coords[bodySize - 2].y+1;
               if (r == 4) {
                    putGApple(hWnd);
               }
               else putApple(hWnd);
          }
          else
          {
               gameOver(hWnd);
          }
     }
}
void putApple(HWND hWnd)
{
     pts += 50;
     if (pts >= 2100)
     {
          MessageBox(hWnd, __T("You have won the game!!!"), __T("Snake"), MB_OK);
          gameOverWin(hWnd); 
     }
     if (pts / level == 200 || (pts - 500) / level == 200 || (pts - 1000) / level == 200 || (pts - 1500) / level == 200 || (pts - 2000) / level == 200)
     {
          level++;
     }
     if (pts / Map_level == 200)
     {
          Map_level++; newLevel(hWnd); 
     }
     srand((unsigned int)time(0));
     POSITION rn;
     rn.x = rand() % 13;
     rn.y = rand() % 13;
     while (map[rn.x][rn.y] != GRASS_IND)
     {
          rn.x = rand() % 13;
          rn.y = rand() % 13;
     }
     map[rn.x][rn.y] = APPLE_IND;


}
void putGApple(HWND hWnd)
{
     pts += 50;
     if (pts >= 2100)
     {
          MessageBox(hWnd, __T("You have won the game!!!"), __T("Snake"), MB_OK);
          gameOverWin(hWnd);
     }
     Map_level++; newLevel(hWnd);
     if (Map_level > 5) Map_level = 0;
     srand((unsigned int)time(0));
     POSITION rn;
     rn.x = rand() % 13;
     rn.y = rand() % 13;
     while (map[rn.x][rn.y] != GRASS_IND)
     {
          rn.x = rand() % 13;
          rn.y = rand() % 13;
     }
     map[rn.x][rn.y] = GAPPLE_IND;
}
void gameOverWin(HWND hWnd)
{
     level = 1;
     bodySize = 3;
     coords[0].x = 2;
     coords[0].y = 2;
     coords[1].x = 2;
     coords[1].y = 3;
     coords[2].x = 2;
     coords[2].y = 4;
     oldCoords[0].x = 2;
     oldCoords[0].y = 2;
     oldCoords[1].x = 2;
     oldCoords[1].y = 3;
     oldCoords[2].x = 2;
     oldCoords[2].y = 4;
     headDirection = 5;
     for (int i = 1; i < 12; i++)
     {
          for (int j = 1; j < 12; j++)
          {
               map[i][j] = GRASS_IND;
          }
     }
     map[2][2] = SNAKE_IND;
     map[2][3] = SNAKE_IND;
     map[2][4] = SNAKE_IND;
}
void gameOver(HWND hWnd)
{
     level = 1;
     bodySize = 3;
     Map_level = 1;
     coords[0].x = 2;
     coords[0].y = 2;
     coords[1].x = 2;
     coords[1].y = 3;
     coords[2].x = 2;
     coords[2].y = 4;
     oldCoords[0].x = 2;
     oldCoords[0].y = 2;
     oldCoords[1].x = 2;
     oldCoords[1].y = 3;
     oldCoords[2].x = 2;
     oldCoords[2].y = 4;
     headDirection = 5;
     for (int i = 1; i < 12; i++)
     {
          for (int j = 1; j < 12; j++)
          {
               map[i][j] = GRASS_IND;
          }
     }
     map[2][2] = SNAKE_IND;
     map[2][3] = SNAKE_IND;
     map[2][4] = SNAKE_IND;
     pts = 0;
     putApple(hWnd);
}
void newLevel(HWND hWnd)
{
     nextLevel(hWnd);
     pts = (Map_level * 500) - 500;
     srand((unsigned int)time(0));
     POSITION rn;
     rn.x = rand() % 13;
     rn.y = rand() % 13;
     while (map[rn.x][rn.y] != APPLE_IND)
     {
          rn.x = rand() % 13;
          rn.y = rand() % 13;
     }
     map[rn.x][rn.y] = GRASS_IND;
     for (int i = 0; i < (6 * (pts / 500)); i++)
     {
          randomWall(); 
     }
     for (int i = 0; i < (2 * (pts / 500)); i++)
     {
          randomBomb();
     }
}
void nextLevel(HWND hWnd)
{
     level = 1;
     bodySize = 3;
     coords[0].x = 2;
     coords[0].y = 2;
     coords[1].x = 2;
     coords[1].y = 3;
     coords[2].x = 2;
     coords[2].y = 4;
     oldCoords[0].x = 2;
     oldCoords[0].y = 2;
     oldCoords[1].x = 2;
     oldCoords[1].y = 3;
     oldCoords[2].x = 2;
     oldCoords[2].y = 4;
     headDirection = 5;
     for (int i = 1; i < 12; i++)
     {
          for (int j = 1; j < 12; j++)
          {
               map[i][j] = GRASS_IND;
          }
     }
     map[2][2] = SNAKE_IND;
     map[2][3] = SNAKE_IND;
     map[2][4] = SNAKE_IND;
     pts = 0;
     putApple(hWnd);
}
void randomWall()
{
     srand((unsigned int)time(0));
     POSITION rn;
     rn.x = rand() % 13;
     rn.y = rand() % 13;
     while (map[rn.x][rn.y] != GRASS_IND)
     {
          rn.x = rand() % 13;
          rn.y = rand() % 13;
     }
     map[rn.x][rn.y] = WALL_IND;
}
void randomBomb()
{
     srand((unsigned int)time(0));
     POSITION rn;
     rn.x = rand() % 13;
     rn.y = rand() % 13;
     while (map[rn.x][rn.y] != GRASS_IND )
     {
          rn.x = rand() % 13;
          rn.y = rand() % 13;
     }
     map[rn.x][rn.y] = BOMB_IND;
}