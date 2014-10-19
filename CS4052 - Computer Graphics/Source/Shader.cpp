/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Wrapper class for GLSL shader programs.
*/
#include "Shader.hpp"
#include "Log.hpp"

#include <fstream>
#include <string>


Shader::Shader() :
	m_vert_shader(0),
	m_frag_shader(0),
	m_geom_shader(0),
	m_tesc_shader(0),
	m_tese_shader(0),
	m_shader_program(0),
	m_was_destroyed(false)
{
	// default initialisation...
}


Shader::~Shader()
{
	if (!m_was_destroyed) this->unLoad();
}


bool Shader::loadFromFile(const std::string& p_vert_src,
						  const std::string& p_frag_src)
{
	// Load just the required vertex and fragment shaders:
	return this->loadFromFile(p_vert_src,
							  p_frag_src,
							  std::string(),
							  std::string(),
							  std::string());
}


bool Shader::loadFromFile(const std::string& p_vert_src,
						  const std::string& p_frag_src,
						  const std::string& p_geom_src)
{
	return this->loadFromFile(p_vert_src,
							  p_frag_src,
							  p_geom_src,
							  std::string(),
							  std::string());
}


bool Shader::loadFromFile(const std::string& p_vert_src,
						  const std::string& p_frag_src,
						  const std::string& p_tesc_src,
						  const std::string& p_tese_src)
{
	return this->loadFromFile(p_vert_src,
							  p_frag_src,
							  std::string(),
							  p_tesc_src,
							  p_tese_src);
}


bool Shader::loadFromFile(const std::string& p_vert_src,
						  const std::string& p_frag_src,
						  const std::string& p_geom_src,
						  const std::string& p_tesc_src,
						  const std::string& p_tese_src)
{
	// Dispose of previous shaders:
	if (!this->unLoad())
	{
		Log::getInstance().writeError("Shader.cpp",
									  __LINE__,
									  "\tFailed to dispose of previously-active shaders!\n\n");
		return false;
	}

	m_shader_program = glCreateProgram();

	// Load required vertex shader components:
	if (p_vert_src.empty() ||
		p_vert_src.compare("") == 0 ||
		!loadComponentFromFile(Type::VERTEX, p_vert_src))
	{
		Log::getInstance().writeError("Shader.cpp",
									  __LINE__,
									  "\tMissing vertex shader component!\n\n");
		return false;
	}
	else
	{
		Log::getInstance().writeMessage("Loaded vertex shader component from file: '%s'.\n",
										p_vert_src.c_str());
		glAttachShader(m_shader_program, m_vert_shader);
	}

	// Load required fragment shader components:
	if (p_frag_src.empty() ||
		p_frag_src.compare("") == 0 ||
		!loadComponentFromFile(Type::FRAGMENT, p_frag_src))
	{
		Log::getInstance().writeError("Shader.cpp",
									  __LINE__,
									  "\tMissing fragment shader component!\n\n");
		return false;
	}
	else
	{
		Log::getInstance().writeMessage("Loaded fragment shader component from file: '%s'.\n",
										p_frag_src.c_str());
		glAttachShader(m_shader_program, m_frag_shader);
	}

	// Load optional geometry shader component:
	if (!p_geom_src.empty() && p_geom_src.compare("") != 0 && loadComponentFromFile(Type::GEOMETRY, p_geom_src))
	{
		// Attach program only if loaded:
		Log::getInstance().writeMessage("Loading optional geometry shader component from file: '%s'.\n",
										p_geom_src.c_str());
		glAttachShader(m_shader_program, m_geom_shader);
	}

	// Load optional tesselation shader components:
	if (!p_tese_src.empty() && p_tese_src.compare("") != 0 && loadComponentFromFile(Type::TESSEVAL, p_tese_src) &&
		!p_tesc_src.empty() && p_tesc_src.compare("") != 0 && loadComponentFromFile(Type::TESSCTRL, p_tesc_src))
	{
		// Attach program only if both tesselation components loaded:
		Log::getInstance().writeMessage("Loading optional tesselation shader components from file: '%s' and '%s'.\n",
										p_tesc_src.c_str(),
										p_tese_src.c_str());

		glAttachShader(m_shader_program, m_tese_shader);
		glAttachShader(m_shader_program, m_tesc_shader);
	}

	// Link program and check for errors:
	glLinkProgram(m_shader_program);
	return (!hasOpenGLErrors("Shader.cpp", __LINE__) &&
			isValidShaderProgram(m_shader_program, "Shader.cpp", __LINE__));
}


