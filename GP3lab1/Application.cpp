#include "Application.h"
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Physics.h"
#include "BoxShape.h"
#include "SphereShape.h"
#include "RigidBody.h"


Application* Application::m_application = nullptr;

Application::Application()
{

}

void Application::Init()
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError());
		exit(-1);
	}
	//setting openGL version to 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	//creating window
	m_window = SDL_CreateWindow("I was named by the application class", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight,
		SDL_WINDOW_OPENGL);

	SDL_CaptureMouse(SDL_TRUE);
	//SDL_ShowCursor(SDL_DISABLE);

	OpenGlInit();
	GameInit();

	//SDL_WarpMouseInWindow(NULL, m_windowWidth / 2, m_windowHeight / 2);
	baseMousePos = INPUT->GetMousePos();
	oldMousePos = glm::vec2(0, 0);



#pragma region making a plane I think?
	Entity * a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("cube.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("Wood.jpg")));
	MeshRenderer* m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -100.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.f,100.f)));
	a->GetTransform()->SetRotation(glm::vec3(0.f, 0.f, 0.f));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));
	for (int i = 0; i < 5; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("cube.obj"),Resources::GetInstance()->GetShader("ADS"),Resources::GetInstance()->GetTexture("Wood.jpg")));
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -18.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new SphereShape(1.f)); // new BoxShape(glm::vec3(1.f,1.f, 1.f))); // 
		a->GetComponent<RigidBody>()->Get()->setMassProps(1.f, btVector3());
		a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
		a->GetTransform()->SetRotation(glm::vec3(0.f, 0.f, 0.f));
	}

#pragma endregion

}

void Application::OpenGlInit()
{
	
	//creating context (our opengl statemachine in which all our GL calls
	//will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();
	SDL_GL_SetSwapInterval(1);
	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	

	//Smooth shading
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));
	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));
	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));
	//enabling blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	GL_ATTEMPT(glCullFace(GL_BACK));
	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);


}

void Application::Loop()
{
	m_appState = AppState::RUNNING;
	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;
	while (m_appState != AppState::QUITTING)
	{
		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					m_appState = AppState::QUITTING;
					break;
				case SDL_WINDOWEVENT_CLOSE:
					m_appState = AppState::QUITTING;
					break;
				case SDL_KEYDOWN:
					INPUT->SetKey(event.key.keysym.sym, true);
					break;

				case SDL_KEYUP:
					INPUT->SetKey(event.key.keysym.sym, false);
					break;

				case SDL_MOUSEMOTION:
					{	

						INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));

						//std::cout << event.motion.xrel << std::endl;

					}
					break;

				case SDL_MOUSEBUTTONDOWN: // https://stackoverflow.com/questions/35165716/sdl-mouse-click
				{
					if (INPUT->LeftMousePressed(event.button))
					{
						lookEnabled = true;
						//SDL_ShowCursor(SDL_DISABLE);
						//SDL_WarpMouseInWindow(NULL, m_windowWidth / 2, m_windowHeight / 2);
						oldMousePos = INPUT->GetMousePos();
					}
				}
				break;

				case SDL_MOUSEBUTTONUP: // https://stackoverflow.com/questions/35165716/sdl-mouse-click
				{
					if (INPUT->LeftMousePressed(event.button))
					{
						//SDL_ShowCursor(SDL_ENABLE);
						lookEnabled = false;
					}
				}
				break;
			}

			
		}


		#pragma region camera controls
		if (INPUT->GetKey(SDLK_w))
		{
			m_entities[EntityEnums::CAM]->GetTransform()->AddPosition(m_entities[EntityEnums::CAM]->GetTransform()->GetForward());
		}

		if (INPUT->GetKey(SDLK_a))
		{
			m_entities[EntityEnums::CAM]->GetTransform()->AddPosition(-m_entities[EntityEnums::CAM]->GetTransform()->GetRight());
		}

		if (INPUT->GetKey(SDLK_d))
		{
			m_entities[EntityEnums::CAM]->GetTransform()->AddPosition(m_entities[EntityEnums::CAM]->GetTransform()->GetRight());
		}

		if (INPUT->GetKey(SDLK_s))
		{
			m_entities[EntityEnums::CAM]->GetTransform()->AddPosition(-m_entities[EntityEnums::CAM]->GetTransform()->GetForward());
		}

		if (INPUT->GetKey(SDLK_SPACE))
		{
			m_entities[EntityEnums::CAM]->GetTransform()->AddPosition(m_entities[EntityEnums::CAM]->GetTransform()->GetUp());
		}

		if (INPUT->GetKey(SDLK_LCTRL))
		{
			m_entities[EntityEnums::CAM]->GetTransform()->AddPosition(-m_entities[EntityEnums::CAM]->GetTransform()->GetUp());
		}



		if (INPUT->GetKey(SDLK_e))
		{
			//baseMousePos = INPUT->GetMousePos();
			m_entities[EntityEnums::CAM]->GetTransform()->RotateEulerAxis(1, glm::vec3(0, 1, 0));
		}

		if (INPUT->GetKey(SDLK_q))
		{
			m_entities[EntityEnums::CAM]->GetTransform()->RotateEulerAxis(-1, glm::vec3(0, 1, 0));
		}

		

		
		
		//oldMousePos = deltaMousePos;

		if (lookEnabled)
		{
			deltaMousePos = oldMousePos - INPUT->GetMousePos(); //-oldMousePos;

			m_entities[EntityEnums::CAM]->GetTransform()->RotateEulerAxis(deltaMousePos.x * 0.2, glm::vec3(0,1,0));
			m_entities[EntityEnums::CAM]->GetTransform()->RotateEulerAxis(deltaMousePos.y * 0.2, m_entities[EntityEnums::CAM]->GetTransform()->GetRight());
			//SDL_WarpMouseInWindow(NULL, m_windowWidth / 2, m_windowHeight / 2);

			oldMousePos = INPUT->GetMousePos();
		}

		//m_entities[EntityEnums::CAM]->GetTransform()->RotateEulerAxis(oldMousePos.x * 0.01, glm::vec3(0,1,0));
		//m_entities[EntityEnums::CAM]->GetTransform()->RotateEulerAxis(oldMousePos.y * 0.01, m_entities[EntityEnums::CAM]->GetTransform()->GetRight());


		

		//--------------------------------------------sfdsfdsfdsfdsf
		if (INPUT->GetKey(SDLK_i))
		{
			m_entities[8]->GetComponent<RigidBody>()->AddForce(glm::vec3(0, 50, 0));
		}

		if (INPUT->GetKey(SDLK_j))
		{
			m_entities[8]->GetComponent<RigidBody>()->AddForce(glm::vec3(-20, 0, 0));
		}

		if (INPUT->GetKey(SDLK_l))
		{
			m_entities[8]->GetComponent<RigidBody>()->AddForce(glm::vec3(20, 0, 0));
		}

		if (INPUT->GetKey(SDLK_k))
		{
			m_entities[8]->GetComponent<RigidBody>()->AddForce(glm::vec3(10, 100, 0));
		}


