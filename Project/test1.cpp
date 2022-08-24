// Fail to load textures .. 

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h> // �̱� ��� �̹����ε� ���̺귯��

#include <glm.hpp> // GLM �⺻ ���, ���� �ڷ��� ����
#include <gtc/matrix_transform.hpp>// ��� ��ȯ�� ���Ǵ� �Լ� ����
#include <gtc/type_ptr.hpp>// GLM �ڷ������� �޸� ���� ���

#include <SHADERS/shader.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ��ũ�� ũ��
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfwInit : GLFW �ʱ�ȭ
    // glfwWindowHint : LFW ���� (�Ķ���� 1 : �츮�� �����ϰ��� �ϴ� �ɼ� �˷���, GLFW_ ���ξ� ���� ����
    // 2 : �ɼ� �� ����, ���������� �ɼǰ� ���� ���� �����ϱ� !, Mac OS�� �� �߰��ؾ���  )
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // window ��ü ����, �� window ��ü�� ��� window ������ ����
    // GLFWwindow�� ó�� �ΰ��� �Ķ���ͷ� â�� �ʺ�� ���� ����, ���� ���� �־ �ǰ� ���� ���� ����(�Ʒ��� ����)
    // ����°�� â �̸�
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SONJSGL", NULL, NULL);
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

    // glad �ʱ�ȭ
    //GLFW�� os�� ���� �ùٸ� �Լ��� �����ϴ� glfwGetProcAddress ����
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("6.1.coordinate_systems.vs", "6.1.coordinate_systems.fs");

    // vertices �迭�� 4���� floatŸ�� �÷� ������ �߰�
    // = �ؽ�ó ��ǥ�� vertex �����Ϳ� �߰�
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // ���� ���
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // ���� �ϴ�
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // ���� �ϴ�
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // ���� ���
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    // vertex attribute�� �߰��߱� ������ �ٽ� �˷������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int texture1, texture2;

    // �ؽ�ó 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

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

    // �ؽ�ó �ε� �� ����
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    // �̹��� �ε�
    // �Ķ���� 1 : �̹��� ���� ��� �ޱ�
    // �Ķ���� 2, 3, 4 : �̹����� �ʺ�, ����, �÷� ä�� ��(���� ������ ����)
    unsigned char* data = stbi_load("textures/brick.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // �Ķ���� 1 : �ؽ�ó Ÿ�� ����
        // �Ķ���� 2 : �ؽ�ó �̴ϸ� ������ �������� �����ϰ� ���� �� ���
        // �Ķ���� 3 : �츮�� �����ϰ� ���� �ؽ�ó�� � ������ ������ �� ��, ���⼭�� RGB���� ���
        // �Ķ���� 4, 5 : �ؽ�ó�� �ʺ�� ���� ����
        // �Ķ���� 6 : �׻� 0 �����ؾ���
        // �ĸ����� 7, 8 : �����̹����� ����� ������ Ÿ�� ����
        // �Ķ���� 9 : ���� �̹��� ������
        // ����� ���̽� ����, ���� �̴ϸ� ����ϰ� �ʹٸ� ��� �̹��� ���� ���� �� �Ʒ� �Լ� ���
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        // ���� ���ε��� �ؽ�ó�� ���� �ʿ��� ��� �̴ϸ� �ڵ� ����
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data); // �̹��� �޸� ��ȯ

    // �ؽ�ó 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("test.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // ���� ����(render loop) : �츮�� �׸��϶�� �Ҷ����� ��� ����
    // glfwWindowShouldClose : �� ���� ���۶����� GLFW�� �����ϵ��� ���õǾ����� Ȯ��
    // ���� �׷��ٸ� true��ȯ�ؼ� ���� 
    //��� ������ ����� ������ ���� �ȿ�
    while (!glfwWindowShouldClose(window))
    {
        // �Է� ó��
        processInput(window);

        // ���� �ʱ�ȭ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// ȭ���� �÷� ���� �����, � ������ ������ ����(���� ���� �Լ�)
        glClear(GL_COLOR_BUFFER_BIT);//��� ���� ��Ʈ : color, depth, stencil, �ϴ� �÷����� �����ؼ� �װ͸� ����(���� ��� �Լ�) 

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // glm::perspective : ���� ���̴� ������ �����ϴ� �������� ���� ���ڸ��(����ü) ����
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        // model ��� : vertex���� world space�� ��ȯ�ϱ� ���� �͵�� �̷���� ����
        // vertex * model -> vertex�� world��ǥ�� ��ȯ ����
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // �츮�� �����̰� ���� ����� �ݴ�� scene �̵�
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // �Ķ���� 1 : fov �� ����(view space�� �󸶳� ū��, ���� ������ ������ ���� 45���� ����)
        // �Ķ���� 2 : viewport�� �ʺ� ���̷� �������ν� ���Ǵ� ȭ�� ���� ����
        // �Ķ���� 3, 4 : ����� ���� �� ��� ���� �Ÿ� ����
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);// �÷�����(�ݺ� �� �̹��� �׸��� ȭ�鿡 ����ϴ� ���) ��ü
        glfwPollEvents();// �̺�Ʈ �߻� Ȯ�� -> ������ ���� ������Ʈ, ������ �Լ� ȣ��
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();// ������ ���� ����Ǹ� �Ҵ�Ǿ��� ��� �ڿ� ����/����
    return 0;
}

//�Է� ����
//glfwGetKey : Ű���� Ű�� �Բ� ������ �Է� ���� �� ����, �ش� Ű�� ������ �ִ��� ���� ����
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//ESC �������� Ȯ�� -> �������� true->GLFW ����
        glfwSetWindowShouldClose(window, true);
}

// OpenGL���� ������ ������ ������ �˷���
// glViewport ���ؼ� ���� ���� ����
// 1, 2 �Ķ���ʹ� ������ ���� �Ʒ� �𼭸� ��ġ ����, 3, 4 �Ķ���ʹ� ������ ������ �ʺ�� ���� �ȼ����� (GLFW ��ü���� ������)
// ���ο��� OpenGL�� glViewport �Լ� ���� ������ ������ ����ؼ� 2D->��ũ�� ��ǥ�� ��ȯ
// â�� ũ�� �����ϴ� ���� ����Ʈ�� ���� �ʿ� -> �̰� framebuffer_size_callback
// f_b_s �Լ��� �Ķ���� 1�� GLFWwindow ��ü �ް�, 2, 3 �Ķ���ͷ� â�� ���ο� ũ�� ��Ÿ���� �ΰ��� ���� ����
// â�� ũ�� ����ɶ����� GLFW�� �� �Լ� ȣ�� -> ������ ������ ä���
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}