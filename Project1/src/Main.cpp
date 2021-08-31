// include glad before glfw as glfw requires some open gl files to be included and 
// glad.h include those files
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "shader.h"
#include "VertexObject.h"
#include "Camera.h"
#include "Texture.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include "Model.h"

// some global variables
std::shared_ptr<Camera> camera;
int w_width = 1280, w_height = 720;
bool cameraControls = false, isLoaded = false;
bool firstMouse = false;
float lastX = 400, lastY = 300;


// resize callback function
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	if(w && h)
	{
		w_width = w; w_height = h;
	}
	glViewport(0, 0, w_width, w_height);
	std::cout << w_width << " " << w_height << std::endl;
}	

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window, nullptr);
		glfwSetScrollCallback(window, nullptr);
		isLoaded = false;
		cameraControls = false;
	}
}

std::shared_ptr<Scene> createScene1();
std::shared_ptr<Scene> createScene2();

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = lastX - xpos;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	if(camera)
		camera->mouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if(camera)
		camera->zoom(yoffset);
}

GLFWwindow* initOpengl();

int main()
{

	GLFWwindow* window = initOpengl();
	if (window == nullptr)
		return -1;

	// ========================================setting up imgui===========================
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	const char* glsl_version = "#version 330";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// =====================================end of setting up imgui=======================

	std::shared_ptr<Scene> scenes[10];
	scenes[0] = createScene1();
	scenes[1] = createScene2();

	std::shared_ptr<Scene> current = scenes[0];

	double previousTime = glfwGetTime(), deltaTime = 0.0;
	double fp_previousTime = glfwGetTime(), fp_deltaTime = 0.0;
	unsigned int frameCount = 0;
	bool windowActive = true;
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	//int frameCount = 0;
	// creating a render loop
	while (!glfwWindowShouldClose(window))
	{
		// checks if any events are triggered, updates the window state, and calls the corresponding
		// functions (like any callback functions)
		glfwPollEvents();


		static int item_current_idx = 0; // Here we store our selection data as an index.
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (windowActive)
		{
			ImGui::Begin("Load Camera", &windowActive);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Close")) {
						glfwSetWindowShouldClose(window, true);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			if (ImGui::Button("Load"))
				cameraControls = true;

			const char* items[] = { "BoxBox", "SolarSystem", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
			if (ImGui::BeginListBox("listbox 1"))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}
			ImGui::End();
		}

		if(show_demo_window){
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!", &show_demo_window);                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			//ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}



		// camera stuff
		if (cameraControls && !isLoaded) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(window, mouse_callback);
			glfwSetScrollCallback(window, scroll_callback);
			isLoaded = true;
		}

		double currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		double fp_currentTime = glfwGetTime();
		fp_deltaTime = fp_currentTime - fp_previousTime;
		frameCount++;
		// code for fps
		if (fp_deltaTime >= 1.0)
		{
			std::cout << frameCount << std::endl;
			frameCount = 0;
			fp_previousTime = fp_currentTime;
		}

		// input commands
		if(camera)
			camera->movement(window, deltaTime);
		processInput(window);

		// rendering commands
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT); // clearing the color buffer (state-using function)

		current = scenes[item_current_idx];
		camera = current->draw(window, w_width, w_height);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		std::vector<VertexObject>& objects = current->getObjects();
		std::vector<Light>& lights = current->getLights();

		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i].name == "Earth")
			{
				//std::cout << "lesgo" << std::endl;
				objects[i].rotate(0.2f, glm::vec3(0.0f, 0.0f, -1.0f));
				float radius = 14.0f;
				float earthX = radius * sin(glfwGetTime() / 4);
				float earthZ = radius * cos(glfwGetTime() / 4);
				objects[i].updatePosition(glm::vec3(earthX, 0.0f, earthZ));
			}
			else if (objects[i].name == "Mercury")
			{
				objects[i].rotate(0.2f, glm::vec3(0.0f, 0.0f, -1.0f));
				float radius = 6.0f;
				float mercsX = radius * sin(glfwGetTime() / 3.5);
				float mercsZ = radius * cos(glfwGetTime() / 3.5);
				objects[i].updatePosition(glm::vec3(mercsX, 0.0f, mercsZ));
			}
			else if (objects[i].name == "Venus")
			{
				objects[i].rotate(0.2f, glm::vec3(0.0f, 0.0f, -1.0f));
				float radius = 9.0f;
				float venusX = radius * sin(glfwGetTime() / 2.5);
				float venusZ = radius * cos(glfwGetTime() / 2.5);
				objects[i].updatePosition(glm::vec3(venusX, 0.0f, venusZ));
			}
			else if (objects[i].name == "Mars")
			{
				objects[i].rotate(0.2f, glm::vec3(0.0f, 0.0f, -1.0f));
				float radius = 15.0f;
				float marsX = radius * sin(glfwGetTime() / 3.0);
				float marsZ = radius * cos(glfwGetTime() / 3.0);
				objects[i].updatePosition(glm::vec3(marsX, 0.0f, marsZ));
			}
			else if (objects[i].name == "Jupiter")
			{
				objects[i].rotate(0.2f, glm::vec3(0.0f, 0.0f, -1.0f));
				float radius = 20.0f;
				float jupiterX = radius * sin(glfwGetTime() / 1.6);
				float jupiterZ = radius * cos(glfwGetTime() / 1.6);
				objects[i].updatePosition(glm::vec3(jupiterX, 0.0f, jupiterZ));
			}
		}

		for (int i = 0; i < lights.size(); i++)
		{
			if (lights[i].name == "Sun")
			{
				//std::cout << "lesgo" << std::endl;
				lights[i].rotate(0.05f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
		}
		
		
		// swaps the buffers (two buffers front and back, back is updated with the changes)
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	glfwTerminate();
	return 0;
}

GLFWwindow* initOpengl()
{
	// glfw and glad init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creating a window
	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Project1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(1);

	// initializing the glad library and passing it the function to get the address of
	// all the functions in the openGL with respect to each operating system.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 0;
	}

	// telling the openGL the size of the rendering window
	glViewport(0, 0, w_width, w_height);
	// enabling the z-buffer
	glEnable(GL_DEPTH_TEST);
	return window;
}

