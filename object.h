#pragma once
#include <vector>

//a base class for all the 3D objects we made 

class object
{
protected:
	std::vector <float> vertices;
	int primitivesNo;

public:
	const int getPrimitives();
	float* getVertices();
	std::size_t getSize();
	std::size_t getStride();

};


const int object::getPrimitives() {
	return(primitivesNo);
}

float* object::getVertices() {
	return(&vertices[0]);
}

std::size_t object::getSize() {
	return(sizeof(float) * vertices.size());
}
std::size_t object::getStride() {
	return(sizeof(float) * 6);
}


