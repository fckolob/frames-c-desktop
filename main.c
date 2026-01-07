#include <windows.h>
#include <stdio.h>
#include "logic.h"

#define ID_BTN_CALC 101
#define ID_EDIT_WIDTH 102
#define ID_EDIT_HEIGHT 103
#define ID_EDIT_QTY 104
#define ID_RESULTS 105

HWND hWidth, hHeight, hQty, hResults;

void DoCalculation() {
    char buf[1024];
    GetWindowText(hWidth, buf, 1024);
    double w = atof(buf);
    GetWindowText(hHeight, buf, 1024);
    double h = atof(buf);
    GetWindowText(hQty, buf, 1024);
    int qty = atoi(buf);

    if (w <= 0 || h <= 0 || qty <= 0) {
        SetWindowText(hResults, "Please enter valid numbers.");
        return;
    }

    Opening op;
    memset(&op, 0, sizeof(Opening));
    op.width = w;
    op.height = h;
    op.quantity = qty;
    strcpy(op.serie, "s20");
    strcpy(op.color, "Blanco");
    op.dvh = false;

    Bar bars[MAX_BARS];
    int bar_count = calculate_materials(&op, 1, bars, 5900.0);

    char result_text[8192] = "";
    char line[512];
    sprintf(line, "Results for %d openings of %.0f x %.0f (s20):\r\n\r\n", qty, w, h);
    strcat(result_text, line);

    for (int i = 0; i < bar_count; i++) {
        sprintf(line, "%d bars of %s (%s) [%s]\r\n", bars[i].quantity, bars[i].name, bars[i].color, bars[i].serie);
        strcat(result_text, line);
    }

    SetWindowText(hResults, result_text);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // UI Controls
            CreateWindow("STATIC", "Width:", WS_VISIBLE | WS_CHILD, 10, 10, 50, 20, hwnd, NULL, NULL, NULL);
            hWidth = CreateWindow("EDIT", "1500", WS_VISIBLE | WS_CHILD | WS_BORDER, 70, 10, 100, 20, hwnd, (HMENU)ID_EDIT_WIDTH, NULL, NULL);

            CreateWindow("STATIC", "Height:", WS_VISIBLE | WS_CHILD, 10, 40, 50, 20, hwnd, NULL, NULL, NULL);
            hHeight = CreateWindow("EDIT", "1200", WS_VISIBLE | WS_CHILD | WS_BORDER, 70, 40, 100, 20, hwnd, (HMENU)ID_EDIT_HEIGHT, NULL, NULL);

            CreateWindow("STATIC", "Quantity:", WS_VISIBLE | WS_CHILD, 10, 70, 70, 20, hwnd, NULL, NULL, NULL);
            hQty = CreateWindow("EDIT", "1", WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 70, 80, 20, hwnd, (HMENU)ID_EDIT_QTY, NULL, NULL);

            CreateWindow("BUTTON", "Calculate Materials", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 100, 160, 30, hwnd, (HMENU)ID_BTN_CALC, NULL, NULL);

            hResults = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY, 10, 140, 360, 200, hwnd, (HMENU)ID_RESULTS, NULL, NULL);
            
            // Set default font
            HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
            SendDlgItemMessage(hwnd, ID_EDIT_WIDTH, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendDlgItemMessage(hwnd, ID_EDIT_HEIGHT, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendDlgItemMessage(hwnd, ID_EDIT_QTY, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendDlgItemMessage(hwnd, ID_RESULTS, WM_SETFONT, (WPARAM)hFont, TRUE);

            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == ID_BTN_CALC) {
                DoCalculation();
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    const char CLASS_NAME[] = "FramesCWin";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Frames C App - Fast & Optimal", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) return 0;

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
