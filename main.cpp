#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include "Shader_m.h"
#include "Camera.h"
#include "Animator.h"
#include "Model_animation.h"

#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void set_light(Shader shade, glm::vec3 color);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 25.0f, 60.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// controll filming animation
bool performanse = false;
bool rash = false;
bool reset_camera = false;

//dvigats
struct Dvigats
{
	bool forward;
	bool backward;
	bool left;
	bool right;
	bool rot_clock;
	bool rot_against;
};
Dvigats dvigats;
glm::vec3 init_pose = glm::vec3(-10.0f, -0.2f, -60.0f);
float grad = 0.0f;



// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//glm::vec3 lightPos(1.2f, 10.0f, -80.0f);


int main()
{
	dvigats.forward = false;
	dvigats.backward = false;
	dvigats.left = false;
	dvigats.right = false;
	dvigats.rot_clock = false;
	dvigats.rot_against = false;


	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DISCOTEKA VEKA", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader screamShader("anim_model.vs", "anim_model.fs");
	Shader prayingShader("anim_model.vs", "anim_model.fs");
	Shader swatShader("anim_model.vs", "anim_model.fs");

	// load models of Scream Zombie
	// -----------
	Model screamModel("models/Zombie Scream/Zombie Scream.dae");
	Animation screamAnimation("models/Zombie Scream/Zombie Scream.dae", &screamModel);
	Animator screamAnimator(&screamAnimation);

	//Praying zombi
	Model prayingModel("models/Praying/Praying.dae");
	Animation prayingAnimation("models/Praying/Praying.dae", &prayingModel);
	Animator prayingAnimator(&prayingAnimation);

	//Swat walking
	Model swatModel("models/Zombie Idle/Zombie Idle.dae");
	Animation swatAnimation("models/Zombie Idle/Zombie Idle.dae", &swatModel);
	Animator swatAnimator(&swatAnimation);

	//GROUND !!!!!!!!!!!!!!!!!!!!
	Model carpet("models/stone-ground-07/carpet (1).dae");
	Shader carpShader("vs.vs", "fs.frag");

	// Определение начального времени
	double startTime = glfwGetTime();
	double backgroundTime = glfwGetTime();
	double interval = 1.0; // Интервал в секундах
	double back_interval = 2.0f;


	glm::vec3 randomColor = glm::vec3(1.0f); // до дискотеки свет нормальный
	glm::vec3 randomColor2 = glm::vec3(1.0f); // до дискотеки свет нормальный
	glm::vec3 randomColor3 = glm::vec3(1.0f); // до дискотеки свет нормальный


	glClearColor(0.059, 0, 0.522, 1.0);
	screamAnimator.UpdateAnimation(0.079); //scream
	prayingAnimator.UpdateAnimation(6.1); //praying
	int k = 0;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
		swatAnimator.UpdateAnimation(deltaTime); //SWAT

		if (performanse)
		{
			performanse = false; // получает следующую позицию
			screamAnimator.UpdateAnimation(deltaTime); //scream
			prayingAnimator.UpdateAnimation(deltaTime); //praying
		}

		// render
		// ------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// don't forget to enable shader before setting uniforms
		screamShader.use();
		prayingShader.use();
		swatShader.use();

		lightPos.x = 1.0f + sin(glfwGetTime()) * 60.0f;
		lightPos.y = cos(glfwGetTime() / 2.0f) * 12.0f;

		// SCREAM ZOMBI
		// --------------------------------------------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		screamShader.setMat4("projection", projection); 
		screamShader.setMat4("view", view); 

		auto transforms = screamAnimator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			screamShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		// render the loaded model
		// -----------------------------------------------------------------------------------
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-42.0f, -0.2f, -210.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));	// it's a bit too big for our scene, so scale it down
		screamShader.setMat4("model", model);

		set_light(screamShader, randomColor);
		screamModel.Draw(screamShader);



		//  PRAYING ZOMBI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// --------------------------------------------------------------------------------------
		prayingShader.setMat4("projection", projection);
		prayingShader.setMat4("view", view);

		auto transformsS = prayingAnimator.GetFinalBoneMatrices();
		for (int i = 0; i < transformsS.size(); ++i)
			prayingShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transformsS[i]);

		// render the loaded model
		// -----------------------------------------------------------------------------------
		glm::mat4 modelS = glm::mat4(1.0f);
		modelS = glm::translate(modelS, glm::vec3(30.0f, -0.2f, -210.0f)); // translate it down so it's at the center of the scene
		modelS = glm::scale(modelS, glm::vec3(0.22f, 0.22f, 0.22f));	// it's a bit too big for our scene, so scale it down
		prayingShader.setMat4("model", modelS);

		set_light(prayingShader, randomColor2);
		prayingModel.Draw(prayingShader);




		// !!!!!!!!!!!!!!!!!!!!!!!!!!! SWAT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// --------------------------------------------------------------------------------------
		swatShader.setMat4("projection", projection);
		swatShader.setMat4("view", view);

		auto transformsG = swatAnimator.GetFinalBoneMatrices();
		for (int i = 0; i < transformsG.size(); ++i)
			swatShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transformsG[i]);

		// render the loaded model
		// -----------------------------------------------------------------------------------
		glm::mat4 modelG = glm::mat4(1.0f);
		if (dvigats.forward)
		{
			init_pose.z -= 0.1;
		}
		else if (dvigats.backward)
		{
			init_pose.z += 0.1;
		}
		if (dvigats.left)
		{
			init_pose.x -= 0.1;
		}
		else if (dvigats.right)
		{
			init_pose.x += 0.1;
		}


		modelG = glm::translate(modelG, init_pose); // translate it down so it's at the center of the scene

		if (dvigats.rot_clock)
		{
			grad += 1.0f;
		}
		else if (dvigats.rot_against)
		{
			grad -= 1.0f;
		}
		modelG = glm::rotate(modelG, glm::radians(grad), glm::vec3(0.0f, 1.0f, 0.0f));

		modelG = glm::scale(modelG, glm::vec3(0.3f, 0.3f, 0.3f));
		swatShader.setMat4("model", modelG);

		set_light(swatShader, randomColor3);
		swatModel.Draw(swatShader);


		//GROUND MODEL!!!!!!!!!!!!!!!!!!!!!!!!!!!
		carpShader.use();
		carpShader.setMat4("projection", projection);
		carpShader.setMat4("view", view);
		glm::mat4 modelC = glm::mat4(1.0f);
		modelC = glm::translate(modelC, glm::vec3(0.0f, -0.2f, -210.0f)); // translate it down so it's at the center of the scene
		
		modelC = glm::rotate(modelC, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//modelC = glm::rotate(modelC, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelC = glm::scale(modelC, glm::vec3(250.0f, 250.0f, 1.0f));	
		carpShader.setMat4("model", modelC);
		carpet.Draw(carpShader);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------

	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		performanse = !performanse;
		Sleep(200);
	}
	//forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		dvigats.forward = true;
	}
	else
	{
		dvigats.forward = false;
	}
	//backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		dvigats.backward = true;
	}
	else
	{
		dvigats.backward = false;
	}
	//left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		dvigats.left = true;
	}
	else
	{
		dvigats.left = false;
	}
	//right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		dvigats.right = true;
	}
	else
	{
		dvigats.right = false;
	}
	//rotate clockwice
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		dvigats.rot_clock = true;
	}
	else
	{
		dvigats.rot_clock = false;
	}
	//rotate against clockwise
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		dvigats.rot_against = true;
	}
	else
	{
		dvigats.rot_against = false;
	}



	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void set_light(Shader shade, glm::vec3 color)
{
	shade.setVec3("lightColor", color);
	shade.setVec3("lightPos", lightPos);
	shade.setVec3("viewPos", camera.Position);
}
