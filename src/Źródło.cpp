//#define _USE_MATH_DEFINES
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <cmath>
//#include <algorithm>
//
//const GLchar* vertexShaderSource = R"glsl(
//#version 150 core
//in vec3 position;
//in vec3 color;
//out vec3 Color;
//void main(){
//Color = color;
//gl_Position = vec4(position, 1.0);
//}
//)glsl";
//
//const GLchar* fragmentShaderSource = R"glsl(
//#version 150 core
//in vec3 Color;
//out vec4 outColor;
//void main()
//{
//outColor = vec4(Color, 1.0);
//}
//)glsl";
//
//#define W_WIDTH 640
//#define W_HEIGHT 640
//
//#define VERTEX_DATA_COUNT 6
//static size_t triangles = 1;
//static GLfloat* vertices = new GLfloat[triangles * 3 * VERTEX_DATA_COUNT]{ 1.0f };
//
//static auto primitive = GL_TRIANGLES;
//static auto use_cursor = false;
//
//void draw_triangles(size_t count) {
//    triangles = count;
//    vertices = new GLfloat[triangles * VERTEX_DATA_COUNT * 3];
//
//    constexpr float r = 1.0;
//    auto d_angle = 2 * static_cast<float>(M_PI) / static_cast<float>(triangles);
//    auto prev_color_r = std::abs(sinf(0.0f));
//    auto prev_color_g = std::abs(sinf(0.0f + 1 * M_PI / 6));
//    auto prev_color_b = std::abs(sinf(0.0f + 2 * M_PI / 6));
//    //    auto prev_color_b = 1.0f;
//
//    for (size_t i = 0; i < triangles * VERTEX_DATA_COUNT * 3; i += VERTEX_DATA_COUNT * 3) {
//        auto angle = d_angle * static_cast<float>(static_cast<float>(i) / (VERTEX_DATA_COUNT * 3));
//        auto x = cosf(angle) * r;
//        auto y = sinf(angle) * r;
//        vertices[i + 0] = x;
//        vertices[i + 1] = y;
//        vertices[i + 2] = 0.0f;
//        vertices[i + 3] = prev_color_r;
//        vertices[i + 4] = prev_color_g;
//        vertices[i + 5] = prev_color_b;
//
//        prev_color_r = std::abs(sinf(0.5f * (angle + d_angle + 0 * M_PI / 3)));
//        prev_color_g = std::abs(sinf(0.5f * (angle + d_angle + 1 * M_PI / 3)));
//        prev_color_b = std::abs(sinf(0.5f * (angle + d_angle + 2 * M_PI / 3)));
//
//        x = cosf(angle + d_angle) * r;
//        y = sinf(angle + d_angle) * r;
//        vertices[i + VERTEX_DATA_COUNT + 0] = x;
//        vertices[i + VERTEX_DATA_COUNT + 1] = y;
//        vertices[i + VERTEX_DATA_COUNT + 2] = 0.0f;
//        vertices[i + VERTEX_DATA_COUNT + 3] = prev_color_r;
//        vertices[i + VERTEX_DATA_COUNT + 4] = prev_color_g;
//        vertices[i + VERTEX_DATA_COUNT + 5] = prev_color_b;
//
//        vertices[i + 2 * VERTEX_DATA_COUNT + 0] = 0.0f;
//        vertices[i + 2 * VERTEX_DATA_COUNT + 1] = 0.0f;
//        vertices[i + 2 * VERTEX_DATA_COUNT + 2] = 0.0f;
//        vertices[i + 2 * VERTEX_DATA_COUNT + 3] = 1.0f;
//        vertices[i + 2 * VERTEX_DATA_COUNT + 4] = 1.0f;
//        vertices[i + 2 * VERTEX_DATA_COUNT + 5] = 1.0f;
//    }
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * triangles * VERTEX_DATA_COUNT * 3, vertices, GL_STATIC_DRAW);
//}
//
//void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
//    if (!use_cursor)
//        return;
//    constexpr int max_vert = 200;
//    auto vert = static_cast<size_t>(max_vert * ypos / W_HEIGHT);
//    draw_triangles(vert);
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
//    std::cout << "xoffset = " << xoffset << ", yoffset = " << yoffset << std::endl;
//    triangles = std::max(1ull, triangles + static_cast<size_t>(yoffset));
//    std::cout << triangles << std::endl;
//    draw_triangles(triangles);
//}
//
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//    //    std::cout << "key = " << key << ", scancode = " << scancode << std::endl;`
//    if (action != GLFW_PRESS)
//        return;
//    switch (key) {
//    case (GLFW_KEY_ESCAPE):
//        glfwSetWindowShouldClose(window, true);
//        break;
//    case (GLFW_KEY_GRAVE_ACCENT): // tilda
//        use_cursor = !use_cursor;
//        break;
//    case (GLFW_KEY_1):
//        primitive = GL_POINTS;
//        break;
//    case (GLFW_KEY_2):
//        primitive = GL_LINES;
//        break;
//    case (GLFW_KEY_3):
//        primitive = GL_TRIANGLES;
//        break;
//    default:
//        break;
//    }
//}
//
//int main() {
//    /* Initialize the library */
//    if (!glfwInit()) {
//        return -1;
//    }
//    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    /* Create a windowed mode window and its OpenGL context */
//    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL", nullptr, nullptr);
//
//    if (!window) {
//        std::cout << "Could not create instance of GLFWwindow. Terminating...";
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//    gladLoadGL();
//
//    glViewport(0, 0, W_WIDTH, W_HEIGHT);
//
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
//    glCompileShader(vertexShader);
//
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
//    glCompileShader(fragmentShader);
//
//    GLuint shaderProgram = glCreateProgram();
//
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//
//    glBindFragDataLocation(shaderProgram, 0, "outColor");
//
//    glLinkProgram(shaderProgram);
//    glUseProgram(shaderProgram);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//
//    GLuint VAO{ 0 }, VBO{ 0 };
//
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//
//    draw_triangles(10);
//
//    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
//    glEnableVertexAttribArray(posAttrib);
//    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, VERTEX_DATA_COUNT * sizeof(GLfloat), nullptr);
//
//    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
//    glEnableVertexAttribArray(colAttrib);
//    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, VERTEX_DATA_COUNT * sizeof(GLfloat),
//        (void*)(2 * sizeof(GLfloat)));
//
//    glfwSetCursorPosCallback(window, cursor_position_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//    glfwSetKeyCallback(window, key_callback);
//
//    /* Loop until the user closes the window */
//    while (!glfwWindowShouldClose(window)) {
//        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glDrawArrays(primitive, 0, triangles * 3);
//
//        /* Swap front and back buffers */
//        glfwSwapBuffers(window);
//        /* Poll for and process events */
//        glfwPollEvents();
//
//    }
//
//    glfwTerminate();
//    return 0;
//}