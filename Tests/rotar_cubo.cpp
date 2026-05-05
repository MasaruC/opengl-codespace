#include <stdlib.h>
#include <GL/glut.h>
// declaracion de variables
// GLfloat ...;
// theta[] me indica los ángulos iniciales en los 3 ejes
static GLfloat theta[] = {0.0,0.0,0.0};
// eje es el ángulo a rotar
static GLint eje = 2;
// construya su poligono base
GLfloat vertices[8][3] = {
 {-1.0, -1.0, -1.0},
 { 1.0, -1.0, -1.0},
 { 1.0,  1.0, -1.0},
 {-1.0,  1.0, -1.0},
 {-1.0, -1.0,  1.0},
 { 1.0, -1.0,  1.0},
 { 1.0,  1.0,  1.0},
 {-1.0,  1.0,  1.0}
};
void cara(int a, int b, int c, int d)
{
    glBegin(GL_QUADS);
    glVertex3f(vertices[a][0], vertices[a][1], vertices[a][2]);
    glVertex3f(vertices[b][0], vertices[b][1], vertices[b][2]);
    glVertex3f(vertices[c][0], vertices[c][1], vertices[c][2]);
    glVertex3f(vertices[d][0], vertices[d][1], vertices[d][2]);
    glEnd();
}
void cubo(void)
{
    glPushMatrix();
    
    glColor3f(1,0,0);
    cara(0,1,2,3);
    glColor3f(0,1,0);
    cara(4,5,1,0);
    glColor3f(0,0,1);
    cara(7,3,2,6);
    glColor3f(1,1,0);
    cara(7,4,0,3);
    glColor3f(1,0,1);
    cara(5,1,2,6);
    glColor3f(0,1,1);
    cara(7,6,5,4);
    glEnd();
    glPopMatrix();
    glFlush();
    glPushMatrix();
    glLineWidth(3.0f);
    glColor3f(1.0, 1.0, 0.0);
    glutWireCube(1.0);
    glEnd();
    glPopMatrix();
    glFlush();
}
// dibujamos nuestra escena
void display(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
// composicion de rotaciones
 glRotatef(theta[0],1.0,0.0,0.0);
 glRotatef(theta[1],0.0,1.0,0.0);
 glRotatef(theta[2],0.0,0.0,1.0);
 cubo();
 glFlush();
// intercambiamos los buffers, el que se muestra y el que esta oculto
 glutSwapBuffers();
}
// esta función controla el angulo de rotación según el eje de giro
void girar_objeto_geometrico ()
{
 theta[eje] += 0.1;
 if(theta[eje]>360) theta[eje] -= 360.0;
 display();
}
void teclado(unsigned char tecla,int x,int y)
{
switch(tecla){
case 'a' : eje = 0; break;
case 's' : eje = 1; break;
case 'd' : eje = 2; break;
case 'f' : exit(0) ; break;
}
 glutPostRedisplay();
}
// control de ventana (recuerde el volumen de visualización)
// modifique dicho volumen según su conveniencia
void myReshape(int w, int h)
{
 glViewport(0,0,w,h);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 if(w <=h)
glOrtho(-2.0,2.0,-2.0*(GLfloat)h/(GLfloat)w,
 2.0*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
 else
glOrtho(-2.0*(GLfloat)w/(GLfloat)h,
 2.0*(GLfloat)w/(GLfloat)h, -2.0,2.0,-10.0,10.0);
 glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char **argv)
{
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 glutInitWindowSize(500,500);
 glutCreateWindow("mi objeto bajo rotaciones");
 glutReshapeFunc(myReshape);
 // invocamos a display() para dibujar nuestra escena
 glutDisplayFunc(display);
 // esta funcion llama a girar_objeto_geométrico() mientras no haya evento
 // alguno ocasionado por el usuario
 glutIdleFunc(girar_objeto_geometrico);
 glutKeyboardFunc(teclado);
 /*glutMouseFunc(mouse);*/
 glEnable(GL_DEPTH_TEST);
 glutMainLoop();

 return 0;
}