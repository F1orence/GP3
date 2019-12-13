#pragma once
#include "pch.h"
#include "Log.h"
#include "Entity.h"
#include "Resources.h"
#define WINDOW_W Application::GetInstance()->GetWindowWidth()
#define WINDOW_H Application::GetInstance()->GetWindowHeight()

#include <Windows.h>

enum AppState
{
	INITILISING, RUNNING, QUITTING
};

enum EntityEnums
{
	NADA, CAM, DIR_LIGHT
};

class Camera; //forward declaration

class Application
{
private:
	//private variables
	static Application* m_application;
	SDL_Window* m_window = nullptr;
	SDL_GLContext m_glContext = nullptr;
	int m_windowWidth = 1280;
	int m_windowHeight = 720;
	AppState m_appState = AppState::INITILISING;
	float m_worldDeltaTime = 0.f;

	GLuint currVAO;

	bool mouseSet, lookEnabled;
	glm::vec3 lightDir, lightCol = glm::vec3(0.5f,0.5f,0.5f);
	glm::mat4 m_uiVP;

	std::vector<Entity*> m_entities, m_UIentities;
	Camera* m_mainCamera = nullptr;

	//private functions
	Application();
	void Init();
	void OpenGlInit();
	void Loop();
	void Quit();
	void Update(float deltaTime);
	void Render();
	void GameInit();

	glm::ivec2 deltaMousePos, oldMousePos, baseMousePos;


	int debugMode;
	enum DebugModes { DEBUG_ALL, DEBUG_ERRORS_ONLY, DEBUG_HIDE_ALL };

public:
	//public functions
	~Application();
	static Application* GetInstance();
	void Run();
	inline int GetWindowHeight() { return m_windowHeight; }
	inline int GetWindowWidth() { return m_windowWidth; }

	inline Camera* GetCamera() { return m_mainCamera; }
	inline glm::vec3 GetMainLight() { return lightDir; }
	inline glm::vec3 GetAmbientColour() { return lightCol; }
	inline glm::mat4 GetUICam() { return m_uiVP; }
	void SetCamera(Camera* camera);

};