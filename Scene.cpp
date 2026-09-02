#include "Scene.h"
#include <Windows.h>
#include  <cstdlib>

ActiveScene* ActiveScene::activeScene = nullptr;

void ActiveScene::setInitialScene(Scene* initialScene)
{
	if (currentScene != nullptr) {
		MessageBox(
			nullptr,
			L"Runnig Scene Exists.",
			L"Scene Initialize Error",
			MB_OK | MB_ICONERROR
		);

		std::exit(EXIT_FAILURE);
	}
	currentScene = initialScene;
}

void ActiveScene::update()
{
	if (currentScene != nullptr)
		currentScene->update();

	if (sceneRequested != nullptr) {
		if (currentScene != nullptr)
			delete currentScene;

		currentScene = sceneRequested;
		sceneRequested = nullptr;
	}
	else if (sceneExitRequested == true) {
		if (currentScene != nullptr)
			delete currentScene;
		currentScene = nullptr;
		programEnd = true;
	}
}

void ActiveScene::requestChangeScene(Scene* newScene)
{
	if (sceneRequested != nullptr) {
		MessageBox(
			nullptr,
			L"Two Scenes are requested in Single frame.",
			L"Scene Change Error",
			MB_OK | MB_ICONERROR
		);

		std::exit(EXIT_FAILURE);
	}

	sceneRequested = newScene;
}

void ActiveScene::requestExitScene()
{
	if (sceneRequested != nullptr) {
		MessageBox(
			nullptr,
			L"Two Scenes are requested in Single frame.",
			L"Scene Exit Error",
			MB_OK | MB_ICONERROR
		);

		std::exit(EXIT_FAILURE);
	}

	sceneExitRequested = true;
}