bool Shader::asActiveShader()
{
	if (glIsProgram(m_shader_program))
	{
		glUseProgram(m_shader_program);
		return true;
	}
	else
	{
		Log::getInstance().writeError("Window.cpp",
									  __LINE__,
									  "\tFailed to activate/use shader program: %u!\n\n",
									  m_shader_program);
		glUseProgram(0);
		return false;
	}
}


bool Shader::unLoad()
{
	if (glIsProgram(m_shader_program))
	{
		// Detach shader objects from program and delete them:
		if (glIsShader(m_vert_shader))
		{
			glDetachShader(m_shader_program, m_vert_shader);
			glDeleteShader(m_vert_shader);
		}
		
		if (glIsShader(m_frag_shader))
		{
			glDetachShader(m_shader_program, m_frag_shader);
			glDeleteShader(m_frag_shader);
		}

		if (glIsShader(m_geom_shader))
		{
			glDetachShader(m_shader_program, m_geom_shader);
			glDeleteShader(m_geom_shader);
		}

		if (glIsShader(m_tesc_shader))
		{
			glDetachShader(m_shader_program, m_tesc_shader);
			glDeleteShader(m_tesc_shader);
		}

		if (glIsShader(m_tese_shader))
		{
			glDetachShader(m_shader_program, m_tese_shader);
			glDeleteShader(m_tese_shader);
		}

		// Dispose of the shader program:
		glDeleteProgram(m_shader_program);
	}

	m_shader_program = 0;
	m_vert_shader = 0;
	m_frag_shader = 0;
	m_geom_shader = 0;
	m_tesc_shader = 0;
	m_tese_shader = 0;

	m_was_destroyed = true;
	return !hasOpenGLErrors("Shader.cpp", __LINE__);
}


bool Shader::setAttribute(const std::string& p_str_id, const glm::vec3& p_value)
{
	GLuint uniform_id = glGetUniformLocation(m_shader_program, p_str_id.c_str());

	if (uniform_id < 0)
	{
		Log::getInstance().writeError("Shader.cpp",
									  __LINE__,
									  "\tInvalid uniform name string passed to shader function: '%s'!\n\n",
									  p_str_id.c_str());
		return false; // no valid uniform
	}
	else
	{
		glUniform3fv(uniform_id, 1, glm::value_ptr(p_value));
		return !hasOpenGLErrors("Shader.cpp", __LINE__);
	}
}


bool Shader::setAttribute(const std::string& p_str_id, const glm::vec4& p_value)
{
	GLuint uniform_id = glGetUniformLocation(m_shader_program, p_str_id.c_str());

	if (uniform_id < 0)
	{
		Log::getInstance().writeError("Shader.cpp",
									  __LINE__,
									  "\tInvalid uniform name string passed to shader function: '%s'!\n\n",
									  p_str_id.c_str());
		return false; // no valid uniform
	}
	else
	{
		glUniform4fv(uniform_id, 1, glm::value_ptr(p_value));
		return !hasOpenGLErrors("Shader.cpp", __LINE__);
	}
}


