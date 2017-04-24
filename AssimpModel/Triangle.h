#pragma once

#include "GL\glew.h"
#include "GL\freeglut.h"
#include <iostream>
#include "Camera.h"
#include "Model.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SDL\SDL_mixer.h"

class Triangle
{
public:
	Triangle();
	~Triangle();

	GLfloat speed = 0.02f;
	//for camera
	GLfloat delta_time = 0.0f;
	GLfloat last_frame = 0.0f;
	GLfloat last_x;
	GLfloat last_y;
	GLboolean mouse_first_in;
	Camera camera;

	Shader shader;
	Model ourModel;


	glm::mat4 MVP;
	glm::mat4 perspective_view;
	glm::mat4 perspective_projection;
	glm::mat4 matr_model_1;
	glm::mat4 matr_model_2;

	// text
	glm::mat4 text_matrix_2D; // orthographic projection
	glm::mat4 text_matrix_3D_model_1; // perspective  projection
	glm::mat4 text_matrix_3D_model_2; // perspective  projection

	void init();
	void update();
	void render();
	void playSound();

	static GLuint loadDDS(const char* image_path, int* w = nullptr, int* h = nullptr);
	static GLuint loadImageToTexture(const char* image_path);

};

