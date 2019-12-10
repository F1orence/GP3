#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"

class MeshRenderer : public Component
{
private:
	Mesh* m_mesh;
	std::shared_ptr<Model> m_model;
	std::shared_ptr<ShaderProgram> m_program;
	std::shared_ptr<Texture> m_texture;
	glm::mat4 m_MVP;


public:
	// Inherited via Component
	//MeshRenderer(Mesh* mesh, std::shared_ptr<ShaderProgram> program);
	//MeshRenderer(Model* model, ShaderProgram* program, Texture* texture);
	MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture);
	MeshRenderer(Mesh* mesh, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture);
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	glm::mat4 GetMVP() {return m_MVP;}
};


