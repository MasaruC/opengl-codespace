#include <stdlib.h>
#include <GL/glut.h>

// --- VARIABLES GLOBALES DE ESTADO ---
// Ángulos de rotación para los ejes X, Y, Z
static GLfloat theta[] = {0.0, 0.0, 0.0};
// 0 = Eje X, 1 = Eje Y, 2 = Eje Z
static GLint eje_actual = 2; 

// --- EJES XYZ WIRE ---
void dibujarEjes(){
    glPushMatrix();
    glBegin(GL_LINES);
        // Eje X en rojo
        glColor3f(1.0, 0.0, 0.0); 
        glVertex3f(-20.0, 0.0, 0.0);
        glVertex3f(20.0, 0.0, 0.0);
        
        // Eje Y en verde
        glColor3f(0.0, 1.0, 0.0); 
        glVertex3f(0.0, -20.0, 0.0);
        glVertex3f(0.0, 20.0, 0.0);
        
        // Eje Z en azul
        glColor3f(0.0, 0.0, 1.0); 
        glVertex3f(0.0, 0.0, -20.0);
        glVertex3f(0.0, 0.0, 20.0);
    glEnd();
    glPopMatrix();
}
// --- ZONA DE MODELADO GEOMÉTRICO ---
// Aquí definirás los vértices y caras de tus futuros objetos 3D
void dibujarPrismaRectangularContorno(float x, float y, float z, float ancho, float alto, float profundidad) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0, 1.0, 0.0); // Amarillo
    glLineWidth(2.0); 

    // Guardamos la mitad de las dimensiones para facilitar el cálculo
    float dx = ancho / 2.0;
    float dy = alto / 2.0;
    float dz = profundidad / 2.0;

    glBegin(GL_LINES);
        // --- CARA FRONTAL ---
        glVertex3f(-dx, -dy, dz); glVertex3f( dx, -dy, dz); // Abajo
        glVertex3f( dx, -dy, dz); glVertex3f( dx,  dy, dz); // Derecha
        glVertex3f( dx,  dy, dz); glVertex3f(-dx,  dy, dz); // Arriba
        glVertex3f(-dx,  dy, dz); glVertex3f(-dx, -dy, dz); // Izquierda

        // --- CARA TRASERA ---
        glVertex3f(-dx, -dy, -dz); glVertex3f( dx, -dy, -dz); // Abajo
        glVertex3f( dx, -dy, -dz); glVertex3f( dx,  dy, -dz); // Derecha
        glVertex3f( dx,  dy, -dz); glVertex3f(-dx,  dy, -dz); // Arriba
        glVertex3f(-dx,  dy, -dz); glVertex3f(-dx, -dy, -dz); // Izquierda

        // --- CONEXIONES ENTRE CARAS (Profundidad) ---
        glVertex3f(-dx, -dy,  dz); glVertex3f(-dx, -dy, -dz);
        glVertex3f( dx, -dy,  dz); glVertex3f( dx, -dy, -dz);
        glVertex3f(-dx,  dy,  dz); glVertex3f(-dx,  dy, -dz);
        glVertex3f( dx,  dy,  dz); glVertex3f( dx,  dy, -dz);
    glEnd();
    glPopMatrix();
}
void dibujarEsfera(float x, float y, float z, float radio, int slices, int stacks) {
    glPushMatrix();           
    glTranslatef(x, y, z);
    glutWireSphere(radio, slices, stacks);
    glPopMatrix(); 
}

// --- RENDERIZADO (DISPLAY) ---
void display(void) {
    // 1. Limpiar colores y el buffer de profundidad (CRÍTICO EN 3D)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 2. Reiniciar transformaciones
    glLoadIdentity();
    
    // 3. Aplicar rotaciones según los ángulos calculados
    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);
    
    // 4. Dibujar el objeto
    dibujarEjes(); 
    dibujarPrismaRectangularContorno(0.0, 0.0, 0.0, 10.0, 2.0, 1.0);
    dibujarEsfera(-4.0, -10.0, 0.0, 2.0, 20, 20);
    dibujarEsfera(0.0, -10.0, 0.0, 2.0, 20, 20);
    dibujarEsfera(4.0, -10.0, 0.0, 2.0, 20, 20);
    // 5. Intercambiar buffers para animación suave (Double Buffering)
    glutSwapBuffers();
}

// --- LÓGICA DE ANIMACIÓN ---
void animacionContinua() {
    // Incrementar el ángulo del eje seleccionado
    theta[eje_actual] += 0.05;
    
    // Reiniciar a 0 si da la vuelta completa (evita desbordamiento numérico)
    if(theta[eje_actual] > 360.0) {
        theta[eje_actual] -= 360.0;
    }
    
    // Solicitar que se vuelva a dibujar la pantalla
    glutPostRedisplay(); // Mejor práctica que llamar a display() directamente
}

// --- CONTROL DE TECLADO ---
void teclado(unsigned char tecla, int x, int y) {
    switch(tecla) {
        case 'x': case 'X': eje_actual = 0; break; // Rotar en X
        case 'y': case 'Y': eje_actual = 1; break; // Rotar en Y
        case 'z': case 'Z': eje_actual = 2; break; // Rotar en Z
        case 27: exit(0); break;                   // Tecla ESC para salir
    }
}

// --- CONTROL DE CAMARA Y VENTANA (PROYECCIÓN) ---
void redimensionarVentana(int ancho, int alto) {
    // Evitar división por cero
    if (alto == 0) alto = 1;
    
    // Ajustar el Viewport a las nuevas dimensiones
    glViewport(0, 0, ancho, alto);
    
    // Configurar la matriz de proyección (La cámara)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Calculamos la proporción de la ventana
    GLfloat proporcion = (GLfloat)ancho / (GLfloat)alto;
    
    // glOrtho(izquierda, derecha, abajo, arriba, cerca, lejos)
    // X = 20 (-10.0 a 10.0)
    // Y = 20 (-10.0 a 10.0)
    // Z = 40 (-20.0 a 20.0)
    
    if (ancho <= alto) {
        // Si la ventana es más alta que ancha, ajustamos el eje Y
        glOrtho(-20.0, 20.0, -20.0 / proporcion, 20.0 / proporcion, -40.0, 40.0);
    } else {
        // Si la ventana es más ancha que alta, ajustamos el eje X
        glOrtho(-20.0 * proporcion, 20.0 * proporcion, -20.0, 20.0, -40.0, 40.0);
    }
    
    // Volver a la matriz de modelado (Para transformar los objetos)
    glMatrixMode(GL_MODELVIEW);
}
// --- PROGRAMA PRINCIPAL ---
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    
    // Configuración clave 3D: GLUT_DOUBLE (Animación) y GLUT_DEPTH (Profundidad Z)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Plantilla Base 3D - OpenGL");
    
    // Registrar callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(redimensionarVentana);
    //glutIdleFunc(animacionContinua);
    glutKeyboardFunc(teclado);
    
    // Habilitar la prueba de profundidad (Imprescindible en 3D)
    glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
    return 0;
}