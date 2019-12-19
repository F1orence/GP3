#include "pch.h"
#include "Entity.h"

Entity::Entity()
{
	m_transform = new Transform();
}

void Entity::AddComponent(Component* c)
{
	c->m_entity = this;
	c->OnAttach();
	m_components.push_back(c);
}

void Entity::OnUpdate(float deltaTime)
{
	for (auto& c : m_components)
	{
		c->OnUpdate(deltaTime);
	}
}

void Entity::OnRender()
{
	for (auto& c : m_components)
	{
		c->OnRender();
	}
}

void Entity::DeleteComponents()
{
	delete &m_components;
}


//Entity::~Entity
//{
	//delete[] &m_components;
//}

