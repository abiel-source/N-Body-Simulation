#pragma once
#include <GLFW\glfw3.h>

// screen
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// shaders
const char* vertexPath = "shader.vs";
const char* fragmentPath = "shader.fs";

// spherical model
const GLfloat PI = 3.14159265358979323846f;
const int Y_SEGMENTS = 2;
const int X_SEGMENTS = 2;

// engine constants
GLfloat avgM = 1;
const float G = 0.001; // 0.0000000000667408
const float e_soft = 0.005;


// n bodies
#define N 300