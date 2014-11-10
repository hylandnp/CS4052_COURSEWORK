/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Resources/ResourceShader.hpp"
#include "Resources/ResourceImage.hpp"
#include "Resources/ResourceTexture2D.hpp"
#include "Utility/OpenGLErrors.hpp"
#include "Utility/Logger.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <fstream>
#include <cstring>


ResourceShader::ResourceShader() :
	m_program_handle(0),
	m_vert_shader(0),
	m_frag_shader(0),
	m_geom_shader(0),
	m_tese_shader(0),
	m_tesc_shader(0)
{
	// Default initialisation...
	ResourceBase::ResourceBase(ResourceType::SHADER);
}


bool ResourceShader::loadFromString(const char* p_vert_str,
									const char* p_frag_str,
									const char* p_geom_str,
									const char* p_tese_str,
									const char* p_tesc_str)
{
	this->unLoad(); // dispose of previous data
	m_program_handle = glCreateProgram();

	// Vertex shader (required):
	m_vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vert_shader, 1, &p_vert_str, NULL);
	glCompileShader(m_vert_shader);

	if (!checkShaderComponent(m_vert_shader, "ResourceShader.cpp", "loadFromString()", __LINE__)) return false;
	glAttachShader(m_program_handle, m_vert_shader);

	// Fragment shader (required):
	m_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_frag_shader, 1, &p_frag_str, NULL);
	glCompileShader(m_frag_shader);

	if (!checkShaderComponent(m_frag_shader, "ResourceShader.cpp", "loadFromString()", __LINE__)) return false;
	glAttachShader(m_program_handle, m_frag_shader);

	// Geometry shader (optional):
	if (p_geom_str && strcmp(p_geom_str, "") != 0)
	{
		m_geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(m_geom_shader, 1, &p_geom_str, NULL);
		glCompileShader(m_geom_shader);

		if (!checkShaderComponent(m_geom_shader, "ResourceShader.cpp", "loadFromString()", __LINE__)) return false;
		glAttachShader(m_program_handle, m_geom_shader);
	}

	// Tesselation shaders (optional):
	if (p_tese_str &&
		strcmp(p_tese_str, "") != 0 &&
		p_tesc_str &&
		strcmp(p_tesc_str, "") != 0)
	{
		// Tesselation evaluation shader:
		m_tese_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(m_tese_shader, 1, &p_tese_str, NULL);
		glCompileShader(m_tese_shader);

		if (!checkShaderComponent(m_tese_shader, "ResourceShader.cpp", "loadFromString()", __LINE__)) return false;
		glAttachShader(m_program_handle, m_tese_shader);

		// Tesselation control shader:
		m_tesc_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(m_tesc_shader, 1, &p_tesc_str, NULL);
		glCompileShader(m_tesc_shader);

		if (!checkShaderComponent(m_tesc_shader, "ResourceShader.cpp", "loadFromString()", __LINE__)) return false;
		glAttachShader(m_program_handle, m_tesc_shader);
	}

	// Link into shader program and return from function:
	glLinkProgram(m_program_handle);
	if (checkForOpenGLErrors("ResourceShader.cpp", "loadFromString()", __LINE__)) return false;
	return (m_is_loaded = true);
}


