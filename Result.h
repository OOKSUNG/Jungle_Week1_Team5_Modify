#pragma once
#include "Scene.h"
#include "Square.h"
#include "InputManager.h"

enum ResultState {
	CreditButton,
	ReplayButton,
	ExitButton,
	Credit,
	Initial
};

class Result : public Scene {
	bool bGameCleared = false;
	int score = 0;
	Square* backGround = nullptr;
	const FVector backGroundPos = FVector{ 0.0f, 0.0f, 1.0f };
	static constexpr float backGroundSize = 1.0f;

	Square* gameClear = nullptr;
	Square* gameOver = nullptr;
	const FVector titlePos = FVector{ 0.0f, 0.5f, 0.0f };
	static constexpr float titleSize = 0.75f;


	Square* totalScore = nullptr;
	const FVector totalScorePos = FVector{ -0.3f, 0.0f, 0.0f };
	static constexpr float totalScoreSize = 0.5f;

	Square* p10000 = nullptr;
	const FVector p10000Pos = FVector{ 0.38f, 0.0f, 0.0f };
	int p10000Num = 0;
	Square* p01000 = nullptr;
	const FVector p01000Pos = FVector{ 0.49f, 0.0f, 0.0f };
	int p01000Num = 0;
	Square* p00100 = nullptr;
	const FVector p00100Pos = FVector{ 0.6f, 0.0f, 0.0f };
	int p00100Num = 0;
	Square* p00010 = nullptr;
	const FVector p00010Pos = FVector{ 0.71f, 0.0f, 0.0f };
	int p00010Num = 0;
	Square* p00001 = nullptr;
	const FVector p00001Pos = FVector{ 0.82f, 0.0f, 0.0f };
	int p00001Num = 0;
	static constexpr float pSize = 0.1f;

	Square* creditButton = nullptr;
	const FVector creditButtonPos = FVector{ -0.5f, -0.5f, 0.0f };
	Square* replayButton = nullptr;
	const FVector replayButtonPos = FVector{ 0.0f, -0.5f, 0.0f };
	Square* exitButton = nullptr;
	const FVector exitButtonPos = FVector{ 0.5f, -0.5f, 0.0f };
	static constexpr float buttonSize = 0.2f;

	Square* credit = nullptr;
	const FVector creditPos = FVector{ 0.0f, 0.0f, 0.0f };
	static constexpr float creditSize = 0.8f;

	enum ResultState currentState = Initial;
	enum EKeyStatus lastPressing = FIRSTFRAME;

	int frameCounter = 0;
	const int period = 60;
	const int freezingFrameTime = 30;

	int counter2 = 0;

	void createMany();
public:
	Result(bool bGameCleared, int score);
	~Result();
	void loadCreditButtonState();
	void loadReplayButtonState();
	void loadExitButtonState();
	void loadCreditState();

	void update() override;
};