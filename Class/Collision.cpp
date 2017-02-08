#include <tchar.h>
#include <Windows.h>
#include "Collision.h"

RectMaker* rectMaker;

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
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		0, 0, winWidth, winHeight,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	rectMaker = new RectMaker(hWnd);

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
	case WM_DESTROY :
		PostQuitMessage(0);
		return 0;

	case WM_MOUSEMOVE :
		rectMaker->SetMouseAxis(LOWORD(lParam), HIWORD(lParam));
		rectMaker->DrawRect();
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

RectMaker::RectMaker(HWND hWnd)
	: m_hWnd(hWnd)
{
	init();
}

// ShapeMaker 초기화 함수.
void RectMaker::init()
{
	m_hdc = GetDC(m_hWnd);
}

RectMaker::~RectMaker()
{
	ReleaseDC(m_hWnd, m_hdc);
}

void RectMaker::SetMouseAxis(INT x, INT y)
{
	m_MouseAxis.x = x;
	m_MouseAxis.y = y;
	return;
}

void RectMaker::DrawRect()
{
	using namespace RECT_VALUE;

	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);

	// Center Rect
	RectColorSelect();
	Rectangle(m_hdc, centRectX1, centRectY1, centRectX2, centRectY2);
	
	SelectObject(m_hdc, m_OldBrush);

	// Mouse Rect
	Rectangle(m_hdc, m_MouseAxis.x - mouseRectLength, m_MouseAxis.y - mouseRectLength,
		m_MouseAxis.x + mouseRectLength, m_MouseAxis.y + mouseRectLength);


	return;
}

bool RectMaker::IsRectColided()
{
	using namespace RECT_VALUE;

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

void RectMaker::RectColorSelect()
{
	using namespace RECT_VALUE;

	if (IsRectColided())
	{
		m_RectBrush = CreateSolidBrush(colideColor);
	}
	else
	{
		m_RectBrush = CreateSolidBrush(normalColor);
	}

	m_OldBrush = (HBRUSH)SelectObject(m_hdc, m_RectBrush);

	return;
}