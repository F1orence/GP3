#pragma once
#include "pch.h"
#include "Common.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"

class Resources
{
private:
	//TODO: use smart pointers
	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_shaderPrograms;
	std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
	static Resources* m_instance;
	Resources();

public:
	static Resources* GetInstance();

	void AddShader(ShaderProgram* shader, std::string name) 
	{
		m_shaderPrograms[name] = std::shared_ptr<ShaderProgram>(shader);
	};

	//adds model to the model map
	void AddModel(const std::string& directory);

	void AddModel(const std::string& name, Model* m)
	{
		m_models[name] = std::shared_ptr<Model>(m);;
	}

	void AddTexture(const std::string& directory);

	void AddTexture(const std::string& name, Texture* t) 
	{
		m_textures[name] = std::shared_ptr<Texture>(t);;
	};

	std::shared_ptr<ShaderProgram> GetShader(const std::string& name);

	std::shared_ptr<Model> GetModel(const std::string& name);

	std::shared_ptr<Texture> GetTexture(const std::string& name);

	void ReleaseResources();
};

