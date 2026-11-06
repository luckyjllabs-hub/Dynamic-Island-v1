#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#include <objidl.h>
#include <gdiplus.h>
#include <algorithm>
#include <string>

using namespace Gdiplus;

namespace {

constexpr wchar_t kWindowClass[] = L"IslandFreshWindow";
constexpr wchar_t kWindowTitle[] = L"Island for Windows";
constexpr wchar_t kMutexName[] = L"Local\\IslandFreshSingleInstance";

constexpr int kCanvasWidth = 480;
constexpr int kCanvasHeight = 72;
constexpr float kPillWidth = 420.0f;
constexpr float kPillHeight = 50.0f;
constexpr float kPillLeft = (kCanvasWidth - kPillWidth) * 0.5f;
constexpr float kPillRight = kPillLeft + kPillWidth;

HWND g_window = nullptr;
HDC g_memoryDc = nullptr;
HBITMAP g_bitmap = nullptr;
HBITMAP g_oldBitmap = nullptr;
ULONG_PTR g_gdiplusToken = 0;

void BuildRoundedPath(
    GraphicsPath& path,
    float left,
    float top,
    float right,
    float bottom,
    float radius
) {
    const float diameter = radius * 2.0f;
    path.AddArc(left, top, diameter, diameter, 180.0f, 90.0f);
    path.AddArc(right - diameter, top, diameter, diameter, 270.0f, 90.0f);
    path.AddArc(right - diameter, bottom - diameter, diameter, diameter, 0.0f, 90.0f);
    path.AddArc(left, bottom - diameter, diameter, diameter, 90.0f, 90.0f);
    path.CloseFigure();
}

std::wstring CurrentTime() {
    SYSTEMTIME time{};
    GetLocalTime(&time);
    int hour = time.wHour % 12;
    if (hour == 0) hour = 12;
    wchar_t buffer[16]{};
    swprintf_s(buffer, L"%d:%02d", hour, time.wMinute);
    return buffer;
}

void DrawCenteredText(
    Graphics& graphics,
    const std::wstring& text,
    const Font& font,
    const RectF& bounds,
    const Color& color
) {
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);
    SolidBrush brush(color);
    graphics.DrawString(text.c_str(), -1, &font, bounds, &format, &brush);
}

void Render() {
    if (!g_window || !g_memoryDc) return;

    Graphics graphics(g_memoryDc);
    graphics.Clear(Color(0, 0, 0, 0));
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
    graphics.SetCompositingMode(CompositingModeSourceOver);

    GraphicsPath farShadow;
    GraphicsPath nearShadow;
    GraphicsPath capsule;
    BuildRoundedPath(farShadow, kPillLeft - 5.0f, -17.0f, kPillRight + 5.0f, 59.0f, 31.0f);
    BuildRoundedPath(nearShadow, kPillLeft - 2.0f, -18.0f, kPillRight + 2.0f, 55.0f, 29.0f);
    BuildRoundedPath(capsule, kPillLeft, -18.0f, kPillRight, kPillHeight, 28.0f);

    SolidBrush farShadowBrush(Color(20, 0, 0, 0));
    SolidBrush nearShadowBrush(Color(48, 0, 0, 0));
    SolidBrush bodyBrush(Color(252, 12, 14, 17));
    Pen edgePen(Color(22, 255, 255, 255), 1.0f);

    graphics.FillPath(&farShadowBrush, &farShadow);
    graphics.FillPath(&nearShadowBrush, &nearShadow);
    graphics.FillPath(&bodyBrush, &capsule);
    graphics.DrawPath(&edgePen, &capsule);

    SolidBrush accentBrush(Color(255, 77, 219, 207));
    graphics.FillEllipse(&accentBrush, kPillLeft + 27.0f, 18.0f, 14.0f, 14.0f);

    FontFamily variableFamily(L"Segoe UI Variable Display");
    FontFamily fallbackFamily(L"Segoe UI");
    FontFamily* fontFamily = variableFamily.IsAvailable() ? &variableFamily : &fallbackFamily;
    Font timeFont(fontFamily, 18.0f, FontStyleRegular, UnitPixel);
    DrawCenteredText(
        graphics,
        CurrentTime(),
        timeFont,
        RectF(kPillLeft, 0.0f, kPillWidth, kPillHeight),
        Color(255, 246, 247, 250)
    );

    SYSTEM_POWER_STATUS power{};
    GetSystemPowerStatus(&power);
    const bool hasBattery = (power.BatteryFlag & 128) == 0;
    const int percent = hasBattery && power.BatteryLifePercent <= 100
        ? static_cast<int>(power.BatteryLifePercent)
        : 100;

    const float batteryX = kPillRight - 116.0f;
    const float batteryY = 20.0f;
    GraphicsPath battery;
    BuildRoundedPath(battery, batteryX, batteryY, batteryX + 20.0f, batteryY + 10.0f, 2.0f);
    Pen batteryPen(Color(255, 199, 204, 212), 1.2f);
    SolidBrush secondaryBrush(Color(255, 199, 204, 212));
    graphics.DrawPath(&batteryPen, &battery);
    graphics.FillRectangle(&secondaryBrush, batteryX + 21.0f, batteryY + 3.0f, 2.0f, 4.0f);

    const float fillWidth = hasBattery ? 16.0f * percent / 100.0f : 16.0f;
    if (fillWidth > 0.5f) {
        SolidBrush batteryFill(Color(255, 246, 247, 250));
        graphics.FillRectangle(&batteryFill, batteryX + 2.0f, batteryY + 2.0f, fillWidth, 6.0f);
    }

    const std::wstring batteryText = hasBattery ? std::to_wstring(percent) + L"%" : L"AC";
    Font batteryFont(fontFamily, 12.0f, FontStyleBold, UnitPixel);
    DrawCenteredText(
        graphics,
        batteryText,
        batteryFont,
        RectF(kPillRight - 91.0f, 0.0f, 48.0f, kPillHeight),
        Color(255, 199, 204, 212)
    );

    Pen chevronPen(Color(255, 246, 247, 250), 1.8f);
    const float chevronX = kPillRight - 32.0f;
    const float chevronY = 23.0f;
    graphics.DrawLine(&chevronPen, chevronX - 6.0f, chevronY, chevronX, chevronY + 6.0f);
    graphics.DrawLine(&chevronPen, chevronX, chevronY + 6.0f, chevronX + 6.0f, chevronY);

    HDC screen = GetDC(nullptr);
    POINT source{0, 0};
    SIZE size{kCanvasWidth, kCanvasHeight};
    RECT windowRect{};
    GetWindowRect(g_window, &windowRect);
    POINT destination{windowRect.left, windowRect.top};
    BLENDFUNCTION blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

    UpdateLayeredWindow(
        g_window,
        screen,
        &destination,
        &size,
        g_memoryDc,
        &source,
        0,
        &blend,
        ULW_ALPHA
    );
    ReleaseDC(nullptr, screen);
}