#pragma endregion

		//SDL_WarpMouseInWindow(NULL, m_windowWidth/2 , m_windowHeight/2);

		//oldMousePos = INPUT->GetMousePos();

		//m_entities.at(0)->GetTransform()->RotateEulerAxis(1, glm::vec3(0, 1, 0));
		m_entities[DIR_LIGHT]->GetTransform()->RotateEulerAxis(1, glm::vec3(0, 1, 0));
		
		
		// input
		if (INPUT->GetKey(SDLK_z))
		{
			debugMode = DEBUG_ALL;
		}

		if (INPUT->GetKey(SDLK_x))
		{
			debugMode = DEBUG_ERRORS_ONLY;
		}

		if (INPUT->GetKey(SDLK_c))
		{
			debugMode = DEBUG_HIDE_ALL;
		}

		
		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;
		//TODO: print delta time using log
		//Log::Debug("deltaTime is " + std::to_string(deltaTime), debugMode);
		//std::cout << "cam rot is " << m_entities[EntityEnums::CAM]->GetTransform()->GetRotation().x << ", " << m_entities[EntityEnums::CAM]->GetTransform()->GetRotation().y << ", " << m_entities[EntityEnums::CAM]->GetTransform()->GetRotation().z << std::endl;

		Physics::GetInstance()->Update(deltaTime);

		lightDir = m_entities[DIR_LIGHT]->GetTransform()->GetRight();

		m_entities.at(3)->GetTransform()->LookAt(m_entities[EntityEnums::CAM]->GetTransform()->GetPosition()); // this is a thing that maybe should not be

		//glm::vec4 test4 = m_entities.at(3)->GetComponent<MeshRenderer>()->GetMVP() * glm::vec4(m_entities.at(3)->GetTransform()->GetPosition(), 1);
		glm::vec4 test4 = m_mainCamera->Get() * glm::vec4(m_entities.at(3)->GetTransform()->GetPosition(), 1);
		//glm::vec3 test4 = glm::project(m_entities.at(3)->GetTransform()->GetPosition(), m_entities.at(3)->GetTransform()->GetTransformationMatrix(), GetCamera()->Get(), glViewport);
		glm::vec2 test = glm::vec2(test4.x, test4.y);

		std::cout << test.x << "," << test.y << std::endl;

		Update(deltaTime);
		Render();

		SDL_GL_SwapWindow(m_window);

		if (INPUT->GetKey(SDLK_ESCAPE))
		{
			m_appState = AppState::QUITTING;
		}
	}
}

