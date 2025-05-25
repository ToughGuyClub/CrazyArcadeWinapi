#include"pch.h"

#include"Bomb.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2025 Window Programming Exam";
void DrawOutlinedText(HDC hdc, int x, int y, const wchar_t* text) {
    // 폰트 생성 (크고 두꺼운 글씨)
    HFONT hFont = CreateFont(
        48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"맑은 고딕"
    );

    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    // 배경 투명하게 설정
    SetBkMode(hdc, TRANSPARENT);

    // 외곽선(Stroke)을 위한 경로 시작
    BeginPath(hdc);
    TextOut(hdc, x, y, text, wcslen(text));
    EndPath(hdc);

    // 외곽선 색
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

    // 안쪽 채우기 색
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 165, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    StrokeAndFillPath(hdc);

    // 정리
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow) {
    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;




    g_hInst = hInstance;
    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    //위의 12개의 항목을 다 만들어야 윈도우가 생길거임.
    RegisterClassEx(&WndClass);//만든 윈도우를 레지스터에 등록?하는거라함

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL | CS_DBLCLKS,
        0, 0, 1500, 900, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;


}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    static Player p1, p2;
    static Board board[MAP_TILES][MAP_TILES];
    static int selected_stage = 0;
    static std::vector<Bomb> p1bombs;
    static std::vector<Bomb> p2bombs;
    switch (msg) {
    case WM_CREATE:
      
        break;
    case WM_KEYDOWN:
        if (wParam < 256) keyStates[wParam] = true;
        if (wParam == VK_SPACE) {
           
            if (p1.GetMaxBomb() > p1bombs.size()) {
                Bomb newBomb;
                newBomb.make_Bomb(p1, board);

                p1bombs.push_back(newBomb);
            }
        }
        if (wParam == VK_RETURN) {

            if (p2.GetMaxBomb() > p2bombs.size()) {
            Bomb newBomb;
            newBomb.make_Bomb(p2, board);

            p2bombs.push_back(newBomb);
            }
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_KEYUP:
        if (wParam < 256) keyStates[wParam] = false;
        break;
    case WM_TIMER: {
        switch (wParam) {
        case 1: {
            float speed = 1.0f / 8.0f;
            if (selected_stage) {
                HandlePlayerMovement(p1, keyStates, 'W', 'S', 'A', 'D', board);
                UpdatePlayerPosition(p1, speed);

                HandlePlayerMovement(p2, keyStates, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, board);
                UpdatePlayerPosition(p2, speed);


            }
        }
            break;
        case 2:
            for (int i = 0;i < p1bombs.size();++i) {
                if (p1bombs.data()[i].tick_Bomb()) {
                    p1bombs.erase(p1bombs.begin() + i);
                }
            }
            for (int i = 0;i < p2bombs.size();++i) {
                if (p2bombs.data()[i].tick_Bomb()) {
                    p2bombs.erase(p2bombs.begin() + i);
                }
            }
            break;
            }
       
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_CHAR:
        if (selected_stage == 0) {
            if (wParam == '1') {
                selected_stage = 1;
                POINT p1_start_point = { 8,18 };
                POINT p2_start_point = { 11,18 };

                p1.Init_Player(p1_start_point);
                p2.Init_Player(p2_start_point);
                init_board(board);
                SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);
                SetTimer(hWnd, 2, 50, NULL);
            }
            else  if (wParam == '2') {
                selected_stage = 2;
            }
            else  if (wParam == '3') {
                selected_stage = 3;
            }
        }
        break;
    case WM_PAINT: {
        if (selected_stage) {
            hdc = BeginPaint(hWnd, &ps);
            HDC mem = CreateCompatibleDC(hdc);
            HBITMAP bmp = CreateCompatibleBitmap(hdc, 1500, 900);
            HGDIOBJ oldBmp = SelectObject(mem, bmp);
            RECT rc = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
            FillRect(mem, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

			HBITMAP gameui = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_GAMEUI));
			HDC uiDC = CreateCompatibleDC(mem);
			HBITMAP oldUIBitmap = (HBITMAP)SelectObject(uiDC, gameui);
			StretchBlt(mem, 0, 0, 1450, 900, uiDC, 0, 0, 1040, 780, SRCCOPY);

            DrawGrid(mem, g_hInst,board);
           
           

           
            draw_Bomb(mem, g_hInst,p1bombs.data(), static_cast<int>(p1bombs.size()),board,&p1);
            draw_Bomb(mem, g_hInst,p2bombs.data(), static_cast<int>(p2bombs.size()),board,&p2);
            
            p1.Draw_Player(mem, &p1, RGB(255, 0, 255));
            p2.Draw_Player(mem, &p2, RGB(0, 255, 255));

            //임시로 플레이어 사과 출력
            int tempapple = p1.GetCurrentApple();
			
			DrawOutlinedText(mem, 170, 280, std::to_wstring(tempapple).c_str());
			

            BitBlt(hdc, 0, 0, 1500, 900, mem, 0, 0, SRCCOPY);
            SelectObject(mem, oldBmp);
            DeleteObject(bmp);
			SelectObject(uiDC, oldUIBitmap);
			DeleteObject(gameui);
			DeleteDC(uiDC);
            DeleteDC(mem);
            EndPaint(hWnd, &ps);
        }
        return 0;
    }
    case WM_DESTROY:
        KillTimer(hWnd, TIMER_ID);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

