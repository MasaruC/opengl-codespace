#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

enum DrawType { DRAW_FILL, DRAW_STROKE, DRAW_DASHED };

const GLfloat ORTHO_LEFT   = -30.0f;
const GLfloat ORTHO_RIGHT  =  30.0f;
const GLfloat ORTHO_BOTTOM = -30.0f;
const GLfloat ORTHO_TOP    =  30.0f;

void init(void);
void display(void);
void reshape(int,int);
void computeRegularPolygonVertex(int index, int count, GLfloat radius, GLfloat centerX, GLfloat centerY, GLfloat &x, GLfloat &y);
void configureDrawMode(DrawType mode);
void drawShape(int points, GLfloat radius, GLfloat centerX, GLfloat centerY, DrawType mode, GLfloat red, GLfloat green, GLfloat blue);
void drawAxes(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(750,750);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

void init(void)
{
    glClearColor(1.0,1.0,1.0,0.0);
    glShadeModel(GL_FLAT);
}

void computeRegularPolygonVertex(int index, int count, GLfloat radius, GLfloat centerX, GLfloat centerY, GLfloat &x, GLfloat &y)
{
    GLfloat ang = 2.0f * M_PI * index / count;
    x = centerX + radius * cos(ang);
    y = centerY + radius * sin(ang);
}


void drawAxes(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
{
    glPushMatrix();
    glLineWidth(1.0f);
    glDisable(GL_LINE_STIPPLE);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(left, 0.0f);
    glVertex2f(right, 0.0f);
    glVertex2f(0.0f, bottom);
    glVertex2f(0.0f, top);
    glEnd();
    glPopMatrix();
}

void display() {
    int N = 20;
    // Limpiar la pantalla con el color de fondo
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes(-N, N, -N, N);
    glPushMatrix();
    //============
    // Atributos
    //============
    glColor3f(0.1f, 0.3f, 0.6f);
    glBegin(GL_LINES);
    //============
    //  Pintado
    //============
    
    int v1[2] = {-N, 0};
    int v2[2] = {N, 0};
    int v3[2] = {0, N};
    int v4[2] = {0, -N};

    for (int i = 0; i < N; i++){
        //Cuadrantes 1
        glVertex2i(v2[0] - i, 0);
        glVertex2i(0, 0 + i);
        //Cuadrante 2
        glVertex2i(v1[0] + i, 0);
        glVertex2i(0, 0 + i);
        //Cuadrante 3
        glVertex2i(v1[0] + i, 0);
        glVertex2i(0, 0 - i);
        //Cuadrante 4
        glVertex2i(v2[0] - i, 0);
        glVertex2i(0, 0 - i);
    }
    glEnd();
    glPopMatrix();
    glFlush(); // Asegurar que todo se dibuje en pantalla
}

void reshape(int w, int h)
{
    glViewport(0,0,(GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
