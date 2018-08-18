#include<stdexcept>
#include<iostream>
#include<vector>
#include<string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<ktx.h>


void check(const char * file, int line)//for application error cheking (non GLSL)
{
	GLenum err_code;

	std::string err_str;
	while ((err_code = glGetError()) != GL_NO_ERROR)//read till GL_NO_ERROR is reached
	{
		switch (err_code)
		{
		case GL_INVALID_ENUM:
			err_str = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			err_str = "INVALID_VALUE";
			break;

		case GL_INVALID_OPERATION:
			err_str = "INVALID_OPERATION";
			break;

		case GL_STACK_OVERFLOW:
			err_str = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			err_str = "STACK_UNDERFLOW";
			break;

		case GL_OUT_OF_MEMORY:
			err_str = "OUT_OF_MEMORY";
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			err_str = "INVALID_FRAMEBUFFER_OPERATION";
			break;


		}


		std::cout << err_str << " ERROR:(" << file << "," << line << ") " << std::endl;
	}


}

void check(KTX_error_code err_code, const char *file, int line)//overloaded error check function for KTX Loader functions
{
	std::string err_str;

	if (err_code != KTX_SUCCESS)
	{

		switch (err_code)
		{
		case KTX_FILE_OPEN_FAILED:
			err_str = "FILE_OPEN_FAILED: The target file could not be opened";
			break;


		case KTX_FILE_WRITE_ERROR:
			err_str = "FILE_WRITE_ERROR: An error occurred while writing to the file";
			break;

		case KTX_GL_ERROR:
			err_str = "GL_ERROR: GL operations resulted in an error";
			break;

		case KTX_INVALID_OPERATION:
			err_str = "INVALID_OPERATION : The operation is not allowed in the current state";
			break;


		case KTX_INVALID_VALUE:
			err_str = "INVALID_VALUE :  A parameter value was not valid";
			break;

		case KTX_NOT_FOUND:
			err_str = "NOT_FOUND: Requested key was not found";
			break;

		case KTX_OUT_OF_MEMORY:
			err_str = "OUT_OF_MEMORY: Not enough memory to complete the operation";
			break;

		case KTX_UNEXPECTED_END_OF_FILE:
			err_str = "UNEXPECTED_END_OF_FILE: The file did not contain enough data";
			break;

		case KTX_UNKNOWN_FILE_FORMAT:
			err_str = "UNKNOWN_FILE_FORMAT: The file is not a KTX file";
			break;

		case KTX_UNSUPPORTED_TEXTURE_TYPE:
			err_str = "UNSUPPORTED_TEXTURE_TYPE: The KTX file specifies an unsupported texture type";
			break;

		}

		std::cout << err_str << " ERROR:(" << file << "," << line << ") " << std::endl;
	}





}

#define chk() check(__FILE__,__LINE__) //here we are using __FILE__, __LINE__ because they are managed by preprocessor and whenever macro containing them as arg is found by preprocessor
//it replaces them with file, line (reptv) where macro call was found


#define chk_ktx(E_C)  check(E_C,__FILE__, __LINE__)

void error(std::string s)
{

	throw std::runtime_error(s);
}

void check_compile(GLuint sh, std::string name)//put call to this function every time after compilation of a shader obeject
{
	GLint succ = 0;
	glGetShaderiv(sh, GL_COMPILE_STATUS, &succ);//feeds compilation status in succ
	if (succ == GL_FALSE)//in opengl GL_FALSE and GL_TRUE are the bool values
	{
		GLint log_length = 0;
		glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<GLchar> e_log(log_length);

		glGetShaderInfoLog(sh, log_length, &log_length, &e_log[0]);

		std::cerr << name << std::endl;
		for (int i = 0; i < e_log.size(); ++i)
		{
			std::cerr << (char)e_log[i];

		}
		std::cerr << std::endl;
		glDeleteShader(sh);
	}


}


void check_link(GLuint pro, std::string name)
{
	GLint succ = 0;
	glGetProgramiv(pro, GL_LINK_STATUS, &succ);
	if (succ == GL_FALSE)
	{
		GLint log_length = 0;
		glGetProgramiv(pro, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<GLchar>e_log(log_length);

		glGetProgramInfoLog(pro, log_length, &log_length, &e_log[0]);


		std::cerr << name << std::endl << "LINKER ERROR: ";
		for (int i = 0; i < e_log.size(); ++i)
		{
			std::cerr << (char)e_log[i];

		}
		std::cerr << std::endl;
		glDeleteProgram(pro);

	}

}
