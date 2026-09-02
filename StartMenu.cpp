#include "StartMenu.h"
#include "GamePlay.h"
#include "Scene.h"

void StartMenu::BreakAllPanels()
{
	if (backGround != nullptr) {
		delete backGround;
		backGround = nullptr;
	}
	if (foreGround != nullptr) {
		delete foreGround;
		foreGround = nullptr;
	}
}

StartMenu::StartMenu() {
	LoadBackGroundState();
}

StartMenu::~StartMenu()
{
	BreakAllPanels();
}

void StartMenu::LoadBackGroundState()
{
	FColor dummy = FColor{ 0,0,0,0 };
	currentState = BackGround;
	BreakAllPanels();
	backGround = new Square(FVector{ 0.0f, 0.0f, 1.0f }, dummy, 1.0f, EImage::EI_BackgroundStart_With_Menu);
	//backGround = new Square(FVector{0.0f, 0.0f, 1.0f}, dummy, 1.0f, EImage::EI_BackgroundStart);
	frameCounter = 0;
}

void StartMenu::update()
{
	FColor dummy = FColor{ 0,0,0,0 };
	if (currentState == BackGround) {
		frameCounter++;
		if (frameCounter > flashCycle) {
			if (foreGround != nullptr) { delete foreGround; foreGround = nullptr; }
			else foreGround = new Square(FVector{ 0.0f, 0.0f, 0.0f }, dummy, 1.0f, EImage::EI_BackgroundStart);
			//else foreGround = new Square(FVector{ 0.0f, 0.0f, 0.0f }, dummy, 1.0f, EImage::EI_BackgroundStart_With_Menu);
			frameCounter = 0;
		}

		if (lastPressing == EKeyStatus::NONE) {

			if (InputManager::GetInstance()->GetState(EKeyStatus::SPACE))
			{
				lastPressing = SPACE;
				ActiveScene* activeScene = ActiveScene::getInstance();

				//OutputDebugStringA("Space pressed");

				GamePlay* newGameplay = new GamePlay();
				activeScene->requestChangeScene(newGameplay);
			}
		}
		
		else {
			
			if (!InputManager::GetInstance()->GetState(lastPressing))
				lastPressing = NONE;
		}
		
	}
}