#include <windows.h>
#include <WebView2.h>
#include <wrl.h>
#include <string>
#include <memory>

using namespace Microsoft::WRL;

// ========== Global variables ==========
static HWND g_hwnd = nullptr;
static ComPtr<ICoreWebView2> g_webview = nullptr;
static ComPtr<ICoreWebView2Controller> g_controller = nullptr;

// ========== Helper functions ==========
std::wstring GetExePath() {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    std::wstring full(path);
    size_t pos = full.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        return full.substr(0, pos + 1);
    }
    return L"";
}

// ========== Window procedure ==========
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_SIZE:
            if (g_controller) {
                RECT bounds;
                GetClientRect(hwnd, &bounds);
                g_controller->put_Bounds(bounds);
            }
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ========== Register window class ==========
bool RegisterWindowClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"NapperWindow";
    
    return RegisterClassExW(&wcex) != 0;
}

// ========== Create main window ==========
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow) {
    g_hwnd = CreateWindowExW(
        0,
        L"NapperWindow",
        L"Napper - AI Batch Assistant",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1200, 800,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
    
    if (!g_hwnd) {
        return false;
    }
    
    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);
    return true;
}

// ========== Init WebView2 ==========
void InitWebView2() {
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (!env) {
                    MessageBoxW(nullptr, L"Failed to create WebView2 environment", L"Error 101", MB_OK | MB_ICONERROR);
                    return E_FAIL;
                }

                env->CreateCoreWebView2Controller(
                    g_hwnd,
                    Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                            if (!controller) {
                                MessageBoxW(nullptr, L"Failed to create WebView2 controller", L"Error 102", MB_OK | MB_ICONERROR);
                                return E_FAIL;
                            }

                            g_controller = controller;
                            g_controller->get_CoreWebView2(&g_webview);

                            // Set bounds
                            RECT bounds;
                            GetClientRect(g_hwnd, &bounds);
                            g_controller->put_Bounds(bounds);

                            // Load HTML file
                            std::wstring htmlPath = GetExePath() + L"web\\index.html";
                            std::wstring url = L"file:///" + htmlPath;
                            for (auto& c : url) {
                                if (c == L'\\') c = L'/';
                            }
                            g_webview->Navigate(url.c_str());

                            return S_OK;
                        }
                    ).Get()
                );
                return S_OK;
            }
        ).Get()
    );
}

// ========== Main ==========
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize COM
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to initialize COM", L"Error 003", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Register window class
    if (!RegisterWindowClass(hInstance)) {
        MessageBoxW(nullptr, L"Failed to register window class", L"Error 001", MB_OK | MB_ICONERROR);
        CoUninitialize();
        return 1;
    }

    // Create main window
    if (!CreateMainWindow(hInstance, nCmdShow)) {
        MessageBoxW(nullptr, L"Failed to create window", L"Error 002", MB_OK | MB_ICONERROR);
        CoUninitialize();
        return 1;
    }

    // Initialize WebView2
    InitWebView2();

    // Message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();
    return 0;
}