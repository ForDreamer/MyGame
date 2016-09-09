#include "Global.h"
#include "../../MetalMax/resource.h"
#include <mmsystem.h>
#include <strsafe.h>
#include <assert.h>
#include "time.h"

#include "../Engine/FmodSound/FmodSound.h"
#include "../Engine/Manager/TextureManager.h"
#include "../Engine/Manager/RenderManager.h"
#include "../Engine/Scene/Scene.h"
#include "../Game/UI/UIManager.h"
#include "../Game/Battle/BattleUIMgr.h"

HWND Global::gHWnd = NULL;
const int Global::gWinWidth = 640;
const int Global::gWinHeight = 480;

Timer Global::gTimer;
IDirect3DDevice9* Global::gpD3DDevice = NULL;

IDirectInputDevice8* Global::gpDInputDevice = NULL;
BYTE Global::gKeyStatePrev[256] = {0};
BYTE Global::gKeyState[256] = {0};

ID3DXFont* Global::gpBigFont = NULL;
ID3DXFont* Global::gpSmallFont = NULL;
ID3DXSprite* Global::gpTextSprite = NULL;

Timer::Timer(void)
{
	baseTime = 0.0f;
	beginTime = 0.0f;
	endTime = 0.0f;
	int fpsCount = 0;
	timeDelta = 0.0f;
	timeFPS = 0;
}

void Timer::SetupBaseTime(void)
{
	baseTime = (float)timeGetTime();
}

void Global::BeginFrame(void)
{
	gTimer.beginTime = (float)timeGetTime();

	GetDInputKeyBoardState();
}

void Global::EndFrame(void)
{
	SavePrevKeyState();

	gTimer.endTime = (float)timeGetTime();
	gTimer.timeDelta = (gTimer.endTime - gTimer.beginTime) * 0.001f;
	gTimer.fpsCount++;
	if(gTimer.endTime - gTimer.baseTime >= 1000)
	{
		gTimer.timeFPS = gTimer.fpsCount;
		gTimer.fpsCount = 0;
		gTimer.baseTime = gTimer.endTime;
	}
}
/************************************************************************/
/* 获取键盘输入的状态                                                         */
/************************************************************************/
HRESULT Global::GetDInputKeyBoardState(void)
{
	gpDInputDevice->Acquire();
	if(NULL == gpDInputDevice) 
		return S_OK;
	
	ZeroMemory(gKeyState, sizeof(gKeyState));
	HRESULT hr = gpDInputDevice->GetDeviceState(sizeof(gKeyState), gKeyState);
	if(FAILED(hr)) 
	{
		return E_FAIL;
	}

	return S_OK;
}
/************************************************************************/
/* 保存输入的信息                                                         */
/************************************************************************/
void Global::SavePrevKeyState(void)
{
	for(int i=0; i<256; i++)
	{
		gKeyStatePrev[i] = gKeyState[i];
	}
}

void Global::ClearKeyState(void)
{
	for(int i=0; i<256; i++)
	{
		gKeyState[i] = 0x01;
	}
}

bool Global::KeyDown(BYTE key)
{
	assert(key>0 && key<256);
	
	if(Global::gKeyStatePrev[key] == 0 && Global::gKeyState[key] & 0x80)
		return true;
	
	return false;
}

bool Global::KeyUp(BYTE key)
{
	assert(key>0 && key<256);
	
	if(Global::gKeyStatePrev[key] & 0x80 && Global::gKeyState[key] == 0)
		return true;
	
	return false;
}

bool Global::KeyPressed(BYTE key)
{
	assert(key>0 && key<256);
	
	if(Global::gKeyStatePrev[key] & 0x80 && Global::gKeyState[key] & 0x80)
		return true;
	
	return false;
}
/************************************************************************/
/* 输出文字信息                                                           */
/************************************************************************/
void Global::PrintText(float x, float y, LPCSTR str, FontType type, DWORD color)
{
	// coordinate consistent with hud system
	x += gWinWidth/2;
	y += gWinHeight/2;
	y = gWinHeight-y;
	
	Global::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);

	gpTextSprite->Begin(D3DXSPRITE_ALPHABLEND|D3DXSPRITE_SORT_TEXTURE);
	RECT rect = {(int)x, (int)y, gWinWidth, gWinHeight};
	if(type == FT_SMALLFONT)
		gpSmallFont->DrawText(gpTextSprite, str, -1, &rect, DT_TOP|DT_LEFT, color);
	else if(type == FT_BIGFONT)
		gpBigFont->DrawText(gpTextSprite, str, -1, &rect, DT_TOP|DT_LEFT, color);
	gpTextSprite->End();

	Global::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

