//lifeGameMain.cpp
#include "lifeGameMain.h"
#include "lifeGameTools.h"

LPCTSTR lpszClassName = TEXT("win01");

//WinMain�֐�
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;

	//�E�B���h�E�N���X�̓o�^
	if (!InitApp(hCurInst))
		return FALSE;
	//���C���E�B���h�E�̍쐬
	if (!InitInstance(hCurInst, nCmdShow))
		return FALSE;
	//���b�Z�[�W���[�v
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0){
		if (bRet == -1){
			break; //�G���[���
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg); //���b�Z�[�W���E�B���h�E�v���V�[�W���ɓ]������B
		}
	}
	return (int)msg.wParam;
}

// �E�B���h�E�N���X�̓o�^
ATOM InitApp(HINSTANCE hInst)
{
	WNDCLASSEX wc; //WNDCLASSEX�^�\���̂�錾�B�ȉ������o�l�ւ̑���B
	wc.cbSize = sizeof(WNDCLASSEX);		//�\���̂̃T�C�Y
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	/* �N���X�̃X�^�C��
	CS_DBLDCLKS�̓_�u���N���b�N�̌��o */
	wc.lpfnWndProc = WndProc;		//�E�B���h�E�v���V�[�W���ւ̃|�C���^
	wc.cbClsExtra = 0;		//�⏕������
	wc.cbWndExtra = 0;		//�⏕������
	wc.hInstance = hInst;		//�C���X�^���X�n���h��
	wc.hIcon = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);		//�A�C�R��
	wc.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);		//�J�[�\��
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//�w�i�u���V
	wc.lpszMenuName = NULL;		//���j���[��(NULL�̓f�t�H���g�̃��j���[�������Ȃ�)
	wc.lpszClassName = lpszClassName;		//�N���X��
	wc.hIconSm = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);		//�T���l�C���A�C�R��

	return (RegisterClassEx(&wc));		//�E�B���h�E�N���X��o�^
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

//�E�B���h�E�̐���
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
	HWND hWnd;

	hWnd = CreateWindow(lpszClassName,		//�o�^����Ă���N���X��
		TEXT("���C�t�Q�[��"),		//�E�B���h�E��
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,		//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,		//x���W
		CW_USEDEFAULT,		//y���W
		CW_USEDEFAULT,		//��
		CW_USEDEFAULT,		//����
		NULL,		//�e�E�B���h�E�̃n���h���A�e�����Ƃ���NULL
		NULL,		//���j���[�n���h���A�N���X���j���[���g���Ƃ���NULL
		hInst,		//�C���X�^���X�n���h��
		NULL);		//�E�B���h�E�쐬�f�[�^
	if (!hWnd)
		return FALSE;

	funcSetClientSize( hWnd, (BSIZE) * CSIZE, (BSIZE) * CSIZE );
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

//�E�B���h�E�v���V�[�W���F�E�B���h�E�ɂ���Ă��郁�b�Z�[�W����������B
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC hdc;
	PAINTSTRUCT ps; //PAINTSTRUCT�\���� �`�����ێ��B
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