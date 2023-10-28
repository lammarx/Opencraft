#pragma once

class GLFWwindow;

class Window {
public:
	static int init(int width, int height, const char* title);
	static void terminate();

	static bool isShouldClose();
	static void swapBuffers();
private:
	static GLFWwindow* window;
};
