#ifdef __APPLE__
#include <glew.h>
#include <GLUT/GLUT.h>
#include <OpenGL/glext.h>
#elif defined (_WIN32)
#include <GL\glew.h>
#include <GL\glut.h>
#include <GL\GL.h>
#endif

#include <iostream>
#include <fstream>
#include <math.h>

#include "simpleShaders.h"
 
using namespace std;

#define PI 3.141592

int winWidth = 0;
int winHeight = 0;
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height; 
 
	winWidth = width;
	winHeight = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60.0, ar, 1.0, 1000.0);
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
} 

GLfloat xpos = 0.0f;
GLfloat zpos = 0.0f;
GLfloat angle = 0.0f;
GLfloat vertangle = 0.0f;

GLfloat lightXpos = 0.0f;
GLfloat lightZpos = -1500.0f;
bool lightMoving = false;

bool keyStates[256];

GLuint program;

GLuint width, height;

GLint waveDelta;
GLint waveUniform;

simpleShaderProgram *shader;
simpleShaderProgram *waveShader;

bool currentShader = true;

const GLfloat light_position[] = { 0.0f, 150.0f, -20.0f, 1.0f };

char walk = NULL;
char strafe = NULL;

// Render a string using glut font specified
void renderString(float x, float y, float z, void *font, char *string){
    glRasterPos3f(x, y, z);
    
    for (char *c = string; *c != '\0'; ++c){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
}

static GLint glutTime = 0;
static void display(void)
{ 
	GLint bufferTime = glutGet(GLUT_ELAPSED_TIME);
	GLint deltaTime = bufferTime - glutTime;
	float factor = 0.02f * deltaTime;
	if (deltaTime < 0.0f) factor = 0.001f;
	glutTime = bufferTime;

	float c = cos(PI * angle / 180.0f);
	float s = sin(PI * angle / 180.0f);

	if (lightMoving){
		if (keyStates['s']){
			lightZpos += factor * c;
			lightXpos -= factor * s;
		}
		else if (keyStates['w']){
			lightZpos -= factor * c;
			lightXpos += factor * s;
		}
		
		if (keyStates['d']){
			lightXpos += factor * c;
			lightZpos += factor * s;
		}
		else if (keyStates['a']){
			lightXpos -= factor * c;
			lightZpos -= factor * s;
		}
	}
	else {
		if (keyStates['s']){
			zpos -= factor * c;
			xpos += factor * s;
		}
		else if (keyStates['w']){
			zpos += factor * c;
			xpos -= factor * s;
		}
		
		if (keyStates['d']){
			xpos -= factor * c;
			zpos -= factor * s;
		}
		else if (keyStates['a']){
			xpos += factor * c;
			zpos += factor * s;
		}
	}

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//angle += 1.0f;

	/*GLfloat vertices[] = {
		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,
		0.0f, -1.0f, -1.0f
	};*/
	 
	int width = 512;
	int length = 1024;

	static bool init = true;
	static GLfloat *vertices = new GLfloat[width*length*12];
	static GLubyte *colors = new GLubyte[width*length*4*4];

	if (init) {
		init = false;

		for (int i = 0; i < width; ++i){
			for (int j = 0; j < length; ++j){
				for (int k = 0; k < 4; ++k){
					/*colors[i*length*16 + j*16 + k*4] = (i + j) % 4 ? 255 : 0;
					colors[i*length*16 + j*16 + k*4 + 1] = (i + j) % 3 ? 255 : 0;
					colors[i*length*16 + j*16 + k*4 + 2] = (i + j) % 2 ? 255 : 0;
					colors[i*length*16 + j*16 + k*4 + 3] = 255;*/

					colors[i*length*16 + j*16 + k*4] = 0;
					colors[i*length*16 + j*16 + k*4 + 1] = 0;
					colors[i*length*16 + j*16 + k*4 + 2] = 255;
					colors[i*length*16 + j*16 + k*4 + 3] = 255;
				}

				vertices[i*length*12 + j*12] = i;
				vertices[i*length*12 + j*12 + 1] = -1.0f;
				vertices[i*length*12 + j*12 + 2] = (GLfloat)(-j);

				vertices[i*length*12 + j*12 + 3] = (GLfloat)(i + 1);
				vertices[i*length*12 + j*12 + 4] = -1.0f;
				vertices[i*length*12 + j*12 + 5] = (GLfloat)(-j);

				vertices[i*length*12 + j*12 + 6] = (GLfloat)(i + 1);
				vertices[i*length*12 + j*12 + 7] = -1.0f;
				vertices[i*length*12 + j*12 + 8] = (GLfloat)(-j - 1);

				vertices[i*length*12 + j*12 + 9] = (GLfloat)i;
				vertices[i*length*12 + j*12 + 10] = -1.0f;
				vertices[i*length*12 + j*12 + 11] = (GLfloat)(-j - 1);
			}
		}
	}

	/*GLfloat vertices[] = {
		0.0f, -1.0f, 0.0f,
		width, -1.0f, 0.0f,
		width, -1.0f, -length,
		0.0f, -1.0f, -length
	};*/

	static GLint totalDelta = 0;
	static GLint totalDeltaCount = 0;
	static GLint fps = 0;
	totalDelta += deltaTime;
	totalDeltaCount++;

	if (totalDelta > 1000){
		fps = (totalDeltaCount * 1000) / totalDelta;

		totalDelta = 0;
		totalDeltaCount = 0;
	}
	
	char buffer[40];

	sprintf(buffer, "%i", fps);

	glLoadIdentity();
    
    //renderString(-(float)winHeight/(float)winWidth, (float)winWidth/(float)winHeight, -2.0f, GLUT_BITMAP_9_BY_15, buffer);
	renderString(-0.5f, 0.5f, -1.0f, GLUT_BITMAP_9_BY_15, buffer);
    
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glRotatef(vertangle, c, 0.0f, s);
	glTranslatef(xpos, -30.0f, zpos);
	GLfloat newLightPos[] = {lightXpos, light_position[1], lightZpos, light_position[3]};
	glLightfv(GL_LIGHT0, GL_POSITION, newLightPos);
	glPushMatrix();
		glTranslatef(newLightPos[0], newLightPos[1], newLightPos[2]);
		glColor3ub(255, 255, 255);
		glutSolidSphere(1.0, 20, 40);
	glPopMatrix();
    
    
	//setGLSLvariables();
	//glUniform1f(waveDelta, bufferTime);
	if (currentShader){
		shader->enable();
        glUniform1f(waveUniform, bufferTime);
    }
	else {
		waveShader->enable();
		glUniform1f(waveUniform, bufferTime);
	}
	

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);

	glPushMatrix();
		glTranslatef(- width / 2.0f, -1.0f, -4.0f);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glColor3ub(255, 255, 255);
		glDrawArrays(GL_QUADS, 0, width*length*4);
        glTranslatef(0.0f, 2.0f, 0.0f);
    
        if (currentShader)
            ;//shader->disable();
        else {
            waveShader->disable();
            shader->enable();
        }
    
		/*glColor3ub(0, 0, 255);
		for (int j = 0; j < width; ++j){
			for (int i = 0; i < length; i++){
				glTranslatef(0.0f, 0.0f, -1.0f);
				if ((i + j) % 4 == 0){
					glutSolidCube(1.0);
				}
			}
			glTranslatef(1.0f, 0.0f, (float)length);
		}*/
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

    shader->disable();
	
    glColor3d(1,0,0); 
    glPushMatrix();
		//glRotatef(angle, 0.0f, 0.1f, 1.0f);
        glTranslated(0.0,1.2,-6);
        //glutSolidSphere(1,50,50);
		glutSolidCube(1.0);
    glPopMatrix(); 
 
    glPushMatrix();
		//glRotatef(angle, 0.0f, 0.1f, 1.0f);
        glTranslated(0.0,-1.2,-6);
        //glutWireSphere(1,16,16);
		glutWireCube(1.0);
    glPopMatrix(); 
 
    glutSwapBuffers();
} 