bool CreateSurface() {
    HDC screen = GetDC(nullptr);
    g_memoryDc = CreateCompatibleDC(screen);
    ReleaseDC(nullptr, screen);
    if (!g_memoryDc) return false;

    BITMAPINFO info{};
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth = kCanvasWidth;
    info.bmiHeader.biHeight = -kCanvasHeight;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = BI_RGB;

    void* pixels = nullptr;
    g_bitmap = CreateDIBSection(g_memoryDc, &info, DIB_RGB_COLORS, &pixels, nullptr, 0);
    if (!g_bitmap) return false;
    g_oldBitmap = static_cast<HBITMAP>(SelectObject(g_memoryDc, g_bitmap));
    return true;
}

void DestroySurface() {
    if (g_memoryDc && g_oldBitmap) SelectObject(g_memoryDc, g_oldBitmap);
    if (g_bitmap) DeleteObject(g_bitmap);
    if (g_memoryDc) DeleteDC(g_memoryDc);
    g_bitmap = nullptr;
    g_memoryDc = nullptr;
    g_oldBitmap = nullptr;
}

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_NCHITTEST: {
            POINT point{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(window, &point);
            if (point.x >= kPillLeft && point.x <= kPillRight &&
                point.y >= 0 && point.y <= kPillHeight) {
                return HTCLIENT;
            }
            return HTTRANSPARENT;
        }
        case WM_TIMER:
        case WM_TIMECHANGE:
        case WM_POWERBROADCAST:
            Render();
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(window, message, wParam, lParam);
    }
}

} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int) {
    HANDLE mutex = CreateMutexW(nullptr, TRUE, kMutexName);
    if (!mutex || GetLastError() == ERROR_ALREADY_EXISTS) {
        if (mutex) CloseHandle(mutex);
        return 0;
    }

    SetProcessDPIAware();

    GdiplusStartupInput gdiplusInput;
    if (GdiplusStartup(&g_gdiplusToken, &gdiplusInput, nullptr) != Ok) return 1;

    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.lpszClassName = kWindowClass;
    RegisterClassExW(&windowClass);

    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int x = (screenWidth - kCanvasWidth) / 2;
    g_window = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        kWindowClass,
        kWindowTitle,
        WS_POPUP,
        x,
        0,
        kCanvasWidth,
        kCanvasHeight,
        nullptr,
        nullptr,
        instance,
        nullptr
    );

    if (!g_window || !CreateSurface()) return 1;

    Render();
    ShowWindow(g_window, SW_SHOWNOACTIVATE);
    SetTimer(g_window, 1, 30000, nullptr);

    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    KillTimer(g_window, 1);
    DestroySurface();
    GdiplusShutdown(g_gdiplusToken);
    ReleaseMutex(mutex);
    CloseHandle(mutex);
    return 0;
}
