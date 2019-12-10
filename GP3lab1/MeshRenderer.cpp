#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"

MeshRenderer::MeshRenderer(Mesh* mesh, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture)
{
	m_mesh = mesh;
	m_program = program;
	m_texture = texture;
	m_texture->Bind();
}

/*
MeshRenderer::MeshRenderer(Mesh* mesh, ShaderProgram* program)
{
	m_mesh = mesh;
	m_program = program;
}

MeshRenderer::MeshRenderer(Model* model, ShaderProgram* program, Texture* texture)
{
	m_mesh = model->GetMeshes().at(0);
	m_program = program;

	m_model = model;
	m_texture = texture;
	m_texture->Bind();

	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();
		if (m_texture) m_texture->Bind();
		GL_ATTEMPT(glDrawElements(GL_TRIANGLES,
			mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	}
}
*/

MeshRenderer::MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture)
{
	m_mesh = model->GetMeshes().at(0);
	m_program = program;

	m_model = model;
	m_texture = texture;
	m_texture->Bind();

	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();
		if (m_texture) m_texture->Bind();
		GL_ATTEMPT(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	}
}

void MeshRenderer::OnUpdate(float deltaTime)
{
	
}

void MeshRenderer::OnRender()
{
	m_program->Use();
	//set uniforms here!

	glm::mat4 model = m_entity->GetTransform()->GetTransformationMatrix();

	glm::mat4 mvp = Application::GetInstance()->GetCamera()->Get() * model;

	m_MVP = mvp;

	glBindAttribLocation(m_program->Get(), 0, "position");


	GLuint loc = glGetUniformLocation(m_program->Get(), "MVP");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(mvp)); // give updated MVP

	glm::vec3 oColor = glm::vec3(1.f, 0.f, 0.f);
	loc = glGetUniformLocation(m_program->Get(), "objectColor");
	glUniform3f(loc, oColor.x, oColor.y, oColor.z);

	loc = glGetUniformLocation(m_program->Get(), "diffuse");
	glUniform1i(loc, 0); // THIS SHOULD NOT EXIST LIKE THIS, SHOULD GET A UNIQUE INT FROM TEXTURE

	// -------------------------------------------------------------------MAYBE PUT THIS IN AN IF STATEMENT
	loc = glGetUniformLocation(m_program->Get(), "lightDir");
	glm::vec3 lightVec = Application::GetInstance()->GetMainLight();
	glUniform3f(loc,  lightVec.x, lightVec.y, lightVec.z);

	loc = glGetUniformLocation(m_program->Get(), "ambLight");
	lightVec = Application::GetInstance()->GetAmbientColour();
	glUniform3f(loc, lightVec.x, lightVec.y, lightVec.z);


	loc = glGetUniformLocation(m_program->Get(), "specValue");
	//glm::vec3 lightVec = Application::GetInstance()->GetMainLight();
	glUniform1f(loc, 0.3);

	loc = glGetUniformLocation(m_program->Get(), "modelMat");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(model));

	m_texture->Bind();
	m_mesh->Bind();

	GL_ATTEMPT(glDrawElements(GL_TRIANGLES, m_mesh->GetIndiciesCount(),GL_UNSIGNED_INT, 0));

}
