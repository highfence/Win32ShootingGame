
// Basic Settings
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Collision");

class RectMaker 
{
public :
	
	RectMaker(HWND);
	~RectMaker();

	void init();
	void SetMouseAxis(INT, INT);
	void DrawRect();

private :

	RectMaker();

	HWND m_hWnd;
	HDC m_hdc;
	POINT m_MouseAxis;

};

namespace RECT_VALUE
{
	INT centRectX1 = 300;
	INT centRectY1 = 300;
	INT centRectX2 = 600;
	INT centRectY2 = 600;

	COLORREF normalColor = RGB(255, 100, 0);
	COLORREF colideColor = RGB(0, 100, 255);

	INT mouseRectLength = 50;
};