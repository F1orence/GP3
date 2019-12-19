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

MeshRenderer::MeshRenderer(Mesh* mesh, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture, int type)
{
	m_mesh = mesh;
	m_program = program;
	m_texture = texture;
	m_texture->Bind();
	m_type = type;
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

MeshRenderer::MeshRenderer(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture, int type)
{
	m_mesh = model->GetMeshes().at(0);
	m_program = program;

	m_model = model;
	m_texture = texture;
	m_texture->Bind();

	m_type = type;

	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();
		if (m_texture) m_texture->Bind();
		GL_ATTEMPT(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	}

	if (m_type == 1)
	{
		m_isLit = true;
	}
}

void MeshRenderer::OnUpdate(float deltaTime)
{
	
}

void MeshRenderer::OnAttach()
{

}

void MeshRenderer::OnRender()
{
	m_program->Use();

	glBindAttribLocation(m_program->Get(), 0, "position");

	GLuint loc;

	glm::vec4 oColor = Application::GetInstance()->GetSelectionColour();
	loc = glGetUniformLocation(m_program->Get(), "objectColor");
	glUniform4f(loc, oColor.r, oColor.g, oColor.b, oColor.a);

	switch (m_type)
	{
	case (0): // basic, with perspective
	{
		glm::mat4 model = m_entity->GetTransform()->GetTransformationMatrix();

		glm::mat4 mvp = Application::GetInstance()->GetCamera()->Get() * model;

		m_MVP = mvp;

		loc = glGetUniformLocation(m_program->Get(), "MVP");
		glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(mvp));
	}
	break;

	case (1): // ADS lighting
	{
		glm::mat4 model = m_entity->GetTransform()->GetTransformationMatrix();
		glm::mat4 mvp = Application::GetInstance()->GetCamera()->Get() * model;
		m_MVP = mvp;
		loc = glGetUniformLocation(m_program->Get(), "MVP");
		glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(mvp));


		loc = glGetUniformLocation(m_program->Get(), "lightDir");
		glm::vec3 lightVec = Application::GetInstance()->GetMainLight();
		glUniform3f(loc, lightVec.x, lightVec.y, lightVec.z);

		loc = glGetUniformLocation(m_program->Get(), "ambLight");
		lightVec = Application::GetInstance()->GetAmbientColour();
		glUniform3f(loc, lightVec.x, lightVec.y, lightVec.z);


		loc = glGetUniformLocation(m_program->Get(), "specValue");
		glUniform1f(loc, m_spec);

		loc = glGetUniformLocation(m_program->Get(), "modelMat");
		glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(model));

		loc = glGetUniformLocation(m_program->Get(), "pLightDir");
		glUniform3f(loc, m_plDir.x, m_plDir.y, m_plDir.z);


		loc = glGetUniformLocation(m_program->Get(), "pLightInt");
		glUniform1f(loc, m_plInt);
	}
	break;

	case (2): // basic, using a fixed perspective
	{
		glm::mat4 model = m_entity->GetTransform()->GetTransformationMatrix();

		glm::mat4 mvp = Application::GetInstance()->GetUICam() * model;

		m_MVP = mvp;

		GLuint loc = glGetUniformLocation(m_program->Get(), "MVP");
		glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(mvp));
	}
	break;


	case (3): // basic, with perspective and repeating textures
	{
		glm::mat4 model = m_entity->GetTransform()->GetTransformationMatrix();

		glm::mat4 mvp = Application::GetInstance()->GetCamera()->Get() * model;

		m_MVP = mvp;

		loc = glGetUniformLocation(m_program->Get(), "MVP");
		glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(mvp));

		loc = glGetUniformLocation(m_program->Get(), "repeats");
		glUniform2f(loc, 15, 15);
	}
	break;
	}


	loc = glGetUniformLocation(m_program->Get(), "diffuse");
	glUniform1i(loc, 0); 

	m_texture->Bind();
	m_mesh->Bind();

	GL_ATTEMPT(glDrawElements(GL_TRIANGLES, m_mesh->GetIndiciesCount(),GL_UNSIGNED_INT, 0));

}

MeshRenderer::~MeshRenderer()
{
	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->ClearBuffers();
		delete mesh;
	}
}

/*
void MeshRenderer::OnRender(int type)
{
	m_program->Use();
	//set uniforms here!

	glm::mat4 model = m_entity->GetTransform()->GetTransformationMatrix();

	//if (type == 1)
	//{
		glm::mat4 mvp = Application::GetInstance()->GetUICam() * model;
		m_MVP = mvp;
	//}

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
	glUniform3f(loc, lightVec.x, lightVec.y, lightVec.z);

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

	GL_ATTEMPT(glDrawElements(GL_TRIANGLES, m_mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));

}
*/
