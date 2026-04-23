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
    int N = 16;
    // Limpiar la pantalla con el color de fondo
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    // 1. Definir los 3 vértices del triángulo (centrado en la pantalla)
    float v1[2] = { 0.0f,  18.0f}; // Vértice Superior
    float v2[2] = {-18.f, -16.0f}; // Vértice Inferior Izquierdo
    float v3[2] = { 18.0f, -16.0f}; // Vértice Inferior Derecho

    // Opcional: Dibujar el triángulo base (contorno negro)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2fv(v1);
        glVertex2fv(v2);
        glVertex2fv(v3);
    glEnd();

    // 2. Generar las redes de líneas
    glBegin(GL_LINES);
    for(int i = 0; i <= N; ++i) {
        // Calcular la proporción t (de 0.0 a 1.0)
        float t = (float)i / N;

        // Calcular el punto actual en cada arista
        // Arista Izquierda (de V1 a V2)
        float L_x = v1[0] + t * (v2[0] - v1[0]);
        float L_y = v1[1] + t * (v2[1] - v1[1]);

        // Arista Inferior (de V2 a V3)
        float B_x = v2[0] + t * (v3[0] - v2[0]);
        float B_y = v2[1] + t * (v3[1] - v2[1]);

        // Arista Derecha (de V3 a V1)
        float R_x = v3[0] + t * (v1[0] - v3[0]);
        float R_y = v3[1] + t * (v1[1] - v3[1]);

        // --- Trazar las conexiones como se ve en la imagen ---

        // Líneas Rojas: Conectan la Arista Izquierda con la Inferior
        glColor3f(1.0f, 0.2f, 0.2f); // Color rojo
        glVertex2f(L_x, L_y);
        glVertex2f(B_x, B_y);

        // Líneas Azules/Celestes: Conectan la Arista Inferior con la Derecha
        glColor3f(0.2f, 0.6f, 1.0f); // Color azul claro
        glVertex2f(B_x, B_y);
        glVertex2f(R_x, R_y);

        // Líneas Verdes: Conectan la Arista Derecha con la Izquierda
        glColor3f(0.4f, 0.8f, 0.4f); // Color verde
        glVertex2f(R_x, R_y);
        glVertex2f(L_x, L_y);
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
