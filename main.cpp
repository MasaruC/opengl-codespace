#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>

float Lado = 0.5;

void ejes(int longitud);
GLdouble angulo=0, incremento_angulo=0.1;
GLdouble const radio=0.5;
GLfloat px0=0,py0=0,pz0=5;
GLfloat px1=0,py1=0,pz1=4;
static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis = 2 ;
GLUquadric *quadric;

// ===== LOS 8 VÉRTICES DE UN CUBO =====
// Índices: 0-3 = cara frontal (Z negativo), 4-7 = cara trasera (Z positivo)
// Orden: siguiendo las aristas del cubo

// Obtener vértice i de un cubo de lado L centrado en origen
void obtenerVertice(int i, float L, float *x, float *y, float *z)
{
    float h = L / 2.0f;  // mitad del lado
    
    // Los 8 vértices del cubo
    switch(i) {
        case 0: *x=-h; *y=-h; *z=-h; break;  // inferior-izq-frontal
        case 1: *x= h; *y=-h; *z=-h; break;  // inferior-der-frontal
        case 2: *x= h; *y= h; *z=-h; break;  // superior-der-frontal
        case 3: *x=-h; *y= h; *z=-h; break;  // superior-izq-frontal
        case 4: *x=-h; *y=-h; *z= h; break;  // inferior-izq-trasero
        case 5: *x= h; *y=-h; *z= h; break;  // inferior-der-trasero
        case 6: *x= h; *y= h; *z= h; break;  // superior-der-trasero
        case 7: *x=-h; *y= h; *z= h; break;  // superior-izq-trasero
    }
}

void dibujarCubo(float x, float y, float z, float lado, float rotation)
{
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(rotation,1.0,0.0,0.0);
    glutWireCube(lado);
    glPopMatrix();
}

void dibujarEsfera(float x, float y, float z, float radio, float r, float g, float b)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(r, g, b);
    glutSolidSphere(radio, 20, 20);
    glPopMatrix();
}

// Cilindro entre dos puntos (versión mejorada)
void dibujarCilindro(float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float radio, float r, float g, float b)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    float len = sqrt(dx*dx + dy*dy + dz*dz);
    
    if (len < 0.0001) return;
    
    glPushMatrix();
    glTranslatef(x1, y1, z1);
    
    // Vector dirección
    float nx = dx/len, ny = dy/len, nz = dz/len;
    
    // Calcular ángulo y eje de rotación
    // El cilindro se dibuja a lo largo del eje Z, necesitamos rotarlo
    float angle = acos(nz) * 57.2957795f;  // a grados
    
    // Eje de rotación = (0,0,1) × (nx,ny,nz) = (-ny, nx, 0)
    if (nz > -0.9999f && nz < 0.9999f) {
        glRotatef(angle, -ny, nx, 0);
    } else if (nz < 0) {
        glRotatef(180, 1, 0, 0);
    }
    
    glColor3f(r, g, b);
    gluCylinder(quadric, radio, radio, len, 16, 1);
    glPopMatrix();
}

// ===== ARISTAS DEL CUBO (12 conexiones) =====
int aristas[12][2] = {
    {0,1}, {1,2}, {2,3}, {3,0},  // Cara frontal
    {4,5}, {5,6}, {6,7}, {7,4},  // Cara trasera  
    {0,4}, {1,5}, {2,6}, {3,7}   // Conexiones frontales-traseras
};

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(px0, 0.0, pz0, px1, 0.0, pz1, 0, 1, 0);
    
    // Ejes de referencia
    glColor3f(0.5, 0.5, 0.5);
    ejes(2);
    
    // Rotación global
    glRotatef(theta[0], 1, 0, 0);
    glRotatef(theta[1], 0, 1, 0);
    glRotatef(theta[2], 0, 0, 1);
    
    float L_int = 1.0f;   // lado cubo interno
    float L_ext = 2.0f;   // lado cubo externo
    
    // ===== 1. DIBUJAR CILINDROS INTERNOS (CUBO INTERNO) =====
    float rc = 0.025f;  // radio cilindros internos
    for (int i = 0; i < 12; i++) {
        float x1,y1,z1, x2,y2,z2;
        obtenerVertice(aristas[i][0], L_int, &x1, &y1, &z1);
        obtenerVertice(aristas[i][1], L_int, &x2, &y2, &z2);
        dibujarCilindro(x1,y1,z1, x2,y2,z2, rc, 1.0, 1.0, 0.0);  // Amarillo
    }
    
    // ===== 2. DIBUJAR CILINDROS EXTERNOS (CONEXIÓN INTERNO→EXTERNO) =====
    float re = 0.035f;  // radio cilindros externos
    for (int i = 0; i < 8; i++) {
        float xi,yi,zi, xe,ye,ze;
        obtenerVertice(i, L_int, &xi, &yi, &zi);
        obtenerVertice(i, L_ext, &xe, &ye, &ze);
        dibujarCilindro(xi,yi,zi, xe,ye,ze, re, 0.0, 1.0, 1.0);  // Cyan
    }
    
    // ===== 3. DIBUJAR ESFERAS INTERNAS (ROJAS) =====
    float ri = 0.15f;
    for (int i = 0; i < 8; i++) {
        float x,y,z;
        obtenerVertice(i, L_int, &x, &y, &z);
        dibujarEsfera(x, y, z, ri, 1.0, 0.0, 0.0);  // Rojo
    }
    
    // ===== 4. DIBUJAR ESFERAS EXTERNAS (VERDES) =====
    float re_s = 0.1f;
    for (int i = 0; i < 8; i++) {
        float x,y,z;
        obtenerVertice(i, L_ext, &x, &y, &z);
        dibujarEsfera(x, y, z, re_s, 0.0, 1.0, 0.0);  // Verde
    }
    
    glPopMatrix();
    glutSwapBuffers();
}
void CubeSpin()
{
    theta[axis] += .2;
    if(theta[axis]>360) theta[axis] -= 360.0;
    display();
}
void rotacamara()
{
    px1=px0+radio*sin(angulo);
    pz1=pz0-radio*cos(angulo);
}
void avanza()
{
    px0=px1;pz0=pz1;
    px1=px0+radio*sin(angulo);
    pz1=pz0-radio*cos(angulo);
}
void retro()
{
    px1=px0;pz1=pz0;
    px0=px0-radio*sin(angulo);
    pz0=pz0+radio*cos(angulo);
}
void teclado(unsigned char tecla,int x,int y)
{
    switch(tecla){
        case 'i' : avanza();break;
        case 'm' : retro(); break;
        case 'j' : angulo=angulo+incremento_angulo;rotacamara(); break;
        case 'k' : angulo=angulo-incremento_angulo;rotacamara(); break;
        case 'a' : axis = 0; break;
        case 's' : axis = 1; break;
        case 'd' : axis = 2; break;
        case 'f' : exit(0); break;
    }
}
void ejes(int longitud)
{
    glBegin(GL_LINES);
    glVertex3d(0,0,longitud);
    glVertex3d(0,0,0);
    glEnd();
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(0,longitud,0);
    glEnd();
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(longitud,0,0);
    glEnd();
}
void myReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Perspectiva: FOV, aspecto, near, far
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}
void init(void)
{
    glClearColor(0.15, 0.15, 0.15, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = {5, 5, 5, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glEnable(GL_COLOR_MATERIAL);
    quadric = gluNewQuadric();
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("camara movil");
    init();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(CubeSpin);
    glutKeyboardFunc(teclado);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}