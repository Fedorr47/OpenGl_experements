#pragma once
#include <vector>

#include "primitive_object.h"
#include "glengine.h"
#include "shader.h"
#include "camera.h"
#include "box.h"

class GameEntry;

std::unique_ptr<GameEntry> game_ptr;

class GameEntry
{
	std::vector<PrimitiveObject> objects_;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Shader> shaders_;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void do_movement();

	GLuint VBO, VAO;
	// Holds uniform value of texture mix
	GLfloat mixValue = 0.2f;
	const GLuint screenWidth = 800, screenHeight = 600;
public:
	bool keys[1024];
	GLfloat deltaTime;
	GLfloat lastFrame;

	GLfloat lastX, lastY;
	bool firstMouse;

	GameEntry() 
	{
		lastX = 400, lastY = 300;
		firstMouse = true;
		GlEngine_ptr.reset(new GlEngine());

		camera_.reset(new Camera(glm::vec3(0.0f, 0.0f, 3.0f)));

		GlEngine_ptr->add_callback(GlEngine_ptr->callback_type::KEY, key_callback);
		GlEngine_ptr->add_callback(GlEngine_ptr->callback_type::MOUSE, mouse_callback);
		GlEngine_ptr->add_callback(GlEngine_ptr->callback_type::SCROLL, scroll_callback);

		// Build and compile our shader program
		shaders_.reset(new Shader(
			"C:/git/OpenGl_experements/OpenGl_experemnts/shaders/shader_triangle.vs",
			"C:/git/OpenGl_experements/OpenGl_experemnts/shaders/fragment_triangle.frag"
		)
		);
	}
	~GameEntry();
	void GameLoop()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

		// Load and create a texture 
		GLuint texture1;
		GLuint texture2;
		// --== TEXTURE 1 == --
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load, create texture and generate mipmaps
		int width, height;
		unsigned char* image = SOIL_load_image("C:/git/OpenGl_experements/OpenGl_experemnts/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
		// --== TEXTURE 2 == --
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load, create texture and generate mipmaps
		image = SOIL_load_image("C:/git/OpenGl_experements/OpenGl_experemnts/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		while (!glfwWindowShouldClose(GlEngine_ptr->get_window()))
		{
			// Set frame time
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Check and call events
			glfwPollEvents();
			do_movement();

			// Render
			// Clear the colorbuffer
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaders_->Use();

			// Bind Textures using texture units
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(glGetUniformLocation(shaders_->Program, "ourTexture1"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glUniform1i(glGetUniformLocation(shaders_->Program, "ourTexture2"), 1);

			// Set current value of uniform mix
			glUniform1f(glGetUniformLocation(shaders_->Program, "mixValue"), mixValue);


			glm::mat4 view;
			view = (*camera_).GetViewMatrix();
			glm::mat4 projection;
			projection = glm::perspective((*camera_).Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
			// Get their uniform locatio
			GLint modelLoc = glGetUniformLocation(shaders_->Program, "model");
			GLint viewLoc = glGetUniformLocation(shaders_->Program, "view");
			GLint projLoc = glGetUniformLocation(shaders_->Program, "projection");
			// Pass them to the shaders
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

			// Draw container
			glBindVertexArray(VAO);
			for (GLuint i = 0; i < 10; i++)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 0.0f;
				model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			glBindVertexArray(0);
			glfwSwapBuffers(GlEngine_ptr->get_window());
		}
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
	
};


GameEntry::~GameEntry()
{
	
}

void GameEntry::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		game_ptr->keys[key] = true;
	else if (action == GLFW_RELEASE)
		game_ptr->keys[key] = false;
}

void GameEntry::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (game_ptr->firstMouse)
	{
		game_ptr->lastX = xpos;
		game_ptr->lastY = ypos;
		game_ptr->firstMouse = false;
	}

	GLfloat xoffset = xpos - game_ptr->lastX;
	GLfloat yoffset = game_ptr->lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	game_ptr->lastX = xpos;
	game_ptr->lastY = ypos;

	(*game_ptr->camera_).ProcessMouseMovement(xoffset, yoffset);
}

void GameEntry::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//(*camera_).ProcessMouseScroll(yoffset);
}

void GameEntry::do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		(*camera_).ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		(*camera_).ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		(*camera_).ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		(*camera_).ProcessKeyboard(RIGHT, deltaTime);
}

