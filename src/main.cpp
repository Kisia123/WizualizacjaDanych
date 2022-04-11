// Nag³ówki
#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <../stb_image.h>

// Kody shaderów
const GLchar* vertexSource = R"glsl(
#version 150 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
in vec2 aTexCoord;
out vec2 TexCoord;
in vec3 position;
in vec3 color;
out vec3 Color;
in vec3 aNormal;
out vec3 Normal;
out vec3 FragPos;
void main(){
TexCoord = aTexCoord;
Color = color;
gl_Position = proj*view*model*vec4(position, 1.0);
}
)glsl";

const GLchar* fragmentSource = R"glsl(
#version 150 core
in vec2 TexCoord;
uniform sampler2D texture1;
in vec3 Color;
out vec4 outColor;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform float ambientStrength = 0.1;
uniform vec3 ambientlightColor = vec3(1.0,1.0,1.0);

void main()
{
vec4 ambient = ambientStrength * vec4(ambientlightColor,1.0);
vec3 difflightColor = vec3(1.0,1.0,1.0);
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm,lightDir),0.0);
vec3 diffuse = diff*difflightColor;
outColor = (ambient + vec4(diffuse,1.0))*texture(texture1,TexCoord);
}
)glsl";

double mouseX;
double mouseY;
double yaw = -90; //os Y
double pitch = 0; // os X
double sensitivity = 0.01;
double deltaTime;

int PRYMITYW;
int numberOfTriangles = 2;
//GLfloat* vertices = new GLfloat[numberOfTriangles * 6 * 3];

GLfloat vertices[] = {
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,1.0f,0.0f,
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,1.0f,1.0f,
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,1.0f,1.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,1.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,

-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,1.0f,0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,1.0f,1.0f,
0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,1.0f,1.0f,
-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,0.0f,1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,

-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,
-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,1.0f,0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,1.0f, 1.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,1.0f,1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,0.0f,1.0f,
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,

0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,0.0f,
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,1.0f,0.0f,
0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,1.0f,1.0f,
0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,1.0f,1.0f,
0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,0.0f,1.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,

-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,
0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,1.0f,0.0f,
0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,1.0f,1.0f,
0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,1.0f,1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,0.0f,1.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,

-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,1.0f,0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,1.0f,1.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,1.0f,1.0f,
-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,0.0f,1.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,0.0f
};

//Ustawianie shaderów tak juak powinno siê wg rozpisek - https://learnopengl.com/Getting-started/Shaders

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLuint shaderProgram;

float obrot = 0.0f;
float rot = 0.0f;
glm::mat4 view;
GLint uniView;

void camera_move() {
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}

void key_callback(GLFWwindow* window, int key, int scnacode, int action, int mods) {

	float cameraSpeed = 0.6*deltaTime;

	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);

	if (key == GLFW_KEY_UP) {
		cameraPos += cameraSpeed * cameraFront;
	}

	if (key == GLFW_KEY_DOWN) {
		cameraPos -= cameraSpeed * cameraFront;
	}

	if (key == GLFW_KEY_LEFT) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (key == GLFW_KEY_RIGHT) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (key == GLFW_KEY_W) {
		cameraPos += cameraSpeed * cameraUp;
	}
	if (key == GLFW_KEY_S) {
		cameraPos -= cameraSpeed * cameraUp;
	}
	if (key == GLFW_KEY_B) {
		GLint color = glGetUniformLocation(shaderProgram, "ambientlightColor");
		glUniform3f(color, 0.0, 0.0, 1.0);
	}
	if (key == GLFW_KEY_R) {
		GLint color = glGetUniformLocation(shaderProgram, "ambientlightColor");
		glUniform3f(color, 1.0, 0.0, 0.0);
	}
	if (key == GLFW_KEY_G) {
		GLint color = glGetUniformLocation(shaderProgram, "ambientlightColor");
		glUniform3f(color, 0.0, 1.0, 0.0);
	}
	if (key == GLFW_KEY_9) {
		GLint brightness = glGetUniformLocation(shaderProgram, "ambientStrength");
		glUniform1f(brightness, 1.0);
	}
	if (key == GLFW_KEY_0) {
		GLint brightness = glGetUniformLocation(shaderProgram, "ambientStrength");
		glUniform1f(brightness, 0.6);
	}
	camera_move();
}


void mouse_callback(GLFWwindow* window, double x, double y) {
	//if (y > mouseY) {
	//	numberOfTriangles++;
	//}
	//else if (y < mouseY&&numberOfTriangles>0) {
	//	numberOfTriangles -- ;
	//}
	double xoffset = x - mouseX;
	double yoffset = y - mouseY;
	mouseX = x;
	mouseY = y;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch -= yoffset;
	if (pitch > 89) {
		pitch = 89;
	}
	if (pitch < -89) {
		pitch = -89;
	}
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	camera_move();

}


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	GLFWwindow* window = glfwCreateWindow(1000, 1000, "Okno OpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "nie udalo sie" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	
	glfwSetKeyCallback(window, key_callback);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, 1000, 1000);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint isCompiled = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		GLchar errorLog[16 * 1024];
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		std::cout << errorLog << std::endl;
		glDeleteShader(fragmentShader); // Don't leak the shader.
		return false;
	}
	//return true;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLuint vbo;
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*8*24, vertices, GL_STATIC_DRAW);
	
	GLint TexCoord = glGetAttribLocation(shaderProgram, "aTexCoord");
	glEnableVertexAttribArray(TexCoord);
	glVertexAttribPointer(TexCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(void*)(6 * sizeof(GLfloat)));


	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

	
	view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	GLint NorAttrib = glGetAttribLocation(shaderProgram, "aNormal");
	glEnableVertexAttribArray(NorAttrib);
	glVertexAttribPointer(NorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	GLint uniLightPos = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3fv(uniLightPos, 1, &lightPos[0]);

	uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1000.0f / 1000.0f, 0.06f, 100.0f);
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(posAttrib);
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(24 * sizeof(GLfloat)));
	

	
	double previousTime = glfwGetTime();
	double FPS = 60;

	//tekstury
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int w, h, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("../texture3.jpg", &w, &h, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture1);

	while (!glfwWindowShouldClose(window)) {
	

		double currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		if (currentTime - previousTime >= 1.0 / FPS) {
			glClearColor(0.357f, 0.071f, 0.271f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLES, 0, 24);
			glfwSwapBuffers(window);
			glfwSetWindowTitle(window, std::to_string(1/deltaTime).c_str());

			previousTime = currentTime;
		}

		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}