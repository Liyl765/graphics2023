#include "app.h"
//libs
#define STB_IMAGE_IMPLEMENTATION
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//std
#include <stdexcept>
#include <array>
#include <chrono>
#include <iostream>

Camera camera;
Camera modelCamera;
bool cameraState;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
glm::vec3 pressnum = glm::vec3(0.0f);
glm::vec3 press_scale = glm::vec3(0.1f);
float press_rotate = 0.0f;

App::App(){}

App::~App() {}

void App::run()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    //��������
    //stbi_set_flip_vertically_on_load(true);

    //��Ȳ���
    glEnable(GL_DEPTH_TEST);

    //һЩ��ǰ����
    camera.Position = glm::vec3(0.0f, 1.0f, 3.0f);
    camera.state = false;
    glViewport(0, 0, WIDTH, HEIGHT);
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    MovementController cameraController{};
    cameraController.initMouse(window.getGLFWwindow());
    cameraState = false;
    //�����ص�
    glfwSetKeyCallback(window.getGLFWwindow(), key_callback);

    //��ɫ��
    Shader lightshader{"./resources/light.vert", "./resources/light.frag"};
    Shader shader{ "./resources/vertshader.vert", "./resources/fragshader.frag" };
    Shader bgshader{ "./resources/bg.vert", "./resources/bg.frag" };

    // ��ȡģ��
    Model quadModel("resources/models/quad/quad.obj");
    Model charModel("resources/models/diona/diona.pmx");
    Model bgModel("resources/models/bg/bg.obj");
    
    // light vertices
    float vertices[] = {
        // positions          
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f, 0.5f,  0.5f, -0.5f, 0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f,  -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, 0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    //��Դ
    uint32_t lightVAO, lVBO;
    glGenBuffers(1, &lVBO);
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //��Ⱦѭ��
    while (!window.shouldClose())
    {
        //ʱ��
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //����л�
        if (cameraState == false)
        {
            //keyboard
            cameraController.moveInPlaneXZ(window.getGLFWwindow(), deltaTime, camera);
            //mouse
            if (cameraController.state)
                cameraController.ProcessMouseMovement(window.getGLFWwindow(), deltaTime, camera);
        }

		//��Ⱦָ��
		//glClearColor���������Ļ���õ���ɫ----״̬���ú���
		//glClear���������ɫ����֮����Ļ�����ΪglClearColor�е���ɫ��---״̬ʹ�ú���
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //������ɫ��
        shader.use();
        shader.setVec3("viewPos", camera.Position);
        shader.setFloat("material.shininess", 32.0f);
        // directional light
        shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        shader.setVec3("pointLights[0].position", pointLightPositions[0]);
        shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights[0].constant", 1.0f);
        shader.setFloat("pointLights[0].linear", 0.09f);
        shader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        shader.setVec3("pointLights[1].position", pointLightPositions[1]);
        shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights[1].constant", 1.0f);
        shader.setFloat("pointLights[1].linear", 0.09f);
        shader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        shader.setVec3("pointLights[2].position", pointLightPositions[2]);
        shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights[2].constant", 1.0f);
        shader.setFloat("pointLights[2].linear", 0.09f);
        shader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        shader.setVec3("pointLights[3].position", pointLightPositions[3]);
        shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights[3].constant", 1.0f);
        shader.setFloat("pointLights[3].linear", 0.09f);
        shader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        shader.setVec3("spotLight.position", camera.Position);
        shader.setVec3("spotLight.direction", camera.Front);
        shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("spotLight.constant", 1.0f);
        shader.setFloat("spotLight.linear", 0.09f);
        shader.setFloat("spotLight.quadratic", 0.032f);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        //ת������
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = cameraState ? modelCamera.GetViewMatrix() : camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        charModel.position = glm::mat4(1.0f);
        //��ģ��ģʽ�¸ı�ģ�ͣ����������ڻص�������
        charModel.position = glm::translate(charModel.position, pressnum);
        charModel.position = glm::scale(charModel.position, press_scale);
        charModel.position = glm::rotate(charModel.position, glm::radians(press_rotate), glm::vec3(0.0f, 1.0f, 0.0f));

        //modelCamera
        modelCamera.Position = glm::vec3(pressnum) + glm::vec3(0.0f, 1.0f, 5.0f);

        //draw model
        shader.setMat4("model", charModel.position);
        charModel.Draw(shader);

        //�ذ�
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        quadModel.position = glm::mat4(1.0f);
        quadModel.position = glm::scale(quadModel.position, glm::vec3(10.0f, 10.0f, 10.0f));
        shader.setMat4("model", quadModel.position);
        quadModel.Draw(shader);


        //��Դ
        lightshader.use();
        lightshader.setMat4("projection", projection);
        lightshader.setMat4("view", view);
        glBindVertexArray(lightVAO);
        for (uint32_t i = 0; i < 4; ++i)
        {
            glm::mat4 lmodel = glm::mat4(1.0f);
            lmodel = glm::translate(lmodel, pointLightPositions[i]);
            lmodel = glm::scale(lmodel, glm::vec3(0.05f)); // Make it a smaller cube
            lightshader.setMat4("model", lmodel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //����
        bgshader.use();
        bgshader.setMat4("projection", projection);
        bgshader.setMat4("view", view);
        bgModel.position = glm::mat4(1.0f);
        bgModel.position = glm::scale(bgModel.position, glm::vec3(50.0f, 50.0f, 50.0f));
        bgshader.setMat4("model", bgModel.position);
        bgModel.Draw(lightshader);
		//�������¼�����������
		//˫���壺ǰ�滺��һ�Ż���������ʾ����̨����⻺����һ�ţ�����һ�Ž�����ֱ�ӽ�����Ч���ϱ㲻�����������л��ƴ�����������
		glfwSwapBuffers(window.getGLFWwindow());//������ɫ����
		glfwPollEvents();//�����꣬���̣����´���״̬�������ö�Ӧ�Ļص�������
    }
    //delete
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lVBO);
    //�˳�glfw
    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
        glfwSetWindowShouldClose(window, true);
    if ((key == GLFW_KEY_F) && (action == GLFW_PRESS))
        cameraState = !cameraState;
    if (key == GLFW_KEY_A && action == GLFW_PRESS && cameraState) {
        pressnum[0] -= 0.5f;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS && cameraState) {
        pressnum[0] += 0.5f;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS && cameraState) {
        pressnum[2] -= 0.5f;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS && cameraState) {
        pressnum[2] += 0.5f;
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS && cameraState) {
        pressnum[1] += 0.5f;
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS && cameraState) {
        pressnum[1] -= 0.5f;
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS && cameraState) {
        press_scale += glm::vec3(0.01f);
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && cameraState) {
        press_scale -= glm::vec3(0.01f);
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS && cameraState) {
        press_rotate += 5.0f;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS && cameraState) {
        press_rotate -= 5.0f;
    }
}