int prevx = 0;
int prevy = 0;
bool moveBack = false;
void motion(int x, int y){
	int newx = x - prevx;
	int newy = y - prevy;

	if (!moveBack){
		angle += (float)newx / 16;
		vertangle += (float)newy / 16;

		glutWarpPointer(winWidth / 2, winHeight / 2);
	}

	moveBack = !moveBack;
	prevx = x;
	prevy = y;
	display();
}

void mouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			prevx = x;
			prevy = y;
		}
	}
}

void keyboard (unsigned char key, int x, int y){
	keyStates[key] = true;
	switch (key){
		case 'a':
			strafe = 'a';
			break;
		case 'd':
			strafe = 'd';
			break;
		case 'w':
			walk = 'w';
			break;
		case 's':
			walk = 's';
			break;
		case 'b':
			currentShader = !currentShader;
			break;
        case 'u':
            vertangle -= 2.0f;
            break;
        case 'j':
            vertangle += 2.0f;
            break;
        case 'k':
            angle += 2.0f;
            break;
        case 'h':
            angle -= 2.0f;
            break;
		default:
			break;
	}
}

void keyboardUp (unsigned char key, int x, int y){
	keyStates[key] = false;
	switch (key){
		case 'a':
			strafe = NULL;
			break;
		case 'd':
			strafe = NULL;
			break;
		case 'w':
			walk = NULL;
			break;
		case 's':
			walk = NULL;
			break;
		case 'l':
			lightMoving = !lightMoving;
			break;
		default:
			break;
	}
}

void GLSLInit(){
	glewInit();

	shader = new simpleShaderProgram("bPhongVert.vert", "bPhongFrag.frag");
	waveShader = new simpleShaderProgram("waveVertShader.c", "glsl.frag");

	waveUniform = glGetUniformLocation(waveShader->getProgram(), "waveDelta");
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800,640);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 
 
    glutCreateWindow("Programming Techniques - 3D Spheres"); 

    cout << glGetString(GL_VERSION) << endl;
    //cout << glGetString(GL_EXTENSIONS) << endl;
	
	GLSLInit();
 
    glutReshapeFunc(resize);
    glutDisplayFunc(display); 
	glutIdleFunc(display);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutMouseFunc(mouse);
    glClearColor(1,1,1,1);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 
 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
 
    glEnable(GL_NORMALIZE);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position); 


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 
    glutMainLoop(); 
 
    return EXIT_SUCCESS;
}