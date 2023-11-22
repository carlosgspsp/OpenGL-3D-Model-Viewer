#pragma once


#include "ModuleCamera.h"
class Mesh
{
private:
	unsigned VBO, EBO, VAO, programID;
public:
	void CreateProgram();
	void LoadVBO();
	void LoadEBO();
	void CreateVAO();
	void Draw();
};

