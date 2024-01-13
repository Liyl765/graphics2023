#include "glew/glew.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Utils.h"
#include "ImportedModel.h"
#include "camera.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

static const int screen_width = 1920;
static const int screen_height = 1080;

static const int numVAOs = 1;
static const int numVBOs = 3;

static const float pai = 3.1415926f;

GLuint renderingProgram = 0;
GLuint vao[numVAOs] = { 0 };
GLuint vbo[numVBOs] = { 0 };

int width = 0;
int height = 0;
float aspect = 0.f;
float objLocX = 0.f, objLocY = 0.f, objLocZ = 0.f;
GLuint mvLoc = 0;
GLuint projLoc = 0;

GLuint shuttleTextureId = 0;

glm::mat4 mMat(1.f), vMat(1.f), pMat(1.f), mvMat(1.f);

Camera camera(glm::vec3(0.f, 0.f, 2.f));
float cameraX, cameraY, cameraZ;
ImportedModel myModel("resources/shuttle.obj");

GLboolean keys[1024] = { GL_FALSE };
GLboolean firstMouse = GL_TRUE;
float deltaTime = 0.f;
float lastFrame = 0.f;
float lastLocX = 0.f;
float lastLocY = 0.f;

float toRadians(float degrees)
{
	return (degrees * 2.f * pai) / 360.f;
}

