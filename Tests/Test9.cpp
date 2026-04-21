#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
float vertices[4][2]={{-1,-5},{10,0},{8,10},{4,8}};
void init(void);
void display(void);
void reshape(int,int);
void malla_polar(void);
void circunferencia(int);
void caracol(void);
void rosal(void);
void segmentos(int,int,int);

int main(int argc, char** argv)
{
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
glutInitWindowSize(500,500);
glutInitWindowPosition(100,100);
glutCreateWindow(argv[0]);
init();
glutDisplayFunc(display);
glutReshapeFunc(reshape);
glutMainLoop();
return 0;
}


void malla_polar(void)
{
    int N=10,radio=8;
    float ang,x,y;
    for (int i=0;i<radio;i++)
        {
           circunferencia(i);
        }

    glBegin(GL_LINES);
    for (ang = 0.0f; ang < 2 * M_PI; ang += 2*M_PI/N)
        {
        x = radio * cos(ang);
        y = radio * sin(ang);
        glVertex2f(0,0);
        glVertex2f(x,y);
        }
    glEnd();
}

void circunferencia(int radio)
{
    int N=20;
    float ang,x,y;
    glBegin(GL_LINE_LOOP);
    for (ang = 0.0f; ang < 2 * M_PI; ang += 2*M_PI/N)
        {
        x = radio * cos(ang);
        y = radio * sin(ang);
        glVertex2f(x,y);
        }
    glEnd();
}

void caracol(void)
{
    float ang,x,y,radio;
    glBegin(GL_LINE_LOOP);
    for (ang = 0.0f; ang < 2 * M_PI; ang += 2*M_PI/200)
        {
            radio=2+4*sin(ang);
            x = radio * cos(ang);
            y = radio * sin(ang);
            glVertex2f(x,y);
        }
    glEnd();
}


void rosal(void)
{
    float ang,x,y,radio;
    glBegin(GL_LINE_LOOP);
    for (ang = 0.0f; ang < 2 * M_PI; ang += 2*M_PI/200)
        {
            radio=3*cos(2*ang);
            x = radio * cos(ang);
            y = radio * sin(ang);
            glVertex2f(x,y);
        }
    glEnd();
}

void segmentos(int a,int b,int c)
{
    int N=10;
    float t,paso=1.0/N,x0,y0,x1,y1;
    glBegin(GL_LINES);
    for(int i=0;i<N;i++)
        {
            t=i*paso;
            x0=(1-t)*vertices[a][0]+t*vertices[b][0];
            y0=(1-t)*vertices[a][1]+t*vertices[b][1];
            x1=(1-t)*vertices[b][0]+t*vertices[c][0];
            y1=(1-t)*vertices[b][1]+t*vertices[c][1];
            glVertex2f(x0,y0);
            glVertex2f(x1,y1);
        }
    glEnd();
}

void init(void)
{
glClearColor(1.0,1.0,1.0,0.0);  //parametros: rojo, amarillo, azul, el cuarto es el parametro alpha
glShadeModel(GL_FLAT);
}
void display(void)
{

glClear(GL_COLOR_BUFFER_BIT);
glPushMatrix();


//malla_polar();
//glColor3f(1,0,0);
//caracol();
//glColor3f(0,1,0);
//rosal();
glColor3f(0,0,1);
glBegin(GL_LINE_LOOP);
    glVertex2fv(vertices[0]);
    glVertex2fv(vertices[1]);
    glVertex2fv(vertices[2]);
    glVertex2fv(vertices[3]);
glEnd();
glColor3f(1,0,0);
segmentos(0,1,2);
glColor3f(0,1,0);
segmentos(1,2,3);
glColor3f(1,1,0);
segmentos(2,3,0);
glColor3f(0,1,1);
segmentos(3,0,1);

glPopMatrix();      // reecupera el estado del matriz
glFlush();
}
void reshape(int w, int h)
{
glViewport(0,0,(GLsizei)w, (GLsizei)h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-10.0,10.0,-10.0,10,-10.0,10.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
}