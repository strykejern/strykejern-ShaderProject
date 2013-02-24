#ifdef __APPLE__
#include <glew.h>
#include <GLUT/GLUT.h>
#include <OpenGL/glext.h>
#elif defined (_WIN32)
#include <GL\glew.h>
#include <GL\glut.h>
#include <GL\GL.h>
#endif

#include "simpleShaders.h"

#include <iostream>
#include <fstream>

using namespace std;

simpleShaderProgram::simpleShaderProgram(void){
	program = glCreateProgram();
}

simpleShaderProgram::simpleShaderProgram(const char *vertexFile, const char *fragFile){
	program = glCreateProgram();

	attachVertexShader(vertexFile);
	attachFragmentShader(fragFile);

	linkShader();
}

void simpleShaderProgram::loadShader(const GLchar *filename, GLint shader, GLuint program){
	GLuint vshader = glCreateShader(shader);
    
    const char *vs = readTextFile(filename);
    
    glShaderSource(vshader, 1, &vs, NULL);
    
	char output[2000];

    glCompileShader(vshader);
	glGetShaderInfoLog(vshader, 2000, NULL, output);
	cout << output << endl;
    
	glAttachShader(program,vshader);
    
    delete [] vs;
}

char *simpleShaderProgram::readTextFile(const char *filename){
    ifstream TFile;
    TFile.open(filename);
    
    if (TFile.is_open()){
        TFile.seekg(0, ios::end);
        int length = (int)TFile.tellg();
        TFile.seekg(0, ios::beg);
        
        char *buffer = new char[length+1];
        
        TFile.read(buffer, length);
        
        buffer[length] = '\0';
        
        TFile.close();
        
        return buffer;
    }
    else{
        cout << "Error opening file" << endl;
        throw 404;
    }
}

void simpleShaderProgram::attachVertexShader(const char *filename){
	loadShader(filename, GL_VERTEX_SHADER, program);
}

void simpleShaderProgram::attachFragmentShader(const char *filename){
	loadShader(filename, GL_FRAGMENT_SHADER, program);
}

void simpleShaderProgram::enable(void){
	glUseProgram(program);
}

void simpleShaderProgram::disable(void){
	glUseProgram(NULL);
}

void simpleShaderProgram::linkShader(void){
	glLinkProgram(program);
}

GLint simpleShaderProgram::getProgram(void){
	return program;
}