void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}

Application::~Application()
{
}

Application* Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}

void Application::GameInit()
{

	#pragma region load resources
	//loading all resources
	Resources::GetInstance()->AddModel("cube.obj");
	Resources::GetInstance()->AddModel("man.obj");
	Resources::GetInstance()->AddModel("xyzOBJ.obj");
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddTexture("testText3.png");
	Resources::GetInstance()->AddTexture("pixels.jpg");
	Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "simple_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"),"simple");
	Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "simple_VERT.glsl", ASSET_PATH + "inverted_FRAG.glsl"), "inverted");
	Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "normals_VERT.glsl", ASSET_PATH + "ADS_FRAG.glsl"), "ADS");
	Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "UI_VERT.glsl", ASSET_PATH + "UI_FRAG.glsl"), "UI");
	#pragma endregion

	#pragma region
	m_UIentities.push_back(new Entity());
	m_UIentities.at(m_UIentities.size() - 1)->AddComponent(new MeshRenderer(new Mesh(Quad::quadVertices, Quad::quadIndices), Resources::GetInstance()->GetShader("UI"), Resources::GetInstance()->GetTexture("testText3.png")));
	m_UIentities.at(m_UIentities.size() - 1)->GetTransform()->SetPosition(glm::vec3(101, 0, 10));

	m_UIentities.push_back(new Entity());
	m_UIentities.at(m_UIentities.size() - 1)->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("xyzOBJ.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("pixels.jpg")));
	m_UIentities.at(m_UIentities.size() - 1)->GetTransform()->SetPosition(glm::vec3(0, 0, -100));
	m_UIentities.at(m_UIentities.size() - 1)->GetTransform()->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

	//Students should aim to have a better way of managing the scene for coursework
	m_entities.push_back(new Entity());
	m_entities.at(m_entities.size() - 1)->AddComponent(new MeshRenderer(new Mesh(Quad::quadVertices, Quad::quadIndices), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("testText3.png")));
	m_entities.at(m_entities.size() - 1)->GetTransform()->SetPosition(glm::vec3(0, 0, -11));

	m_entities.push_back(new Entity()); // --------!---------THIS COULD BE IN WRONG PLACE --------!--------!-------
	CameraComp* cc = new CameraComp();
	m_entities.at(m_entities.size() - 1)->AddComponent(cc);
	m_entities.at(m_entities.size() - 1)->GetTransform()->SetPosition(glm::vec3(0, 0, 100));

	m_entities.push_back(new Entity()); // Light
	m_entities.at(m_entities.size() - 1)->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("man.obj"), Resources::GetInstance()->GetShader("simple"), Resources::GetInstance()->GetTexture("pixels.jpg")));
	m_entities.at(m_entities.size() - 1)->GetTransform()->SetPosition(glm::vec3(0, 0, 50));
	m_entities.at(m_entities.size() - 1)->GetTransform()->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));


	m_entities.push_back(new Entity());
	m_entities.at(m_entities.size() - 1)->GetTransform()->SetPosition(glm::vec3(0, 0, -100));
	m_entities.at(m_entities.size() - 1)->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("man.obj"), Resources::GetInstance()->GetShader("ADS"),Resources::GetInstance()->GetTexture("Wood.jpg")));

	//m_entities.at(2)->AddComponent(new Entity());

	deltaMousePos = INPUT->GetMousePos(); // may not be required
	oldMousePos = INPUT->GetMousePos();

	cc->Start();

}

void Application::Run()
{
	Init();
	Loop();
	Quit();
}

void Application::Update(float deltaTime)
{

	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}
void Application::Render()
{
	/* Clear context */
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));
	GL_ATTEMPT(glEnable(GL_CULL_FACE));

	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_mainCamera->Recalculate();

	for (auto& a : m_entities)
	{
		a->OnRender();
	}


	GL_ATTEMPT(glDisable(GL_DEPTH_TEST));
	GL_ATTEMPT(glDisable(GL_CULL_FACE));

	for (auto& a : m_UIentities)
	{
		a->OnRender();
	}
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}

