#pragma once

#include "glad/glad.h"

#include <string>

class Shader
{
public:
	// The Program ID
	unsigned int ID;

	// Constructor reads and build the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// Use/Activate Shader
	void Use();
	// Uniform utility functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};