# 🖥️ C++ OpenGL Development Environment

![OpenGL](https://img.shields.io/badge/OpenGL-4.5-green?logo=opengl)
![C++](https://img.shields.io/badge/C++-17-blue?logo=c%2B%2B)
![GitHub Codespaces](https://img.shields.io/badge/GitHub_Codespaces-Ready-black?logo=github)

## ✨ Descripción

Entorno de desarrollo para **programación de gráficos 3D en C++** usando **OpenGL**, para **GitHub Codespaces**. Incluye las librerías esenciales y la infraestructura necesaria para renderizar gráficos directamente en tu navegador.

---

## 🛠️ Stack Tecnológico

### 📦 Librerías Gráficas

| Librería | Versión | Propósito | Flag de compilación |
|----------|---------|-----------|---------------------|
| **Mesa 3D** | OpenGL Core | Motor de renderizado principal, implementación open-source de OpenGL para Linux | `-lGL` |
| **FreeGLUT** | 3.x | Utilidades clásicas: creación de ventanas, manejo de eventos, primitivas (`glBegin`, `glVertex2f`) | `-lglut` |
| **GLFW** | 3.x | Alternativa moderna y ligera para gestión de ventanas e input | `-lglfw` |
| **GLEW** | 2.x | Carga dinámica de extensiones y funciones modernas de OpenGL (shaders, buffers) | `-lGLEW` |

### 🏗️ Infraestructura de Visualización

| Componente | Función |
|------------|---------|
| **Xorg** (`xorg-dev`) | Servidor de display base de Linux. Intermediario entre las librerías OpenGL y el sistema operativo para renderizar ventanas |
| **Desktop-lite + noVNC** | Contenedor VNC integrado que actúa como monitor virtual. Permite visualizar la salida gráfica en el navegador through puerto `6080` |

---

## 🚀 Quick Start

### 1. Abrir el Codespace

```bash
# Click en "Use this template" → "Open in Codespace"
# O clona el repositorio:
git clone <tu-repo-url>
cd <nombre-del-repo>
codespace create
```

### 2. Acceder al Entorno Gráfico

Una vez iniciado el Codespace:

1. Abre la pestaña **"Ports"** (icono de red ⚡)
2. Localiza el puerto **6080**
3. Click en la dirección URL que aparece
4. ¡Se abrirá una nueva pestaña con tu escritorio virtual! 🖥️

### 3. Compilar y Ejecutar

#### Ejemplo con FreeGLUT (Clásico):

```cpp
// main.cpp
#include <GL/glut.h>
#include <cstdlib>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 1.0f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(1.0f, -1.0f);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("OpenGL Test");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
```

**Compilar:**
```bash
g++ main.cpp -o app -lGL -lglut && ./app
```

#### Ejemplo con GLFW + GLEW (Moderno):

```cpp
// modern.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Modern OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    glewInit(); // Carga funciones modernas
    
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
```

---

## 📁 Estructura del Proyecto

```
.
├── .devcontainer/
│   └── devcontainer.json    # Configuración del Codespace
├── .vscode/
│   └── launch.jason
│   └── task.jason
├── build
├── CMakeListis.txt
├── main
├── main.cpp
└── README.md                # Este archivo
```

---

## ⚙️ Configuración del Dev Container

De `.devcontainer/devcontainer.json`:

```json
{
  "name": "C++ OpenGL Environment",
  "image": "mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04",
  "features": {
    "ghcr.io/devcontainers/features/desktop-lite:1": {}
  },
  "postCreateCommand": "sudo apt-get update && sudo apt-get install -y libgl1-mesa-dev libglew-dev libglfw3-dev xorg-dev freeglut3-dev",
  "customizations": {
    "vscode": {
      "extensions": [
        "ms-vscode.cpptools",
        "ms-vscode.cpptools-extension-pack"
      ]
    }
  },
  "forwardPorts": [6080],
  "portsAttributes": {
    "6080": {
      "label": "Escritorio Virtual",
      "onAutoForward": "openBrowser"
    }
  }
}
```

---

## 🎓 Recursos de Aprendizaje

### Tutoriales Recomendados:
- 📘 [LearnOpenGL](https://learnopengl.com/) - Tutorial exhaustivo de OpenGL moderno
- 📗 [OpenGL Wiki](https://www.khronos.org/opengl/wiki/) - Documentación oficial
- 📙 [FreeGLUT Documentation](https://freeglut.sourceforge.net/docs/api.php) - Referencia de la API clásica
- 📕 [GLFW Guide](https://www.glfw.org/docs/latest/) - Guía oficial de GLFW

### Conceptos Clave:
```
Pipeline de Renderizado:
[Vertices] → [Vertex Shader] → [Rasterization] → [Fragment Shader] → [Pantalla]
     ↑              ↑                    ↑                    ↑
  Datos de       Transformaciones      Píxeles             Color final
  geometría      3D → 2D            generados
```
---

## 🤖 Aviso de uso de Inteligencia Artificial

Este proyecto (o partes de este proyecto) ha sido desarrollado con la asistencia de herramientas de Inteligencia Artificial. El código y los contenidos han sido revisados y adaptados para cumplir con los objetivos del proyecto.
