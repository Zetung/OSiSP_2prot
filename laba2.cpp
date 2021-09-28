#include <Windows.h>
#include <tchar.h>

//#include <xstring>
//typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,
//	std::allocator<TCHAR> > String;
//char* TextSource;
//char* TextFF() {
//	char TextSource[180];
//	DWORD count;
//	HANDLE hFile = CreateFile(L"text.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (hFile == INVALID_HANDLE_VALUE)
//	{
//		MessageBox(NULL, TEXT("Неудалось открыть файл"), TEXT("Warning"), MB_OK);
//		CloseHandle(hFile);
//		return TextSource;
//	}
//
//	
//	ReadFile(hFile, &TextSource, sizeof(TextSource), &count, NULL);
//	WCHAR szTest[10];
//	swprintf_s(szTest, 10, L"%d", count);
//	szTest[4] = TextSource[3];
//	//MessageBox(NULL, szTest, TEXT("Check"), MB_OK);
//	CloseHandle(hFile);
//	return TextSource;
//}

int nowX, nowY, nowHeight;

int getStringWidth(TCHAR* text, HFONT font) {
	HDC dc = GetDC(NULL);
	SelectObject(dc, font);

	RECT rect = { 0, 0, 0, 0 };
	DrawText(dc, text, wcslen(text), &rect, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);
	int textWidth = abs(rect.right - rect.left);

	DeleteDC(dc);
	return textWidth;
}


void CarryText(HDC hdc, TEXTMETRIC metricT, HFONT newFont, TCHAR* str, int sizeT, RECT *rc) {
	int sizeChar = metricT.tmAveCharWidth;
	int sizeCarry = (sizeT - rc->right) / sizeChar;
	if ((sizeT - rc->right) % sizeChar)
		sizeCarry++;
	TCHAR* strTemp = new TCHAR[sizeCarry];
	int i = 0;
	do {
		strTemp[i] = str[wcslen(str) - sizeCarry + i];
		i++;
	} while (i != sizeCarry);
	strTemp[sizeCarry] = '\0';
	//_tcscpy(strTemp, str);
	rc->top += 20;
	DrawText(hdc, strTemp, wcslen(strTemp), rc, DT_LEFT);
	int sizeTNext = getStringWidth(strTemp, newFont);
	if (sizeTNext - metricT.tmAveCharWidth + 2 > rc->right)
		CarryText(hdc, metricT, newFont, strTemp, sizeTNext, rc);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = _T("MainFrame");
int APIENTRY _tWinMain(HINSTANCE This,
	HINSTANCE Prev,
	LPTSTR cmd,
	int mode)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wc;
	wc.hInstance = This;
	wc.lpszClassName = WinName;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	if (!RegisterClass(&wc)) return 0;

	nowX = CW_USEDEFAULT;
	nowY = CW_USEDEFAULT;
	nowHeight = 300;
	hWnd = CreateWindow(WinName, 
		_T("Laba2"), 
		WS_OVERLAPPEDWINDOW, 
		nowX,
		nowY,
		550, 
		nowHeight,
		HWND_DESKTOP, 
		NULL, 
		This, 
		NULL); 
	ShowWindow(hWnd, mode); 
	//TextSource = TextFF();
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{ 
	PAINTSTRUCT ps;
	HDC hdc;

	int x, y;
	static int sx, sy;

	int sizeT;
	TCHAR str[] = L"qwertyuiopasdfgh";

	TEXTMETRIC metricT;

	HFONT newFont;
	RECT rc{2,2,sy,sx};
	switch (message)
	{
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		nowHeight = sy;
		break;
	
	//case WM_SIZING:
	//	hdc = BeginPaint(hWnd, &ps);
	//	GetTextMetrics(hdc, &metricT);
	//	if (rc.top + metricT.tmHeight > rc.bottom) {
	//		sy = rc.top + metricT.tmHeight * 3;
	//		MoveWindow(hWnd, nowX, nowY, sx, sy, NULL);
	//	}
	//	break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		for (x = 0; x < sx-20; x += sx / 3)
		{
			MoveToEx(hdc, x, 0, NULL);
			LineTo(hdc, x, sy);
		}
		for (y = 0; y < sy; y += sy / 3)
		{
			MoveToEx(hdc, 0, y, NULL);
			LineTo(hdc, sx, y);
		}
		rc.left = 2;
		rc.top = 2;
		rc.bottom = sy / 3;
		rc.right = sx / 3;
		//MessageBoxA(NULL, TextSource, "dur", MB_OK);
		newFont = CreateFont(20, 0, 0, 0, 700, 1, 0, 0,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
		SetTextColor(hdc, RGB(200, 100, 200));
		SelectObject(hdc, newFont);
		GetTextMetrics(hdc, &metricT);
		DrawText(hdc, str, wcslen(str), &rc,DT_LEFT);
		sizeT = getStringWidth(str, newFont);
		if (sizeT - metricT.tmAveCharWidth + 2 > rc.right) {
			CarryText(hdc, metricT, newFont, str, sizeT, &rc);
			if (rc.top + metricT.tmHeight > rc.bottom) {
				nowHeight = rc.top + metricT.tmHeight * 3;
				MoveWindow(hWnd, nowX, nowY, sx, nowHeight, NULL);
				//SetWindowPos(hWnd,HWND_TOP, nowX, nowY, sx, nowHeight, )
			}
		}
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: PostQuitMessage(0);
		break; 
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}