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
	int m_type = 0; // 0 = basic shading, 1 = ADS shading, 2 = UI, 3 = tiled texture
	float m_spec = 0;
	bool m_isLit = false;
	glm::vec3 m_plDir;
	float m_plInt;


public:
	// Inherited via Component
	//MeshRenderer(Mesh* mesh, std::shared_ptr<ShaderProgram> program);
	//MeshRenderer(Model* model, ShaderProgram* program, Texture* texture);
	MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture);
	~MeshRenderer();
	MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture, int type);
	MeshRenderer(Mesh* mesh, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture);
	MeshRenderer(Mesh* mesh, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture, int type);
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnAttach() override;
	void GiveTexture(std::shared_ptr<Texture> texture) { m_texture = texture; };
	void SetSpecular(float spec) { m_spec = spec; };
	bool IsLit() { return m_isLit; };
	void SetPLVal(glm::vec3 dir, float intensity) { m_plDir = dir; m_plInt = intensity; };
	glm::mat4 GetMVP() {return m_MVP;}
};


