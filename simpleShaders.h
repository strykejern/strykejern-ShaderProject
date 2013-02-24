#ifndef SIMPLESHADERS_H
#define SIMPLESHADERS_H

#ifdef __APPLE__
#include <glew.h>
#include <GLUT/GLUT.h>
#include <OpenGL/glext.h>
#elif defined (_WIN32)
#include <GL\glew.h>
#include <GL\glut.h>
#include <GL\GL.h>
#endif

class simpleShaderProgram{
private:
	GLint program;
	GLint vshader, fshader;

	char *readTextFile(const char *filename);

	void loadShader(const GLchar *filename, GLint shader, GLuint program);
public:
	simpleShaderProgram(void);
	simpleShaderProgram(const char *vertexFile, const char *fragFile);

	void attachVertexShader(const char *filename);
	void attachFragmentShader(const char *filename);

	GLint getProgram(void);

	void linkShader(void);

	void enable(void);
	void disable(void);
};

#endif