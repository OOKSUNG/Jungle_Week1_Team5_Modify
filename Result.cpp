#include "Result.h"

#include "GamePlay.h"
#include "StartMenu.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "SoundManager.h"

void Result::createMany()
{
	FColor dummy = FColor{ 0,0,0,0 };
	if (bGameCleared)
		gameClear = new Square(titlePos, dummy, titleSize, EImage::EI_StringGameClear);
	else
		gameOver = new Square(titlePos, dummy, titleSize, EImage::EI_StringGameOver);

	totalScore = new Square(totalScorePos, dummy, totalScoreSize, EImage::EI_StringTotalScore);

	p10000 = new Square(p10000Pos, dummy, pSize, (EImage)p10000Num);
	p01000 = new Square(p01000Pos, dummy, pSize, (EImage)p01000Num);
	p00100 = new Square(p00100Pos, dummy, pSize, (EImage)p00100Num);
	p00010 = new Square(p00010Pos, dummy, pSize, (EImage)p00010Num);
	p00001 = new Square(p00001Pos, dummy, pSize, (EImage)p00001Num);

	creditButton = new Square(creditButtonPos, dummy, buttonSize, EImage::EI_ButtonCredit);
	replayButton = new Square(replayButtonPos, dummy, buttonSize, EImage::EI_ButtonReplay);
	exitButton = new Square(exitButtonPos, dummy, buttonSize, EImage::EI_ButtonExit);
}

Result::Result(bool bGameCleared, int score):
	bGameCleared(bGameCleared), score(score)
{
	if (bGameCleared)
	{
		SoundManager::GetInstance()->PlaySoundEffect(ESoundEffect::ESE_Win);
	}
	else
	{
		SoundManager::GetInstance()->PlaySoundEffect(ESoundEffect::ESE_Lost);
	}

	p00001Num = score % 10;
	score /= 10;
	p00010Num = score % 10;
	score /= 10;
	p00100Num = score % 10;
	score /= 10;
	p01000Num = score % 10;
	score /= 10;
	p10000Num = score % 10;
	loadReplayButtonState();

	//SoundManager::GetInstance()->PlaySoundEffect(ESoundEffect::ESE_End);
}

Result::~Result()
{
	if (backGround != nullptr) delete backGround;
	if (gameClear != nullptr) delete gameClear;
	if (gameOver != nullptr) delete gameOver;
	if (totalScore != nullptr) delete totalScore;
	if (p10000 != nullptr) delete p10000;
	if (p01000 != nullptr) delete p01000;
	if (p00100 != nullptr) delete p00100;
	if (p00010 != nullptr) delete p00010;
	if (p00001 != nullptr) delete p00001;
	if (creditButton != nullptr) delete creditButton;
	if (replayButton != nullptr) delete replayButton;
	if (exitButton != nullptr) delete exitButton;
	if (credit != nullptr) delete credit;


	SoundManager::GetInstance()->StopAllSoundEffect();
}

void Result::loadCreditButtonState()
{
	FColor dummy = FColor{ 0,0,0,0 };
	if (currentState == ReplayButton || currentState == ExitButton) {
		replayButton->setRadius(buttonSize);
		exitButton->setRadius(buttonSize);
	}
	else if (currentState == Credit) {
		delete credit;
		credit = nullptr;
		createMany();
	}
	currentState = CreditButton;
	frameCounter = 0;
}

void Result::loadReplayButtonState()
{
	FColor dummy = FColor{ 0,0,0,0 };
	if (currentState == Initial || currentState == Credit) {
		if (currentState == Initial) {
			if (bGameCleared)
			{
				backGround = new Square(backGroundPos, dummy, backGroundSize, EImage::EI_BackgroundGameClear, 2);
			}
			else
			{
				backGround = new Square(backGroundPos, dummy, backGroundSize, EImage::EI_BackgroundGameOver, 2);
			}
		}
		else {
			delete credit;
			credit = nullptr;
		}
		createMany();
	}
	else if (currentState == CreditButton || currentState == ExitButton) {
		creditButton->setRadius(buttonSize);
		exitButton->setRadius(buttonSize);
	}
	currentState = ReplayButton;
	frameCounter = 0;
}

void Result::loadExitButtonState()
{
	FColor dummy = FColor{ 0,0,0,0 };
	if (currentState == ReplayButton || currentState == CreditButton) {
		replayButton->setRadius(buttonSize);
		creditButton->setRadius(buttonSize);
	}
	else if (currentState == Credit) {
		delete credit;
		credit = nullptr;
		createMany();
	}
	currentState = ExitButton;
	frameCounter = 0;
}

