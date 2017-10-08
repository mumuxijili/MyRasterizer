// MyRasterizer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MyRasterizer.h"
#include "Math3D.h"
#include "Util.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <iostream>
using namespace std;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HBITMAP g_hBMP = NULL;
HWND g_hWnd = NULL;


#ifdef D3D
IDXGISwapChain *swapchain;
ID3D11Device *device;
ID3D11DeviceContext *dc;
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader
ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer
#endif // D3D

// a struct to define a single vertex
struct VERTEX { FLOAT X, Y, Z; Vec4 Color; };

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

#ifdef D3D
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void CleanD3D(void);        // closes Direct3D and releases memory
void RenderFrame(void);     // renders a single frame
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//----begin yang chao
	//----create console
	AllocConsole();   // 建立控制台
	SetConsoleTitle(_T("Debug Output"));      // 设置控制台窗口标题
	// 重定向 STDIN
	{
		FILE*fp;
		errno_t eno = freopen_s(&fp, "CONIN$", "r+t", stdin);
		if (eno != 0) {//不成功
			MessageBox(NULL, _T("重定向stdin失败!"), _T("error"), MB_OK | MB_ICONEXCLAMATION);
			exit(0);
		}
	}
	// 重定向STDOUT
	{
		FILE*fp;
		errno_t eno = freopen_s(&fp, "CONOUT$", "w+t", stdout);
		if (eno != 0) {//不成功
			MessageBox(NULL, _T("重定向stdout失败!"), _T("error"), MB_OK | MB_ICONEXCLAMATION);
			exit(0);
		}

	}
	//----end yang chao

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYRASTERIZER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYRASTERIZER));

    MSG msg;

    // Main message loop:
//	while (GetMessage(&msg, nullptr, 0, 0))
//	{
//		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		SYSTEMTIME time = { 0 };
//		GetLocalTime(&time);
//		int blue = (int)((double)time.wMilliseconds / 1000 * 255);
//		int green = sizeof(DWORD);
//		DWORD red = ((blue) + ((2) << 8) + ((4) << 16) + ((0) << 24));
//		//memcpy(dwFrameBuffer, &red, WinWidth * WinHeight * sizeof(DWORD));
//		for (int i = 0; i < WinHeight; i++)
//			for (int j = 0; j < WinWidth; j++)
//				dwFrameBuffer[i * WinWidth + j] = red;
//		InvalidateRect(g_hWnd, nullptr, true);
//		UpdateWindow(g_hWnd);
//
//#ifdef D3D
//		RenderFrame();
//#endif // D3D
//
//	}
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			InvalidateRect(g_hWnd, nullptr, true);
			UpdateWindow(g_hWnd);
		}
	}

#ifdef D3D
	// clean up DirectX and COM
	CleanD3D();
#endif // D3D

	//----begin yang chao
	//destroy console
	FreeConsole();//销毁控制台
	//----end yang chao
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYRASTERIZER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYRASTERIZER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  // CW_USEDEFAULT, 0, WinWidth, WinHeight, nullptr, nullptr, hInstance, nullptr);

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, WinWidth, WinHeight, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

#ifdef D3D
   // set up and initialize Direct3D
   InitD3D(hWnd);
#endif

   return TRUE;
}


