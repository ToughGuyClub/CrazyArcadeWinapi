#include"pch.h"

#include"Bomb.h"
#include"ResourceManager.h"
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
    ResourceManager::LoadAll(g_hInst);
  
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
    ResourceManager::UnloadAll();
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
	static bool game_started = false;
    static int p1_check_color_pos;
	static int p2_check_color_pos;
    static int p1Applebox = 0;
    static int p2Applebox = 0;
    switch (msg) {
    case WM_CREATE:
        p1_check_color_pos = 270;
        p2_check_color_pos = 270;
        p1.SetColor(0);
		p2.SetColor(0);
        break;
    case WM_KEYDOWN:
        if (wParam < 256) keyStates[wParam] = true;
        if (game_started && selected_stage) {
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
        }
        else if (game_started) {
            if (wParam == VK_LEFT) {
                if (p2_check_color_pos > 270) {
					p2_check_color_pos -= 155;
                    p2.SetColor(p2.GetColor() - 1);
                }
            }
            else if (wParam == VK_RIGHT) {
                if (p2_check_color_pos < 1200) {
                    p2_check_color_pos += 155;
                    p2.SetColor(p2.GetColor() + 1);
                }
            }
        }
        if (selected_stage == 1) {
            p1.on_applebox(&p1, &p1Applebox, 8, 19);
            p2.on_applebox(&p2, &p2Applebox, 11, 19);
			if (p1Applebox >= 10) {
				//p1승리 이제 win메시지 띄우고 전부 초기화
			}
            else if (p2Applebox >= 10) {
                //p2승리 이제 win메시지 띄우고 전부 초기화
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
				if (!p1bombs.data()[i].GetIsCollisionPlayer()&&p1bombs.data()[i].GetState() == 2) {
                    if (p1bombs.data()[i].collision_bomb_player(p1bombs.data()[i], p2)) {
                        if (selected_stage == 1) {
                            if (p2.GetCurrentApple() > 0)
                                p2.SetCurrentApple(p2.GetCurrentApple() - 1);
                            p1bombs.data()[i].SetIsCollisionPlayer(true); // 플레이어 2와 충돌한 경우
                        }
                    }
				}
            }
            for (int i = 0;i < p2bombs.size();++i) {
                if (!p2bombs.data()[i].GetIsCollisionPlayer() && p2bombs.data()[i].GetState() == 2) {
                    if (p2bombs.data()[i].collision_bomb_player(p2bombs.data()[i], p1)) {
                        if (selected_stage == 1) {
                            if (p1.GetCurrentApple() > 0)
                                p1.SetCurrentApple(p1.GetCurrentApple() - 1);
                            p2bombs.data()[i].SetIsCollisionPlayer(true); // 플레이어 1과 충돌한 경우
                        }
                    }
                }
            }
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
    case WM_LBUTTONDOWN: {
		int pos_x = LOWORD(lParam);
        if(!game_started&&pos_x<800)
		game_started = true;
        
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_CHAR:
        if (selected_stage == 0) {
            if (wParam == '1') {
                selected_stage = 1;
                POINT p1_start_point = { 8,18 };
                POINT p2_start_point = { 11,18 };
                int hueFilterMin = 0;   // 원하는 색 필터 범위
                int hueFilterMax = 360;
             
                HBITMAP original = ResourceManager::GetBitmap(BitmapID::Player1Moving);
                
                HBITMAP recolored = AdjustBitmapHueWithFilter(original, p1.GetColor(), hueFilterMin, 20);
                ResourceManager::SetBitmap(BitmapID::Player1Moving, recolored);

				original = ResourceManager::GetBitmap(BitmapID::Player2Moving);
				recolored = AdjustBitmapHueWithFilter(original, p2.GetColor(), hueFilterMin, 20);
				ResourceManager::SetBitmap(BitmapID::Player2Moving, recolored);

                p1.Init_Player(p1_start_point);
                p2.Init_Player(p2_start_point);
                p2.SetPlayerNum(2);
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
            else if (wParam == 'a') {
             if (game_started) {
                 
                    if (p1_check_color_pos > 270) {
                        p1_check_color_pos -= 155;
                        p1.SetColor(p1.GetColor() - 1);
                    }
               
                
                 }
            }
            else if (wParam == 'd') {
                if (game_started) {
                if (p1_check_color_pos < 1200) {
                    p1_check_color_pos += 155;
                    p1.SetColor(p1.GetColor() + 1);
                }
            }
            }
        }
        if (selected_stage) {
			
          
        }
        break;
    case WM_PAINT: {
       
            hdc = BeginPaint(hWnd, &ps);
            HDC mem = CreateCompatibleDC(hdc);
            HBITMAP bmp = CreateCompatibleBitmap(hdc, 1500, 900);
            HGDIOBJ oldBmp = SelectObject(mem, bmp);
            RECT rc = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
            FillRect(mem, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            if (!game_started) {
				// 게임이 시작되지 않은 상태에서의 처리
				HBITMAP loginBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_GAME_LOGIN));
				HDC loginDC = CreateCompatibleDC(mem);
				HBITMAP oldLoginBitmap = (HBITMAP)SelectObject(loginDC, loginBitmap);
				StretchBlt(mem, 0, 0, 1500, 900, loginDC, 0, 0, 800, 600, SRCCOPY);
				SelectObject(loginDC, oldLoginBitmap);
				DeleteObject(loginBitmap);
				DeleteDC(loginDC);
			
            }
            else if (game_started) {
				// 게임이 시작된 상태에서의 처리
                if (selected_stage == 0) {
					HBITMAP selectStageBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_SELECT_STAGE));
					HDC selectStageDC = CreateCompatibleDC(mem);
					HBITMAP oldSelectStageBitmap = (HBITMAP)SelectObject(selectStageDC, selectStageBitmap);
					StretchBlt(mem, 0, 0, 1500, 900, selectStageDC, 0, 0, 1500, 900, SRCCOPY);
					SelectObject(selectStageDC, oldSelectStageBitmap);
					DeleteObject(selectStageBitmap);

                    //체크표시 그리기
					HBITMAP p1CheckBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_P1_CHECK));
					HDC p1CheckDC = CreateCompatibleDC(mem);
					HBITMAP oldP1CheckBitmap = (HBITMAP)SelectObject(p1CheckDC, p1CheckBitmap);
                    TransparentBlt(mem, p1_check_color_pos, 670, 100, 50, p1CheckDC, 0, 0, 224, 150, RGB(255,0,255));
					HBITMAP p2CheckBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_P2_CHECK));
					HDC p2CheckDC = CreateCompatibleDC(mem);
					HBITMAP oldP2CheckBitmap = (HBITMAP)SelectObject(p2CheckDC, p2CheckBitmap);
                    TransparentBlt(mem, p2_check_color_pos, 640, 100, 50, p2CheckDC, 0, 0, 224, 150, RGB(255, 0, 255));

					DeleteDC(selectStageDC);
                }
				else if (selected_stage == 1) {
                    HBITMAP gameui = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_GAMEUI));
                    HDC uiDC = CreateCompatibleDC(mem);
                    HBITMAP oldUIBitmap = (HBITMAP)SelectObject(uiDC, gameui);
                    StretchBlt(mem, 0, 0, 1450, 900, uiDC, 0, 0, 1040, 780, SRCCOPY);

                    DrawGrid(mem, g_hInst, board,&p1,&p2);




                    draw_Bomb(mem, g_hInst, p1bombs.data(), static_cast<int>(p1bombs.size()), board, &p1);
                    draw_Bomb(mem, g_hInst, p2bombs.data(), static_cast<int>(p2bombs.size()), board, &p2);

                    //p1.Draw_Player(mem, &p1, RGB(255, 0, 255));
                    //p2.Draw_Player(mem, &p2, RGB(0, 255, 255));



                    HBITMAP appleboxBitmap = ResourceManager::GetBitmap(BitmapID::ForestAppleBox);
                    HDC appleboxDC = CreateCompatibleDC(hdc);
                    HBITMAP oldAppleboxBitmap = (HBITMAP)SelectObject(appleboxDC, appleboxBitmap);
					TransparentBlt(mem, 50,260,64,64, appleboxDC, 0, 0, 64, 64, RGB(255, 0, 255));
                    TransparentBlt(mem, 1200, 260, 64, 64, appleboxDC, 0, 0, 64, 64, RGB(255, 0, 255));

					HBITMAP applexBitmap = ResourceManager::GetBitmap(BitmapID::ForestApple);
					HDC applexDC = CreateCompatibleDC(hdc);
					HBITMAP oldApplexBitmap = (HBITMAP)SelectObject(applexDC, applexBitmap);
                    TransparentBlt(mem, 50, 220, 64, 64, applexDC, 0, 0, 40, 40, RGB(255, 0, 255));
                    TransparentBlt(mem, 1200, 220, 64, 64, applexDC, 0, 0, 40, 40, RGB(255, 0, 255));
                    //임시로 플레이어 사과 출력
                    int tempapple = p1.GetCurrentApple();
                    int tempapple2 = p2.GetCurrentApple();
                    DrawOutlinedText(mem, 125, 220, L" : ");
                    DrawOutlinedText(mem, 1280, 220, L" : ");
                    DrawOutlinedText(mem, 215, 220, std::to_wstring(tempapple).c_str());
                    DrawOutlinedText(mem, 1370, 220, std::to_wstring(tempapple2).c_str());
                    DrawOutlinedText(mem, 125, 270, L" : ");
                    DrawOutlinedText(mem, 1280, 270, L" : ");
                    DrawOutlinedText(mem, 215, 270, std::to_wstring(p1Applebox).c_str());
                    DrawOutlinedText(mem, 1370, 270, std::to_wstring(p2Applebox).c_str());

                   
					SelectObject(appleboxDC, oldAppleboxBitmap);
					SelectObject(applexDC, oldApplexBitmap);
                    SelectObject(uiDC, oldUIBitmap);
                    DeleteObject(gameui);
                    DeleteDC(uiDC);
				}
            
            
            }
			
            BitBlt(hdc, 0, 0, 1500, 900, mem, 0, 0, SRCCOPY);
            SelectObject(mem, oldBmp);
            DeleteObject(bmp);
         
            DeleteDC(mem);
            EndPaint(hWnd, &ps);
        
        return 0;
    }
    case WM_DESTROY:
        KillTimer(hWnd, TIMER_ID);
       
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

