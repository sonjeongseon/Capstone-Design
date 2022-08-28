// camera ��ɱ��� ���� ��

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>// �̱� ��� �̹����ε� ���̺귯��

#include <glm/glm.hpp> // GLM �⺻ ���, ���� �ڷ��� ����
#include <glm/gtc/matrix_transform.hpp>// ��� ��ȯ�� ���Ǵ� �Լ� ����
#include <glm/gtc/type_ptr.hpp>// GLM �ڷ������� �޸� ���� ���

#include <SHADERS/shader.h>
#include <CAMERA/camera.h>
#include <MODEL/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool fisrtMouse = true;


// ���� ����
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// ������ �ϴµ� �ɸ��� �ð�
float deltaTime = 0.0f;	// ������ frame ~ ���� frame
float lastFrame = 0.0f; // ������ frame

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SONGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // ���� �������� context�� ���� �������� �� context�� ����
    glfwMakeContextCurrent(window);
    // buffer�� ũ�Ⱑ ����� �� ȣ��Ǵ� �ݹ�
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);// cursor capture -> ��Ŀ�� �Ǹ� Ŀ���� ������ â �ȿ� ���α�

    // glad: load all OpenGL function pointers
    // glad �ʱ�ȭ
    // GLFW�� os�� ���� �ùٸ� �Լ��� �����ϴ� glfwGetProcAddress ����
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // Ȱ��ȭ
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    //glEnable(GL_DEPTH_TEST); // ���� �߰��ϸ鼭 ��Ȱ��ȭ

    Shader ourShader("test5.vs", "test5.fs");

    // load models
    // -----------
    Model ourModel("objects/lamp.obj");

    // vertices �迭�� floatŸ�� �÷� ������ �߰�
    // = �ؽ�ó ��ǥ�� vertex �����Ϳ� �߰�
    float vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left  
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    // vertex attribute�� �߰��߱� ������ �ٽ� �˷������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    GLuint texture[2];

    // texture 1
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    // �ؽ�ó ����
    // �Ķ���� 1 : �ؽ�ó Ÿ�� ����
    // �Ķ���� 2 : ������ �ɼ�, � �࿡ ������ ������ ����
    // �Ķ���� 3 : �ؽ�ó wrapping ��� ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // �ؽ�ó ���͸�
    // GL_LINEAR�� GL_NEAREST���� �� �� ��谡 �Һи� -> �Ų�����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    // �̹��� �ε�
    // �Ķ���� 1 : �̹��� ���� ��� �ޱ�
    // �Ķ���� 2, 3, 4 : �̹����� �ʺ�, ����, �÷� ä�� ��(���� ������ ����)
    unsigned char* data = stbi_load("textures/brick.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture", 0);

    // loop �������� GPU�� perspective projection ���ε�
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);


    while (!glfwWindowShouldClose(window))
    {
        //fame���� delta �� ���
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input ó��
        processInput(window);

        // buffer �ʱ�ȭ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// ȭ���� �÷� ���� �����, � ������ ������ ����(���� ���� �Լ�)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //��� ���� ��Ʈ : color, depth, stencil, �ϴ� �÷����� �����ؼ� �װ͸� ����(���� ��� �Լ�) 

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        // activate shader
        ourShader.use();

        // glm::perspective : ���� ���̴� ������ �����ϴ� �������� ���� ���ڸ��(����ü) ����
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render box
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
            ourShader.setMat4("model", model);
            ourModel.Draw(ourShader);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);// �÷�����(�ݺ� �� �̹��� �׸��� ȭ�鿡 ����ϴ� ���) ��ü
        glfwPollEvents();// �̺�Ʈ �߻� Ȯ�� -> ������ ���� ������Ʈ, ������ �Լ� ȣ��
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();// ������ ���� ����Ǹ� �Ҵ�Ǿ��� ��� �ڿ� ����/����
    return 0;
}

//�Է� ����
//glfwGetKey : Ű���� Ű�� �Բ� ������ �Է� ���� �� ����, �ش� Ű�� ������ �ִ��� ���� ����
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//ESC �������� Ȯ�� -> �������� true->GLFW ����
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// yaw, pitch �� ��� ���� 
// xposln , yposln : ���� ���콺 ��ġ
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    // 1) ���콺 offset ���ϱ�
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // 2) ���콺 callback �Լ����� ������~���� ������ ������ offset ���
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    // ���콺 ���� ����
    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // �̻��� ������ ����
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// zoom
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}