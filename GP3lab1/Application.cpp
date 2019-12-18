#include "Application.h"
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Physics.h"
#include "BoxShape.h"
#include "SphereShape.h"
#include "CapsuleShape.h"
#include "CylinderShape.h"
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//creating window
	m_window = SDL_CreateWindow("I was named by the application class", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);

	SDL_CaptureMouse(SDL_TRUE);
	//SDL_ShowCursor(SDL_DISABLE);

	OpenGlInit();
	GameInit();

	SDL_WarpMouseInWindow(m_window, m_windowWidth / 2, m_windowHeight / 2);
	INPUT->ResetMouse();
	baseMousePos = INPUT->GetMousePos();
	oldMousePos = glm::vec2(0, 0);

	m_mainCamera->Recalculate();

	m_uiVP = GetCamera()->Get();

	m_UIentities[0]->GetTransform()->SetPosition(m_camEntity->GetTransform()->GetPosition() + (m_camEntity->GetTransform()->GetForward() * glm::vec3(4)));
	m_UIentities[0]->GetTransform()->SetRotation(m_camEntity->GetTransform()->GetRotation());

	//m_sceneLoader = new SceneLoader;

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
	//GL_ATTEMPT(glShadeModel(GL_SMOOTH));
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
		/*
		if (mouseSet)
		{
			switch (mouseInt)
			{
			case(0):
			{
				//SDL_WarpMouseInWindow(m_window, m_windowWidth / 2, m_windowHeight / 2);
				INPUT->MoveMouse(glm::vec2(-50, 0));
			}
			break;

			case(1):
			{
				INPUT->MoveMouse(glm::vec2(50, 0));
			}
			break;

			case(2):
			{
				INPUT->MoveMouse(glm::vec2(0, 50));
			}
			break;

			case(3):
			{
				INPUT->MoveMouse(glm::vec2(0, -50));
			}
			break;
			}

			mouseSet = false;
		}
		*/

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

						//deltaMousePos = INPUT->GetMousePos + baseMousePos;//INPUT->set

						/*
						if (INPUT->GetMousePos().x < mouseConstraints.z)
						{
							//INPUT->MoveMouse(glm::vec2(50,0));
							//SDL_WarpMouseInWindow(m_window, m_windowWidth / 2, (m_windowHeight / 2) + mouseConstraints.y - 50 + INPUT->GetMousePos().y); // 2);
							SDL_WarpMouseInWindow(m_window, m_windowWidth / 2, (m_windowHeight / 2) + INPUT->GetMousePos().y - (mouseConstraints.y - 50));
							
							mouseConstraints.z -= 50;
							mouseConstraints.x -= 50;
							mouseSet = true; // INPUT->MoveMouse(glm::vec2(-50, 0));
							mouseInt = 0;
						}

						if (INPUT->GetMousePos().x > mouseConstraints.x)
						{
							//INPUT->MoveMouse(glm::vec2(50,0));
							SDL_WarpMouseInWindow(m_window, m_windowWidth / 2, (m_windowHeight / 2) + mouseConstraints.y - 50 + INPUT->GetMousePos().y);
							mouseConstraints.z += 50;
							mouseConstraints.x += 50;
							mouseSet = true; // INPUT->MoveMouse(glm::vec2(50, 0));
							mouseInt = 1;
						}

						if (INPUT->GetMousePos().y > mouseConstraints.y)
						{
							//INPUT->MoveMouse(glm::vec2(50,0));
							SDL_WarpMouseInWindow(m_window, (m_windowWidth / 2) + mouseConstraints.x - 50 + INPUT->GetMousePos().x, m_windowHeight / 2);
							mouseConstraints.y += 50;
							mouseConstraints.w += 50;
							mouseSet = true; //INPUT->MoveMouse(glm::vec2(0, 50));
							mouseInt = 2;
						}

						if (INPUT->GetMousePos().y < mouseConstraints.w)
						{
							//INPUT->MoveMouse(glm::vec2(50,0));
							SDL_WarpMouseInWindow(m_window, (m_windowWidth / 2) + mouseConstraints.x - 50 + INPUT->GetMousePos().x, m_windowHeight / 2);
							mouseConstraints.y -= 50;
							mouseConstraints.w -= 50;
							mouseSet = true; // INPUT->MoveMouse(glm::vec2(0, -50));
							mouseInt = 3;
						}


						//std::cout << event.motion.xrel << std::endl;
						*/

					}
					break;

				case SDL_MOUSEBUTTONDOWN: // https://stackoverflow.com/questions/35165716/sdl-mouse-click
				{
					if (INPUT->RightMousePressed(event.button))
					{
						lookEnabled = true;
						SDL_ShowCursor(SDL_DISABLE);
						//SDL_WarpMouseInWindow(NULL, m_windowWidth / 2, m_windowHeight / 2);
						oldMousePos = INPUT->GetMousePos();

						m_UIentities[0]->GetComponent<MeshRenderer>()->GiveTexture(Resources::GetInstance()->GetTexture("selectionText1.png"));
					}

					if (INPUT->LeftMousePressed(event.button))
					{
						if (m_targetEntity != NULL & lookEnabled)
						{
							m_currEntity = m_targetEntity;
						}
						else
						{
							isPossessing = false;
							m_currEntity = NULL;
							m_UIentities[0]->GetComponent<MeshRenderer>()->GiveTexture(Resources::GetInstance()->GetTexture("mainText1.png"));
							m_transformSelection = TransformStates::NONE;
							selectionCol = glm::vec4(0.9f, 0.9f, 0.9f, 0.6f);

						}
					}
					
				}
				break;

				case SDL_MOUSEBUTTONUP: // https://stackoverflow.com/questions/35165716/sdl-mouse-click
				{
					if (INPUT->RightMousePressed(event.button))
					{
						SDL_ShowCursor(SDL_ENABLE);
						lookEnabled = false;

						if (m_currEntity == NULL)
						{
							m_UIentities[0]->GetComponent<MeshRenderer>()->GiveTexture(Resources::GetInstance()->GetTexture("mainText1.png"));
						}
						else
							m_UIentities[0]->GetComponent<MeshRenderer>()->GiveTexture(Resources::GetInstance()->GetTexture("transformText1.png"));
					}
				}
				break;

			}
		}


		//SDL_WarpMouseInWindow(NULL, m_windowWidth/2 , m_windowHeight/2);

		//oldMousePos = INPUT->GetMousePos();

		//m_entities.at(0)->GetTransform()->RotateEulerAxis(1, glm::vec3(0, 1, 0));
		//m_entities[DIR_LIGHT]->GetTransform()->RotateEulerAxis(1, glm::vec3(0, 1, 0));
		
		
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


		#pragma region physics object user interactions
		if (m_currEntity != NULL && m_currEntity->GetComponent<RigidBody>() != nullptr)
		{
			if (INPUT->GetKey(SDLK_g))
			{
				m_currEntity->GetComponent<RigidBody>()->ToggleGravity();
			}

			if (INPUT->GetKey(SDLK_p))
			{
				if (isPossessing)
				{
					isPossessing = false;
				}
				else
				{
					isPossessing = true;
				}
				
			}
		}

		if (isPossessing)
		{

			if (!lookEnabled)
			m_camEntity->GetTransform()->SetRotation(m_posessionTransform.GetRotation());

			m_camEntity->GetTransform()->SetPosition(m_currEntity->GetTransform()->GetPosition() + (glm::normalize(m_posessionTransform.GetUp()) * glm::vec3(3)) - (glm::normalize(m_posessionTransform.GetForward()) * glm::vec3(15)));

			if (INPUT->GetKey(SDLK_w))
			{
				m_currEntity->GetComponent<RigidBody>()->ApplyDamping(0.01);
				m_currEntity->GetComponent<RigidBody>()->AddForce(glm::normalize(m_camEntity->GetTransform()->GetForward())* glm::vec3(5));
			}

			if (INPUT->GetKey(SDLK_a))
			{
				m_posessionTransform.RotateEulerAxis(-1, m_posessionTransform.GetUp());
			}

			if (INPUT->GetKey(SDLK_d))
			{
				m_posessionTransform.RotateEulerAxis(1, m_posessionTransform.GetUp());
			}

			if (INPUT->GetKey(SDLK_s))
			{
				m_currEntity->GetComponent<RigidBody>()->ApplyDamping(0.1); 
			}

			if (INPUT->GetKey(SDLK_SPACE))
			{
				m_currEntity->GetComponent<RigidBody>()->AddForce(glm::vec3(0,100,0));
			}
		}
