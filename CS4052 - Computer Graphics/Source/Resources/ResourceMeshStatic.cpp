/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Loading code adapted from ASSIMP examples given.
*/
#include "Resources/ResourceMeshStatic.hpp"
#include "Utility/Logger.hpp"
#include "Utility/OpenGLErrors.hpp"

#include <GL/glew.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <cstdlib>


ResourceMeshStatic::ResourceMeshStatic() :
	m_vertex_count(0),
	m_vao_handle(0),
	m_vbo_handle(0)
{
	// Default initialisation...
	ResourceBase::ResourceBase(ResourceType::MESH_STATIC);
}


bool ResourceMeshStatic::loadFromFile(const std::string& p_file_src)
{
	this->unLoad(); // dispose of previous instance

	// Import from 3D file:
	auto import_scene = aiImportFile(p_file_src.c_str(),
									 aiProcessPreset_TargetRealtime_Fast |
									 aiProcess_JoinIdenticalVertices |
									 aiProcess_OptimizeMeshes |
									 aiProcess_Triangulate |
									 aiProcess_GenUVCoords |
									 aiProcess_GenNormals);

	if (!import_scene)
	{
		Logger::getInstance().writeError("ResourceMeshStatic.cpp",
										 "loadFromFile()",
										 __LINE__,
										 "Failed to load/import 3D objects, message: '%s'!",
										 aiGetErrorString());
		return false;
	}
	
	// Extract single (i.e. first) mesh found and create vertex arrays/buffers:
	auto import_mesh = import_scene->mMeshes[0];
	m_vertex_count = import_mesh->mNumFaces * 3;

	float* attributes = new float[sizeof(float) * 8 * m_vertex_count]; // temporary vertex data
	GLuint count = 0; // increment by 8 for each new vertex point

	for (std::size_t i = 0; i < import_mesh->mNumFaces; i++)
	{
		auto& import_face = import_mesh->mFaces[i];

		// For each face (set of vertices), get the vertex points, UVs and normals:
		for (std::size_t j = 0; j < 3; j++)
		{
			aiVector3D import_vertex = import_mesh->mVertices[import_face.mIndices[j]];
			aiVector3D import_normals = import_mesh->mNormals[import_face.mIndices[j]];
			aiVector3D import_uvs = ((import_mesh->HasTextureCoords(0)) ? import_mesh->mTextureCoords[0][import_face.mIndices[j]] : aiVector3D(0.f));

			// Store vertices:
			attributes[count] = import_vertex.x;
			attributes[count + 1] = import_vertex.y;
			attributes[count + 2] = import_vertex.z;

			// Store normals:
			attributes[count + 3] = import_normals.x;
			attributes[count + 4] = import_normals.y;
			attributes[count + 5] = import_normals.z;

			// Store UV-texcoords:
			attributes[count + 6] = import_uvs.x;
			attributes[count + 7] = import_uvs.y;

			count += 8;
		}
	}

	// Create vertex array object:
	glGenVertexArrays(1, &m_vao_handle);
	glBindVertexArray(m_vao_handle);

	// Create vertex buffer object:
	glGenBuffers(1, &m_vbo_handle);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, attributes, GL_STATIC_DRAW);

	// Add vertices to buffer:
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)0);

	// Add normals to buffer:
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));

	// Add UV-texcoords to buffer:
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	// Release read-only data and return from function:
	delete[] attributes;
	aiReleaseImport(import_scene);
	glBindVertexArray(0);

	if (checkForOpenGLErrors("ResourceMeshState.cpp", "loadFromFile()", __LINE__)) return false;

	Logger::getInstance().write("Loaded static/simple mesh from file: '%s' successfully!",
								p_file_src.c_str());
	return (m_is_loaded = true);
}


void ResourceMeshStatic::unLoad()
{
	if (glIsBuffer(m_vbo_handle) == GL_TRUE) glDeleteBuffers(1, &m_vbo_handle);
	if (glIsVertexArray(m_vao_handle) == GL_TRUE) glDeleteVertexArrays(1, &m_vao_handle);

	m_vertex_count = 0;
	m_vao_handle = 0;
	m_vbo_handle = 0;

	ResourceBase::unLoad();
}


bool ResourceMeshStatic::setActive(bool p_vao, bool p_vbo)
{
	if (p_vao && glIsVertexArray(m_vao_handle) == GL_TRUE) glBindVertexArray(m_vao_handle);
	if (p_vbo && glIsBuffer(m_vbo_handle) == GL_TRUE) glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handle);
	return !checkForOpenGLErrors("ResourceMeshStatic.cpp", "setActive()", __LINE__);
}


std::size_t ResourceMeshStatic::getVertexCount()
{
	return m_vertex_count;
}


unsigned int ResourceMeshStatic::getVertexArrayHandle()
{
	return m_vao_handle;
}


unsigned int ResourceMeshStatic::getVertexBufferHandle()
{
	return m_vbo_handle;
}
