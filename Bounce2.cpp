// Bounce2.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Bounce2.h"

#define MAX_LOADSTRING 100
#define ID_TIMER 1 

static LOGPALETTE * plp;
HPALETTE CreateRoutine(HWND hwnd)
{
	HPALETTE hPalette;
	int i;
	plp = (LOGPALETTE *)malloc(sizeof(LOGPALETTE) + 33 * sizeof(PALETTEENTRY));
	plp->palVersion = 0x0300;
	plp->palNumEntries = 34;
	for (i = 0; i < 34; i++)
	{
		plp->palPalEntry[i].peRed = 255;
		plp->palPalEntry[i].peGreen = (i == 0 ? 0 : 255);
		plp->palPalEntry[i].peBlue = (i == 0 ? 0 : 255);
		plp->palPalEntry[i].peFlags = (i == 33 ? 0 : PC_RESERVED);
	}
	hPalette = CreatePalette(plp);
	SetTimer(hwnd, ID_TIMER, 50, NULL);
	return hPalette;
}
void PaintRoutine(HDC hdc, int cxClient, int cyClient)
{
	HBRUSH hBrush;
	int i, x1, x2, y1, y2;
	RECT rect;
	// Draw window background using palette index 33
	SetRect(&rect, 0, 0, cxClient, cyClient);
	hBrush = CreateSolidBrush(PALETTEINDEX(33));
	FillRect(hdc, &rect, hBrush);
	DeleteObject(hBrush);
	// Draw the 33 balls
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (i = 0; i < 33; i++)
	{
		x1 = i * cxClient / 33;
		x2 = (i + 1)* cxClient / 33;
		if (i < 9)
		{
			y1 = i * cyClient / 9;
			y2 = (i + 1) * cyClient / 9;
		}
		else if (i < 17)
		{
			y1 = (16 - i) * cyClient / 9;
			y2 = (17 - i) * cyClient / 9;
		}
		else if (i < 25)
		{
			y1 = (i - 16) * cyClient / 9;
			y2 = (i - 15) * cyClient / 9;
		}
		else
		{
			y1 = (32 - i) * cyClient / 9;
			y2 = (33 - i) * cyClient / 9;
		}
		hBrush = CreateSolidBrush(PALETTEINDEX(i));
		SelectObject(hdc, hBrush);
		Ellipse(hdc, x1, y1, x2, y2);
		DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
	}
	return;
}
void TimerRoutine(HDC hdc, HPALETTE hPalette)
{
	static BOOL bLeftToRight = TRUE;
	static int iBall = 0;
	// Set old ball to white
	plp->palPalEntry[iBall].peGreen = 255;
	plp->palPalEntry[iBall].peBlue = 255;
	iBall += (bLeftToRight ? 1 : -1);
	if (iBall == (bLeftToRight ? 33 : -1))
	{
		iBall = (bLeftToRight ? 31 : 1);
		bLeftToRight ^= TRUE;
	}
	// Set new ball to red
	plp->palPalEntry[iBall].peGreen = 0;
	plp->palPalEntry[iBall].peBlue = 0;
	// Animate the palette
	int adfasdf = AnimatePalette(hPalette, 0, 33, plp->palPalEntry);
	return;
}
void DestroyRoutine(HWND hwnd, HPALETTE hPalette)
{
	KillTimer(hwnd, ID_TIMER);
	DeleteObject(hPalette);
	free(plp);
	return;
}

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BOUNCE2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOUNCE2));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

BOOL CheckDisplay(HWND hwnd)
{
	HDC hdc;
	int iPalSize;
	hdc = GetDC(hwnd);
	iPalSize = GetDeviceCaps(hdc, SIZEPALETTE);
	ReleaseDC(hwnd, hdc);
	if (iPalSize != 256)
	{
		//MessageBox(hwnd, TEXT("This program requires that the video ") TEXT("display mode have a 256-color palette."), szWindowClass, MB_ICONERROR);
		//return FALSE;
	}
	return TRUE;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOUNCE2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BOUNCE2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HPALETTE hPalette;
	static int cxClient, cyClient;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_CREATE:
		if (!CheckDisplay(hwnd))
			return -1;
		hPalette = CreateRoutine(hwnd);
		return 0;

	case WM_DISPLAYCHANGE:
		if (!CheckDisplay(hwnd))
			DestroyWindow(hwnd);
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectPalette(hdc, hPalette, FALSE);
		RealizePalette(hdc);
		PaintRoutine(hdc, cxClient, cyClient);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		SelectPalette(hdc, hPalette, FALSE);
		TimerRoutine(hdc, hPalette);
		ReleaseDC(hwnd, hdc);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_QUERYNEWPALETTE:
		if (!hPalette)
			return FALSE;
		hdc = GetDC(hwnd);
		SelectPalette(hdc, hPalette, FALSE);
		RealizePalette(hdc);
		InvalidateRect(hwnd, NULL, TRUE);
		ReleaseDC(hwnd, hdc);
		return TRUE;
	case WM_PALETTECHANGED:
		if (!hPalette || (HWND)wParam == hwnd)
			break;
		hdc = GetDC(hwnd);
		SelectPalette(hdc, hPalette, FALSE);
		RealizePalette(hdc);
		UpdateColors(hdc);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_DESTROY:
		DestroyRoutine(hwnd, hPalette);

		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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