#pragma endregion
		


		
		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;
		
		Physics::GetInstance()->Update(deltaTime);

		lightDir = m_entities[DIR_LIGHT]->GetTransform()->GetForward();


		
		if (lookEnabled)
		{

			deltaMousePos = oldMousePos - INPUT->GetMousePos();

			std::cout << "delta: " << deltaMousePos.x << " old: " << oldMousePos.x << " input: " << INPUT->GetMousePos().x << std::endl << std::endl;

			m_camEntity->GetTransform()->RotateEulerAxis(deltaMousePos.x * -0.2, glm::vec3(0, 1, 0));
			m_camEntity->GetTransform()->RotateEulerAxis(deltaMousePos.y * -0.2, m_camEntity->GetTransform()->GetRight());

			oldMousePos = INPUT->GetMousePos();

			#pragma region object selection
			m_targetEntity = NULL;
			float closest = 0;

			for (auto& a : m_entities)
			{

				glm::vec3 Cfwd = glm::normalize(m_camEntity->GetTransform()->GetForward());;

				glm::vec3 Opos = a->GetTransform()->GetPosition();  //a->GetTransform()->GetPosition(); // calculate distance between object and camera
				glm::vec3 Cpos = m_camEntity->GetTransform()->GetPosition();

				glm::vec3 Ovec = glm::normalize(Cpos - Opos);

				float val = acos(glm::dot(Cfwd, Ovec)); // https://community.khronos.org/t/angle-between-two-vectors/42042

				if (val > 2.9)
				{

					if (val > closest) // if angle is closest yet
					{
						closest = val;

						m_UIentities[1]->GetTransform()->SetPosition(a->GetTransform()->GetPosition());

						m_targetEntity = a;
					}

				}

				if (closest == 0)
					m_UIentities[1]->GetTransform()->SetPosition(m_camEntity->GetTransform()->GetPosition() - m_camEntity->GetTransform()->GetForward());

			}
			#pragma endregion
		}
		else if (m_currEntity != NULL) // if not looking, but something is selected
		{
			m_UIentities[1]->GetTransform()->SetPosition(m_currEntity->GetTransform()->GetPosition()); // place selection object

			#pragma region transform controls

			if (INPUT->GetKey(SDLK_z))
			{
				m_transformSelection = TransformStates::POS;
				selectionCol = glm::vec4(0.4f, 0.4f, 0.8f, 0.6f);
			}
			else if (INPUT->GetKey(SDLK_x))
			{
				m_transformSelection = TransformStates::ROT;
				selectionCol = glm::vec4(0.4f, 0.8f, 0.4f, 0.6f);
			}
			else if (INPUT->GetKey(SDLK_c))
			{
				m_transformSelection = TransformStates::SCALE;
				selectionCol = glm::vec4(0.8f, 0.4f, 0.4f, 0.6f);
			}

			#pragma region JL
			if (INPUT->GetKey(SDLK_j))
			{
				switch (m_transformSelection)
				{
					case (TransformStates::POS):
					{
						m_currEntity->GetTransform()->AddPosition(glm::vec3(-1, 0, 0));
					}
					break;

					case (TransformStates::ROT):
					{
						m_currEntity->GetTransform()->RotateEulerAxis(1, m_currEntity->GetTransform()->GetUp());
					}
					break;

					case (TransformStates::SCALE):
					{
						m_currEntity->GetTransform()->addScale(glm::vec3(-0.04f, 0, 0));
					}
					break;
				}
			}

			if (INPUT->GetKey(SDLK_l))
			{
				switch (m_transformSelection)
				{
				case (TransformStates::POS):
				{
					m_currEntity->GetTransform()->AddPosition(glm::vec3(1, 0, 0));
				}
				break;

				case (TransformStates::ROT):
				{
					m_currEntity->GetTransform()->RotateEulerAxis(-1, m_currEntity->GetTransform()->GetUp());
				}
				break;

				case (TransformStates::SCALE):
				{
					m_currEntity->GetTransform()->addScale(glm::vec3(0.04f, 0, 0));
				}
				break;
				}
			}
			#pragma endregion

			#pragma region IK
			if (INPUT->GetKey(SDLK_i))
			{
				switch (m_transformSelection)
				{
				case (TransformStates::POS):
				{
					m_currEntity->GetTransform()->AddPosition(glm::vec3(0, 0, -1));
				}
				break;

				case (TransformStates::ROT):
				{
					m_currEntity->GetTransform()->RotateEulerAxis(1, m_currEntity->GetTransform()->GetRight());
				}
				break;

				case (TransformStates::SCALE):
				{
					m_currEntity->GetTransform()->addScale(glm::vec3(0, 0, 0.04f));
				}
				break;
				}
			}

			if (INPUT->GetKey(SDLK_k))
			{
				switch (m_transformSelection)
				{
				case (TransformStates::POS):
				{
					m_currEntity->GetTransform()->AddPosition(glm::vec3(0, 0, 1));
				}
				break;

				case (TransformStates::ROT):
				{
					m_currEntity->GetTransform()->RotateEulerAxis(-1, m_currEntity->GetTransform()->GetRight());
				}
				break;

				case (TransformStates::SCALE):
				{
					m_currEntity->GetTransform()->addScale(glm::vec3(0, 0, -0.04f));
				}
				break;
				}
			}
			#pragma endregion

			#pragma region UO
			if (INPUT->GetKey(SDLK_u))
			{
				switch (m_transformSelection)
				{
				case (TransformStates::POS):
				{
					m_currEntity->GetTransform()->AddPosition(glm::vec3(0, -1, 0));
				}
				break;

				case (TransformStates::ROT):
				{
					m_currEntity->GetTransform()->RotateEulerAxis(1, m_currEntity->GetTransform()->GetForward());
				}
				break;

				case (TransformStates::SCALE):
				{
					m_currEntity->GetTransform()->addScale(glm::vec3(0, -0.04f, 0));
				}
				break;
				}
			}

			if (INPUT->GetKey(SDLK_o))
			{
				switch (m_transformSelection)
				{
				case (TransformStates::POS):
				{
					m_currEntity->GetTransform()->AddPosition(glm::vec3(0, 1, 0));
				}
				break;

				case (TransformStates::ROT):
				{
					m_currEntity->GetTransform()->RotateEulerAxis(-1, m_currEntity->GetTransform()->GetForward());
				}
				break;

				case (TransformStates::SCALE):
				{
					m_currEntity->GetTransform()->addScale(glm::vec3(0, 0.04f, 0));
				}
				break;
				}
			}
			#pragma endregion

			#pragma endregion

		}
		else // if not looking, and nothing is selected, hide the select object above the camera
		{
			m_UIentities[1]->GetTransform()->SetPosition(m_camEntity->GetTransform()->GetPosition() + m_camEntity->GetTransform()->GetUp());
		}
		

		#pragma region selection control
		float selectionScale = ((m_camEntity->GetTransform()->GetPosition().x - m_UIentities[1]->GetTransform()->GetPosition().x) * (m_camEntity->GetTransform()->GetPosition().x - m_UIentities[1]->GetTransform()->GetPosition().x) +
			(m_camEntity->GetTransform()->GetPosition().y - m_UIentities[1]->GetTransform()->GetPosition().y) * (m_camEntity->GetTransform()->GetPosition().y - m_UIentities[1]->GetTransform()->GetPosition().y) +
			(m_camEntity->GetTransform()->GetPosition().z - m_UIentities[1]->GetTransform()->GetPosition().z) * (m_camEntity->GetTransform()->GetPosition().z - m_UIentities[1]->GetTransform()->GetPosition().z));

		selectionScale = sqrt(selectionScale) / 300;

		if (m_currEntity == m_targetEntity || m_targetEntity == NULL)
		{
			selectionScale *= 1 + (0.2f * sinf(scaleBounce));
			scaleBounce += 0.04f;
		}

		m_UIentities[1]->GetTransform()->SetScale(glm::vec3(selectionScale, selectionScale, selectionScale));
