#include "Application.h"
#include "SceneLoader.h"
#include "MeshRenderer.h"
#include "CameraComp.h"
#include "Input.h"
#include "Physics.h"
#include "BoxShape.h"
#include "SphereShape.h"
#include "Resources.h"
#include "CapsuleShape.h"
#include "CylinderShape.h"
#include "RigidBody.h"
#include "Common.h"




SceneLoader::SceneLoader()
{
	
}


std::vector<Entity*> SceneLoader::LoadScene(int scene)// std::vector<Entity*> entities)
{
	std::vector<Entity*> m_entities;

	if (scene == 1)
	{
		#pragma region load resources
		//loading all resources
		Resources::GetInstance()->AddModel("cube.obj");
		Resources::GetInstance()->AddModel("sphere.obj");
		Resources::GetInstance()->AddModel("capsule.obj");
		Resources::GetInstance()->AddModel("cylinder.obj");
		Resources::GetInstance()->AddModel("LightBulb.obj");
		Resources::GetInstance()->AddModel("Lamp.obj");

		Resources::GetInstance()->AddModel("man.obj");
		Resources::GetInstance()->AddModel("xyzOBJ.obj");

		Resources::GetInstance()->AddTexture("Wood.jpg");
		Resources::GetInstance()->AddTexture("bulbTex.png");
		Resources::GetInstance()->AddTexture("lampTex.png");
		Resources::GetInstance()->AddTexture("WoodGrain.png");
		Resources::GetInstance()->AddTexture("Ground1.jpg");
		Resources::GetInstance()->AddTexture("xyzTex.jpg");
		Resources::GetInstance()->AddTexture("mainText1.png");
		Resources::GetInstance()->AddTexture("selectionText1.png");
		Resources::GetInstance()->AddTexture("transformText1.png");
		Resources::GetInstance()->AddTexture("pixels.jpg");
		Resources::GetInstance()->AddTexture("manTexture1.png");
		Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "simple_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"), "simple");
		Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "simpleRepeat_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"), "repeating");
		Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "simple_VERT.glsl", ASSET_PATH + "inverted_FRAG.glsl"), "inverted");
		Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "normals_VERT.glsl", ASSET_PATH + "ADS_FRAG.glsl"), "ADS");
		Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "simple_VERT.glsl", ASSET_PATH + "UI_FRAG.glsl"), "UI");
		Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "simple_VERT.glsl", ASSET_PATH + "oColor_FRAG.glsl"), "flatColour");