std::shared_ptr<Scene> createScene1()
{
	auto scene = std::make_shared<Scene>("scenes/scene1");
	unsigned int objectShader = scene->addShader("vertexshader.shader", "fragmentshader.shader");
	unsigned int lightingShader = scene->addShader("vertexshader.shader", "lighting_fs2.shader");
	scene->addObject("cube.txt", "Cube1");
	scene->addObject("cube.txt", "Cube2");
	scene->addObject("cube.txt", "Cube3");
	scene->addObject("ground.txt", "ground");
	std::vector<VertexObject>& objects = scene->getObjects();
	scene->addLight("cube.txt", 0, glm::vec3(3.0f, 2.0f, 4.0), "Light1");
	scene->addLight("cube.txt", 0, glm::vec3(-3.0f, 2.0f, -4.0), "Light2");
	scene->updateObjectPosition("object", 0, glm::vec3(0.0f, 0.5001f, 0.0f));
	scene->updateObjectPosition("object", 1, glm::vec3(0.0f, 1.50011f, 0.0f));
	scene->updateObjectPosition("object", 2, glm::vec3(1.0f, 0.5001f, 0.0f));
	scene->addTexture(0, "diffuse.png", "specular.png");
	scene->addTexture(1, "diffuse.png", "specular.png");
	scene->addTexture(2, "diffuse.png", "specular.png");
	scene->addTexture(3, "concrete.jpg", "concrete.jpg");
	return scene;
}

std::shared_ptr<Scene> createScene2()
{
	auto scene = std::make_shared<Scene>("scenes/scene2");
	scene->addShader("vertexshader.shader", "fragmentshader.shader");
	scene->addShader("vertexshader.shader", "lighting_fs2.shader");
	scene->addShader("vertexshader.shader", "sun_fs.shader");
	scene->addLight("sphere.txt", 0, glm::vec3(0, 0, 0), "Sun");
	scene->addObject("sphere.txt", "Mercury");
	scene->addObject("sphere.txt", "Venus");
	scene->addObject("sphere.txt", "Earth");
	scene->addObject("sphere.txt", "Mars");
	scene->addObject("sphere.txt", "Jupiter");

	scene->addLightTexture(0, "8k_sun.jpg", "8k_sun.jpg");
	scene->addTexture(0, "2k_mercury.jpg", "2k_mercury.jpg");
	scene->addTexture(1, "2k_venus_atmosphere.jpg", "2k_venus_atmosphere.jpg");
	scene->addTexture(2, "2k_earth_daymap.jpg", "2k_earth_daymap.jpg", false, false);
	scene->addTexture(3, "2k_mars.jpg", "2k_mars.jpg");
	scene->addTexture(4, "2k_jupiter.jpg", "2k_jupiter.jpg");
	std::vector<VertexObject>& objects = scene->getObjects();
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].rotate(90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
		if (objects[i].name == "Mercury")
		{
			objects[i].updatePosition(glm::vec3(6.0f, 0.0f, 0.0f));
			objects[i].scaleObject(0.05f);
		}
		if (objects[i].name == "Venus")
		{
			objects[i].updatePosition(glm::vec3(9.0f, 0.0f, 0.0f));
			objects[i].scaleObject(0.1f);
		}
		if (objects[i].name == "Earth")
		{
			objects[i].updatePosition(glm::vec3(12.0f, 0.0f, 0.0f));
			objects[i].scaleObject(0.17f);
		}
		if (objects[i].name == "Mars")
		{
			objects[i].updatePosition(glm::vec3(15.0f, 0.0f, 0.0f));
			objects[i].scaleObject(0.15f);
		}
		if (objects[i].name == "Jupiter")
		{
			objects[i].updatePosition(glm::vec3(20.0f, 0.0f, 0.0f));
			objects[i].scaleObject(0.3f);
		}
	}
	std::vector<Light>& lights = scene->getLights();
	for (int i = 0; i < lights.size(); i++)
	{
		lights[i].rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		lights[i].setLightProperties(glm::vec3(1.0f, 1.0f, 1.0f));
	}
	scene->setShader("light", 0, 2);
	return scene;
}