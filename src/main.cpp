#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <math.h>
#include <iostream>
#include "libs/laky_camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "libs/laky_resmanager.h"

#include "libs/stb_image.h"

// SETTINGS
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// CALLBACKS
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  // Resize callback
static void error_callback(int error, const char* description); // Error callback

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); // Key callback
char keys[1024]; // Array to store the state of each key

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn); // Mouse callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); // Scroll callback

// FUNCTIONS
void processInput(GLFWwindow *window);
void render();

// ERROR CHECKS
void get_lightingShader_error(unsigned int lightingShader);
void get_link_error(unsigned int lightingShaderProgram);

// DELTA TIME
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// CAMERA
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float cameraSpeed = 2.5f;

float camWidth = SCR_WIDTH;
float camHeight = SCR_HEIGHT;
float camAspect = camWidth / camHeight;

//float x = 0.0f;
//float y = 0.0f;

// LIGHTING
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// MAIN
int main()
{

	stbi_set_flip_vertically_on_load(true);

	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Laky's First Modern OpenGL", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide cursor

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}   

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glEnable(GL_DEPTH_TEST);


	Shader lightingShader = ResourceManager::LoadShader("assets/shaders/material.vert", "assets/shaders/material.frag", "material_shader");
	Shader lightCubeShader = ResourceManager::LoadShader("assets/shaders/lighting.vert", "assets/shaders/lighting.frag", "light_cube");

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	//--------------------------------------------------------------------------------------------


	// Cube Positions ;)
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	//Buffers

	// cubes VBO & VAO
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light VAO (VAO is same as the cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	lightingShader.use();
	
	// Textures

    Texture2D diffuse_map, specular_map;

	diffuse_map = ResourceManager::LoadTexture("assets/textures/woodcontainer_albedo.png", true, "container");
	specular_map = ResourceManager::LoadTexture("assets/textures/woodcontainer_specular.png", true, "container_specular");

	//--------------------------------------------------------------------------------------------

	// Projections, Views, Models

	float near = 0.1f;
	float far = 100.0f;
	float fov = 45.0f;

	// Perspective projection (ooh 3D!)
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, near, far);

	// Camera
	glm::mat4 view;
	view = camera.GetViewMatrix();

	lightingShader.setInt("texture1", 0);
	lightingShader.setInt("texture2", 1);

	lightingShader.setMat4("projection", projection);
	lightingShader.setMat4("view", view);


	lightCubeShader.setMat4("projection", projection);
	lightCubeShader.setMat4("view", view);

	// Game loop
	while(!glfwWindowShouldClose(window))
	{
		processInput(window); // Process keyboard events

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;  

		cameraSpeed = 5.0f * deltaTime;

		// Clear the screen
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.setInt("material.diffuse", 0);

		glActiveTexture(GL_TEXTURE0);
		diffuse_map.Bind();

		lightingShader.setInt("material.specular", 1);

		glActiveTexture(GL_TEXTURE1);
		specular_map.Bind();

		// change the light's position values over time (can be done anywhere in the render loop actually, but try to do it at least before using the light source positions)
        lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		lightPos.z = 1.0f + cos(glfwGetTime()) * 2.0f;

		// Use the lightingShader program
        lightingShader.use();
        lightingShader.setVec3f("light.position", lightPos);
        lightingShader.setVec3f("viewPos", camera.Position);

		// Set material
		lightingShader.setVec3f("material.ambient", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3f("material.diffuse", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3f("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setFloat("material.shininess", 32.0f);

		// Set light properties

		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		/*lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);*/

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

		lightingShader.setVec3f("light.ambient", ambientColor);
		lightingShader.setVec3f("light.diffuse", diffuseColor);
		lightingShader.setVec3f("light.specular", 1.0f, 1.0f, 1.0f);


		// create transformations
		glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view          = glm::mat4(1.0f);
		glm::mat4 projection    = glm::mat4(1.0f);
		


		projection = glm::perspective(glm::radians(camera.Zoom), camAspect, near, far);

		view = camera.GetViewMatrix();

		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("model", model);
		lightingShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(cubeVAO);
        /*for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to lightingShader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

			lightingShader.setMat4("projection", projection);
			lightingShader.setMat4("model", model);
			lightingShader.setMat4("view", view);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/

		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

		glDrawArrays(GL_TRIANGLES, 0, 36);



        // also draw the lamp object
        lightCubeShader.use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setMat4("model", model);

		lightCubeShader.setVec3f("lightColor", lightColor);
		

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);



		glfwSwapBuffers(window);
		glfwPollEvents();    
	}

	glfwTerminate();
	return 0;
}

// FUNCTIONS

void render()
{


}

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(keys[GLFW_KEY_W])
	{
		std::cout << "W key pressed" << std::endl;
	}

	// Toggle wireframe mode
	if(keys[GLFW_KEY_SPACE])
	{
		GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

		if(polygonMode == GL_FILL)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			// cooldown for 0.2 seconds
			glfwSetTime(0.0);
			while(glfwGetTime() < 0.1)
			{
				// do nothing
			}
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			// cooldown for 0.2 seconds
			glfwSetTime(0.0);
			while(glfwGetTime() < 0.1)
			{
				// do nothing
			}
		}
	}

	if(keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, cameraSpeed);
	}
	if(keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, cameraSpeed);
	}

	if(keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, cameraSpeed);
	}
	if(keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, cameraSpeed);
	}

	if(keys[GLFW_KEY_Q])
	{
		camera.ProcessKeyboard(DOWN, cameraSpeed);
	}

	if(keys[GLFW_KEY_E])
	{
		camera.ProcessKeyboard(UP, cameraSpeed);
	}
}



// CALLBACKS

// Resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	camWidth = width;
	camHeight = height;
	camAspect = camWidth / camHeight;

	glViewport(0, 0, width, height);
} 

// Error callback
static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
 
// Key callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		keys[key] = 1;
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = 0;
	}
}

// Mouse callback
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

// Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// ERROR CHECKS
void get_lightingShader_error(unsigned int lightingShader)
{
	int success;
	char infoLog[512];
	glGetShaderiv(lightingShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(lightingShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void get_link_error(unsigned int lightingShaderProgram)
{
	int success;
	char infoLog[512];
	glGetProgramiv(lightingShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(lightingShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
}