bool ResourceShader::loadFromFile(const std::string& p_vert_src,
								  const std::string& p_frag_src,
								  const std::string& p_geom_src,
								  const std::string& p_tese_src,
								  const std::string& p_tesc_src)
{
	// Load each shader component from file into their respective strings:
	std::string vert_str,
				frag_str,
				geom_str,
				tese_str,
				tesc_str;

	// Vertex shader (required):
	std::ifstream input(p_vert_src, std::ios::in);

	if (input.is_open() && input.good() && !input.bad())
	{
		vert_str.assign((std::istreambuf_iterator<char>(input)),
						std::istreambuf_iterator<char>());
		input.close();
		input.clear();

		Logger::getInstance().write("Loaded vertex shader component: '%s' successfully.", p_vert_src.c_str());
	}
	else
	{
		input.close(); // force closure
		Logger::getInstance().writeError("ResourceShader.cpp",
										 "loadFromFile()",
										 __LINE__,
										 "Failed to load vertex shader from file: '%s', may be missing or corrupted!",
										 p_vert_src.c_str());
		return false;
	}
	
	// Fragment shader (required):
	input.open(p_frag_src, std::ios::in);

	if (input.is_open() && input.good() && !input.bad())
	{
		frag_str.assign((std::istreambuf_iterator<char>(input)),
						std::istreambuf_iterator<char>());
		input.close();
		input.clear();

		Logger::getInstance().write("Loaded fragment shader component: '%s' successfully.", p_frag_src.c_str());
	}
	else
	{
		input.close(); // force closure
		Logger::getInstance().writeError("ResourceShader.cpp",
										 "loadFromFile()",
										 __LINE__,
										 "Failed to load fragment shader from file: '%s', may be missing or corrupted!",
										 p_frag_src.c_str());
		return false;
	}

	// Geometry shader (optional):
	if (!p_geom_src.empty() && p_geom_src.compare("") != 0)
	{
		Logger::getInstance().write("Loading optional geometry shader...");
		input.open(p_geom_src, std::ios::in);

		if (input.is_open() && input.good() && !input.bad())
		{
			geom_str.assign((std::istreambuf_iterator<char>(input)),
							std::istreambuf_iterator<char>());
			input.close();
			input.clear();

			Logger::getInstance().write("Loaded geometry shader component: '%s' successfully.", p_geom_src.c_str());
		}
		else
		{
			input.close(); // force closure
			Logger::getInstance().writeError("ResourceShader.cpp",
											 "loadFromFile()",
											 __LINE__,
											 "Failed to load geometry shader from file: '%s', may be missing or corrupted!",
											 p_geom_src.c_str());
			return false;
		}
	}

	// Tesselation shaders (optional):
	if (!p_tese_src.empty() &&
		p_tese_src.compare("") != 0 &&
		!p_tesc_src.empty() &&
		p_tesc_src.compare("") != 0)
	{
		Logger::getInstance().write("Loading optional tesselation shaders...");

		// Tesselation evaluation shader:
		input.open(p_tese_src, std::ios::in);

		if (input.is_open() && input.good() && !input.bad())
		{
			tese_str.assign((std::istreambuf_iterator<char>(input)),
							std::istreambuf_iterator<char>());
			input.close();
			input.clear();

			Logger::getInstance().write("Loaded tesselation evaluation shader component: '%s' successfully.", p_tese_src.c_str());
		}
		else
		{
			input.close(); // force closure
			Logger::getInstance().writeError("ResourceShader.cpp",
											 "loadFromFile()",
											 __LINE__,
											 "Failed to load tesselation evaluation shader from file: '%s', may be missing or corrupted!",
											 p_tese_src.c_str());
			return false;
		}

		// Tesselation control shader:
		input.open(p_tesc_src, std::ios::in);

		if (input.is_open() && input.good() && !input.bad())
		{
			tesc_str.assign((std::istreambuf_iterator<char>(input)),
							std::istreambuf_iterator<char>());
			input.close();
			input.clear();

			Logger::getInstance().write("Loaded tesselation control shader component: '%s' successfully.", p_tesc_src.c_str());
		}
		else
		{
			input.close(); // force closure
			Logger::getInstance().writeError("ResourceShader.cpp",
											 "loadFromFile()",
											 __LINE__,
											 "Failed to load tesselation control shader from file: '%s', may be missing or corrupted!",
											 p_tesc_src.c_str());
			return false;
		}
	}

	// Now parse & compile the loaded shader strings:
	return loadFromString(vert_str.c_str(),
						  frag_str.c_str(),
						  geom_str.c_str(),
						  tese_str.c_str(),
						  tesc_str.c_str());
}


void ResourceShader::unLoad()
{
	if (glIsProgram(m_program_handle) == GL_TRUE)
	{
		// Detach all linked shader components:
		if (glIsShader(m_vert_shader) == GL_TRUE) glDetachShader(m_program_handle, m_vert_shader);
		if (glIsShader(m_frag_shader) == GL_TRUE) glDetachShader(m_program_handle, m_frag_shader);
		if (glIsShader(m_geom_shader) == GL_TRUE) glDetachShader(m_program_handle, m_geom_shader);
		if (glIsShader(m_tese_shader) == GL_TRUE) glDetachShader(m_program_handle, m_tese_shader);
		if (glIsShader(m_tesc_shader) == GL_TRUE) glDetachShader(m_program_handle, m_tesc_shader);
	}

	// Delete all compiled shader components:
	if (glIsShader(m_vert_shader) == GL_TRUE) glDeleteShader(m_vert_shader);
	if (glIsShader(m_frag_shader) == GL_TRUE) glDeleteShader(m_frag_shader);
	if (glIsShader(m_geom_shader) == GL_TRUE) glDeleteShader(m_geom_shader);
	if (glIsShader(m_tese_shader) == GL_TRUE) glDeleteShader(m_tese_shader);
	if (glIsShader(m_tesc_shader) == GL_TRUE) glDeleteShader(m_tesc_shader);

	// Reset handles:
	m_program_handle = 0;
	m_vert_shader = 0;
	m_frag_shader = 0;
	m_geom_shader = 0;
	m_tese_shader = 0;
	m_tesc_shader = 0;

	ResourceBase::unLoad();
}


bool ResourceShader::setActive(bool p_active)
{
	if (p_active && glIsProgram(m_program_handle) == GL_TRUE)
	{
		glUseProgram(m_program_handle);
		return true;
	}
	else
	{
		glUseProgram(0);
		return false;
	}
}


int ResourceShader::getAttributeLocation(const std::string& p_attrib_str)
{
	return ((glIsProgram(m_program_handle) == GL_TRUE) ? glGetAttribLocation(m_program_handle, p_attrib_str.c_str()) : -1);
}


int ResourceShader::getUniformLocation(const std::string& p_uniform_str)
{
	return ((glIsProgram(m_program_handle) == GL_TRUE) ? glGetUniformLocation(m_program_handle, p_uniform_str.c_str()) : -1);
}