LRESULT CALLBACK Global::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

/************************************************************************/
/* 调用系统函数生成运行窗口                                                */
/************************************************************************/
bool Global::InitD3DWindow(void)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW|CS_VREDRAW, WndProc, 0L, 0L,
                      hInstance, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)), 
					  LoadCursor(0, IDC_ARROW), NULL, NULL,
                      "MetalMax", NULL };
	if(!RegisterClassEx(&wc)) 
	{
		MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return false;
	}

	RECT desktopRect;
	GetWindowRect(GetDesktopWindow(), &desktopRect);
	int setx = (desktopRect.right - gWinWidth)/2;
	int	sety = (desktopRect.bottom - gWinHeight)/2;

	DWORD windowStyle =	(WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_BORDER|WS_MINIMIZEBOX); //WS_OVERLAPPEDWINDOW
	gHWnd = CreateWindow("MetalMax", "MetalMax",
                           windowStyle, setx, sety, gWinWidth, gWinHeight,
                           NULL, NULL, wc.hInstance, NULL);
	if(!gHWnd)
	{
		MessageBox(0, "CreateWindow() - FAILED", 0, 0);
		return false;
	}
	ShowWindow(gHWnd, SW_SHOWDEFAULT);
	UpdateWindow(gHWnd);

	HRESULT hr = 0;
	IDirect3D9* d3d9 = NULL;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(!d3d9)
	{
		MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}

	D3DCAPS9 caps;
	D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	int vp = 0;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = gWinWidth;
	d3dpp.BackBufferHeight           = gWinHeight;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = gHWnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE; //D3DPRESENT_INTERVAL_ONE;

	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, gHWnd, vp, &d3dpp, &gpD3DDevice);
	if(FAILED(hr))
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, gHWnd, vp, &d3dpp, &gpD3DDevice);
		if(FAILED(hr))
		{
			d3d9->Release();
			MessageBox(0, "CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release();
	
	return true;
}
/************************************************************************/
/* 初始化Directx的输入                                                    */
/************************************************************************/
bool Global::InitDirectInput(void)
{
	DWORD dwCoopFlags = DISCL_NONEXCLUSIVE|DISCL_FOREGROUND;

	IDirectInput8* pDInput = NULL;
	if(FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, NULL)))
	{
		MessageBox(NULL,"DirectInput8Create() Failed","Error",MB_ICONERROR);
		return false;
	}
	if(FAILED(pDInput->CreateDevice( GUID_SysKeyboard, &gpDInputDevice, NULL)))
	{
		MessageBox(NULL,"pDInput->CreateDevice() Failed","Error",MB_ICONERROR);
		return false;
	}

	if(FAILED(gpDInputDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(NULL,"gpDInputDevice->SetDataFormat() Failed","Error",MB_ICONERROR);
		return false;
	}
	HRESULT hr = gpDInputDevice->SetCooperativeLevel(gHWnd, dwCoopFlags);
	if(hr == DIERR_UNSUPPORTED)
	{
		MessageBox(NULL,"gpDInputDevice->SetCooperativeLevel() DIERR_UNSUPPORTED","Error",MB_OK);
		return false;
	}
	if(FAILED(hr))
	{
		MessageBox(NULL,"gpDInputDevice->SetCooperativeLevel() Failed","Error",MB_ICONERROR);
		return false;
	}

	gpDInputDevice->Acquire();

	pDInput->Release();

	return true;
}
/************************************************************************/
/* 初始化显示的字体														*/
/************************************************************************/
bool Global::InitD3DXFont(FontType type)
{
	D3DXCreateSprite(gpD3DDevice, &gpTextSprite);

	D3DXFONT_DESCA font;
	ZeroMemory(&font, sizeof(D3DXFONT_DESCA));
	font.Italic = false;      
	font.CharSet = DEFAULT_CHARSET;
	font.Quality = 0;           
	font.PitchAndFamily = 0;           
	StringCbCopyA(font.FaceName, 32, "Arial");
	
	if(type == FT_SMALLFONT)
	{
		font.Height = 24;
		font.Width = 0;
		font.Weight = 700;

		if(FAILED(D3DXCreateFontIndirect(gpD3DDevice, &font, &gpSmallFont)))
		{
			MessageBox(0, "D3DXCreateFontIndirect() - FAILED", 0, 0);
			return false;
		}
	}
	else if(type == FT_BIGFONT)
	{
		font.Height = 32;
		font.Width = 0;     
		font.Weight = 700;

		if(FAILED(D3DXCreateFontIndirect(gpD3DDevice, &font, &gpBigFont)))
		{
			MessageBox(0, "D3DXCreateFontIndirect() - FAILED", 0, 0);
			return false;
		}
	}

 	return true;
}
/************************************************************************/
/* 初始化界面渲染状态                                                      */
/************************************************************************/
void Global::InitD3DRenderState(void)
{
	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, (float)gWinWidth, (float)gWinHeight, -100.0f, 100.0f);
	GetDevice()->SetTransform(D3DTS_PROJECTION, &matProj);

	GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	GetDevice()->SetRenderState(D3DRS_ZENABLE, true);
	Global::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);

	GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}
