/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include "Resources/ResourceBase.hpp"
#include <string>
#include <glm/fwd.hpp>

// Forward declaration(s):
class ResourceTexture2D;


/*
GLSL shader class.
This encapsulates a set of OpenGL/GLSL shaders and their shader program.
*/
class ResourceShader : public ResourceBase
{
public:
	// Constructor:
	ResourceShader();

	// Load from string function(s):
	bool loadFromString(const char* p_vert_str,
						const char* p_frag_str,
						const char* p_geom_str = nullptr,
						const char* p_tese_str = nullptr,
						const char* p_tesc_str = nullptr);

	// Load from file function(s):
	bool loadFromFile(const std::string& p_vert_src,
					  const std::string& p_frag_src,
					  const std::string& p_geom_src = std::string(),
					  const std::string& p_tese_src = std::string(),
					  const std::string& p_tesc_src = std::string());

	// Inherited disposal function:
	void unLoad();

	// Bind function:
	bool setActive(bool p_active = true);

	// Utility function(s):
	int getAttributeLocation(const std::string& p_attrib_str);
	int getUniformLocation(const std::string& p_uniform_str);

	// Attribute/uniform function(s):
	bool setUniformAttribute(const std::string& p_uniform_str,
							 const glm::vec2& p_uniform_data);
	bool setUniformAttribute(const std::string& p_uniform_str,
							 const glm::vec3& p_uniform_data);
	bool setUniformAttribute(const std::string& p_uniform_str,
							 const glm::vec4& p_uniform_data);
	bool setUniformAttribute(const std::string& p_uniform_str, 
							 const glm::mat2& p_uniform_data);
	bool setUniformAttribute(const std::string& p_uniform_str,
							 const glm::mat3& p_uniform_data);
	bool setUniformAttribute(const std::string& p_uniform_str,
							 const glm::mat4& p_uniform_data);
	bool setUniformAttribute(const std::string& p_uniform_str,
							 ResourceTexture2D& p_uniform_data);

	// Getter function(s):
	unsigned int getRawShaderProgramHandle();
	unsigned int getVertexShader();
	unsigned int getFragmentShader();
	unsigned int getGeometryShader();
	unsigned int getTesselationControlShader();
	unsigned int getTesselationEvaluationShader();
private:
	unsigned int m_program_handle,
				 m_vert_shader,
				 m_frag_shader,
				 m_geom_shader,
				 m_tese_shader,
				 m_tesc_shader;
};
