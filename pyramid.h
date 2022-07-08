#pragma once
#include "object.h"

class Pyramid : public object
{
    const std::vector<float> verts= { -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f,	0.5f,	1.0f,
		0.5f, -0.5f, 0.5f,	1.0f, 0.0f,

		0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f
	};
	const int numberofPrimitives = 12;

public:
    //constructor for pyramid object
	Pyramid();
    
};

Pyramid::Pyramid() {
	Pyramid::vertices = this->verts;
	Pyramid::primitivesNo = this->numberofPrimitives;
}


