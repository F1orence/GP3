#pragma once
#include "pch.h"
#include "Log.h"
#include "Entity.h"
#include "Resources.h"
#define WINDOW_W Application::GetInstance()->GetWindowWidth()
#define WINDOW_H Application::GetInstance()->GetWindowHeight()

#include <Windows.h>
#include "SceneLoader.h"

enum AppState
{
	INITILISING, RUNNING, QUITTING
};

enum EntityEnums
{
	POINT_LIGHT, DIR_LIGHT
};

enum TransformStates
{
	NONE, POS, ROT, SCALE
};

class Camera; //forward declaration
//class SceneLoader;

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
	float scaleBounce = 0;

	GLuint currVAO;

	glm::vec4 mouseConstraints = glm::vec4(50, 50, -50, -50);
	int mouseInt;
	bool mouseSet, lookEnabled, isPossessing;
	glm::vec3 lightDir, lightCol = glm::vec3(0.5f,0.5f,0.5f);
	glm::mat4 m_uiVP;

	Transform m_posessionTransform;

	std::vector<Entity*> m_entities, m_UIentities;
	Camera* m_mainCamera = nullptr;
	Entity* m_camEntity;
	Entity* m_targetEntity = NULL;
	Entity* m_currEntity = NULL;

	SceneLoader m_sceneLoader;

	//private functions
	Application();
	void Init();
	void OpenGlInit();
	void Loop();
	void Quit();
	void Update(float deltaTime);
	void Render();
	void GameInit();
	void PossessionUpdate();

	glm::ivec2 deltaMousePos, oldMousePos, baseMousePos, mouseOffset, currMousePos;

	glm::vec4 selectionCol = glm::vec4(0.9f,0.9f,0.9f,0.6f);

	glm::vec3 m_pointLightPos;

	int debugMode, m_transformSelection;
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
	inline glm::vec4 GetSelectionColour() { return selectionCol; }

	inline glm::mat4 GetUICam() { return m_uiVP; }
	void SetCamera(Camera* camera);

	// getters for loading
	inline std::vector<Entity*> GetEntities() { return m_entities; }
	inline std::vector<Entity*> GetUIEntities() { return m_UIentities; }
	inline Entity* GetCamEntity() { return m_camEntity; }


};