#pragma endregion

		#pragma region point light

		if (m_entities[0]->GetTransform()->GetPosition() != m_pointLightPos) // if the point light has moved
		{

			m_pointLightPos = m_entities[0]->GetTransform()->GetPosition();

			for (auto& a : m_entities)
			{
				if (a->GetComponent<MeshRenderer>()->IsLit())
				{
					// get distance
					glm::vec3 mPos = a->GetTransform()->GetPosition();
					float dist = ((mPos.x - m_pointLightPos.x) * (mPos.x - m_pointLightPos.x)) + ((mPos.y - m_pointLightPos.y) * (mPos.y - m_pointLightPos.y)) + ((mPos.z - m_pointLightPos.z) * (mPos.z - m_pointLightPos.z));

					if (dist < 10000)
					{
						a->GetComponent<MeshRenderer>()->SetPLVal(-glm::normalize(mPos - m_pointLightPos), (10000 - dist) / 10000);
					}
					else
					{
						a->GetComponent<MeshRenderer>()->SetPLVal(glm::vec3(0, 0, 0), 0);
					}


				}
			}
		}
#pragma endregion


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
	//delete[] &m_entities;
	for (auto& a : m_entities)
	{
		a->DeleteComponents();
	}
	m_entities.clear();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	//Resources::GetInstance
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

	m_camEntity = new Entity();
	CameraComp* cc = new CameraComp();
	m_camEntity->AddComponent(cc);
	m_camEntity->GetTransform()->SetPosition(glm::vec3(0, 0, 100));

	m_camEntity->OnUpdate(m_worldDeltaTime);

	//m_uiVP = m_mainCamera->Get();

	m_entities = m_sceneLoader.LoadScene(1);

	deltaMousePos = INPUT->GetMousePos(); // may not be required
	oldMousePos = INPUT->GetMousePos();

	m_UIentities.push_back(new Entity());
	m_UIentities.at(m_UIentities.size() - 1)->AddComponent(new MeshRenderer(new Mesh(Quad::quadVertices, Quad::quadIndices), Resources::GetInstance()->GetShader("UI"), Resources::GetInstance()->GetTexture("mainText1.png"), 2));
	m_UIentities.at(m_UIentities.size() - 1)->GetTransform()->SetPosition(glm::vec3(0, 0, 1));
	m_UIentities.at(m_UIentities.size() - 1)->GetTransform()->SetScale(glm::vec3(3.6f, 2.0f, 2.0f));

	m_UIentities.push_back(new Entity());
	m_UIentities.at(m_UIentities.size() - 1)->AddComponent(new MeshRenderer(Resources::GetInstance()->GetModel("sphere.obj"), Resources::GetInstance()->GetShader("flatColour"), Resources::GetInstance()->GetTexture("xyzTex.jpg")));
	m_UIentities.at(m_UIentities.size() - 1)->GetTransform()->SetPosition(glm::vec3(0, 0, -100));
	m_UIentities.at(m_UIentities.size() - 1)->GetTransform()->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

}

void Application::Run()
{
	Init();
	Loop();
	Quit();
}

void Application::Update(float deltaTime)
{
	if (!isPossessing)
	{
		m_camEntity->OnUpdate(deltaTime);
	}

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

