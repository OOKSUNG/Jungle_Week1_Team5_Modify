#include <windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "Math.h"
#include "Renderer.h"
#include "Sphere.h"
#include "Primitive.h"
#include "Ball.h"
#include "Circle.h"
#include "Triangle.h"
#include "Square.h"
#include "SoundManager.h"

#include "InputManager.h"
#include "Scene.h"
#include "StartMenu.h"

enum ETypePrimitive
{
	EPT_Triangle,
	EPT_Cube,
	EPT_Sphere,
	EPT_Max
};

int UBall::TotalNumBalls = 0;
bool UBall::bGravity = true;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 각종 메시지를 처리할 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}

	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			InputManager::GetInstance()->OnKeyDown(SPACE);
			break;
		case VK_LEFT:
			InputManager::GetInstance()->OnKeyDown(LEFT);
			break;
		case VK_RIGHT:
			InputManager::GetInstance()->OnKeyDown(RIGHT);
			break;
		case VK_UP:
			InputManager::GetInstance()->OnKeyDown(UP);
			break;
		case VK_DOWN:
			InputManager::GetInstance()->OnKeyDown(DOWN);
			break;
		case VK_RETURN:
			InputManager::GetInstance()->OnKeyDown(ENTER);
			break;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_SPACE:
			InputManager::GetInstance()->OnKeyUP(SPACE);
			break;
		case VK_LEFT:
			InputManager::GetInstance()->OnKeyUP(LEFT);
			break;
		case VK_RIGHT:
			InputManager::GetInstance()->OnKeyUP(RIGHT);
			break;
		case VK_UP:
			InputManager::GetInstance()->OnKeyUP(UP);
			break;
		case VK_DOWN:
			InputManager::GetInstance()->OnKeyUP(DOWN);
			break;
		case VK_RETURN:
			InputManager::GetInstance()->OnKeyUP(ENTER);
			break;
		}
		return 0;
	case WM_DESTROY:
		// Signal that the app should quit
		OutputDebugStringA("!!! WM_DESTROY !!!\n");
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPervInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// 윈도우 클래스 이름
	WCHAR WindowClass[] = L"jungleWindowClass";

	// 윈도우 클래스 이름
	WCHAR Title[] = L"Game Tech Lab";

	// 각종 메시지를 처리할 함수인 WndProc의 함수 포인터를 WindowClass 구조체에 넣는다.
	WNDCLASSW wndclass = {0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

	// 윈도우 클래스 등록
	RegisterClassW(&wndclass);

	// 1024x1024 크기에 윈도우 생성
	// HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024, nullptr, nullptr, hInstance, nullptr);
	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024, nullptr, nullptr, hInstance, nullptr);

	// 각종 생성하는 코드를 여기에 추가

	// Renderer Class 생성
	URenderer renderer;

	// D3D11 생성하는 함수를 호출
	renderer.Create(hWnd);
	renderer.CreateShader();
	renderer.CreateConstantBuffer();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(renderer.Device, renderer.DeviceContext);

	ETypePrimitive typePrimitive = EPT_Cube;

	// 도형의 움직임 정도를 담을 offset 변수
	//const int targetFPS = 30;
	const int targetFPS = 60;
	const double targetFrameTime = 1000.0 / targetFPS;

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	double elapsedTime = 0.0;

	float DeltaTime = (float)(elapsedTime / 1000.0);
	bool bIsExit = false;

	// Initialize
	{
		renderer.InitializeResources();

		//SoundManager::CreateInstance();
		InputManager::GetInstance();
	}

	// Start scene
	{
		Square* startBackGround = new Square(FVector{ 0.0f, 0.0f, 0.0f }, FColor{ 0.5f, 0.3f, 0.5f }, 1.f, EImage::EI_BackgroundStart);

		//delete startBackGround;
		//SoundManager::GetInstance()->PlaySoundEffect(ESoundEffect::ESE_BGM);
	}

	bool bWithMenu = false;

	ActiveScene* activeScene = ActiveScene::getInstance();
	activeScene->setInitialScene(new StartMenu());

	// Main Loop 
	while (!activeScene->programEnd)
	{
		DeltaTime = (float)(elapsedTime / 1000.0);
		QueryPerformanceCounter(&startTime);
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
		}

		// System update
		{
			
			//SoundManager::GetInstance()->Update();

			/*
			if (InputManager::GetInstance()->GetState(EKeyStatus::SPACE))
			{
				OutputDebugStringA("Space pressed");
			}
			*/
		}

		// Update
		{
			activeScene->update();
		}

		// Render
		{
			renderer.Prepare();
			renderer.PrepareShader();

			// Todo: Test start scene
			/*
			{
				if (bWithMenu)
				{
					renderer.SetTextureSRV(EImage::EI_Number_9);
					bWithMenu = false;
				}
				else
				{
					renderer.SetTextureSRV(EImage::EI_BallRed);
					bWithMenu = true;
				}
			}
			*/

			Renderable::RenderAll(renderer);
		}

		// Update ImGui
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			// 이후 ImGui UI 컨트롤 추가는 여기에 위치
			ImGui::Begin("Jungle Property Window");
			{
				ImGui::Text("Hello Jungle World!");
			}
			ImGui::End();

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		renderer.SwapBuffer();

		do
		{
			Sleep(0);
			QueryPerformanceCounter(&endTime);

			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;

		} while (elapsedTime < targetFrameTime);
		////////////////////////////////////////////////////
	}

	// 소멸하는 코드를 여기에 추가
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	renderer.ReleaseConstantBuffer();
	renderer.ReleaseShader();
	renderer.Release();

	{
		//SoundManager::DeleteInstance();
		ActiveScene::DestroyInstance();
		InputManager::DestroyInstance();
	}

	//renderer.ReleaseVertexBuffer(vertexBufferTriangle);
	//renderer.ReleaseVertexBuffer(vertexBufferSquare);
	//renderer.ReleaseVertexBuffer(vertexBufferSphere);

	return 0;
}