#pragma endregion

		#pragma region add stuff

		m_entities.push_back(new Entity());
		m_entities.at(m_entities.size() - 1)->GetTransform()->SetPosition(glm::vec3(-100, 100, -101));
		m_entities.at(m_entities.size() - 1)->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("LightBulb.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("bulbTex.png"), 0));
		m_entities.at(m_entities.size() - 1)->GetTransform()->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

		m_entities.push_back(new Entity()); // Light
		m_entities.at(m_entities.size() - 1)->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("Lamp.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("lampTex.png")));
		m_entities.at(m_entities.size() - 1)->GetTransform()->SetPosition(glm::vec3(-50, 100, -100));
		m_entities.at(m_entities.size() - 1)->GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));


		m_entities.push_back(new Entity());
		m_entities.at(m_entities.size() - 1)->GetTransform()->SetPosition(glm::vec3(100, 100, -101));
		m_entities.at(m_entities.size() - 1)->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("man.obj"), Resources::GetInstance()->GetShader("ADS"), Resources::GetInstance()->GetTexture("manTexture1.png"), 1));
		m_entities.at(m_entities.size() - 1)->GetComponent<MeshRenderer>()->SetSpecular(0.8f);
		m_entities.at(m_entities.size() - 1)->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));


	#pragma region adding physics objects

		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("cube.obj"), Resources::GetInstance()->GetShader("repeating"), Resources::GetInstance()->GetTexture("Ground1.jpg"),3));
		MeshRenderer* m = a->GetComponent<MeshRenderer>();
		a->GetTransform()->SetPosition(glm::vec3(0, -100.f, -20.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(2000.f, 1.f, 2000.f)));
		a->GetTransform()->SetRotation(glm::vec3(0.f, 0.f, 0.f));
		a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3(1.f, 1.f, 1.f));
		a->GetTransform()->SetScale(glm::vec3(2000.f, 1.f, 2000.f));
		a->GetComponent<RigidBody>()->Get()->setRestitution(0.6f);
		for (int i = 0; i < 10; i++)
		{
			Entity* a = new Entity();
			m_entities.push_back(a);
			a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("sphere.obj"), Resources::GetInstance()->GetShader("ADS"), Resources::GetInstance()->GetTexture("WoodGrain.png"), 1));
			a->GetComponent<MeshRenderer>()->SetSpecular(0.3f);
			a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -18.f));
			a->AddComponent<RigidBody>();
			a->GetComponent<RigidBody>()->Init(new SphereShape(1.f)); //new CylinderShape(glm::vec3(1.0f, 1.0f, 1.0f))); // //new CapsuleShape(1.f, 2.5f)); //BoxShape(glm::vec3(1.f,1.f, 1.f))); // 
			a->GetComponent<RigidBody>()->Get()->setMassProps(1.0f, btVector3(0.5f, 0.5f, 0.5f));
			//a->GetComponent<RigidBody>()->Get()->setAngularVelocity(btVector3(2.0f, 1.0f, 3.0f));
			a->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
			a->GetComponent<RigidBody>()->Get()->setRestitution(0.6f);
			a->GetComponent<RigidBody>()->Get()->setFriction(1.0f);
			//a->GetTransform()->SetRotation(glm::vec3(0.f, 0.f, 0.f));
			//a->GetComponent<RigidBody>()->ToggleGravity();
		}

		m_entities.push_back(a = new Entity());
		a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("cube.obj"), Resources::GetInstance()->GetShader("ADS"), Resources::GetInstance()->GetTexture("WoodGrain.png"), 1));
		a->GetTransform()->SetPosition(glm::vec3(18, -33, -18.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(5.f, 0.3f, 1.f)));
		a->GetTransform()->SetScale(glm::vec3(5.0f, 0.3f, 1.0f));
		a->GetComponent<RigidBody>()->Get()->setMassProps(1.0f, btVector3(1.0f, 1.0f, 1.0f));

		m_entities.push_back(a = new Entity());
		a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("cube.obj"), Resources::GetInstance()->GetShader("ADS"), Resources::GetInstance()->GetTexture("Wood.jpg"), 1));
		a->GetComponent<MeshRenderer>()->SetSpecular(0.8f);
		a->GetTransform()->SetPosition(glm::vec3(20, -35, -18.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));
		a->GetComponent<RigidBody>()->Get()->setMassProps(1.0f, btVector3(1.0f, 1.0f, 1.0f));

		m_entities.push_back(a = new Entity());
		a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("cube.obj"), Resources::GetInstance()->GetShader("ADS"), Resources::GetInstance()->GetTexture("WoodGrain.png"), 1));
		a->GetComponent<MeshRenderer>()->SetSpecular(0.7f);
		a->GetTransform()->SetPosition(glm::vec3(15, -28, -18.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));
		a->GetComponent<RigidBody>()->Get()->setMassProps(0.1f, btVector3(1.0f, 1.0f, 1.0f));

		m_entities.push_back(a = new Entity());
		a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("cube.obj"), Resources::GetInstance()->GetShader("ADS"), Resources::GetInstance()->GetTexture("Wood.jpg"), 1));
		a->GetComponent<MeshRenderer>()->SetSpecular(0.7f);
		a->GetTransform()->SetPosition(glm::vec3(15, -35, -18.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));
		a->GetComponent<RigidBody>()->Get()->setMassProps(0.5f, btVector3(0.1f, 1.0f, 1.0f));


		m_entities.push_back(a = new Entity());
		a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("sphere.obj"), Resources::GetInstance()->GetShader("ADS"), Resources::GetInstance()->GetTexture("WoodGrain.png"), 1));
		a->GetComponent<MeshRenderer>()->SetSpecular(0.8f);
		a->GetTransform()->SetPosition(glm::vec3(22, 120, -18.f));
		a->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new SphereShape(1.f));
		a->GetComponent<RigidBody>()->Get()->setMassProps(1.0f, btVector3(3.0f, 1.0f, 1.0f));
		a->GetComponent<RigidBody>()->Get()->setRestitution(0.6f);
		a->GetComponent<RigidBody>()->ToggleGravity();

#pragma endregion
#pragma endregion

		return m_entities;
	}

}
