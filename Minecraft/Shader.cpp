#include "Shader.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Shader::Shader(unsigned int id) : id(id) {
}

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::use() {
	glUseProgram(id);
}


Shader* load_shader(std::string vertexFile, std::string fragmentFile) {
	//std::cout << std::filesystem::current_path() << std::endl;
	std::string path = std::filesystem::current_path().string();	
	vertexFile = path + "\\" + vertexFile;
	fragmentFile = path + "\\" + fragmentFile;

	// Reading Files
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile(vertexFile, std::ifstream::in);
	std::ifstream fShaderFile(fragmentFile, std::ifstream::in);

	//vShaderFile.exceptions(std::ifstream::badbit);
	//fShaderFile.exceptions(std::ifstream::badbit);
	//vShaderFile.open(vertexFile);
	if (!vShaderFile.good())
	{
		std::cerr << "Failed to open " + vertexFile << std::endl;
		return nullptr;
	}
	if (!fShaderFile.good())
	{
		std::cerr << "Failed to open " + fragmentFile << std::endl;
		return nullptr;
	}
	
	std::stringstream vShaderStream, fShaderStream;

	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();

	vShaderFile.close();
	fShaderFile.close();

	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();
	
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cerr << "SHADER::VERTEX: compilation failed" << std::endl;
		std::cerr << infoLog << std::endl;
		return nullptr;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cerr << "SHADER::FRAGMENT: compilation failed" << std::endl;
		std::cerr << infoLog << std::endl;
		return nullptr;
	}

	// Shader Program
	GLuint id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		std::cerr << "SHADER::PROGRAM: linking failed" << std::endl;
		std::cerr << infoLog << std::endl;

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return nullptr;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	
	return new Shader(id);
}