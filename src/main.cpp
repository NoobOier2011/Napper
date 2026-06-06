#include <windows.h>
#include <string>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MessageBoxW(NULL, L"Hello, World!", L"Napper", MB_OK);
    return 0;
}