bool ResourceShader::setUniformAttribute(const std::string& p_uniform_str,
										 const glm::vec2& p_uniform_data)
{
	auto location = this->getUniformLocation(p_uniform_str);

	if (location == -1)
	{
		Logger::getInstance().writeError("ResourceShader.cpp",
										 "setUniformAttribute()",
										 __LINE__,
										 "Failed to locate shader uniform vector: '%s', may be missing or invalid!",
										 p_uniform_str.c_str());
		return false;
	}

	glUniform2fv(location, 1, glm::value_ptr(p_uniform_data));
	return !checkForOpenGLErrors("ResourceShader.cpp", "setUniformAttribute()", __LINE__);
}


bool ResourceShader::setUniformAttribute(const std::string& p_uniform_str,
										 const glm::vec3& p_uniform_data)
{
	auto location = this->getUniformLocation(p_uniform_str);

	if (location == -1)
	{
		Logger::getInstance().writeError("ResourceShader.cpp",
										 "setUniformAttribute()",
										 __LINE__,
										 "Failed to locate shader uniform vector: '%s', may be missing or invalid!",
										 p_uniform_str.c_str());
		return false;
	}

	glUniform3fv(location, 1, glm::value_ptr(p_uniform_data));
	return !checkForOpenGLErrors("ResourceShader.cpp", "setUniformAttribute()", __LINE__);
}



bool ResourceShader::setUniformAttribute(const std::string& p_uniform_str,
										 const glm::vec4& p_uniform_data)
{
	auto location = this->getUniformLocation(p_uniform_str);

	if (location == -1)
	{
		Logger::getInstance().writeError("ResourceShader.cpp",
										 "setUniformAttribute()",
										 __LINE__,
										 "Failed to locate shader uniform vector: '%s', may be missing or invalid!",
										 p_uniform_str.c_str());
		return false;
	}

	glUniform4fv(location, 1, glm::value_ptr(p_uniform_data));
	return !checkForOpenGLErrors("ResourceShader.cpp", "setUniformAttribute()", __LINE__);
}


bool ResourceShader::setUniformAttribute(const std::string& p_uniform_str,
										 const glm::mat2& p_uniform_data)
{
	auto location = this->getUniformLocation(p_uniform_str);

	if (location == -1)
	{
		Logger::getInstance().writeError("ResourceShader.cpp",
										 "setUniformAttribute()",
										 __LINE__,
										 "Failed to locate shader uniform matrix: '%s', may be missing or invalid!",
										 p_uniform_str.c_str());
		return false;
	}

	glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(p_uniform_data));
	return !checkForOpenGLErrors("ResourceShader.cpp", "setUniformAttribute()", __LINE__);
}


bool ResourceShader::setUniformAttribute(const std::string& p_uniform_str,
										 const glm::mat3& p_uniform_data)
{
	auto location = this->getUniformLocation(p_uniform_str);

	if (location == -1)
	{
		Logger::getInstance().writeError("ResourceShader.cpp",
										 "setUniformAttribute()",
										 __LINE__,
										 "Failed to locate shader uniform matrix: '%s', may be missing or invalid!",
										 p_uniform_str.c_str());
		return false;
	}

	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(p_uniform_data));
	return !checkForOpenGLErrors("ResourceShader.cpp", "setUniformAttribute()", __LINE__);
}


bool ResourceShader::setUniformAttribute(const std::string& p_uniform_str,
										 const glm::mat4& p_uniform_data)
{
	auto location = this->getUniformLocation(p_uniform_str);

	if (location == -1)
	{
		Logger::getInstance().writeError("ResourceShader.cpp",
										 "setUniformAttribute()",
										 __LINE__,
										 "Failed to locate shader uniform matrix: '%s', may be missing or invalid!",
										 p_uniform_str.c_str());
		return false;
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(p_uniform_data));
	return !checkForOpenGLErrors("ResourceShader.cpp", "setUniformAttribute()", __LINE__);
}


bool ResourceShader::setUniformAttribute(const std::string& p_uniform_str,
										 ResourceTexture2D& p_uniform_data)
{
	auto location = this->getUniformLocation(p_uniform_str);

	if (location == -1)
	{
		Logger::getInstance().writeError("ResourceShader.cpp",
										 "setUniformAttribute()",
										 __LINE__,
										 "Failed to locate shader uniform texture/sampler: '%s', may be missing or invalid!",
										 p_uniform_str.c_str());
		return false;
	}

	p_uniform_data.setActive(true);
	glUniform1i(location, 0);
	return !checkForOpenGLErrors("ResourceShader.cpp", "setUniformAttribute()", __LINE__);
}


unsigned int ResourceShader::getRawShaderProgramHandle()
{
	return m_program_handle;
}


unsigned int ResourceShader::getVertexShader()
{
	return m_vert_shader;
}


unsigned int ResourceShader::getFragmentShader()
{
	return m_frag_shader;
}


unsigned int ResourceShader::getGeometryShader()
{
	return m_geom_shader;
}


unsigned int ResourceShader::getTesselationControlShader()
{
	return m_tesc_shader;
}


unsigned int ResourceShader::getTesselationEvaluationShader()
{
	return m_tese_shader;
}
