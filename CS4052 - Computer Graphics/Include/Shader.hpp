/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

Wrapper class for GLSL shader programs.
*/
#pragma once
#include "OpenGL.hpp"

#include <string>


class Shader
{
	public:
		enum class Type : unsigned char
		{
			VERTEX = 0,
			FRAGMENT,
			GEOMETRY,
			TESSCTRL,
			TESSEVAL
		};

		// Constructor:
		Shader();

		// Destructor:
		~Shader();

		// Basic load function:
		bool loadFromFile(const std::string& p_vert_src,
						  const std::string& p_frag_src);

		// Load with geometry shader:
		bool loadFromFile(const std::string& p_vert_src,
						  const std::string& p_frag_src,
						  const std::string& p_geom_src);

		// Load with tesselation shaders:
		bool loadFromFile(const std::string& p_vert_src,
						  const std::string& p_frag_src,
						  const std::string& p_tesc_src,
						  const std::string& p_tese_src);

		// Complete load function:
		bool loadFromFile(const std::string& p_vert_src,
						  const std::string& p_frag_src,
						  const std::string& p_geom_src,
						  const std::string& p_tesc_src,
						  const std::string& p_tese_src);

		// Bind function:
		bool asActiveShader();

		// Unload function:
		bool unLoad();

		// Getter function:
		GLuint getRawShaderHandle(Type p_shader_type);
		GLuint getRawProgramHandle();
	private:
		// Shader handle data:
		GLuint m_vert_shader,
			   m_frag_shader,
			   m_geom_shader,
			   m_tesc_shader,
			   m_tese_shader,
			   m_shader_program;

		bool m_was_destroyed;

		// Internal load function:
		bool loadComponentFromFile(Type p_shader_type, const std::string& p_shader_src);
		bool loadComponentFromString(Type p_shader_type, const char* p_shader_str);
};
