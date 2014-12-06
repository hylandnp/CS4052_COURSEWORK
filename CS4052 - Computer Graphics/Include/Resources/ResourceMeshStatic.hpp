/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include "Resources/ResourceBase.hpp"
#include <cstddef>
#include <string>


// Forward declaration(s):
class btCollisionShape;
class btTriangleMesh;

/*
Simple mesh class.
This class represents a 'static' (i.e. non-animated) OpenGL mesh object.

Attributes sent to vertex shader are in the following order:
0 = vertex point
1 = normals
2 = uv coords
*/
class ResourceMeshStatic : public ResourceBase
{
public:
	// Constructor:
	ResourceMeshStatic();

	// Load function(s):
	bool loadFromFile(const std::string& p_file_src, bool p_static_physics = true);

	// Inherited disposal function:
	void unLoad();

	// Bind function:
	bool setActive(bool p_vao = true, bool p_vbo = true);

	// Getter(s):
	std::size_t getVertexCount();
	unsigned int getVertexArrayHandle();
	unsigned int getVertexBufferHandle();
	btTriangleMesh* getPhysicsMeshObject();
	btCollisionShape* getCollisionMeshObject();
	bool isStaticPhysicsObject();
private:
	std::size_t m_vertex_count;
	unsigned int m_vao_handle,
				 m_vbo_handle;

	// Physics collision mesh shape:
	btTriangleMesh* m_physics_triangle_mesh;
	btCollisionShape* m_collision_mesh;
	bool m_static_physics_object;
};
