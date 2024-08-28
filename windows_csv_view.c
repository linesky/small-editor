
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
//gcc csv_viewer.c -o csv_viewer -mwindows -lcomctl32
#define IDM_OPEN 1

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void LoadCSVFile(HWND hwnd, const char *filename);
void InitListViewColumns(HWND hwndLV);
void InsertListViewItems(HWND hwndLV, const char *filename);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "CSVViewerClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(0, "CSVViewerClass", "CSV File Viewer", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hwndLV;

    switch (uMsg) {
        case WM_CREATE: {
            // Create a ListView control
            hwndLV = CreateWindow(WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | LVS_REPORT,
                                  10, 10, 600, 400, hwnd, NULL, NULL, NULL);

            // Create a button to open CSV files
            CreateWindow("BUTTON", "Open", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                         10, 420, 100, 30, hwnd, (HMENU)IDM_OPEN, NULL, NULL);
            break;
        }

        case WM_COMMAND: {
            if (LOWORD(wParam) == IDM_OPEN) {
                OPENFILENAME ofn = {0};
                char filename[MAX_PATH] = "";

                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwnd;
                ofn.lpstrFile = filename;
                ofn.nMaxFile = sizeof(filename);
                ofn.lpstrFilter = "CSV Files\0*.csv\0All Files\0*.*\0";
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

                if (GetOpenFileName(&ofn)) {
                    LoadCSVFile(hwndLV, filename);
                }
            }
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void LoadCSVFile(HWND hwndLV, const char *filename) {
    // Clear any existing items
    ListView_DeleteAllItems(hwndLV);

    // Initialize columns based on the first line of the CSV
    InitListViewColumns(hwndLV);

    // Insert items into the ListView
    InsertListViewItems(hwndLV, filename);
}

void InitListViewColumns(HWND hwndLV) {
    LVCOLUMN lvc = {0};
    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.cx = 100;

    for (int i = 0; i < 5; i++) {  // Assumindo até 5 colunas
        char header[64];
        sprintf(header, "Column %d", i + 1);
        lvc.iSubItem = i;
        lvc.pszText = header;
        ListView_InsertColumn(hwndLV, i, &lvc);
    }
}

void InsertListViewItems(HWND hwndLV, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        MessageBox(NULL, "Could not open file.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    char line[1024];
    int row = 0;

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        int col = 0;

        LVITEM lvi = {0};
        lvi.mask = LVIF_TEXT;
        lvi.iItem = row;
        lvi.iSubItem = col;
        lvi.pszText = token;
        ListView_InsertItem(hwndLV, &lvi);

        while (token) {
            lvi.iSubItem = col;
            lvi.pszText = token;
            ListView_SetItem(hwndLV, &lvi);
            token = strtok(NULL, ",");
            col++;
        }

        row++;
    }

    fclose(file);
}