void setupVertices(void)
{
	vector<glm::vec3> vert = myModel.getVertices();
	vector<glm::vec2> text = myModel.getTextureCoords();
	vector<glm::vec3> norm = myModel.getNormals();

	vector<float> pValues;
	vector<float> tValues;
	vector<float> nValues;

	for (int i=0; i< myModel.getNumVertices(); i++)
	{
		/*pValues.push_back(vert[i * 3 + 0].x);
		pValues.push_back(vert[i * 3 + 1].y);
		pValues.push_back(vert[i * 3 + 2].z);

		tValues.push_back(text[i * 2 + 0].s);
		tValues.push_back(text[i * 2 + 1].t);

		nValues.push_back(norm[i * 3 + 0].x);
		nValues.push_back(norm[i * 3 + 1].y);
		nValues.push_back(norm[i * 3 + 2].z);*/

		pValues.push_back(vert[i].x);
		pValues.push_back(vert[i].y);
		pValues.push_back(vert[i].z);

		tValues.push_back(text[i].s);
		tValues.push_back(text[i].t);

		nValues.push_back(norm[i].x);
		nValues.push_back(norm[i].y);
		nValues.push_back(norm[i].z);
	}

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pValues.size() * sizeof(float), &(pValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, myModel.getVertices().size() * sizeof(float), &(pVlaues[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tValues.size() * sizeof(float), &(tValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, myModel.getTextureCoords().size() * sizeof(float), &(tValues[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nValues.size() * sizeof(float), &(nValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, myModel.getNormals().size() * sizeof(float), &(nValues[0]), GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	//glfwGetWindowSize(window, &width, &height);
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 2.f;
	objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.0f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(toRadians(45.f), aspect, 0.01f, 1000.f);
	
	setupVertices();
	shuttleTextureId = Utils::loadTexture("resources/spstob_1.jpg");

	/*lastLocX = (float)screen_width / 2.f;
	lastLocY = (float)screen_height / 2.f;*/
}


void window_size_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	//��Ļ����ʹ��ڵ�֡����
	/*GLFW���������������ĵ��е���������ϵ��
		�����֮����������������ڼ������� / �򴰿ڵģ������Բ�һ����Ӧ����ʵ��Ļ���ص����쵥Ԫ������ ��DPI���ű�����ʱ�����磬�ڴ�������Ĥ��ʾ����Mac�ϣ������������ˡ�
		�봰��������ȣ�֡�������Ĵ�С��������أ��Ա���glViewport OpenGLsҪ����ƥ�䡣
		��ע�⣬��ĳЩϵͳ�ϣ�����������������������ͬ�����ⲻһ������ȷ�ġ�*/
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);

	cameraX = 0.0f; cameraY = 0.0f; cameraZ =4.f;
	objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.0f;
	mMat = glm::translate(glm::mat4(1.f), glm::vec3(objLocX, objLocY, objLocZ));
	//pMat = glm::perspective(glm::radians(45.f), aspect, 0.001f, 1000.f);
	pMat = glm::perspective(camera.Zoom, aspect, 0.001f, 1000.f);
}

void do_movement(void)
{
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.5f, 0.5f, 1.f);

	//������ɫ������,��GPU�ϰ�װGLSL����,�ⲻ��������ɫ������
	glUseProgram(renderingProgram);

	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;

	do_movement();

	//��ȡuniform��������ɫ�������е�λ����ţ�ͨ������ſ�������һ�±�����ֵ�����û�иñ����򷵻�-1
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	mMat = glm::translate(glm::mat4(1.f), glm::vec3(objLocX, objLocY, objLocZ));
	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	vMat = camera.GetViewMatrix();


	mMat = glm::rotate(mMat, toRadians(0.f), glm::vec3(1.f, 0.f, 0.f));
	mMat = glm::rotate(mMat, toRadians(35.f), glm::vec3(0.f, 1.f, 0.f));
	mMat = glm::rotate(mMat, toRadians(35.f), glm::vec3(0.f, 0.f, 1.f));

	//������Ҫ�У���������н�ʧЧ
	pMat = glm::perspective(camera.Zoom, aspect, 0.01f, 1000.f);

	mvMat = vMat * mMat;

	//����һ��uniform��������������ֵ��Ҫ���ĵ�uniform������λ����locationָ����location��ֵӦ����glGetUniformLocation��������
	// ��͸�Ӿ����MV �����Ƹ���Ӧ��ͳһ����
	/*ͨ��һ�±�����uniform���εı��������ý�һ�±���ֵ������Ⱦ���ߡ�
		location : uniform��λ�á�
		count : ��Ҫ�������ݵ�����Ԫ�ص�����������Ҫ�޸ĵľ����������
		transpose : ָ��������������(column major)����GL_FALSE������������(row major)����GL_TRUE����
		value : ָ����count��Ԫ�ص������ָ�롣
	*/
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));  //GL_FALSE �������ܴ�����޷���ʾobjģ��

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shuttleTextureId);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//ָ������Ⱦʱ����ֵΪ index �Ķ���������������ݸ�ʽ��λ��
	/*Parameters
	index
		ָ��Ҫ�޸ĵĶ������Ե�����ֵ

		size
		ָ��ÿ���������Ե��������������Ϊ1��2��3����4����ʼֵΪ4������ά����position����3����x, y, z����ɣ�����ɫ��4����r, g, b, a����

		type
		ָ��������ÿ��������������͡����õķ��ų�����GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, �� GL_FLOAT����ʼֵΪGL_FLOAT��

		normalized
		ָ����������ʱ���̶�������ֵ�Ƿ�Ӧ�ñ���һ����GL_TRUE������ֱ��ת��Ϊ�̶���ֵ��GL_FALSE����

		stride
		ָ��������������֮���ƫ���������Ϊ0����ô�������Իᱻ���Ϊ�������ǽ���������һ��ġ���ʼֵΪ0��

		pointer
		ָ��һ��ָ�룬ָ�������е�һ���������Եĵ�һ���������ʼֵΪ0��
		*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//������ã�������ͨ�ö������������е�ֵ�����ڵ��ö������������glDrawArrays��glDrawElements��ʱ���ڳ��֡�
	//��Ӧ������ɫ����layout(location=0) in vec3 position;
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shuttleTextureId);

	glEnable(GL_DEPTH_TEST);
	//ָ��������Ȼ���Ƚ�ֵ��
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());
}


void press_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (action == GLFW_PRESS)
	{
		keys[key] = GLFW_TRUE;
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = GLFW_FALSE;
	}
}

void mouse_move_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastLocX = xPos;
		lastLocY = yPos;
		firstMouse = GL_FALSE;
	}

	double offsetLocX = xPos - lastLocX;
	double offsetLocY = lastLocY - yPos;

	lastLocX = xPos;
	lastLocY = yPos;

	camera.ProcessMouseMovement(offsetLocX, offsetLocY);
}

void mouse_scroll_callback(GLFWwindow* window, double xPos, double yPos)
{
	camera.ProcessMouseScroll(yPos);
}



int main(int argc, char** argv)
{
	int glfwState = glfwInit();
	if (GLFW_FALSE == glfwState)
	{
		cout << "GLFW initialize failed,invoke glfwInit()......Error file:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/*��Ϊ����Ҫʹ��OpenGL 4.6���������ǰ�GLFW_CONTEXT_VERSION_MAJOR��GLFW_CONTEXT_VERSION_MINOR��Ӧ��hint������Ϊ4��6��
	��Ϊ����Ҫʹ��OpenGL����ģʽ�����������ᵽ���ࣩ���������ǰ�GLFW_OPENGL_PROFILE��Ӧ��hint����ΪGLFW_OPENGL_CORE_PROFILE��
	��ʾʹ��OpenGL����ģʽ����󣬰�GLFW_RESIZABLE��Ӧ��hint����ΪGLFW_FALSE����ʾ���ڲ������û�������С��
	֮��������������Ϊ��������û�������С����С�����仯�󣬴��ڵĻ�������Ĭ�ϲ��䣨��Ȼ��ԭ�����ڵ����򣩣�
	Ҳ����˵�����ϻ��Ƶ�ͼ��Ĵ�С��λ�ò��ᷢ���ı䡣Ϊ�˱������������������Ǿͼ򵥵ز����û��������ڴ�С
	����ȻҲ�и��õķ�����������GLFW����һ�����ڴ�С�Ļص��������������Ƚϼ򵥣���*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Load obj file model", nullptr, nullptr);
	if (!window)
	{
		cout << "GLFW create window failed,invoke glfwCreateWindow()......Error line:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/*�˺���ʹ�����߳��ϵ�ָ�����ڵ� OpenGL �� OpenGL ES �����ĳ�Ϊ��ǰ�����ġ�
	  һ��ֻ���ڵ����߳���ʹ�����ĳ�Ϊ��ǰ�����ģ�����ÿ���߳�һ��ֻ����һ����ǰ�����ġ�
	  ���߳�֮���ƶ�������ʱ��������ʹ���ھ��߳��ϱ�Ϊ�ǵ�ǰ״̬��Ȼ���������߳��ϱ�Ϊ��ǰ״̬��
	*/
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, press_key_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	//�������ģʽ
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	int glewState = glewInit();
	if (GLEW_OK != glewState)
	{
		cout << "GLEW initialize failed,invoke glewInit()......Error file:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/*�˺������õ�ǰ OpenGL �� OpenGL ES �����ĵĽ�����������ӵ���glfwSwapBuffers��ʼ�ȴ�����Ļ���´�����
	  Ȼ���ٽ��������������ء�����ʱ��Ϊ��ֱͬ������ֱ��ɨͬ�����vsync��
	  ֧��WGL_EXT_swap_control_tear��GLX_EXT_swap_control_tear��չ��������Ҳ���ܸ����������������������������������
	  ��ʹ֡�����е���������ʹ��glfwExtensionSupported�����Щ��չ��
	  �����ı����ڵ����߳��������µġ���û�е�ǰ�����ĵ�����µ��ô˺���������GLFW_NO_CURRENT_CONTEXT����
	  �˹��ܲ������� Vulkan�������ʹ�� Vulkan ������Ⱦ�����Ϊ�鿴�������ĵ�ǰģʽ��
	*/
	glfwSwapInterval(1);

	printf("%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));//��ʼ��ʼ������
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version : %s\n", glslVersion);

	glGetError(); // Debug GLEW bug fix


	/*��Ϊ����Ҫʹ��OpenGL 4.6���������ǰ�GLFW_CONTEXT_VERSION_MAJOR��GLFW_CONTEXT_VERSION_MINOR��Ӧ��hint������Ϊ4��6��
	��Ϊ����Ҫʹ��OpenGL����ģʽ�����������ᵽ���ࣩ���������ǰ�GLFW_OPENGL_PROFILE��Ӧ��hint����ΪGLFW_OPENGL_CORE_PROFILE��
	��ʾʹ��OpenGL����ģʽ����󣬰�GLFW_RESIZABLE��Ӧ��hint����ΪGLFW_FALSE����ʾ���ڲ������û�������С��
	֮��������������Ϊ��������û�������С����С�����仯�󣬴��ڵĻ�������Ĭ�ϲ��䣨��Ȼ��ԭ�����ڵ����򣩣�
	Ҳ����˵�����ϻ��Ƶ�ͼ��Ĵ�С��λ�ò��ᷢ���ı䡣Ϊ�˱������������������Ǿͼ򵥵ز����û��������ڴ�С
	����ȻҲ�и��õķ�����������GLFW����һ�����ڴ�С�Ļص��������������Ƚϼ򵥣���*/



	/*Ĭ������£��������ܿ��ǣ����ж�����ɫ�������ԣ�Attribute���������ǹرյģ�
	  ��ζ����������ɫ�����ǲ��ɼ��ģ����������Ѿ��ϴ���GPU����glEnableVertexAttribArray����ָ�����ԣ�
	  �ſ��ڶ�����ɫ���з����𶥵���������ݡ�glVertexAttribPointer��VBOֻ�ǽ���CPU��GPU֮����߼����ӣ�
	  �Ӷ�ʵ����CPU�����ϴ���GPU�����ǣ�������GPU���Ƿ�ɼ���������ɫ���ܷ��ȡ�����ݣ����Ƿ������˶�Ӧ�����Ծ�����
	  �����glEnableVertexAttribArray�Ĺ��ܣ���������ɫ����ȡGPU���������ˣ����ݡ�
	 */
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, (float)glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}