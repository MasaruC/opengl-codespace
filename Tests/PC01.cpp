#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <utility>
/*
========================ZONA DE INFORMACION===========================
NOMBRE: Juan Masaru Campos Luque
CODIGO: 24200153
*/

enum DrawType { DRAW_FILL, DRAW_STROKE, DRAW_DASHED };

// Definición de los límites del sistema de coordenadas ortográficas
const GLfloat ORTHO_LEFT   = -30.0f;
const GLfloat ORTHO_RIGHT  =  30.0f;
const GLfloat ORTHO_BOTTOM = -30.0f;
const GLfloat ORTHO_TOP    =  30.0f;

int N = 20;
int dientes = 16;
float R = 15.0f;
float r = 7.0f;

void init(void);
void display(void);
void reshape(int,int);
void computeRegularPolygonVertex(int index, int count, GLfloat radius, GLfloat centerX, GLfloat centerY, GLfloat &x, GLfloat &y);
void configureDrawMode(DrawType mode);
void drawShape(int points, GLfloat radius, GLfloat centerX, GLfloat centerY, DrawType mode, GLfloat red, GLfloat green, GLfloat blue);
void drawAxes(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);

int main(int argc, char** argv)
{
    dientes = dientes*2;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100,100);
    glutCreateWindow("24200153 - Evaluacion 1");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

void init(void)
{
    glClearColor(0.74,0.74,0.74,0.74);
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
    glColor3f(0.0f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex2f(left, 0.0f);
    glVertex2f(right, 0.0f);
    glVertex2f(0.0f, bottom);
    glVertex2f(0.0f, top);
    glEnd();
    glPopMatrix();
}
void Algoritmo_Bresenham(int x0, int y0, int x1, int y1)
{
    // Algoritmo de Bresenham para dibujar una línea entre (x0, y0) y (x1, y1)
    int sx,sy;// Variables para la dirección del paso en x e y
    // Calcular las diferencias absolutas y los incrementos
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    // Determinar la dirección del paso en x e y
    if (x0 < x1){
        sx = 1;
    }
    else{
        sx = -1;
    }
    if (y0 < y1){
        sy = 1;
    }
    else{
        sy = -1;
    }

    // Determinar si la línea es más empinada en y o en x
    bool steep = dy > dx;

    // Si la línea es más empinada en y, intercambiar los roles de x e y
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(dx, dy);
        std::swap(sx, sy);
    }

    int err = 2 * dy - dx; // Inicializar el error
    int x = x0;
    int y = y0;

    glBegin(GL_POINTS);
    for (int i = 0; i <= dx; ++i) {
        if (steep) { // Si la línea es más empinada en y, dibujar con las coordenadas intercambiadas
            glVertex2i(y, x);
        } else {    // Si la línea es más empinada en x, dibujar normalmente
            glVertex2i(x, y);
        }
        // Actualizar el error y las coordenadas
        if (err > 0) {
            y += sy;
            err -= 2 * dx;
        }
        x += sx;
        err += 2 * dy;
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes(-N, N, -N, N);
    glPointSize(7.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < dientes; ++i)
    {
        // Calcular los ángulos y radios para los vértices del engranaje
        GLfloat ang0 = 2.0f * M_PI * i / dientes;
        GLfloat ang1 = 2.0f * M_PI * ((i + 1) % dientes) / dientes;
        GLfloat radius0 = (i % 2 == 0) ? R : r; // Alternar entre R y r para cada diente
        GLfloat radius1 = (((i + 1) % dientes) % 2 == 0) ? R : r; // Alternar entre R y r para el siguiente diente
        // Calcular las coordenadas de los vértices
        int x0 = (int)round(radius0 * cos(ang0));
        int y0 = (int)round(radius0 * sin(ang0));
        int x1 = (int)round(radius1 * cos(ang1));
        int y1 = (int)round(radius1 * sin(ang1));

        Algoritmo_Bresenham(x0, y0, x1, y1);
    }
    /*
    //======================DIBUJADO DE CIRCULOS========================
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    // r^2 = x^2 + y^2 => y = sqrt(r^2 - x^2) => x = sqrt(r^2 - y^2)
    for (int i = 0; i < 360; ++i)
    {
        GLfloat ang = 2.0f * M_PI * i / 360;
        GLfloat x = R * cos(ang);
        GLfloat y = R * sin(ang);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    // r^2 = x^2 + y^2 => y = sqrt(r^2 - x^2) => x = sqrt(r^2 - y^2)
    for (int i = 0; i < 360; ++i)
    {
        GLfloat ang = 2.0f * M_PI * i / 360;
        GLfloat x = r * cos(ang);
        GLfloat y = r * sin(ang);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();
    //===================DIBUJADO A LINEAS===========================
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < dientes; ++i)
    {
        GLfloat x, y;
        GLfloat ang = 2.0f * M_PI * i / dientes;
        GLfloat radius = (i % 2 == 0) ? R : r;
        x = radius * cos(ang);
        y = radius * sin(ang);
        glVertex2f(x, y);
    }

    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glPopMatrix();
    */
        // Otra alternativa sería hacer solo un cuadrante u octante y hacer transformaciones, pero honestamente no se como hacerlo :c

    glFlush();
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