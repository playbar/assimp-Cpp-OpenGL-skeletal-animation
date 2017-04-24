#include "ForShader.h"



ForShader::ForShader()
{
}


ForShader::~ForShader()
{
}


const GLchar* ForShader::readShader(const char* filename)
{
#ifdef WIN32
	FILE* infile;
	fopen_s(&infile, filename, "rb");
#else
	FILE* infile = fopen(filename, "rb");
#endif // WIN32

	if (!infile) {
#ifdef _DEBUG
		std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar* source = new GLchar[len + 1];

	fread(source, 1, len, infile);
	fclose(infile);

	source[len] = 0;

	return const_cast<const GLchar*>(source);
}

GLuint ForShader::makeProgram(const char* vertex, const char* fragment)
{
	const char* vertexShaderCode = readShader(vertex); 
	const char* fragmentShaderCode = readShader(fragment); 
	std::cout << vertexShaderCode << std::endl;
	std::cout << fragmentShaderCode << std::endl;

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//const char* adapter[1];
	//adapter[0] = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, &vertexShaderCode, 0);
	delete[] vertexShaderCode;
	//adapter[0] = fragmentShaderCode;
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, 0);
	delete[] fragmentShaderCode;

	glCompileShader(vertexShaderID);
#ifdef _DEBUG
	GLint compiled;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compiled); // �������� ���� ��?���������������� 
	if (compiled == GL_FALSE) {
		GLsizei len;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &len); // ?- GL_INFO_LOG_LENGTH - ������ ������ ������

		GLchar* log = new GLchar[len + 1]; // ��� ������??������?����?������
		glGetShaderInfoLog(vertexShaderID, len, &len, log); // �������� ������ ���������� �� ������
		std::cout << "Vertex Shader compilation failed: " << log << std::endl;
		delete[] log;
	}
	else { std::cout << "Vertex Shader compiled " << std::endl; }
#endif // DEBUG

	glCompileShader(fragmentShaderID);
#ifdef _DEBUG
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compiled); // �������� ���� ��?���������������� 
	if (compiled == GL_FALSE) {
		GLsizei len;
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &len); // ?- GL_INFO_LOG_LENGTH - ������ ������ ������

		GLchar* log = new GLchar[len + 1]; // ��� ������??������?����?������
		glGetShaderInfoLog(fragmentShaderID, len, &len, log); // �������� ������ ���������� �� ������
		std::cout << "Fragment Shader compilation failed: " << log << std::endl;
		delete[] log;
	}
	else { std::cout << "Fragment Shader compiled " << std::endl; }
#endif // DEBUG

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);
#ifdef _DEBUG
	GLint linked;
	glGetProgramiv(programID, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE)
	{
		GLsizei len;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(programID, len, &len, log);
		std::cerr << "glLinkProgram(programID) failed: " << log << std::endl;
		delete[] log;
	}
#endif // DEBUG

	// ��������?������?��?��
	//glDetachShader(programID, vertexShaderID);   // removes shader from program
	glDeleteShader(vertexShaderID);				 // delete
	//glDetachShader(programID, fragmentShaderID); // removes shader from program
	glDeleteShader(fragmentShaderID);			 // delete

	return programID;

}