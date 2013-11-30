//lifeGameMain.cpp
#include "lifeGameMain.h"
#include "lifeGameTools.h"

LPCTSTR lpszClassName = TEXT("win01");

//WinMain関数
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;

	//ウィンドウクラスの登録
	if (!InitApp(hCurInst))
		return FALSE;
	//メインウィンドウの作成
	if (!InitInstance(hCurInst, nCmdShow))
		return FALSE;
	//メッセージループ
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0){
		if (bRet == -1){
			break; //エラー回避
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg); //メッセージをウィンドウプロシージャに転送する。
		}
	}
	return (int)msg.wParam;
}

// ウィンドウクラスの登録
ATOM InitApp(HINSTANCE hInst)
{
	WNDCLASSEX wc; //WNDCLASSEX型構造体を宣言。以下メンバ値への代入。
	wc.cbSize = sizeof(WNDCLASSEX);		//構造体のサイズ
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	/* クラスのスタイル
	CS_DBLDCLKSはダブルクリックの検出 */
	wc.lpfnWndProc = WndProc;		//ウィンドウプロシージャへのポインタ
	wc.cbClsExtra = 0;		//補助メモリ
	wc.cbWndExtra = 0;		//補助メモリ
	wc.hInstance = hInst;		//インスタンスハンドル
	wc.hIcon = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);		//アイコン
	wc.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);		//カーソル
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//背景ブラシ
	wc.lpszMenuName = NULL;		//メニュー名(NULLはデフォルトのメニューを持たない)
	wc.lpszClassName = lpszClassName;		//クラス名
	wc.hIconSm = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);		//サムネイルアイコン

	return (RegisterClassEx(&wc));		//ウィンドウクラスを登録
}

static VOID funcSetClientSize( HWND hWnd, LONG sx, LONG sy )
{
    RECT rc1;
    RECT rc2;
    
    GetWindowRect( hWnd, &rc1 );
    GetClientRect( hWnd, &rc2 );
    sx += ((rc1.right - rc1.left) - (rc2.right - rc2.left));
    sy += ((rc1.bottom - rc1.top) - (rc2.bottom - rc2.top));
    SetWindowPos( hWnd, NULL, 0, 0, sx, sy, (SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOMOVE) );
}

//ウィンドウの生成
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
	HWND hWnd;

	hWnd = CreateWindow(lpszClassName,		//登録されているクラス名
		TEXT("ライフゲーム"),		//ウィンドウ名
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,		//ウィンドウスタイル
		CW_USEDEFAULT,		//x座標
		CW_USEDEFAULT,		//y座標
		CW_USEDEFAULT,		//幅
		CW_USEDEFAULT,		//高さ
		NULL,		//親ウィンドウのハンドル、親を作るときはNULL
		NULL,		//メニューハンドル、クラスメニューを使うときはNULL
		hInst,		//インスタンスハンドル
		NULL);		//ウィンドウ作成データ
	if (!hWnd)
		return FALSE;

	funcSetClientSize( hWnd, (BSIZE) * CSIZE, (BSIZE) * CSIZE );
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

//ウィンドウプロシージャ：ウィンドウにやってくるメッセージを処理する。
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC hdc;
	PAINTSTRUCT ps; //PAINTSTRUCT構造体 描画情報を保持。
	int i,j,k;
	static HPEN hPen;
	static HBRUSH hBrush[4];
	static int lifetable[BSIZE][BSIZE], lifesumtable[BSIZE][BSIZE];
	RECT rc;

	switch (msg) {
	case WM_CREATE:
		hPen = CreatePen(PS_SOLID, 1, LINECOLOR);
		hBrush[0] = CreateSolidBrush(LIGHTCOLOR);
		hBrush[1] = CreateSolidBrush(DARKCOLOR);
		hBrush[2] = CreateSolidBrush(ALIVE2COLOR);
		hBrush[3] = CreateSolidBrush(ALIVE3COLOR);

		SetTimer(hWnd, ID_PERIOD, TIME_PRERIOD, NULL);

		for (i = 0; i < BSIZE; i++ ) {
			for (j = 0; j < BSIZE; j++ ) {
				lifetable[i][j] = 0;
				lifesumtable[i][j] = 0;
			}
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SelectObject(hdc, hPen);
		for (k = 0; k < 1 + BSIZE; k++) {
			CellDraw(hdc, k);
		}

		CellFill(hdc, hBrush ,lifetable ,lifesumtable);
		break;
	case WM_LBUTTONDOWN:
		CellClicked(LOWORD(lp), HIWORD(lp), lifetable);
		break;
	case WM_LBUTTONDBLCLK:
		StillLifeTub(LOWORD(lp), HIWORD(lp), lifetable);
		break;
	case WM_TIMER:
		if (wp != ID_PERIOD)
			return DefWindowProc(hWnd, msg, wp, lp);
		GetClientRect(hWnd, &rc);

		CellCalculation(lifetable ,lifesumtable);

		InvalidateRect(hWnd, NULL, FALSE);

		break;
	case WM_DESTROY:
		KillTimer(hWnd, ID_PERIOD);
		DeleteObject(hPen);
		DeleteObject(hBrush[0]);
		DeleteObject(hBrush[1]);
		PostQuitMessage(0);
		break;
	default:
		return (DefWindowProc(hWnd, msg, wp, lp));
	}
		return 0;
}