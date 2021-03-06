#include "main.h"
#include "Lession1/shader.h"
#include "Lession1/texture.h"
#include "Lession1/creator.h"
#include "camera.h"
#include "Lession3/model.h"

Controller* colorRectController = nullptr;
Controller* textureRectController = nullptr;
Controller* cubeController = nullptr;
Controller* objectController = nullptr;
Controller* lampController = nullptr;
Controller* modelController = nullptr;
Controller* currentController = nullptr;
Camera* camera = nullptr;

void drawColorRect()
{
	currentController->use();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void createColorRect()
{
	camera->setEnable(false);
	if (!colorRectController)
	{
		colorRectController = new Controller("Lession1/vs.glsl", "Lession1/fs.glsl");
		createColorRectInfo(colorRectController);
		colorRectController->setDraw(drawColorRect);
		colorRectController->setClearColor(0.2f, 0.3f, 0.3f);
	}
	currentController = colorRectController;
}

void drawTextureRect()
{
	currentController->use();
	currentController->activeTexture(GL_TEXTURE0, 0);
	currentController->shader->setInt("texture1", 0);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	currentController->shader->setMat4("transform", trans);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void createTextureRect()
{
	camera->setEnable(false);
	if (!textureRectController)
	{
		textureRectController = new Controller("Lession1/texvs.glsl", "Lession1/texfs.glsl");
		createTextureRectInfo(textureRectController);
		textureRectController->addTexture("Lession1/container.jpg");
		textureRectController->setDraw(drawTextureRect);
		textureRectController->setClearColor(0.3f, 0.2f, 0.3f);
	}
	currentController = textureRectController;
}

glm::mat4 getRotateView()
{
	float radius = 10.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	return glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void drawCube()
{
	currentController->use();
	currentController->activeTexture(GL_TEXTURE0, 0);
	currentController->shader->setInt("texture1", 0);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	glm::mat4 view = glm::mat4(1.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//view = getRotateView();
	view = camera->getView();
	glm::mat4 projection = glm::mat4(1.0f);
	//projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	projection = camera->getProjection();
	currentController->shader->setMat4("model", model);
	currentController->shader->setMat4("view", view);
	currentController->shader->setMat4("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void createCube()
{
	camera->setEnable(true);
	if (!cubeController)
	{
		cubeController = new Controller("Lession1/cubevs.glsl", "Lession1/texfs.glsl");
		createCubeInfo(cubeController);
		cubeController->addTexture("Lession1/container.jpg");
		cubeController->setDraw(drawCube);
		cubeController->setDepthEnable(true);
		cubeController->setClearColor(0.3f, 0.3f, 0.2f);
	}
	currentController = cubeController;
}

glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

void drawLamp()
{
	lampController->use();

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera->getView();
	glm::mat4 projection = camera->getProjection();
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	lampController->shader->setMat4("model", model);
	lampController->shader->setMat4("view", view);
	lampController->shader->setMat4("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void createLamp()
{
	camera->setEnable(true);
	if (!lampController)
	{
		lampController = new Controller("Lession2/lampvs.glsl", "Lession2/lampfs.glsl");
		createLightInfo(lampController, false);
		lampController->setDraw(drawLamp);
	}
}

void drawLight()
{
	currentController->use();
	currentController->activeTexture(GL_TEXTURE1, 0);
	currentController->activeTexture(GL_TEXTURE2, 1);

	currentController->shader->setVec3("viewPos", camera->pos);
	currentController->shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	//currentController->shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//currentController->shader->setVec3("lightPos", lightPos);
	// material
	//currentController->shader->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	//currentController->shader->setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
	//currentController->shader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);

	currentController->shader->setInt("material.diffuse", 1);
	currentController->shader->setInt("material.specular", 2);
	currentController->shader->setFloat("material.shininess", 64.0f);
	//// light
	//currentController->shader->setInt("light.type", 1);
	//currentController->shader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
	////currentController->shader->setVec3("light.position", lightPos);
	//currentController->shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	//currentController->shader->setVec3("light.diffuse", 0.5f, 0.5, 0.5f);
	//currentController->shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	// ��ʱ��仯
	//glm::vec3 lightColor = glm::vec3(1.0f);
	//lightColor.x = sin(glfwGetTime() * 2.0f);
	//lightColor.y = sin(glfwGetTime() * 0.7f);
	//lightColor.z = sin(glfwGetTime() * 1.3f);
	//glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
	//currentController->shader->setVec3("light.ambient", ambientColor);
	//currentController->shader->setVec3("light.diffuse", diffuseColor);

	// spotlight
	currentController->shader->setVec3("light.direction", camera->front);
	currentController->shader->setVec3("light.position", camera->pos);
	currentController->shader->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
	currentController->shader->setFloat("light.outCutOff", glm::cos(glm::radians(17.5f)));
	currentController->shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	currentController->shader->setVec3("light.diffuse", 0.5f, 0.5, 0.5f);
	currentController->shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera->getView();
	glm::mat4 projection = camera->getProjection();
	currentController->shader->setMat4("model", model);
	currentController->shader->setMat4("view", view);
	currentController->shader->setMat4("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	if (lampController)
		lampController->draw();
}

void createLight()
{
	camera->setEnable(true);
	if (!objectController)
	{
		objectController = new Controller("Lession2/objectvs.glsl", "Lession2/objectfs.glsl");
		createLightInfo(objectController);
		objectController->addTexture("Lession2/container2.png");
		objectController->addTexture("Lession2/container2_specular.png");
		objectController->setDraw(drawLight);
		objectController->setDepthEnable(true);
		objectController->setClearColor(0.1f, 0.1f, 0.1f);
	}
	currentController = objectController;

	createLamp();
}

Model *myModel = nullptr;

void drawModel()
{
	currentController->use();

	glm::mat4 projection = camera->getProjection();
	glm::mat4 view = camera->getView();
	currentController->shader->setMat4("projection", projection);
	currentController->shader->setMat4("view", view);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	currentController->shader->setMat4("model", model);

	if (!myModel)
		myModel = new Model("Lession3/nanosuit/nanosuit.obj");
	myModel->Draw(*(currentController->shader));
}

void createModel()
{
	camera->setEnable(true);
	if (!modelController)
	{
		modelController = new Controller("Lession3/modelvs.glsl", "Lession3/modelfs.glsl");
		modelController->setDraw(drawModel);
		modelController->setDepthEnable(true);
		modelController->setClearColor(0.05f, 0.05f, 0.05f);
	}
	currentController = modelController;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool firstMouse = true;
double lastX;
double lastY;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	
	camera->mouseMove(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->mouseScroll(yoffset);
}

GLFWwindow* initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Create window failed" << std::endl;
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Init GLAD fialed" << std::endl;
		return NULL;
	}

	return window;
}

float deltaTime = 0;
float lastFrame = 0;

void calcTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W))
		camera->keyMove(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S))
		camera->keyMove(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A))
		camera->keyMove(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D))
		camera->keyMove(Camera_Movement::RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		createColorRect();
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		createTextureRect();
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		createCube();
	else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		createLight();
	else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		createModel();
}

int start()
{
	GLFWwindow* window = initGLFW();
	if (window == NULL)
	{
		glfwTerminate();
		system("pause");
		return -1;
	}

	camera = new Camera();
	createColorRect();

	while (!glfwWindowShouldClose(window))
	{
		calcTime();
		processInput(window);

		if (currentController->shader == nullptr || currentController->shader->ID == NULL)
		{
			glfwTerminate();
			system("pause");
			return -1;
		}

		currentController->setDepthEnable();
		currentController->clear();
		currentController->draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	currentController = nullptr;
	delete colorRectController;
	colorRectController = nullptr;
	delete textureRectController;
	textureRectController = nullptr;
	delete cubeController;
	cubeController = nullptr;
	delete objectController;
	objectController = nullptr;
	delete lampController;
	lampController = nullptr;
	delete camera;
	camera = nullptr;

	glfwTerminate();
	return 0;
}

int main()
{
	return start();
}