#pragma once

class Scene {
public:
	virtual void update() = 0;

	virtual ~Scene() = default;
};

class ActiveScene {
private:
	static ActiveScene* activeScene;

	Scene* currentScene = nullptr;
	Scene* sceneRequested = nullptr;
	bool sceneExitRequested = false;

public:
	bool programEnd = false;
	ActiveScene& operator=(const ActiveScene&) = delete;

	static ActiveScene* getInstance() {
		if (activeScene == nullptr)
		{
			activeScene = new ActiveScene;
		}
		
		return activeScene;
	}

	static void DestroyInstance()
	{
		delete activeScene;
	}

	// called by main
	void setInitialScene(Scene* initialScene);

	// called by main (every frame)
	void update();

	// called by Scene
	void requestChangeScene(Scene* newScene); // newScene으로 nullptr 주면 게임종료

	void requestExitScene();

private:
	ActiveScene() = default;
	~ActiveScene() = default;

};

