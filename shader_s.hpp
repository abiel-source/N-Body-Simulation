#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream> //file i/o
#include <sstream> //read to strings
#include <iostream> //console log

//constants
const std::string TYPE_PROG = "PROGRAM";
const std::string TYPE_VERTEX_SHADER = "VERTEX_SHADER";
const std::string TYPE_FRAGMENT_SHADER = "FRAGMENT_SHADER";

class Shader
{
public:
	//CONSTRUCTORS
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	//MUTATORS
	void set();

	//ACCESSORS
	unsigned int getProgramID();


private:
	unsigned int programID;

	//UTILITY
	void checkCompileError(unsigned int ID, std::string type);
};

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//get shader source
	std::ifstream vertexFile, fragmentFile;
	std::stringstream vertexStream, fragmentStream;
	std::string vertexStrSrc, fragmentStrSrc;
	const char* vertexSrc;
	const char* fragmentSrc;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexStrSrc = vertexStream.str();
		fragmentStrSrc = fragmentStream.str();

		vertexFile.close();
		fragmentFile.close();
	}
	catch (const std::exception& exc)
	{
		std::cout << "EXCEPTION::SHADER_FILE_OPEN" << std::endl;
		std::cout << exc.what() << std::endl;
	}

	vertexSrc = vertexStrSrc.c_str();
	fragmentSrc = fragmentStrSrc.c_str();

	//compile shader source
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
	glCompileShader(vertexShader);
	checkCompileError(vertexShader, TYPE_VERTEX_SHADER);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
	glCompileShader(fragmentShader);
	checkCompileError(fragmentShader, TYPE_FRAGMENT_SHADER);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);
	checkCompileError(programID, TYPE_PROG);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(programID);
}

//MUTATORS
void Shader::set()
{
	glUseProgram(programID);
}

//ACCESSORS
unsigned int Shader::getProgramID()
{
	return programID;
}

//UTILITY
void Shader::checkCompileError(unsigned int ID, std::string type)
{
	int success;
	char infoLog[512];
	if (type == TYPE_PROG)
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, nullptr, infoLog);
			std::cout << "ERROR::PROGRAM_LINK_ERROR\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER_COMPILE_ERROR\n" << infoLog << std::endl;
		}
	}
}