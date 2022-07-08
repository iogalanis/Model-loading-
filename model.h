#pragma once
#include "glm.hpp"
#include <vector>
#include <stdio.h>
#include <glad/glad.h>
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"


/*each node contains 0,x childen and all nodes have a parent except the root node.
Each node may contain multiple meshes,which need processing individually.Each mesh has vertices
indices, and certain materials which act as textures.We recursively process every node,every 
mesh within a node ,get the material textures for each mesh(if it has one) and stop when we are done processing
each node. This way all the information contained in a 3d file is stored in our program via vectors.
For some optimization,some materials are used more than once. We don't have to load every time each material locally
because it is costly. We create a texture vector for the loaded materials and we query each time to check if the 
certain material is already loaded in our program.*/


class model {
public:
	std::string path;
	model(std::string path);

	void draw(Shader& shader);
	void loadModel(std::string path);

private:
	
	std::string directory;
	
	std::vector<mesh> meshes;
	std::vector<Texture> textures_loaded;

	void processNode(aiNode* node,const aiScene *scene);
	mesh processMesh(aiMesh* mesh,const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* material,aiTextureType type);
	unsigned int getTextureFromFile(aiString path);



};

model::model(std::string path) {
	this->path = path;

	loadModel(this->path);
	
	
	

}

void model::draw(Shader& shader) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

void model::loadModel(std::string path) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (scene == nullptr) {
		std::cout << "ERROR::ASSIMP" << importer.GetErrorString() << std::endl;
		return;
	}

	bool tex = scene->HasTextures();

	this->directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

	importer.FreeScene();
}

void model::processNode(aiNode* node,const aiScene *scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]],scene));
	}

	for (unsigned int i= 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i],scene);
	}

}

mesh model::processMesh(aiMesh* m, const aiScene* scene) {
	bool normals;

	std::vector<Vertex> vertices;
	std::vector<unsigned int>indices;
	std::vector<Texture>diffuses;
	std::vector<Texture>speculars;
	std::vector<Texture>textures;
	

	normals = m->HasNormals();
	if(normals)
		for (unsigned int i= 0; i < m->mNumVertices; i++) {
			Vertex vertex;
			vertex.Position = glm::vec3(m->mVertices[i].x,m->mVertices[i].y,m->mVertices[i].z);
			vertex.Normal = glm::vec3(m->mNormals[i].x, m->mNormals[i].y, m->mNormals[i].z);
			vertex.TexCoords = glm::vec2(m->mTextureCoords[0][i].x, m->mTextureCoords[0][i].y);
			vertices.push_back(vertex);
		}
	else {
		std::cout << "process::mesh::" << " this mesh doesnt contain normals or texCoords" << std::endl;
	}

	for (unsigned int i = 0; i < m->mNumFaces; i++) {
		for(unsigned int j=0;j<m->mFaces[i].mNumIndices;j++)
			indices.push_back(m->mFaces[i].mIndices[j]);

	}

	if(m->mMaterialIndex>=0){
		aiMaterial* meshMaterial = scene->mMaterials[m->mMaterialIndex];

		
		diffuses = loadMaterialTextures(meshMaterial, aiTextureType_DIFFUSE);
		speculars = loadMaterialTextures(meshMaterial, aiTextureType_SPECULAR);

		textures.insert(textures.end(), diffuses.begin(), diffuses.end());
		textures.insert(textures.end(), speculars.begin(), speculars.end());
	}
	
	return(mesh(vertices, indices, textures));

}


std::vector<Texture> model::loadMaterialTextures(aiMaterial* material,aiTextureType type) {
	Texture texture;
	std::vector<Texture>matTextures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString path;

		material->GetTexture(type, i, &path);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.c_str(), path.C_Str()) == 0) {
				matTextures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {

			texture.id = getTextureFromFile(path);
			if (type == aiTextureType_DIFFUSE)
				texture.type = "texture_diffuse";
			else if (type == aiTextureType_SPECULAR)
				texture.type = "texture_specular";

			texture.path = path.C_Str();

			matTextures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return matTextures;

}

unsigned int model::getTextureFromFile(aiString path) {
	std::string filePath;
	filePath = directory + '/' + path.C_Str();

	stbi_set_flip_vertically_on_load(true);

	unsigned int texID;
	glGenTextures(1, &texID);

	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texID);

	//texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(&filePath[0], &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		//texture target,mipmap level,store rgb values,dimensions,always zero (legacy), format and data,actual data
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::STBI_LOAD::FAILED" << std::endl;
		return(0);
	}

	free(data);

	return(texID);

}


