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
Camera *camera;
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

	//Shader shader("shaders/VertexShader.shader", "shaders/FragmentShader.shader");
	//Shader shader("scenes/scene1/shaders/vertexshader.shader", "scenes/scene1/shaders/fragmentshader.shader");
	//Shader lightShader("scenes/scene1/shaders/vertexshader.shader", "scenes/scene1/shaders/lighting_fs2.shader");
	
	Scene scene;
	unsigned int objectShader = scene.addObjectShader("scenes/scene1/shaders/vertexshader.shader", "scenes/scene1/shaders/fragmentshader.shader");
	unsigned int lightingShader = scene.addLightShader("scenes/scene1/shaders/vertexshader.shader", "scenes/scene1/shaders/lighting_fs2.shader");
	scene.addObject("scenes/scene1/VertexData/cube_normals.txt");
	scene.addLight("scenes/scene1/VertexData/cube.txt", 0, glm::vec3(3.0f, 2.0f, 4.0));
	scene.addLight("scenes/scene1/VertexData/cube.txt", 0, glm::vec3(-3.0f, 2.0f, -4.0));
	scene.setObjectModel(0, 0);
	scene.setObjectModel(0, 0);
	scene.updateObjectModel(0, 1, glm::vec3(0.0f, 5.0f, 0.0f));
	scene.updateObjectModel(0, 1, 0.2f);
	scene.addTexture(0, "scenes/scene1/textures/diffuse.png", "scenes/scene1/textures/specular.png");
	//VertexObject object("scenes/scene1/VertexData/cube_normals.txt");
	//object.unbind();
	//VertexObject light();
	//light.unbind();

	//Texture diffuse("scenes/scene1/textures/diffuse.png", true);
	//Texture specular("scenes/scene1/textures/specular.png", true);
	//diffuse.activeTexture(0);
	//specular.activeTexture(1);

	//Shader shader2("shaders/lighting_vs.shader", "shaders/lighting_fs2.shader");

	// getting the texture image from the file system
	// this function loads the image of any image type and converts it to char data
	
	// similar to VAO, VBO, EBO and shaders, the textures also work as objects and require
	// a ID to reference the object



	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	//shader.use();
	//// material
	//shader.setInt("material.diffuse", 0);
	//shader.setInt("material.specular", 1);
	//shader.setFloat("material.shininess", 64.0f);

	//// light distance components
	//shader.setFloat("light.constant", 1.0f);
	//shader.setFloat("light.linear", 0.09f);
	//shader.setFloat("light.quadratic", 0.032f);

	//// light properties
	//shader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	//shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f); // darken diffuse light a bit
	//shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	//lightShader.use();
	//lightShader.setVec3("lightColor", lightColor);

	double previousTime = glfwGetTime(), deltaTime = 0.0;
	bool windowActive = true;
	//int frameCount = 0;
	// creating a render loop
	while (!glfwWindowShouldClose(window))
	{
		// checks if any events are triggered, updates the window state, and calls the corresponding
		// functions (like any callback functions)
		glfwPollEvents();


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (windowActive)
		{
			ImGui::Begin("Load Camera", &windowActive, ImGuiWindowFlags_MenuBar);

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
			ImGui::End();
		}
		ImGui::Render();

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
		//frameCount++;
		// code for fps
		/*if (deltaTime >= 1.0)
		{
			std::cout << frameCount << std::endl;
			frameCount = 0;
			previousTime = currentTime;
		}*/

		// input commands
		if(camera)
			camera->movement(window, deltaTime);
		processInput(window);

		// rendering commands
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT); // clearing the color buffer (state-using function)



		// translating and rotating the rectangle
		//glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		//unsigned int transformloc = glGetUniformLocation(shader.getShaderID(), "transform");
		//glUniformMatrix4fv(transformloc, 1, GL_FALSE, glm::value_ptr(trans));

		glm::vec3 lightPos(3.0f, 2.0f, 4.0f);

		camera = scene.draw(window, w_width, w_height);

		//shader.use();
		//shader.setVec3("viewPos", camera->getPosition());
		//shader.setVec3("lightPos", lightPos);



		//glm::mat4 view = camera->getView();
		//int viewLoc = glGetUniformLocation(shader.getShaderID(), "view");
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// creating the projection matrix
		//glm::mat4 projection = glm::mat4(1.0f);
		//projection = glm::perspective(glm::radians(camera->getZoom()), (float)w_width / (float)w_height, 0.1f, 100.0f);

		//int projectionLoc = glGetUniformLocation(shader.getShaderID(), "projection");
		//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.01f));

		//int modelLoc = glGetUniformLocation(shader.getShaderID(), "model");
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//object.drawCall();


		// The draw call
		//lightShader.use();
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f));

		//modelLoc = glGetUniformLocation(lightShader.getShaderID(), "model");
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//viewLoc = glGetUniformLocation(lightShader.getShaderID(), "view");
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		//projectionLoc = glGetUniformLocation(lightShader.getShaderID(), "projection");
		//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//light.bind();
		//light.drawCall();


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
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