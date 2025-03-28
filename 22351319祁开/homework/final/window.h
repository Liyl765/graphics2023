#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "utils.h"
class Window {
public:
	void init(class Application* application);
	void process_key_input(const float deltaTime);
	static void framebufferResizeCallback(GLFWwindow* pWindow, int width, int height);
	GLFWwindow* pWindow;
	class Application* application;
};