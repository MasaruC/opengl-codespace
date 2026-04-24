## Plantilla Base para la Función `display`

Para generar nuevas figuras, tu código debe verse conceptualmente así:

C++

```c++
void display(void)
{
    // 1. Limpiar la pantalla
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 2. Reiniciar transformaciones
    glLoadIdentity();
    
    // 3. Dibujar Figura A (ej: Un fondo o cuadrícula)
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f); // Gris
    glBegin(GL_LINES);
        // ... comportamiento de vérticoes vértices ...
        glVertex2f(x, y);
        // ... comportamiento de vérticoes vértices ...
    glEnd();
    glPopMatrix();
    
	// 4. Dibujar Figura B
	glPushMatrix();
	glPointSize(5.0f);
	glColor3f(1.0f, 0.0f, 0.0f);

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x00FF);

	glBegin(GL_POINTS);
		// ... comportamiento de vérticoes vértices ...
        glVertex2f(x, y);
        // ... comportamiento de vérticoes vértices ...
	glEnd();

	glDisable(GL_LINE_STIPPLE);
	glPopMatrix();
    
    // 5. Forzar la ejecución en pantalla
    glFlush();
}
```

### 1. Primitivas Geométricas: Argumentos de `glBegin()`

El argumento que le pasas a `glBegin()` define exactamente **cómo OpenGL va a conectar los vértices** que declares a continuación.

- **`GL_POINTS`**: Trata cada `glVertex` como un punto individual y aislado.
    
- **`GL_LINES`**: Toma los vértices en pares. El vértice 1 y 2 forman una línea; el 3 y 4 forman otra línea separada.
    
- **`GL_LINE_STRIP`**: Conecta los vértices de forma continua (1 con 2, 2 con 3, 3 con 4) formando una ruta abierta.
    
- **`GL_LINE_LOOP`**: Igual que el anterior, pero al final conecta automáticamente el último vértice con el primero, cerrando la figura.
    
- **`GL_TRIANGLES`**: Toma los vértices en grupos de tres para formar triángulos independientes y rellenos.
    
- **`GL_QUADS`**: Toma los vértices en grupos de cuatro para formar cuadriláteros rellenos. _(Nota: Está obsoleto en versiones modernas de OpenGL, pero es muy común en GLUT clásico)._
    
- **`GL_POLYGON`**: Forma un solo polígono convexo relleno utilizando todos los vértices proporcionados.

### 2. La Familia de Vértices: Variaciones de `glVertex`

Como mencionamos brevemente antes, la estructura del comando es `glVertex` + **\[Cantidad de dimensiones]** + **\[Tipo de dato]**.

**Por número de dimensiones:**

- `glVertex2...`: Espacio 2D ($X, Y$). El eje $Z$ se asume como 0.
    
- `glVertex3...`: Espacio 3D ($X, Y, Z$).
    
- `glVertex4...`: Espacio 4D ($X, Y, Z, W$). Usa coordenadas homogéneas (avanzado).
    

**Por tipo de dato:**

- **`f` (GLfloat):** Números con decimales (ej. `glVertex2f(1.5f, -0.8f)`). Es el estándar de la industria.
    
- **`d` (GLdouble):** Flotantes de doble precisión (ej. `glVertex2d(1.55555, -0.88888)`). Útil para simulaciones científicas que requieren alta precisión espacial.
    
- **`i` (GLint):** Números enteros (ej. `glVertex2i(10, 25)`). Ideal para trabajar en "espacio de pantalla" puro, como en tus algoritmos DDA y Bresenham.
    
- **`s` (GLshort):** Enteros cortos (menos uso de memoria).

### 3. Interruptores del Motor: `glEnable()` y `glDisable()`

Estos controlan las capacidades avanzadas de la máquina de estados.

- **`GL_LINE_STIPPLE`**: Activa los patrones (punteado/discontinuo) en las líneas. Requiere definir el patrón con `glLineStipple`.
    
- **`GL_POINT_SMOOTH` / `GL_LINE_SMOOTH`**: Activa el _antialiasing_. Hace que los puntos dejen de verse como cuadrados duros y pasen a ser círculos, y que las líneas diagonales no tengan bordes de "escalera".
    
- **`GL_BLEND`**: Activa la mezcla de colores (transparencia). Generalmente se usa junto con la función `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)` para que OpenGL sepa cómo mezclar el fondo con la figura translúcida.
    
- **`GL_DEPTH_TEST`**: Activa el Z-buffer. Fundamental en 3D; le dice a OpenGL que calcule qué objetos están cerca de la cámara y oculte los que están detrás, sin importar el orden en el que los dibujaste en el código.
    
- **`GL_CULL_FACE`**: Optimización. Le dice a OpenGL que no dibuje las caras traseras de los objetos 3D (las que no apuntan a la cámara).

### 4. Atributos de Estilo Comunes

Estos son los modificadores de estado que dictan cómo se verán las primitivas una vez que las dibujes.

**Para el color:**

- **`glColor3f(R, G, B)`**: Define el color usando Rojo, Verde y Azul con valores de 0.0 a 1.0. (ej. Blanco es `1.0, 1.0, 1.0`).
    
- **`glColor4f(R, G, B, A)`**: Añade el canal "Alpha" para la opacidad. `1.0` es sólido, `0.5` es semitransparente (requiere `GL_BLEND` activado).
    

**Para la geometría:**

- **`glPointSize(float)`**: Define el diámetro de un `GL_POINT` en píxeles. (ej. `glPointSize(5.0f)`).
    
- **`glLineWidth(float)`**: Define el grosor de las líneas dibujadas. (ej. `glLineWidth(2.5f)`).