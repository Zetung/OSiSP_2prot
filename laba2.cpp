#include <Windows.h>
#include <tchar.h>

//#include <xstring>
//typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,
//	std::allocator<TCHAR> > String;
char* TextSource;
char* TextFF() {
	char TextSource[180];
	DWORD count;
	HANDLE hFile = CreateFile(L"text.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)   //если ошибка то выходим INVALID_HANDLE_VALUE - некоторая переменная с некоторым значением типа #defain INVALID_HANDLE_VALUE Х..З
	{
		MessageBox(NULL, TEXT("Неудалось открыть файл"), TEXT("Warning"), MB_OK);
		CloseHandle(hFile);
		return TextSource;
	}

	
	ReadFile(hFile, &TextSource, sizeof(TextSource), &count, NULL);
	WCHAR szTest[10];
	swprintf_s(szTest, 10, L"%d", count);
	szTest[4] = TextSource[3];
	//MessageBox(NULL, szTest, TEXT("Check"), MB_OK);
	CloseHandle(hFile);
	return TextSource;
}

int getStringWidth(TCHAR* text, HFONT font) {
	HDC dc = GetDC(NULL);
	SelectObject(dc, font);

	RECT rect = { 0, 0, 0, 0 };
	DrawText(dc, text, wcslen(text), &rect, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);
	int textWidth = abs(rect.right - rect.left);

	DeleteDC(dc);
	return textWidth;
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = _T("MainFrame");
int APIENTRY _tWinMain(HINSTANCE This, // Дескриптор текущего приложения
	HINSTANCE Prev, // В современных системах всегда 0
	LPTSTR cmd, // Командная строка
	int mode) // Режим отображения окна
{
	HWND hWnd; // Дескриптор главного окна программы
	MSG msg; // Структура для хранения сообщения
	WNDCLASS wc; // Класс окна
   // Определение класса окна
	wc.hInstance = This;
	wc.lpszClassName = WinName; // Имя класса окна
	wc.lpfnWndProc = WndProc; // Функция окна
	wc.style = CS_HREDRAW | CS_VREDRAW; // Стиль окна
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Стандартная иконка
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Стандартный курсор
	wc.lpszMenuName = NULL; // Нет меню
	wc.cbClsExtra = 0; // Нет дополнительных данных класса
	wc.cbWndExtra = 0; // Нет дополнительных данных окна
	// Заполнение окна белым цветом
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	if (!RegisterClass(&wc)) return 0; // Регистрация класса окна
   // Создание окна
	hWnd = CreateWindow(WinName, // Имя класса окна
		_T("Laba2"), // Заголовок окна
		WS_OVERLAPPEDWINDOW, // Стиль окна
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y Размеры окна
		550, // width
		300, // Height
		HWND_DESKTOP, // Дескриптор родительского окна
		NULL, // Нет меню
		This, // Дескриптор приложения
		NULL); // Дополнительной информации нет
	ShowWindow(hWnd, mode); // Показать окно
	TextSource = TextFF();
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // Функция трансляции кодов нажатой клавиши
		DispatchMessage(&msg); // Посылает сообщение функции WndProc()
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{ 
	PAINTSTRUCT ps,psText;
	HDC hdc, hdcText;
	int x, y;
	static int sx, sy;
	int sizeT;
	TCHAR str[] = L"qwertyuiopasdfgh";
	TCHAR* strTemp;

	HFONT newFont;
	RECT rc;
	switch (message)
	{
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
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
		rc.left = 0;
		rc.top = 20;
		rc.bottom = sy / 3;
		rc.right = sx / 3;
		//MessageBoxA(NULL, TextSource, "durachok", MB_OK);
		
		newFont = CreateFont(20, 0, 0, 0, 700, 1, 0, 0,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
		SetTextColor(hdc, RGB(200, 100, 200));
		SelectObject(hdc, newFont);
		DrawText(hdc, str, wcslen(str), &rc,DT_LEFT);
		sizeT = getStringWidth(str, newFont);
		if (sizeT + 10 > rc.right) {
			strTemp = new TCHAR[wcslen(str)];
			_tcscpy(strTemp, str);
			// strTemp = L"\r\n" + strTemp;
			//wcscat(strTemp, L"g\r\n");
			rc.top = rc.top + 20;
			DrawText(hdc, strTemp, wcslen(strTemp), &rc, DT_LEFT);
	
		}
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: PostQuitMessage(0);
		break; 
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}