#include <stdlib.h>
#include <GL/glut.h>
// eje es el ángulo a rotar por cada redibujo x en el eje z
static GLfloat thetaPrima= 0.01;
// cada nuevo cuadrado es mas grande que el anterior por ladoIncremento
static GLfloat ladoIncremento = 0.05;
void cuadrado(float lado) {
    glBegin(GL_LINE_LOOP);
    glColor3f(1,0,0);
    glVertex3f(-lado, -lado, 0);
    glColor3f(1,0,0);
    glVertex3f(lado, -lado, 0);
    glColor3f(1,0,0);
    glVertex3f(lado, lado, 0);
    glColor3f(1,0,0);
    glVertex3f(-lado, lado, 0);
    glEnd();

}
void display(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glPushMatrix();
 float ladoinicial = 0.2;
 for(int i = 0; i < 20; i++) {
    glRotatef(thetaPrima,0,0,1);
    cuadrado(ladoinicial + ladoIncremento * i);
 }
 glPopMatrix();
 glFlush();
// intercambiamos los buffers, el que se muestra y el que esta oculto
 glutSwapBuffers();
}

// función para animar
void girar(void)
{
 thetaPrima += 0.5;
 if(thetaPrima > 360) thetaPrima -= 360.0;
 display();
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
 glutCreateWindow("Ejercicio 3 Labo 4");
 glutReshapeFunc(myReshape);
 glutDisplayFunc(display);
 glutIdleFunc(girar);
 glEnable(GL_DEPTH_TEST);
 glutMainLoop();

 return 0;
}