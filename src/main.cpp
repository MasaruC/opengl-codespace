#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Incluimos las librerías matemáticas y de modelos solo para probar que CMake las enlaza bien
#include <glm/glm.hpp>
#include <assimp/Importer.hpp> 

// Librerías de ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int main() {
    // 1. Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    // Configurar OpenGL 3.3 Core Profile (Ideal para los shaders de PBR)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Motor PBR - Prueba de Arquitectura", NULL, NULL);
    if (!window) {
        std::cerr << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 2. Inicializar GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW" << std::endl;
        return -1;
    }

    // 3. Configurar Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // Estilo visual oscuro por defecto
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Variable para cambiar el color de fondo desde la interfaz
    ImVec4 clear_color = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);

    // 4. Bucle principal de renderizado
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Iniciar un nuevo frame de ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Crear una ventana flotante de ImGui
        ImGui::Begin("Panel de Control");
        ImGui::Text("¡El entorno con CMake funciona perfectamente!");
        ImGui::Separator();
        ImGui::Text("Prueba cambiar el color del fondo:");
        // Widget interactivo para elegir colores
        ImGui::ColorEdit3("Color", (float*)&clear_color); 
        ImGui::End();

        // Renderizar todo
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        
        // Aplicar el color de fondo elegido en ImGui
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Dibujar la interfaz sobre el fondo
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 5. Limpieza de memoria al cerrar
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}