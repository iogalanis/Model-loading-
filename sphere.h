#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "glm.hpp"
#include <vector>

class sphere 
{
public:
    int sectorCount;
    int stackCount;
    float radius;

    std::vector<float> vertices;
    std::vector<int> indices;
    sphere();
    sphere(float radius, int sectorCount, int stackCount);

    std::size_t getVertSize();
    std::size_t getIndSize();
    std::size_t getStride();
    float* getVertices();
    int* getIndices();
    int getIndCount();
    

private:
    glm::vec3 calculateNormals(float x,float y,float z);
    void createIndices(int stackCount,int sectorCount);
    void createVertices(float radius, int sectorCount, int stackCount);
};

sphere::sphere() {
    this->radius = 1.5f;
    this->sectorCount = 36;
    this->stackCount = 18;

    createVertices(this->radius, this->sectorCount, this->stackCount);
    createIndices(stackCount, sectorCount);
}
sphere::sphere(float radius,int sectorCount,int stackCount) {

    this->radius = radius;
    this->sectorCount = sectorCount;
    this->stackCount = stackCount;

    createVertices(this->radius, this->sectorCount, this->stackCount);
    createIndices(stackCount, sectorCount);

   
}
glm::vec3 sphere::calculateNormals(float x, float y, float z) {
    glm::vec3 gradF=glm::vec3(2 * x, 2 * y, 2 * z);
    float length = 2 * sqrtf(powf(x,2.0) + powf(y,2.0) + powf(z,2.0));

    return gradF / length;
}

void sphere::createVertices(float radius, int sectorCount, int stackCount) {
    float x, y, z, temp;
    std::vector<float> normals;
    float s, t;
    float sectorStep = 2 * (float)M_PI / sectorCount;
    float stackStep = (float)M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; i++) {
        stackAngle = (float)M_PI / 2 - (float)i * stackStep;

        temp = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; j++) {
            sectorAngle = j * sectorStep;

            //vertex position;
            x = temp * cosf(sectorAngle);
            y = temp * sinf(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            glm::vec3 normals = calculateNormals(x, y, z);
            vertices.push_back(normals.x);
            vertices.push_back(normals.y);
            vertices.push_back(normals.z);

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }
}


void sphere::createIndices(int stackCount, int sectorCount) {
    int k1, k2;
    
    for (int i = 0; i < stackCount; i++) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; j++,k1++,k2++) {
            indices.push_back(k1);
            indices.push_back(k2);
            if (i == 0) {
                indices.push_back(k2 + 1);
            }else if (i == stackCount - 1) {
                indices.push_back(k1 + 1);
            }
            else{
                indices.push_back(k1 + 1);
                indices.push_back(k1 + 1);
                indices.push_back(k2 + 1);
                indices.push_back(k2);
            }
        }
    }

}
std::size_t sphere::getVertSize() {
    return(sizeof(float) * vertices.size());
}

std::size_t sphere::getIndSize() {
    return(sizeof(int) * indices.size());
}
std::size_t sphere::getStride() {
    return(8 * sizeof(float));
}
float* sphere::getVertices() {
    return(&vertices[0]);
}
int* sphere::getIndices() {
    return(&indices[0]);
}
int sphere::getIndCount() {
    return(indices.size());
}