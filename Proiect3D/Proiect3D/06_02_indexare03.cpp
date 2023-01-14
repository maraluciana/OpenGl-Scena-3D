/* INDEXARE
Elemente de noutate:
   - folosirea indexarii varfurilor: elemente asociate (matrice, buffer)
   - desenarea se face folosind functia glDrawElements()
   */
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h

#include "loadShaders.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

//////////////////////////////////////

GLuint
	VaoId,
	VboId,
	EboId,
	ColorBufferId,
	ProgramId,
	myMatrixLocation;

float width = 80.f, height = 60.f;
glm::mat4 
	myMatrix, resizeMatrix = glm::ortho(-width, width, -height, height);

struct point {
	float x; float y;
};


void CreateVBO(void)
{
	int k, r, n = 8;
	float PI = 3.141592, vf_x, vf_y;
	point cerc_mic[8], cerc_mare[8];
	for (k = 0; k < n; k++)
	{
		point p;
		//coordonate puncte cerc mic
		r = 1;
		vf_x = r * cos(2 * k * PI / n);
		vf_y = r * sin(2 * k * PI / n);

		p.x = vf_x; p.y = vf_y;

		cerc_mic[k] = p;

		//coordonate puncte cerc mare
		r = 5;
		vf_x = r * cos(2 * k * PI / n);
		vf_y = r * sin(2 * k * PI / n);

		p.x = vf_x; p.y = vf_y;

		cerc_mic[k] = p;
	};

	// coordonatele varfurilor
	static const GLfloat vf_pos[] =
	{
		cerc_mic[0].x, cerc_mic[0].y, 0.0f, 1.0f,
		cerc_mare[0].x, cerc_mare[0].y, 0.0f, 1.0f,
		cerc_mic[1].x, cerc_mic[1].y, 1.0f,
		cerc_mare[1].x, cerc_mare[1].y, 0.0f, 1.0f,
		cerc_mic[2].x, cerc_mic[2].y, 1.0f,
		cerc_mare[2].x, cerc_mare[2].y, 0.0f, 1.0f,
		cerc_mic[3].x, cerc_mic[3].y, 1.0f,
		cerc_mare[3].x, cerc_mare[3].y, 0.0f, 1.0f,
		cerc_mic[4].x, cerc_mic[4].y, 0.0f, 1.0f,
		cerc_mare[4].x, cerc_mare[4].y, 0.0f, 1.0f,
		cerc_mic[5].x, cerc_mic[5].y, 0.0f, 1.0f,
		cerc_mare[5].x, cerc_mare[5].y, 0.0f, 1.0f,
		cerc_mic[6].x, cerc_mic[6].y, 0.0f, 1.0f,
		cerc_mare[6].x, cerc_mare[6].y, 0.0f, 1.0f,
		cerc_mic[7].x, cerc_mic[7].y, 0.0f, 1.0f,
		cerc_mare[7].x, cerc_mare[7].y, 0.0f, 1.0f,

	};

	// culorile varfurilor
	static const GLfloat vf_col[] =
	{
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	// indici pentru trasarea unor primitive
	static const GLuint vf_ind[] =
	{
		0, 1, 2, 3, 1, 2,
		2,3,4,5,3,4,
		4,5,6,7,5,6,
		6,7,8,9,7,8,
		8,9,10,11,9,10,
		10,11,12,13,11,12,
		12,13,14,15,13,14,
		14,15,0,1,15,0,
	};

	// se creeaza un buffer nou pentru varfuri
	glGenBuffers(1, &VboId);
	// buffer pentru indici
	glGenBuffers(1, &EboId);
	// se creeaza / se leaga un VAO (Vertex Array Object)
	glGenVertexArrays(1, &VaoId);

	// legare VAO
	glBindVertexArray(VaoId);

	// buffer-ul este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, VboId);

	// buffer-ul va contine atat coordonatele varfurilor, cat si datele de culoare
	glBufferData(GL_ARRAY_BUFFER, sizeof(vf_col) + sizeof(vf_pos), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vf_pos), vf_pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vf_pos), sizeof(vf_col), vf_col);

	// buffer-ul pentru indici
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vf_ind), vf_ind, GL_STATIC_DRAW);

	// se activeaza lucrul cu atribute; atributul 0 = pozitie, atributul 1 = culoare, acestea sunt indicate corect in VBO
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(vf_pos));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &EboId);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void CreateShaders(void)
{
	ProgramId = LoadShaders("06_02_Shader.vert", "06_02_Shader.frag");
	glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	CreateVBO();
	CreateShaders();
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix"); 
}
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	myMatrix = resizeMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));
	// De realizat desenul folosind segmente de dreapta
	glFlush();
}
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Utilizarea indexarii varfurilor");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