/************************************************************************/
/* 初始化界面，加载资源图片                                                                     */
/************************************************************************/
bool Global::Setup()
{
	srand((unsigned int)time(NULL));

	if(!InitD3DWindow())
	{
		MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return false;
	}
	
	if(!InitD3DXFont(FT_SMALLFONT) || !InitD3DXFont(FT_BIGFONT))
	{
		MessageBox(0, "InitD3DXFont() - FAILED", 0, 0);
		return false;
	}

	if(!InitDirectInput())
	{
		MessageBox(0, "InitDirectInput() - FAILED", 0, 0);
		return false;
	}

	if(!FmodSound::InitFmodSound())
	{
		MessageBox(0, "InitFmodSound() - FAILED", 0, 0);
		return false;
	}

	if(!gTexMgr.LoadSystemTextures())
	{
		MessageBox(0, "LoadSystemTextures() - FAILED", 0, 0);
		return false;
	}
	if(!gTexMgr.LoadTileTextures())
	{
		MessageBox(0, "LoadTileTextures() - FAILED", 0, 0);
		return false;
	}
	if(!gTexMgr.LoadActorTextures())
	{
		MessageBox(0, "LoadActorTextures() - FAILED", 0, 0);
		return false;
	}
	if(!gTexMgr.LoadUITextures())
	{
		MessageBox(0, "LoadUITextures() - FAILED", 0, 0);
		return false;
	}
	if(!gTexMgr.LoadBattleTextures())
	{
		MessageBox(0, "LoadBattleTextures() - FAILED", 0, 0);
		return false;
	}

	gUIMgr.InitUIElements();
	gBattleUIMgr.InitUIElements();
	gSceneMgr.SetFristScene(SCENE_STARTUP);
	
	InitD3DRenderState();

	return true;
}

void Global::Cleanup(void)
{
	gUIMgr.ReleaseUIElements();
	gBattleUIMgr.ReleaseUIElements();
	gTexMgr.ReleaseBattleTexures();
	gTexMgr.ReleaseUITexures();
	gTexMgr.ReleaseActorTexures();
	gTexMgr.ReleaseTileTexures();
	gTexMgr.ReleaseSystemTexures();
	FmodSound::CloseFmodSound();

	if(gpDInputDevice)
	{
		gpDInputDevice->Unacquire();
		gpDInputDevice->Release();
	}

	if(gpBigFont)
		gpBigFont->Release();
	if(gpSmallFont)
		gpSmallFont->Release();
	if(gpTextSprite)
		gpTextSprite->Release();

	if(gpD3DDevice)
		gpD3DDevice->Release();
}
/************************************************************************/
/* 进入消息循环															*/
/************************************************************************/
void Global::EnterMsgLoop(void)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	gTimer.SetupBaseTime();
	SetWindowText(gHWnd, "MatalMax Release Ver 1.0");
	
	while(msg.message != WM_QUIT)
	{
		//判断选择的选项
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
        {	
			BeginFrame();

			gSceneMgr.ProcessScene();
			gRenderMgr.Process();

			EndFrame();
		}
    }
}

void Global::RunApp(void)
{
	if(Setup())
	{
		EnterMsgLoop();
	}
	Cleanup();
}
