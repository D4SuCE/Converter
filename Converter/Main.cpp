#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include "resource.h"
#include <algorithm>
#include <exception>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

RECT rc;

// Main function
int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hwnd; 
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MAIN_ICON));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MAIN_ICON));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
	{
		static HWND hEditBinaryToDecimal;
		static HWND hEditDecimalToBinary;
		switch (uMsg)
		{
			// Draw Background
			case WM_PAINT:
			{
				GetClientRect(hWnd, &rc);
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				HBRUSH backgroundBrush = CreateSolidBrush(RGB(249, 87, 56));
				FillRect(hdc, &rc, backgroundBrush);
				DeleteObject(backgroundBrush);
				EndPaint(hWnd, &ps);
			}
			return 0;

			// Draw Text color
			case WM_CTLCOLORSTATIC:
			{
				HDC hdcStatic = reinterpret_cast<HDC>(wParam);
				SetTextColor(hdcStatic, 0xFFFFFF);
				SetBkColor(hdcStatic, RGB(248, 87, 56));				
				return reinterpret_cast<LRESULT>(GetStockObject(NULL_BRUSH));
			}
			return 0;

			// For creating new elements
			case WM_CREATE:
			{ 
				// Main Window Label
				HWND hMainLabel = CreateWindow(
					L"STATIC",
					L"CONVERTER\nBINARY TO DECIMAL",
					WS_CHILD | WS_VISIBLE | SS_CENTER,
					40, 25, 300, 50, hWnd, nullptr, nullptr, nullptr
				);

				// For Binary To Decimal
				HWND hLabelBinaryToDecimal = CreateWindow(
					L"STATIC",
					L"Binary To Decimal:",
					WS_CHILD | WS_VISIBLE,
					40, 100, 140, 20, hWnd, nullptr, nullptr, nullptr
				);

				hEditBinaryToDecimal = CreateWindow(
					L"EDIT",
					nullptr,
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					40, 120, 300, 20, hWnd, nullptr, nullptr, nullptr
				);

				HWND hButtonBinaryToDecimal = CreateWindow(
					L"BUTTON",
					L"Convert",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					40, 145, 300, 30, hWnd, reinterpret_cast<HMENU>(1), nullptr, nullptr
				);

				// For Decimal To Binary
				HWND hLabelDecimalToBinary = CreateWindow(
					L"STATIC",
					L"Decimal To Binary:",
					WS_CHILD | WS_VISIBLE,
					40, 210, 140, 20, hWnd, nullptr, nullptr, nullptr
				);

				hEditDecimalToBinary = CreateWindow(
					L"EDIT",
					nullptr,
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					40, 230, 300, 20, hWnd, nullptr, nullptr, nullptr
				);

				HWND hButtonDecimalToBinary = CreateWindow(
					L"BUTTON",
					L"Convert",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					40, 255, 300, 30, hWnd, reinterpret_cast<HMENU>(2), nullptr, nullptr
				);

				// Creating FONTS
				HFONT hFont = CreateFont(20, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
				HFONT hFontUnderLine = CreateFont(23, 0, 0, 0, FW_REGULAR, TRUE, TRUE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
				SendMessage(hMainLabel, WM_SETFONT, reinterpret_cast<WPARAM>(hFontUnderLine), TRUE);
				SendMessage(hLabelBinaryToDecimal, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
				SendMessage(hEditBinaryToDecimal, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
				SendMessage(hButtonBinaryToDecimal, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
				SendMessage(hLabelDecimalToBinary, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
				SendMessage(hEditDecimalToBinary, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
				SendMessage(hButtonDecimalToBinary, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
			}
			return 0;

			// Commands, when buttons is clicked
			case WM_COMMAND:
			{
				switch (LOWORD(wParam))
				{
					// Binary to decimal
					case 1:
					{
						try
						{
							char buffer[30];
							GetWindowTextA(hEditBinaryToDecimal, buffer, 30);

							std::string strNumber = buffer;

							long long intNumber = std::stoll(buffer);

							int length = strNumber.size();

							int result = 0;

							int lLength = length - 1;

							for (int i = 0; i < length; i++)
							{
								std::stringstream ss;
								int num;
								ss << strNumber[i];
								ss >> num;
								result += num * std::pow(2, lLength);
								lLength--;
							}

							strNumber = std::to_string(result);
							LPSTR decimal = const_cast<char *>(strNumber.c_str());

							SetWindowTextA(hEditDecimalToBinary, decimal);
						}
						catch (const std::exception &e)
						{
							MessageBoxA(hWnd, e.what(), "Warning", MB_ICONWARNING);
						}						
						break;
					}

					// Decimal to binary
					case 2:
					{
						try
						{
							char buffer[30];
							GetWindowTextA(hEditDecimalToBinary, buffer, 30);

							long long intNumber = std::stoll(buffer);

							std::vector<int> result;

							while (intNumber != 1)
							{
								result.push_back(intNumber % 2);
								intNumber = intNumber / 2;
							}

							result.push_back(intNumber);

							std::reverse(result.begin(), result.end());

							std::string preBinary = "";

							for (auto i : result)
							{
								preBinary += std::to_string(i);
							}

							LPSTR binary = const_cast<char *>(preBinary.c_str());

							SetWindowTextA(hEditBinaryToDecimal, binary);
						}
						catch (const std::exception &e)
						{
							MessageBoxA(hWnd, e.what(), "Warning", MB_ICONWARNING);
						}
						break;
					}
				}				
			}
			return 0;

			// Destroy window, when close
			case WM_DESTROY:
			{
				PostQuitMessage(EXIT_SUCCESS);
			}
			return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	};
	wc.lpszClassName = L"MyAppClass";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		return EXIT_FAILURE;
	}

	if (hwnd = CreateWindow(wc.lpszClassName, L"Converter", WS_SYSMENU, (GetSystemMetrics(SM_CXSCREEN) - 400) / 2, (GetSystemMetrics(SM_CYSCREEN) - 400) / 2, 400, 400, nullptr, nullptr, wc.hInstance, nullptr);
		hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}