void Result::loadCreditState()
{
	FColor dummy = FColor{ 0,0,0,0 };
	if (gameClear != nullptr) { delete gameClear; gameClear = nullptr; }
	if (gameOver != nullptr) { delete gameOver; gameOver = nullptr; }

	delete totalScore;
	delete p10000;
	delete p01000;
	delete p00100;
	delete p00010;
	delete p00001;

	delete creditButton;
	delete replayButton;
	delete exitButton;

	totalScore = nullptr;
	p10000 = nullptr;
	p01000 = nullptr;
	p00100 = nullptr;
	p00010 = nullptr;
	p00001 = nullptr;

	creditButton = nullptr;
	replayButton = nullptr;
	exitButton = nullptr;

	credit = new Square(creditPos, dummy, creditSize, EImage::EI_Credit); // TODO: add Credit Texture
	currentState = Credit;
}

static double currentWave(int& current, int period) {
	current++;
	if (current >= period) current = 0;
	const double pi = 3.14159265358979323846;
	double input = (double)current / (double)period * 2 * pi;
	return sin(input) * 0.2;
}

void Result::update()
{
	if (currentState == ReplayButton) {
		double wave = currentWave(frameCounter, period);
		float size = buttonSize + wave * buttonSize;
 		replayButton->setRadius(size);
		if (lastPressing == Initial) {
			counter2++;
			if (counter2 > freezingFrameTime) {
				lastPressing = NONE;
			}
		}
		else if (lastPressing == NONE) {
			if (InputManager::GetInstance()->GetState(EKeyStatus::SPACE))
			{
				lastPressing = SPACE;
				InputManager::GetInstance()->SetState(EKeyStatus::SPACE, false);

				ActiveScene* activeScene = ActiveScene::getInstance();
				//GamePlay* newGameplay = new GamePlay();
				StartMenu* newStartMenu = new StartMenu();
				
				activeScene->requestChangeScene(newStartMenu);
			}
			else if (InputManager::GetInstance()->GetState(EKeyStatus::LEFT)) {
				lastPressing = LEFT;
				loadCreditButtonState();
			}
			else if (InputManager::GetInstance()->GetState(EKeyStatus::RIGHT)) {
				lastPressing = RIGHT;
				loadExitButtonState();
			}
		}
		else {
			if (!InputManager::GetInstance()->GetState(lastPressing))
				lastPressing = NONE;
		}
	}
	else if (currentState == CreditButton) {
		double wave = currentWave(frameCounter, period);
		float size = buttonSize + wave * buttonSize;
		creditButton->setRadius(size);
		if (lastPressing == NONE) {
			if (InputManager::GetInstance()->GetState(EKeyStatus::SPACE)) {
				lastPressing = SPACE;
				loadCreditState();
			}
			else if (InputManager::GetInstance()->GetState(EKeyStatus::LEFT)) {
				lastPressing = LEFT;
				loadExitButtonState();
			}
			else if (InputManager::GetInstance()->GetState(EKeyStatus::RIGHT)) {
				lastPressing = RIGHT;
				loadReplayButtonState();
			}
		}
		else {
			if (!InputManager::GetInstance()->GetState(lastPressing))
				lastPressing = NONE;
		}
	}
	else if (currentState == ExitButton) {
		double wave = currentWave(frameCounter, period);
		float size = buttonSize + wave * buttonSize;
		exitButton->setRadius(size);
		if (lastPressing == NONE) {
			if (InputManager::GetInstance()->GetState(EKeyStatus::SPACE)) {
				lastPressing = SPACE;
				ActiveScene* activeScene = ActiveScene::getInstance();
				activeScene->requestExitScene();
			}
			else if (InputManager::GetInstance()->GetState(EKeyStatus::LEFT)) {
				lastPressing = LEFT;
				loadReplayButtonState();
			}
			else if (InputManager::GetInstance()->GetState(EKeyStatus::RIGHT)) {
				lastPressing = RIGHT;
				loadCreditButtonState();
			}
		}
		else {
			if (!InputManager::GetInstance()->GetState(lastPressing))
				lastPressing = NONE;
		}
	}
	else {
		if (lastPressing == NONE) {
			if (InputManager::GetInstance()->GetState(EKeyStatus::SPACE))
			{
				lastPressing = SPACE;
				loadCreditButtonState();
			}
		}
		else {
			if (!InputManager::GetInstance()->GetState(lastPressing))
				lastPressing = NONE;
		}
	}
}
