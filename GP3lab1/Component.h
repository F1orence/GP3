#pragma once
class Entity;

class Component
{
protected:
	Entity* m_entity = nullptr;
	friend class Entity;


public:
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
	virtual void OnAttach() = 0;
};

