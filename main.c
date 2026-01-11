#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include "logic.h"

void Log(const char* msg) {
    FILE* f = fopen("debug_log.txt", "a");
    if (f) {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}

#define ID_BTN_ADD 101
#define ID_BTN_PRODUCTION 102
#define ID_BTN_MATERIALS 103
#define ID_BTN_CLEAR 104
#define ID_BTN_PRINT 105

#define ID_EDIT_WIDTH 201
#define ID_EDIT_HEIGHT 202
#define ID_EDIT_QTY 203

#define ID_CB_SERIE 301
#define ID_CB_COLOR 302
#define ID_CB_VIDRIO 303
#define ID_CB_PREMARCO 304

#define ID_RESULTS 401
#define ID_LBL_COUNT 402

// Global UI Handles
HWND hSerie, hColor, hVidrio, hPremarco, hWidth, hHeight, hQty, hResults, hCount;
HFONT hFont, hFontBold, hFontTitle;
HBRUSH hBrushBack, hBrushEdit;

Opening openings[MAX_OPENINGS];
int opening_count = 0;

void SaveOpenings() {
    FILE* f = fopen("openings.dat", "wb");
    if (f) {
        fwrite(&opening_count, sizeof(int), 1, f);
        fwrite(openings, sizeof(Opening), opening_count, f);
        fclose(f);
    }
}

void LoadOpenings() {
    FILE* f = fopen("openings.dat", "rb");
    if (f) {
        if (fread(&opening_count, sizeof(int), 1, f) == 1) {
            fread(openings, sizeof(Opening), opening_count, f);
        }
        fclose(f);
    }
}

void UpdateCount() {
    int total = 0;
    for (int i = 0; i < opening_count; i++) total += openings[i].quantity;
    char buf[128];
    sprintf(buf, "%d Openings Added", total);
    SetWindowText(hCount, buf);
}

void AddOpening() {
    if (opening_count >= MAX_OPENINGS) {
        MessageBox(NULL, "Maximum openings reached.", "Error", MB_ICONERROR);
        return;
    }

    char buf[128];
    GetWindowText(hWidth, buf, 128);
    double w = atof(buf);
    GetWindowText(hHeight, buf, 128);
    double h = atof(buf);
    GetWindowText(hQty, buf, 128);
    int qty = atoi(buf);

    if (w <= 0 || h <= 0 || qty <= 0) {
        MessageBox(NULL, "Please enter valid dimensions and quantity.", "Error", MB_ICONWARNING);
        return;
    }

    for (int i = opening_count; i > 0; i--) {
        openings[i] = openings[i-1];
    }
    
    Opening* op = &openings[0];
    memset(op, 0, sizeof(Opening));
    op->width = w;
    op->height = h;
    op->quantity = qty;

    GetWindowText(hSerie, op->serie, 64);
    GetWindowText(hColor, op->color, 64);
    
    GetWindowText(hVidrio, buf, 128);
    op->dvh = (strcmp(buf, "dvh") == 0);
    
    GetWindowText(hPremarco, buf, 128);
    op->preframe = (strcmp(buf, "Con Premarco") == 0);

    calculate_pieces(op);
    opening_count++;

    SaveOpenings();
    UpdateCount();
    
    SetWindowText(hWidth, "");
    SetWindowText(hHeight, "");
    SetWindowText(hQty, "1");
    
    MessageBox(NULL, "Opening Added Successfully", "Success", MB_OK);
}

void ShowProduction() {
    char* result_text = malloc(128000);
    if (!result_text) return;
    result_text[0] = '\0';

    for (int i = 0; i < opening_count; i++) {
        char line[2048];
        strcat(result_text, "--------------------------------------------------\r\n");
        sprintf(line, "%d Aberturas de %.1f Ancho x %.1f Alto %s %s DVH? = %s\r\n", 
                openings[i].quantity, openings[i].width, openings[i].height, 
                openings[i].serie, openings[i].color, openings[i].dvh ? "true" : "false");
        strcat(result_text, line);
        
        for (int j = 0; j < openings[i].frame_count; j++) {
            Frame* f = &openings[i].frames[j];
            if (f->is_complex) {
                sprintf(line, "%s Anchos = %d de %.1f\r\nAltos = %d de %.1f\r\n", 
                        f->spanish_name, f->width_quantity, f->length_width, f->height_quantity, f->length_height);
            } else {
                sprintf(line, "%d %s %.1f Mitad = %.1f\r\n", f->quantity, f->spanish_name, f->length, f->half);
            }
            strcat(result_text, line);
        }
        strcat(result_text, openings[i].glass_info);
        strcat(result_text, "\r\n");
    }

    SetWindowText(hResults, result_text);
    free(result_text);
}

void ShowMaterials() {
    Bar bars[MAX_BARS];
    int bar_count = calculate_materials(openings, opening_count, bars, 5900.0);

    char* result_text = malloc(128000);
    if (!result_text) return;
    strcpy(result_text, "Materials:\r\n\r\n");
    
    if (bar_count == 0) {
        strcat(result_text, "No materials calculated.\r\n");
    }

    for (int i = 0; i < bar_count; i++) {
        char line[1024];
        sprintf(line, "%d Barra(s) de %s %s %s [L=%.0f, Method:%s] Codes: ", 
                bars[i].quantity, bars[i].name, bars[i].serie, bars[i].color, bars[i].bar_length, bars[i].calculation_method);
        strcat(result_text, line);
        
        for (int j = 0; j < bars[i].code_count; j++) {
            sprintf(line, "%s=%s ", bars[i].codes[j].key, bars[i].codes[j].value);
            strcat(result_text, line);
        }
        strcat(result_text, "\r\n");
        strcat(result_text, bars[i].cutting_map);
        strcat(result_text, "\r\n\r\n");
    }

    SetWindowText(hResults, result_text);
    free(result_text);
}

void PrintOutput(HWND hwnd) {
    char filename[MAX_PATH] = "production_report.txt";
    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "txt";

    if (GetSaveFileName(&ofn)) {
        int len = GetWindowTextLength(hResults);
        char* buf = malloc(len + 1);
        if (buf) {
            GetWindowText(hResults, buf, len + 1);
            FILE* f = fopen(filename, "w");
            if (f) {
                fputs(buf, f);
                fclose(f);
                MessageBox(hwnd, "Report saved successfully.", "Success", MB_OK);
            }
            free(buf);
        }
    }
}

BOOL CALLBACK SetFontCallback(HWND hwnd, LPARAM lParam) {
    SendMessage(hwnd, WM_SETFONT, (WPARAM)lParam, TRUE);
    return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            hBrushBack = CreateSolidBrush(RGB(30, 30, 30));
            hBrushEdit = CreateSolidBrush(RGB(45, 45, 45));
            hFont = CreateFont(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
            hFontBold = CreateFont(-16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
            hFontTitle = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

            HWND hTitle = CreateWindow("STATIC", "Frames", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 20, 800, 40, hwnd, NULL, NULL, NULL);
            SendMessage(hTitle, WM_SETFONT, (WPARAM)hFontTitle, TRUE);

            hCount = CreateWindow("STATIC", "0 Openings Added", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 60, 800, 20, hwnd, (HMENU)ID_LBL_COUNT, NULL, NULL);
            SendMessage(hCount, WM_SETFONT, (WPARAM)hFont, TRUE);

            int x1 = 50, x2 = 250, x3 = 450;
            int y = 100;

            CreateWindow("STATIC", "Serie:", WS_VISIBLE | WS_CHILD, x1, y, 150, 20, hwnd, NULL, NULL, NULL);
            hSerie = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, x1, y+25, 180, 200, hwnd, (HMENU)ID_CB_SERIE, NULL, NULL);
            const char* series[] = {"s20", "s25", "s25TripleRiel", "probbaCorrediza", "probbaCorredizaTripleRiel", "galaCorredizaTripleRiel", "galaCorredizaCuatroRieles"};
            for(int i=0; i<7; i++) SendMessage(hSerie, CB_ADDSTRING, 0, (LPARAM)series[i]);
            SendMessage(hSerie, CB_SETCURSEL, 0, 0);

            CreateWindow("STATIC", "Color:", WS_VISIBLE | WS_CHILD, x2, y, 150, 20, hwnd, NULL, NULL, NULL);
            hColor = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, x2, y+25, 180, 200, hwnd, (HMENU)ID_CB_COLOR, NULL, NULL);
            const char* colors[] = {"anodizado", "anolok", "blanco", "imitacionMadera", "pintadoNegro"};
            for(int i=0; i<5; i++) SendMessage(hColor, CB_ADDSTRING, 0, (LPARAM)colors[i]);
            SendMessage(hColor, CB_SETCURSEL, 2, 0);

            y += 60;
            CreateWindow("STATIC", "Vidrio:", WS_VISIBLE | WS_CHILD, x1, y, 150, 20, hwnd, NULL, NULL, NULL);
            hVidrio = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, x1, y+25, 180, 200, hwnd, (HMENU)ID_CB_VIDRIO, NULL, NULL);
            SendMessage(hVidrio, CB_ADDSTRING, 0, (LPARAM)"simple");
            SendMessage(hVidrio, CB_ADDSTRING, 0, (LPARAM)"dvh");
            SendMessage(hVidrio, CB_SETCURSEL, 0, 0);

            CreateWindow("STATIC", "Premarco:", WS_VISIBLE | WS_CHILD, x2, y, 150, 20, hwnd, NULL, NULL, NULL);
            hPremarco = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, x2, y+25, 180, 200, hwnd, (HMENU)ID_CB_PREMARCO, NULL, NULL);
            SendMessage(hPremarco, CB_ADDSTRING, 0, (LPARAM)"Sin Premarco");
            SendMessage(hPremarco, CB_ADDSTRING, 0, (LPARAM)"Con Premarco");
            SendMessage(hPremarco, CB_SETCURSEL, 0, 0);

            y += 60;
            CreateWindow("STATIC", "Width (mm):", WS_VISIBLE | WS_CHILD, x1, y, 150, 20, hwnd, NULL, NULL, NULL);
            hWidth = CreateWindow("EDIT", "1500", WS_VISIBLE | WS_CHILD | WS_BORDER, x1, y+25, 180, 25, hwnd, (HMENU)ID_EDIT_WIDTH, NULL, NULL);
            CreateWindow("STATIC", "Height (mm):", WS_VISIBLE | WS_CHILD, x2, y, 150, 20, hwnd, NULL, NULL, NULL);
            hHeight = CreateWindow("EDIT", "1200", WS_VISIBLE | WS_CHILD | WS_BORDER, x2, y+25, 180, 25, hwnd, (HMENU)ID_EDIT_HEIGHT, NULL, NULL);
            CreateWindow("STATIC", "Qty:", WS_VISIBLE | WS_CHILD, x3, y, 80, 20, hwnd, NULL, NULL, NULL);
            hQty = CreateWindow("EDIT", "1", WS_VISIBLE | WS_CHILD | WS_BORDER, x3, y+25, 80, 25, hwnd, (HMENU)ID_EDIT_QTY, NULL, NULL);

            y += 70;
            HWND hBtnAdd = CreateWindow("BUTTON", "Agregar Abertura", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, x1, y, 380, 40, hwnd, (HMENU)ID_BTN_ADD, NULL, NULL);
            
            y += 60;
            CreateWindow("BUTTON", "Production", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, x1, y, 120, 35, hwnd, (HMENU)ID_BTN_PRODUCTION, NULL, NULL);
            CreateWindow("BUTTON", "Materials", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, x1+130, y, 120, 35, hwnd, (HMENU)ID_BTN_MATERIALS, NULL, NULL);
            CreateWindow("BUTTON", "Clear Data", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, x1+260, y, 120, 35, hwnd, (HMENU)ID_BTN_CLEAR, NULL, NULL);
            CreateWindow("BUTTON", "PRINT (Save Text)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 550, y, 200, 35, hwnd, (HMENU)ID_BTN_PRINT, NULL, NULL);

            y += 50;
            hResults = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY, 50, y, 700, 350, hwnd, (HMENU)ID_RESULTS, NULL, NULL);
            
            SaveOpenings(); // Clear data on startup
            UpdateCount();

            EnumChildWindows(hwnd, SetFontCallback, (LPARAM)hFont);
            SendMessage(hBtnAdd, WM_SETFONT, (WPARAM)hFontBold, TRUE);
            SendMessage(hTitle, WM_SETFONT, (WPARAM)hFontTitle, TRUE);

            break;
        }
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            HWND hControl = (HWND)lParam;
            if (hControl == hResults) {
                SetTextColor(hdcStatic, RGB(255, 255, 255));
                SetBkColor(hdcStatic, RGB(45, 45, 45));
                SetBkMode(hdcStatic, OPAQUE);
                return (INT_PTR)hBrushEdit;
            }
            SetTextColor(hdcStatic, RGB(200, 200, 200));
            SetBkMode(hdcStatic, TRANSPARENT);
            return (INT_PTR)hBrushBack;
        }
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX: {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, RGB(255, 255, 255));
            SetBkColor(hdcEdit, RGB(45, 45, 45));
            SetBkMode(hdcEdit, OPAQUE);
            return (INT_PTR)hBrushEdit;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_BTN_ADD: AddOpening(); break;
                case ID_BTN_PRODUCTION: ShowProduction(); break;
                case ID_BTN_MATERIALS: ShowMaterials(); break;
                case ID_BTN_CLEAR: 
                    if (MessageBox(hwnd, "Clear all data?", "Confirm", MB_YESNO) == IDYES) {
                        opening_count = 0;
                        SaveOpenings();
                        UpdateCount();
                        SetWindowText(hResults, "");
                    }
                    break;
                case ID_BTN_PRINT: PrintOutput(hwnd); break;
            }
            break;
        }
        case WM_DESTROY: {
            DeleteObject(hBrushBack);
            DeleteObject(hBrushEdit);
            DeleteObject(hFont);
            DeleteObject(hFontBold);
            DeleteObject(hFontTitle);
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    Log("App Starting...");
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icex);
    Log("Common controls initialized");
    
    const char CLASS_NAME[] = "FramesCWin";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClass(&wc)) {
        Log("RegisterClass failed");
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    Log("Class registered");

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Frames C App", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 840, 900, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        Log("CreateWindow failed");
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    Log("Window created");

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);
    Log("Window shown");

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Log("App Exiting normally");

    return (int)msg.wParam;
}