bool Shader::setAttribute(const std::string& p_str_id, const glm::mat4& p_value, bool p_transpose)
{
	GLuint uniform_id = glGetUniformLocation(m_shader_program, p_str_id.c_str());

	if (uniform_id < 0)
	{
		Log::getInstance().writeError("Shader.cpp",
									  __LINE__,
									  "\tInvalid uniform name string passed to shader function: '%s'!\n\n",
									  p_str_id.c_str());
		return false; // no valid uniform
	}
	else
	{
		glUniformMatrix4fv(uniform_id, 1, ((p_transpose) ? GL_TRUE : GL_FALSE), glm::value_ptr(p_value));
		return !hasOpenGLErrors("Shader.cpp", __LINE__);
	}
}


GLuint Shader::getRawShaderHandle(Type p_shader_type)
{
	// Return the raw OpenGL integer handle of the given shader type:
	switch (p_shader_type)
	{
		case Type::VERTEX:
		{
			return m_vert_shader;
		}
		case Type::FRAGMENT:
		{
			return m_frag_shader;
		}
		case Type::GEOMETRY:
		{
			return m_geom_shader;
		}
		case Type::TESSEVAL:
		{
			return m_tese_shader;
		}
		case Type::TESSCTRL:
		{
			return m_tesc_shader;
		}
		default:
		{
			return 0;
		}
	}
}


GLuint Shader::getRawProgramHandle()
{
	return m_shader_program;
}


bool Shader::loadComponentFromFile(Type p_shader_type, const std::string& p_shader_src)
{
	std::ifstream file_input(p_shader_src);
	std::string current_data;

	if (file_input.is_open() && file_input.good() && !file_input.bad())
	{
		// Load string data from file:
		file_input.seekg(std::ios::end);
		current_data.reserve(static_cast<std::size_t>(file_input.tellg()));
		file_input.seekg(std::ios::beg);

		current_data.assign((std::istreambuf_iterator<char>(file_input)),
							std::istreambuf_iterator<char>());

		// Now load shader from string data in memory:
		file_input.close();
		return loadComponentFromString(p_shader_type, current_data.c_str());
	}
	else
	{
		Log::getInstance().writeError("Shader.cpp",
									  __LINE__,
									  "\tFailed to load GLSL shader file: '%s', may be missing or corrupted!\n\n",
									  p_shader_src.c_str());
		file_input.close(); // force stream closure
		return false;
	}
}


bool Shader::loadComponentFromString(Type p_shader_type, const char* p_shader_str)
{
	// Create shader type (TODO - optional shaders):
	switch (p_shader_type)
	{
		case Type::VERTEX:
		{
			m_vert_shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(m_vert_shader, 1, &p_shader_str, NULL);
			glCompileShader(m_vert_shader);

			if (!isValidShaderComponent(m_vert_shader, "Shader.cpp", __LINE__)) return false;
			break;
		}
		case Type::FRAGMENT:
		{
			m_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(m_frag_shader, 1, &p_shader_str, NULL);
			glCompileShader(m_frag_shader);

			if (!isValidShaderComponent(m_frag_shader, "Shader.cpp", __LINE__)) return false;
			break;
		}
		case Type::GEOMETRY:
		{
			m_geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(m_geom_shader, 1, &p_shader_str, NULL);
			glCompileShader(m_geom_shader);

			if (!isValidShaderComponent(m_geom_shader, "Shader.cpp", __LINE__)) return false;
			break;
		}
		case Type::TESSCTRL:
		{
			m_tesc_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(m_tesc_shader, 1, &p_shader_str, NULL);
			glCompileShader(m_tesc_shader);

			if (!isValidShaderComponent(m_tesc_shader, "Shader.cpp", __LINE__)) return false;
			break;
		}
		case Type::TESSEVAL:
		{
			m_tese_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(m_tese_shader, 1, &p_shader_str, NULL);
			glCompileShader(m_tese_shader);

			if (!isValidShaderComponent(m_tese_shader, "Shader.cpp", __LINE__)) return false;
			break;
		}
	}

	return !hasOpenGLErrors("Shader.cpp", __LINE__);
}
