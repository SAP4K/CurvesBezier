#include <windows.h>
#include<iostream>
const wchar_t ClassName[] = L"myWindowClass";
HINSTANCE hin;
int x[4] = {20,500,330,450};
int y[4] = {40,600,110,50};
//pentru ca sai schimbi forma la curba modifica valorile
POINT points[4] = { 20, 40, 300, 200, 330, 110, 450, 50};
//Curata ecranul inainte de a fi desenat
void ClearSurface(HWND hwnd) {
    RECT rcWnd;
    GetClientRect(hwnd, &rcWnd);
    HDC hdc = GetWindowDC(hwnd);
    rcWnd.right += 20;
    rcWnd.bottom += 30;
    FillRect(hdc, &rcWnd, (HBRUSH)(COLOR_WINDOW + 1));
    ReleaseDC(hwnd, hdc);
}
//Redeseneaza curba
void DrawNewPosition(HWND hwnd,int x,int y) {
    std::cout << "este\n";
    HDC hdc = GetWindowDC(hwnd);
    for (int i = 0; i < 4; i++) {
        points[i].x += x;
        points[i].y += y;
    }
    PolyBezier(hdc, points, 4);
    ReleaseDC(hwnd, hdc);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_PAINT: {
        PAINTSTRUCT pn;
        RECT rc;
        HDC hdc = BeginPaint(hwnd, &pn);
        ClearSurface(hwnd);
        PolyBezier(hdc, points, 4);
        EndPaint(hwnd, &pn);
    }break;
    case WM_KEYDOWN: {
        switch (wParam)
        {
        case 'S': {
            ClearSurface(hwnd);
        }
        case VK_UP: {
            std::cout << "UP\n";
            ClearSurface(hwnd);
            DrawNewPosition(hwnd,0,10); 
        }break;
        case VK_DOWN:{
            std::cout << "DOWN\n";
            ClearSurface(hwnd);
            DrawNewPosition(hwnd, 0, -10);
        }break;
        case VK_LEFT: {
            std::cout << "Left\n";
            ClearSurface(hwnd);
            DrawNewPosition(hwnd, -10, 0);
        }break;
        case VK_RIGHT: {
            std::cout << "Right\n";
            ClearSurface(hwnd);
            DrawNewPosition(hwnd, 10, 0);
        }break;
        }
    }break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main()
{
    hin = GetModuleHandle(NULL);
    WNDCLASSEX wc;  
    HWND hwnd;
    MSG Msg;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hin;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = ClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        ClassName,
        L"Main Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,600, 600,
        NULL, NULL, hin, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
 
}