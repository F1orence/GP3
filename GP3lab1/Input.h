#pragma once
#include "Common.h"
#include "InputState.h"

#define INPUT Input::GetInstance()

class Input
{
private:
	Input();
	static Input* m_instance;
	InputState m_state;

public:
	static Input* GetInstance();
	void SetKey(SDL_Keycode key, bool state);
	bool GetKey(SDL_Keycode key); //check if it's currently down or not
	glm::ivec2 GetMousePos() { return m_state.mousePos; }
	void MoveMouse(glm::ivec2 delta) { m_state.mousePos += delta; }
	void ResetMouse() { m_state.mousePos = glm::vec2(0, 0); }
	bool LeftMousePressed(SDL_MouseButtonEvent& button);
	bool RightMousePressed(SDL_MouseButtonEvent& button);
};

