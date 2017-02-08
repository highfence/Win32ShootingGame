
// Basic Settings
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Collision");

// Global Const Values
const int winWidth = 1024;
const int winHeight = 768;

class ShapeMaker 
{
public :
	
	ShapeMaker(HWND);
	~ShapeMaker();

	void init();
	void SetMouseAxis(INT, INT);
	void Draw();
	BOOL RectDraw();
	BOOL CircleDraw();
	void RectColorSelect();
	void CircleColorSelect();
	void MenuProc(INT);
	BOOL IsRectColided();
	BOOL IsCircleColided();

private :
	enum DRAW_VERSION
	{
		RECT = 0, CIRCLE = 1, RECT_CIRCLE = 2, VERSION_NUM = 3
	};

	ShapeMaker();

	HWND m_hWnd;
	HDC m_hdc;
	HBRUSH m_OldBrush;
	HBRUSH m_ShapeBrush;
	POINT m_MouseAxis;
	INT m_DrawVersion;
	FLOAT m_Distance;

	// Function Pointer Handler
	BOOL(ShapeMaker::* m_pDrawHandler[VERSION_NUM])();

};

namespace DRAW_VALUE
{
	const INT centRectX1 = 300;
	const INT centRectY1 = 300;
	const INT centRectX2 = 600;
	const INT centRectY2 = 600;

	const COLORREF normalColor = RGB(0, 100, 255);
	const COLORREF colideColor = RGB(255, 100, 0);

	const INT mouseRectLength = 50;
	const INT mouseCircleLength = 50;

	const INT CirclePointX = centRectX1 + (centRectX2 - centRectX1) / 2;
	const INT CirclePointY = centRectY1 + (centRectY2 - centRectY1) / 2;
};