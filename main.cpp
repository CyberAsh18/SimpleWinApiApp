#define UNICODE
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

const wchar_t className[] = L"myWindowClass";
const wchar_t windowName[] = L"ssoc demo - window bg color randomizer";

HWND hButton;
HBRUSH hBrush = nullptr;

COLORREF GenerateRandomColor()
{
    return RGB(rand() % 256, rand() % 256, rand() % 256);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        if (hBrush)
            DeleteObject(hBrush);
        break;
    case WM_CTLCOLORBTN:
    {
        HDC hdc = reinterpret_cast<HDC>(wParam);
        SetBkMode(hdc, TRANSPARENT);
        return reinterpret_cast<LRESULT>(GetStockObject(NULL_BRUSH));
    }
    case WM_COMMAND:
        if (reinterpret_cast<HWND>(lParam) == hButton)
        {
            COLORREF color = GenerateRandomColor();
            if (hBrush)
                DeleteObject(hBrush);

            hBrush = CreateSolidBrush(color);
            InvalidateRect(hwnd, nullptr, TRUE);
            UpdateWindow(hwnd);
        }
        break;
    case WM_ERASEBKGND:
    {
        HDC hdc = reinterpret_cast<HDC>(wParam);
        RECT rc;
        GetClientRect(hwnd, &rc);
        if (hBrush)
            FillRect(hdc, &rc, hBrush);
        else
            FillRect(hdc, &rc, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));
        return 1;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSW wc = {};
    HWND hwnd;
    MSG Msg;

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    if (!RegisterClassW(&wc))
    {
        MessageBoxW(nullptr, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    std::wcout << L"\nstep 1: registered class!" << std::endl;

    srand(static_cast<unsigned int>(time(nullptr)));

    hwnd = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        className,
        windowName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 480, 320,
        nullptr, nullptr, hInstance, nullptr);

    if (hwnd == nullptr)
    {
        MessageBoxW(nullptr, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    std::cout << "\nstep 2: created window!" << std::endl;

    hButton = CreateWindowExW(
        0,
        L"BUTTON",
        L"Randomize Color",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        150, 100, 180, 30,
        hwnd, nullptr, hInstance, nullptr);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // System events -> message queue -> GetMessage -> TranslateMessage -> DispatchMessageW -> WndProc
    while (GetMessageW(&Msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessageW(&Msg);
    }

    std::cout << "\nstep 3: out of the message loop" << std::endl;
    return static_cast<int>(Msg.wParam);
}