//void Paint(HWND hwnd)
//{
//	PAINTSTRUCT ps;
//	HDC hdc;
//	HDC compatibleDC;
//	BITMAPINFO bmpInfo = { 0 };
//	bmpInfo.bmiHeader.biBitCount = 32;
//	bmpInfo.bmiHeader.biWidth = WinWidth;
//	bmpInfo.bmiHeader.biHeight = WinHeight;
//	bmpInfo.bmiHeader.biPlanes = 1;
//	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//
//	//hdc = GetDC(hwnd);
//	hdc = BeginPaint(hwnd, &ps);
//	compatibleDC = CreateCompatibleDC(hdc);
//
//	HBITMAP mBitmap = CreateCompatibleBitmap(hdc, WinWidth, WinHeight);
//	DWORD pixel = (255) + ((0) << 8) + ((0) << 16) + ((0) << 24);
//	for (int i = 0; i < WinHeight; i++)
//		for (int j = 0; j < WinWidth; j++)
//			dwFrameBuffer[i * WinWidth + j] = pixel;
//	//memset(dwFrameBuffer, ((0)+((0) << 8) + ((0) << 16) + ((0) << 24)), WinWidth * WinHeight * sizeof(DWORD));
//	SetDIBits(compatibleDC, mBitmap, 0, WinHeight, dwFrameBuffer,
//		&bmpInfo, DIB_PAL_COLORS);
//	SelectObject(compatibleDC, mBitmap);
//	BitBlt(hdc, 0, 0, WinWidth, WinHeight, compatibleDC, 0, 0, SRCCOPY);
//	DeleteObject(mBitmap);
//	EndPaint(hwnd, &ps);
//	ReleaseDC(hwnd, hdc);
//}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static HDC hdc;
	static HDC compatibleDC;

	//HBITMAP hBMP = NULL;
    switch (message)
    {
	case WM_CREATE:
		{
#ifdef PAINTLENNA
			g_hBMP = (HBITMAP)LoadImage(NULL, L"Image/Lenna.bmp", IMAGE_BITMAP, 512, 512, LR_LOADFROMFILE);
			if (g_hBMP == NULL)
				MessageBox(hWnd, L"Could not load Image/Lenna.bmp!", L"Error", MB_OK | MB_ICONEXCLAMATION);
#endif

		//	BITMAPINFO bmpInfo = { 0 };
		//	bmpInfo.bmiHeader.biBitCount = 32;
		//	bmpInfo.bmiHeader.biWidth = WinWidth;
		//	bmpInfo.bmiHeader.biHeight = WinHeight;
		//	bmpInfo.bmiHeader.biPlanes = 1;
		//	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		//
		//	hdc = GetDC(hWnd);
		//	compatibleDC = CreateCompatibleDC(hdc);
		//	HBITMAP mBitmap = CreateCompatibleBitmap(hdc, WinWidth, WinHeight);
		////Todo: update dwFrameBuffer here
		////
		//	SYSTEMTIME time = { 0 };
		//	GetLocalTime(&time);
		//	int blue = (int)((double)time.wMilliseconds / 1000 * 255);
		//	int green = sizeof(DWORD);
		//	DWORD red = ((blue)+((2) << 8) + ((4) << 16) + ((0) << 24));
		//	//memcpy(dwFrameBuffer, &red, WinWidth * WinHeight * sizeof(DWORD));
		//	for (int i = 0; i < WinHeight; i++)
		//		for (int j = 0; j < WinWidth; j++)
		//		{
		//			dwFrameBuffer[i * WinWidth + j] = red;
		//			//SetPixel(compatibleDC, i, j, RGB(0, 0, blue));
		//		}
		////
		//	SetDIBits(compatibleDC, mBitmap, 0, WinHeight, dwFrameBuffer,
		//		&bmpInfo, DIB_PAL_COLORS);
		//	SelectObject(compatibleDC, mBitmap);
		//	DeleteObject(mBitmap);
		//	ReleaseDC(hWnd, hdc);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
#ifdef PAINTLENNA
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HDC hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, g_hBMP);

			BitBlt(hdc, 0, 0, 512, 512, hdcMem, 0, 0, SRCCOPY);
			DeleteObject(g_hBMP);
			DeleteDC(hdcMem);
			EndPaint(hWnd, &ps);
#endif
			hdc = BeginPaint(hWnd, &ps);
			BITMAPINFO bmpInfo = { 0 };
			bmpInfo.bmiHeader.biBitCount = 32;
			bmpInfo.bmiHeader.biWidth = WinWidth;
			bmpInfo.bmiHeader.biHeight = WinHeight;
			bmpInfo.bmiHeader.biPlanes = 1;
			bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

			//hdc = GetDC(hWnd);
			compatibleDC = CreateCompatibleDC(hdc);
			HBITMAP mBitmap = CreateCompatibleBitmap(hdc, WinWidth, WinHeight);
			//Todo: update dwFrameBuffer here
			//
			clock_t start = clock();//paint start clock
			{
				SYSTEMTIME time = { 0 };
				GetLocalTime(&time);
				int blue = (int)((double)time.wMilliseconds / 1000 * 255);
				for (int i = 0; i < WinHeight; i++)
					for (int j = 0; j < WinWidth; j++)
					{
						dwFrameBuffer[i * WinWidth + j] = ((blue)+((0) << 8) + ((0) << 16) + ((0) << 24));
					}
			}
			clock_t stop = clock();//paint end clock
			//calculate fps
			double dur = (double)(stop - start) / (double)CLOCKS_PER_SEC;
			int fps = (int)min(1000, 1.0 / dur);
			//
			SetDIBits(compatibleDC, mBitmap, 0, WinHeight, dwFrameBuffer, &bmpInfo, DIB_PAL_COLORS);
			SelectObject(compatibleDC, mBitmap);
			//--draw text
			SetTextColor(compatibleDC, RGB(255, 255, 255));
			SetBkMode(compatibleDC, TRANSPARENT);
			char fpsstr[32];
			sprintf_s(fpsstr, "FPS: %d", fps);
			TextOutA(compatibleDC, 0, 5 + 20 * 2, fpsstr, strlen(fpsstr));

			// draw bitmap
			BitBlt(hdc, 0, 0, WinWidth, WinHeight, compatibleDC, 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
			DeleteObject(mBitmap);
			DeleteDC(compatibleDC);
			ReleaseDC(hWnd, hdc);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		break;
	//禁止背景擦除 防止闪烁?
	case WM_ERASEBKGND:
		return true;
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

#ifdef D3D
// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

															// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&device,
		NULL,
		&dc);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	device->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	dc->OMSetRenderTargets(1, &backbuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	dc->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphics();
}

// this is the function used to render a single frame
void RenderFrame(void)
{
	// clear the back buffer to a deep blue
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	dc->ClearRenderTargetView(backbuffer, color);

	// do 3D rendering on the back buffer here
	// select which vertex buffer to display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// select which primtive type we are using
	dc->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	dc->Draw(3, 0);
	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}

// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
	// close and release all existing COM objects
	swapchain->Release();
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	pVBuffer->Release();
	backbuffer->Release();
	device->Release();
	dc->Release();
}

// this is the function that creates the shape to render
void InitGraphics()
{
	// create a triangle using the VERTEX struct
	VERTEX OurVertices[] =
	{
		{ 0.0f, 0.5f, 0.0f, Vec4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ 0.45f, -0.5, 0.0f, Vec4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ -0.45f, -0.5f, 0.0f, Vec4(0.0f, 0.0f, 1.0f, 1.0f) }
	};


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	device->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


												   // copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	dc->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	dc->Unmap(pVBuffer, NULL);                                      // unmap the buffer
}


// this function loads and prepares the shaders
void InitPipeline()
{
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DCompileFromFile(L"BasicShader.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
	D3DCompileFromFile(L"BasicShader.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);
	// encapsulate both shaders into shader objects
	device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	dc->VSSetShader(pVS, 0, 0);
	dc->PSSetShader(pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	dc->IASetInputLayout(pLayout);
}
#endif // D3D
