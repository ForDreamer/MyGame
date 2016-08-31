#ifndef _Global_H_
#define _Global_H_

#include <d3dx9.h>
#include <dinput.h>

struct Timer
{
	Timer(void);
	void SetupBaseTime(void);

	float baseTime;
	float beginTime;
	float endTime;
	int fpsCount;
	float timeDelta;
	int timeFPS;
};

enum FontType
{
	FT_SMALLFONT = 0,
	FT_BIGFONT = 1,
};

enum FC_KeyType
{
	KEY_UP		= DIK_W,
	KEY_DOWN	= DIK_S,
	KEY_LEFT	= DIK_A,
	KEY_RIGHT	= DIK_D,
	KEY_A		= DIK_L,
	KEY_B		= DIK_K,
	KEY_START	= DIK_RETURN,
	KEY_SELECT	= DIK_SPACE
};

struct Global
{
public:
	static IDirect3DDevice9* GetDevice(void) { return gpD3DDevice; }
	static HWND& GetHWnd(void) { return gHWnd; }

	static bool KeyDown(BYTE key);
	static bool KeyUp(BYTE key);
	static bool KeyPressed(BYTE key);
	static void ClearKeyState(void);
	static void PrintText(float x, float y, LPCSTR str, FontType type, DWORD color = 0xffffffff);

	static float GetTimeDelta(void) { return gTimer.timeDelta; }
	static int GetFPS(void) { return gTimer.timeFPS; }
	static DWORD GetTime(void) { return timeGetTime(); }
	static void ResetTimer(void) { gTimer.beginTime = (float)timeGetTime(); }

	static void RunApp(void);

private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static bool InitD3DWindow(void);
	static bool InitDirectInput(void);
	static bool InitD3DXFont(FontType type);
	static void InitD3DRenderState(void);
	static bool Setup(void);
	static void EnterMsgLoop(void);
	static void Cleanup(void);
	static void BeginFrame(void);
	static void EndFrame(void);

	static HRESULT GetDInputKeyBoardState(void);
	static void SavePrevKeyState(void);

public:
	static const int gWinWidth;
	static const int gWinHeight;

private:
	static HWND gHWnd;
	static Timer gTimer;
	static IDirect3DDevice9* gpD3DDevice;

	static IDirectInputDevice8* gpDInputDevice;
	static BYTE gKeyStatePrev[256];
	static BYTE gKeyState[256];

	static ID3DXFont* gpBigFont;
	static ID3DXFont* gpSmallFont;

public:
	static ID3DXSprite* gpTextSprite;
};

#endif