#include <tchar.h>
#include <Windows.h>
#include "resource1.h"
#include "Collision.h"
#include <math.h>

ShapeMaker* shapeMaker;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE PrevhInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	WNDCLASS WndClass;
	MSG Message;
	HWND hWnd;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		0, 0, winWidth, winHeight,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	shapeMaker = new ShapeMaker(hWnd);

	while (true)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
		}
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE :
		return 0;

	case WM_DESTROY :
		PostQuitMessage(0);
		return 0;

	case WM_MOUSEMOVE :
		shapeMaker->SetMouseAxis(LOWORD(lParam), HIWORD(lParam));
		shapeMaker->Draw();
		return 0;

	case WM_COMMAND :
		shapeMaker->MenuProc(LOWORD(wParam));
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

ShapeMaker::ShapeMaker(HWND hWnd)
	: m_hWnd(hWnd)
{
	init();
}

// ShapeMaker 초기화 함수.
void ShapeMaker::init()
{
	m_hdc = GetDC(m_hWnd);
	m_DrawVersion = DRAW_VERSION::RECT;
	m_pDrawHandler[RECT] = &ShapeMaker::RectDraw;
	m_pDrawHandler[CIRCLE] = &ShapeMaker::CircleDraw;
}

ShapeMaker::~ShapeMaker()
{
	ReleaseDC(m_hWnd, m_hdc);
}

void ShapeMaker::SetMouseAxis(INT x, INT y)
{
	m_MouseAxis.x = x;
	m_MouseAxis.y = y;
	return;
}

void ShapeMaker::Draw()
{
	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);
	(this->*m_pDrawHandler[m_DrawVersion])();
	return;
}

BOOL ShapeMaker::IsRectColided()
{
	using namespace DRAW_VALUE;

	if (m_MouseAxis.x + mouseRectLength < centRectX1)
	{
		return false;
	}
	else if (m_MouseAxis.x - mouseRectLength > centRectX2)
	{
		return false;
	}
	else if (m_MouseAxis.y + mouseRectLength < centRectY1)
	{
		return false;
	}
	else if (m_MouseAxis.y - mouseRectLength > centRectY2)
	{
		return false;
	}

	return true;
}

BOOL ShapeMaker::IsCircleColided()
{
	using namespace DRAW_VALUE;

	INT radius = (centRectX2 - centRectX1) / 2;
	INT x = CirclePointX - m_MouseAxis.x;
	INT y = CirclePointY - m_MouseAxis.y;
	
	m_Distance = fabs(sqrt(x*x + y*y));

	if (m_Distance <= radius + mouseCircleLength)
	{
		return true;
	}
	return false;
}

void ShapeMaker::RectColorSelect()
{
	using namespace DRAW_VALUE;

	if (IsRectColided())
	{
		m_ShapeBrush = CreateSolidBrush(colideColor);
	}
	else
	{
		m_ShapeBrush = CreateSolidBrush(normalColor);
	}

	m_OldBrush = (HBRUSH)SelectObject(m_hdc, m_ShapeBrush);

	return;
}

void ShapeMaker::CircleColorSelect()
{
	using namespace DRAW_VALUE;

	if (IsCircleColided())
	{
		m_ShapeBrush = CreateSolidBrush(colideColor);
	}
	else
	{
		m_ShapeBrush = CreateSolidBrush(normalColor);
	}

	m_OldBrush = (HBRUSH)SelectObject(m_hdc, m_ShapeBrush);

	return;
}


void ShapeMaker::MenuProc(INT selectedMenuId)
{
	switch (selectedMenuId)
	{
	case ID_DRAW_RECT :
		m_DrawVersion = RECT;
		break;

	case ID_DRAW_CIRCLE :
		m_DrawVersion = CIRCLE;
		break;

	case ID_DRAW_RECT40003 :
		m_DrawVersion = RECT_CIRCLE;
		break;

	case ID_FILE_EXIT :
		DestroyWindow(m_hWnd);
		break;

	}

	return;
}

BOOL ShapeMaker::RectDraw()
{
	using namespace DRAW_VALUE;
	// Center Rect
	RectColorSelect();
	Rectangle(m_hdc, centRectX1, centRectY1, centRectX2, centRectY2);
	
	SelectObject(m_hdc, m_OldBrush);

	// Mouse Rect
	Rectangle(m_hdc, m_MouseAxis.x - mouseRectLength, m_MouseAxis.y - mouseRectLength,
		m_MouseAxis.x + mouseRectLength, m_MouseAxis.y + mouseRectLength);

	return true;
}

BOOL ShapeMaker::CircleDraw()
{
	using namespace DRAW_VALUE;
	// Center Rect
	CircleColorSelect();
	Ellipse(m_hdc, centRectX1, centRectY1, centRectX2, centRectY2);

	SelectObject(m_hdc, m_OldBrush);

	// Mouse Rect
	Ellipse(m_hdc, m_MouseAxis.x - mouseCircleLength, m_MouseAxis.y - mouseCircleLength,
		m_MouseAxis.x + mouseCircleLength, m_MouseAxis.y + mouseCircleLength);

	// Distance


	return true;
}
