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
void rash_camera();

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
bool dvigats = false;
glm::vec3 init_pose = glm::vec3(-10.0f, -0.2f, -60.0f);

//controll music
int which_song = 1;
bool play = false;
bool first_sound = true;
BOOL result;


// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//glm::vec3 lightPos(1.2f, 10.0f, -80.0f);


int main()
{
	vector<const char*> audioFiles = { "models/everybody.wav",  "models/satisfaction.wav", "models/PSY.wav" };


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
	Shader ourShader("anim_model.vs", "anim_model.fs");
	Shader secondShader("anim_model.vs", "anim_model.fs");
	Shader grannyShader("anim_model.vs", "anim_model.fs");

	// load models of GrandPa
	// -----------
	Model ourModel("models/Swing Dancing/Swing Dancing.dae");
	Animation danceAnimation("models/Swing Dancing/Swing Dancing.dae", &ourModel);
	Animator animator(&danceAnimation);

	//Shrek
	Model secondModel("models/Dancing/Dancing.dae");
	Animation secondAnimation("models/Dancing/Dancing.dae", &secondModel);
	Animator secondAnimator(&secondAnimation);

	//Granny
	Model grannyModel("models/Dancing Twerk/Dancing Twerk.dae");
	Animation grannyAnimation("models/Dancing Twerk/Dancing Twerk.dae", &grannyModel);
	Animator grannyAnimator(&grannyAnimation);

	//CARPETTTTTT !!!!!!!!!!!!!!!!!!!!
	Model carpet("models/Carpet/carpet.dae");
	Shader carpShader("vs.vs", "fs.frag");

	// Определение начального времени
	double startTime = glfwGetTime();
	double backgroundTime = glfwGetTime();
	double interval = 1.0; // Интервал в секундах
	double back_interval = 2.0f;


	glm::vec3 randomColor = glm::vec3(1.0f); // до дискотеки свет нормальный
	glm::vec3 randomColor2 = glm::vec3(1.0f); // до дискотеки свет нормальный
	glm::vec3 randomColor3 = glm::vec3(1.0f); // до дискотеки свет нормальный


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
		double cur = glfwGetTime(); // текущее время для смены цвета заднего фона
		double elapsed = cur - backgroundTime; // прошедшее время с послежней засечки
		if (performanse)
		{
			animator.UpdateAnimation(deltaTime); //grandPa
			secondAnimator.UpdateAnimation(deltaTime); //Shrek
			grannyAnimator.UpdateAnimation(deltaTime); //Granny
			if (first_sound && play)
			{
				result = PlaySoundA(audioFiles[which_song-1], NULL, SND_FILENAME | SND_ASYNC);
				first_sound = false;
			}
			if (elapsed >= back_interval)
			{
				glm::vec3 back_col = glm::linearRand(glm::vec3(0.1f), glm::vec3(0.9f));
				glClearColor(back_col.x, back_col.y, back_col.z, 1.0f);
				backgroundTime = cur;
				back_interval = glm::linearRand(0.1f, 1.5f);
			}
		}
		else
		{
			glClearColor(0.424f, 0.0f, 0.71f, 1.0f);
		}

		// render
		// ------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		double currentTime = glfwGetTime(); // Получение текущего времени
		double elapsedTime = currentTime - startTime; // Вычисление прошедшего времени с последнего засечки
		if (elapsedTime >= interval)
		{
			randomColor = glm::linearRand(glm::vec3(0.1f), glm::vec3(1.0f));
			randomColor2 = glm::linearRand(glm::vec3(0.1f), glm::vec3(1.0f));
			randomColor3 = glm::linearRand(glm::vec3(0.1f), glm::vec3(1.0f));
			// Обновление начального времени
			startTime = currentTime;
			interval = glm::linearRand(0.1f, 1.5f);
		}
		if (rash)
		{
			rash_camera();
		}
		else if (reset_camera)
		{
			camera.Position = glm::vec3(0.0f, 25.0f, 60.0f);
			reset_camera = false;
		}
		if (!performanse)
		{
			randomColor = glm::vec3(1.0f);
			randomColor2 = glm::vec3(1.0f);
			randomColor3 = glm::vec3(1.0f);
		}

		// don't forget to enable shader before setting uniforms
		ourShader.use();
		secondShader.use();
		grannyShader.use();

		lightPos.x = 1.0f + sin(glfwGetTime()) * 60.0f;
		lightPos.y = cos(glfwGetTime() / 2.0f) * 12.0f;

		// view/projection transformations GRANDPA
		// --------------------------------------------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		//GrandPa
		ourShader.setMat4("projection", projection); 
		ourShader.setMat4("view", view); 

		//GrandPa
		auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			ourShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		// render the loaded model
		// -----------------------------------------------------------------------------------
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-42.0f, -0.2f, -60.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);

		set_light(ourShader, randomColor);
		ourModel.Draw(ourShader);



		// view/projection transformations SHREK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// --------------------------------------------------------------------------------------
		secondShader.setMat4("projection", projection);
		secondShader.setMat4("view", view);

		auto transformsS = secondAnimator.GetFinalBoneMatrices();
		for (int i = 0; i < transformsS.size(); ++i)
			secondShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transformsS[i]);

		// render the loaded model
		// -----------------------------------------------------------------------------------
		glm::mat4 modelS = glm::mat4(1.0f);
		modelS = glm::translate(modelS, glm::vec3(30.0f, -0.2f, -60.0f)); // translate it down so it's at the center of the scene
		modelS = glm::scale(modelS, glm::vec3(0.35f, 0.35f, 0.35f));	// it's a bit too big for our scene, so scale it down
		secondShader.setMat4("model", modelS);

		set_light(secondShader, randomColor2);
		secondModel.Draw(secondShader);

		// !!!!!!!!!!!!!!!!!!!!!!!!!!! view/projection transformations GRANNY !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// --------------------------------------------------------------------------------------
		grannyShader.setMat4("projection", projection);
		grannyShader.setMat4("view", view);

		auto transformsG = grannyAnimator.GetFinalBoneMatrices();
		for (int i = 0; i < transformsG.size(); ++i)
			grannyShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transformsG[i]);

		// render the loaded model
		// -----------------------------------------------------------------------------------
		glm::mat4 modelG = glm::mat4(1.0f);
		if (dvigats)
		{
			init_pose.z -= 0.01;
		}
		modelG = glm::translate(modelG, /*glm::vec3(-10.0f, -0.2f, -60.0f)*/init_pose); // translate it down so it's at the center of the scene
		modelG = glm::scale(modelG, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
		grannyShader.setMat4("model", modelG);

		set_light(grannyShader, randomColor3);
		grannyModel.Draw(grannyShader);



		carpShader.use();
		carpShader.setMat4("projection", projection);
		carpShader.setMat4("view", view);
		glm::mat4 modelC = glm::mat4(1.0f);
		modelC = glm::translate(modelC, glm::vec3(0.0f, -0.2f, -110.0f)); // translate it down so it's at the center of the scene
		
		modelC = glm::rotate(modelC, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelC = glm::rotate(modelC, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelC = glm::scale(modelC, glm::vec3(100.0f, 50.0f, 50.0f));	
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
		if (performanse) camera.Position = glm::vec3(0.0f, 25.0f, 60.0f);
		play = !play;
		if (play == false)
		{
			first_sound = true;
			result = PlaySoundA(NULL, NULL, 0);
		}
		Sleep(200);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		which_song = 1;
		Sleep(200);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		which_song = 2;
		Sleep(200);
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		which_song = 3;
		Sleep(200);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		rash = !rash;
		if (rash == false)
		{
			reset_camera = true;
		}
		Sleep(200);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		dvigats = true;
		//Sleep(200);
	}
	else
	{
		dvigats = false;
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

void rash_camera()
{
	float shake_amount = glm::linearRand(0.1f, 5.5f);
	glm::vec3 shake_direction = glm::vec3(glm::linearRand(-shake_amount, shake_amount),
		glm::linearRand(0.1f, 0.6f),
		glm::linearRand(-glm::linearRand(0.1f, 1.5f), glm::linearRand(0.1f, 1.5f)));

	if (camera.Position.x > 15.0f) camera.Position.x -= 10.0f;
	if (camera.Position.x < -20.0f) camera.Position.x += 15.0f;

	if (camera.Position.y > 0.6f) camera.Position.y -= 0.3f;
	if (camera.Position.y < 0.1f) camera.Position.y += 0.2f;

	if (camera.Position.z > 65.0f) camera.Position.z -= 5.0f;
	if (camera.Position.z < 55.0f) camera.Position.z += 5.0f;

	camera.Position += shake_direction;
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
