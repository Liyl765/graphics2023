#include <iostream>
#include"glad/glad.h"//������ע�����/ǰ�����h�ļ����ϼ�Ŀ¼������ÿ���˰�װ��ͬ��Ҳ�в�ͬ
#include"GLFW/glfw3.h"
using namespace std;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

}

//vertex shader
const GLchar* vertexShader_Code =
"#version 330 core\n"
"layout(location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"}";


const GLchar* fragmentShader_Code =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor=vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";

int main()
{
	//���Ƕ���
	float vertices[] =
	{
		-0.5f,-0.5f,0.0f,//���½�
		0.5f,-0.5f,0.0f,//���½�
		0.0f, 0.5f,0.0f//���˽�
	};
	glfwInit();//��ʼ��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//����GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//����GLFW
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	glfwSetKeyCallback(window, key_callback);


	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();//terminate the glfw(end the glfw)
		return -1;
	}

	glfwMakeContextCurrent(window);//���õ�ǰopenGL����

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	//data
	int isSuccess;
	char infoLog[512];
	//vertexShader data
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShader_Code, NULL);
	glCompileShader(vertexShader);
	/*�����Ƿ�ͨ��*/
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::VertexShader::Compile_failed" << infoLog << endl;
	}
	else
	{
		cout << "SUCCESS::VertexShader" << endl;
	}

	//fragmentShader Data
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShader_Code, NULL);
	glCompileShader(fragmentShader);
	/*�����Ƿ�ͨ��*/
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::FragmentShader::Compile_failed" << infoLog << endl;
	}
	else
	{
		cout << "SUCCESS::FragmentShader" << endl;
	}

	//Shader Pragram
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::ShaderProgram::Link_failed" << infoLog << endl;
	}
	else
	{
		cout << "SUCCESS::ShaderProgram" << endl;
		glUseProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	//vectices data
	unsigned int VAO;
	unsigned int VBO;

	glGenVertexArrays(1, &VAO);//���һ�������������
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();//�����û�д����¼�
		glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);//˫���壨�滻���ڵ�ÿһ������ֵ��
	}
	glfwTerminate();
	return 0;
}

