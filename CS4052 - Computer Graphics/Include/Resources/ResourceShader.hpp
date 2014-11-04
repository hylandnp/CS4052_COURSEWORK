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


class ResourceShader : public ResourceBase
{
public:
	// Constructor:
	ResourceShader();

	// Load from string function(s):
	bool loadFromString(const std::string& p_vert_str,
						const std::string& p_frag_str,
						const std::string& p_geom_str,
						const std::string& p_tese_str,
						const std::string& p_tesc_str);
	bool loadFromString(const std::string& p_vert_str,
						const std::string& p_frag_str,
						const std::string& p_tese_str,
						const std::string& p_tesc_str);
	bool loadFromString(const std::string& p_vert_str,
						const std::string& p_frag_str,
						const std::string& p_geom_str);
	bool loadFromString(const std::string& p_vert_str,
						const std::string& p_frag_str);

	// Load from file function(s):
	bool loadFromFile(const std::string& p_vert_src,
					  const std::string& p_frag_src,
					  const std::string& p_geom_src,
					  const std::string& p_tese_src,
					  const std::string& p_tesc_src);
	bool loadFromFile(const std::string& p_vert_src,
					  const std::string& p_frag_src,
					  const std::string& p_tese_src,
					  const std::string& p_tesc_src);
	bool loadFromFile(const std::string& p_vert_src,
					  const std::string& p_frag_src,
					  const std::string& p_geom_src);
	bool loadFromFile(const std::string& p_vert_src,
					  const std::string& p_frag_src);

	// Bind function:
	bool setActive(bool p_active = true);

	// Attribute/uniform function(s):
	bool setAttribute(const std::string& p_uniform_str,
					  const glm::vec2& p_uniform_data);
	bool setAttribute(const std::string& p_uniform_str,
					  const glm::vec3& p_uniform_data);
	bool setAttribute(const std::string& p_uniform_str,
					  const glm::vec4& p_uniform_data);
	bool setAttribute(const std::string& p_uniform_str, 
					  const glm::mat2& p_uniform_data);
	bool setAttribute(const std::string& p_uniform_str,
					  const glm::mat3& p_uniform_data);
	bool setAttribute(const std::string& p_uniform_str,
					  const glm::mat4& p_uniform_data);
	bool setAttribute(const std::string& p_uniform_str,
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
