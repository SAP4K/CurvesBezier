#include <windows.h>
#include<iostream>
#include<math.h>
const wchar_t ClassName[] = L"myWindowClass";
HINSTANCE hin;
POINT pointss[2] = { 
   200,200,
    300,300
  };
//pentru ca sai schimbi forma la curba modifica valorile
POINT points[4] = { 20+50, 40 + 50, 300 + 50, 200 + 50, 330 + 50, 110 + 50, 450 + 50, 50 + 50 };

//Coordonate Curba Bezie prin Formula
POINT PositonFormuls[4] = {50,300,200, 50,500,50,800,50};
//Indicater Coordonate
int qq = 0;
int Dagre = 5;
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
  
    for (int i = 0; i < 4; i++) {
        points[i].x += x;
        points[i].y += y;
    }
    
}
//formula pentru rotire
void Rotation(HWND hwnd) {
    float Cos = cos(Dagre * 3.14159 / 180);
    float Sin = sin(Dagre * 3.14159 / 180);
    HDC hdc = GetWindowDC(hwnd);
    int MaxX=INT_MIN,MinX= INT_MAX;
    int MaxY= INT_MIN, MinY= INT_MAX;
    int LenghtX, LenghtY;
    for (int i = 0; i < 4; i++) {
        if (points[i].x > MaxX)
            MaxX = points[i].x;
        if (points[i].x < MinX)
            MinX = points[i].x;
        if (points[i].y > MaxY)
            MaxY = points[i].y;
        if (points[i].x < MinY)
            MinY = points[i].y;}
    LenghtX = abs(MaxX - MinX);
    LenghtY = abs(MaxY - MinY);
    float CenterX = LenghtX / 2;
    float CenterY = LenghtY / 2;
    POINT FirstPoint[4];
    for (int i = 0; i < 4; i++) {
        FirstPoint[i].x = points[i].x;
        FirstPoint[i].y = points[i].y;
    }
    for (int i = 0; i < 4; i++) {
        FirstPoint[i].x -= points[0].x; FirstPoint[i].y -= points[0].y;
        FirstPoint[i].x -= CenterX; FirstPoint[i].y -= CenterY;
        float secX = FirstPoint[i].x;
        FirstPoint[i].x = FirstPoint[i].x * Cos - FirstPoint[i].y * Sin;
        FirstPoint[i].y = secX * Sin + FirstPoint[i].y * Cos;
        FirstPoint[i].x += CenterX; FirstPoint[i].y += CenterY;
        FirstPoint[i].x += points[0].x; FirstPoint[i].y += points[0].y;
    }
    PolyBezier(hdc, FirstPoint, 4);
    ReleaseDC(hwnd, hdc);
    
}
//Cu ajutorul formulei  
void FormulBezier(HWND hwnd) {
    HDC hdc = GetWindowDC(hwnd);
    float x;
    float y;
    for (float i = 0; i < 1; i+=0.001) {
        x = pow((1 - i),3) * PositonFormuls[0].x + 3*pow((1-i),2)*i* PositonFormuls[1].x + 3*(1-i)*pow(i,2)* PositonFormuls[2].x+pow(i,3)* PositonFormuls[3].x;
        y = pow((1 - i), 3) * PositonFormuls[0].y + 3 * pow((1 - i), 2) * i * PositonFormuls[1].y + 3 * (1 - i) * pow(i, 2) * PositonFormuls[2].y + pow(i, 3) * PositonFormuls[3].y;
        SetPixel(hdc, x, y,RGB(0,0,0));
    }
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
        Rotation(hwnd);
        FormulBezier(hwnd);
        EndPaint(hwnd, &pn);
    }break;
    case WM_KEYDOWN: {
        switch (wParam)
        {
        case VK_UP: {
            std::cout << "UP\n";
            ClearSurface(hwnd);
            DrawNewPosition(hwnd, 0, -10);
            FormulBezier(hwnd);
            Rotation(hwnd);
        }break;
        case VK_DOWN: {
            std::cout << "DOWN\n";
            ClearSurface(hwnd);
            DrawNewPosition(hwnd, 0, 10);
            FormulBezier(hwnd);
            Rotation(hwnd);
        }break;
        case VK_LEFT: {
            std::cout << "Left\n";
            ClearSurface(hwnd);
            DrawNewPosition(hwnd, -10, 0);
            FormulBezier(hwnd);
            Rotation(hwnd);
        }break;
        case VK_RIGHT: {
            std::cout << "Right\n";
            ClearSurface(hwnd);
            DrawNewPosition(hwnd, 10, 0);
            FormulBezier(hwnd);
            Rotation(hwnd);
        }break;
            //incrementi punctul
        case 'D': {
            if (qq < 3)
                qq++;
           
            ClearSurface(hwnd);
            FormulBezier(hwnd);
            Rotation(hwnd);
        }break;
            //Decrementezi punctul
        case 'A': {
            if (qq > 0)
                qq--;
            
        }break;
            //Decrementezi coordonata x
        case 'W': {
            PositonFormuls[qq].x -= 10;
            ClearSurface(hwnd);
            FormulBezier(hwnd);
            Rotation(hwnd);
        }break;
            //incrementzi coordonata x
        case 'E': {
            PositonFormuls[qq].x += 10;
            ClearSurface(hwnd);
            FormulBezier(hwnd);
            Rotation(hwnd);
        }break;
           //Decrementezi coordonata y
        case 'Z': {
            PositonFormuls[qq].y -= 10;
            ClearSurface(hwnd);
            FormulBezier(hwnd);
            Rotation(hwnd);
        }break;
            //incrementzi coordonata y
        case 'X': {
            PositonFormuls[qq].y += 10;
            ClearSurface(hwnd);
            FormulBezier(hwnd);
            Rotation(hwnd);
        }break;
        case 'F': {
            Dagre -= 5;
            ClearSurface(hwnd);
            Rotation(hwnd);
            FormulBezier(hwnd);
        }break;
        case 'G': {
            Dagre += 5;
            ClearSurface(hwnd);
            Rotation(hwnd);
            FormulBezier(hwnd);
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