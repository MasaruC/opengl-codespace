#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"
#include "Camera.h"

// ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cmath>
#include <deque>

// Variables globales básicas para los callbacks
Camera camera;
float lastX = 400, lastY = 300;
bool firstMouse = true;

// Callback de tamaño de ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Callback de ratón
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

int main() {
    // 1. INICIALIZAR GLFW Y GLEW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Modelos de Iluminacion Local", NULL, NULL);
    if (!window) {
        std::cout << "Fallo al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Inicializar GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Fallo al inicializar GLEW" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // 2. INICIALIZAR IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // 3. CREAR SHADER
    Shader ourShader("../shaders/lighting.vs", "../shaders/lighting.fs"); 

    // 4. GENERADOR DE ESFERA PROCEDURAL
    std::vector<float> vertices;
    float radius = 1.0f;
    int slices = 32;
    int stacks = 32;
    
    for (int i = 0; i < stacks; i++) {
        float theta1 = i * M_PI / stacks;
        float theta2 = (i + 1) * M_PI / stacks;
        
        for (int j = 0; j < slices; j++) {
            float phi1 = j * 2 * M_PI / slices;
            float phi2 = (j + 1) * 2 * M_PI / slices;
            
            auto addVertex = [&](float theta, float phi) {
                float x = sin(theta) * cos(phi) * radius;
                float y = cos(theta) * radius;
                float z = sin(theta) * sin(phi) * radius;
                // Posición (x,y,z) + Normal (x,y,z)
                vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
                vertices.push_back(x / radius); vertices.push_back(y / radius); vertices.push_back(z / radius);
            };
            
            addVertex(theta1, phi1); addVertex(theta2, phi1); addVertex(theta2, phi2);
            addVertex(theta1, phi1); addVertex(theta2, phi2); addVertex(theta1, phi2);
        }
    }

    // Cantidad total de vértices a dibujar
    int totalVerticesToDraw = vertices.size() / 6; 

    // Configurar VAO y VBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Atributo 0: Posiciones
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributo 1: Normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0); 

    // 5. BUCLE DE RENDERIZADO (RENDER LOOP)
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input de teclado
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(0, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(1, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(2, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(3, deltaTime);

        // Limpiar pantalla
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- VARIABLES ESTÁTICAS DE IMGUI ---
        static int shadingMode = 2; // 0=Flat, 1=Gouraud, 2=Phong
        static bool useBlinn = true;
        static float ka = 0.1f;
        static float kd = 0.6f;
        static float ks = 0.8f;
        static float shininess = 32.0f;

        // --- CÁLCULO DE RENDIMIENTO (VENTANA DESLIZANTE DE 5 SEGUNDOS) ---
        static float fps = 0.0f;
        static float frameTimeMs = 0.0f;
        
        // Para el FPS actual (cada medio segundo)
        static float fpsAccumulator = 0.0f;
        static int frameCount = 0;

        // Para el FPS Promedio (Ventana deslizante exacta de 5 segundos)
        static std::deque<float> frameTimeWindow; // Guarda los deltaTime recientes
        static float windowTimeSum = 0.0f;
        const float WINDOW_SIZE = 5.0f; // 5 segundos de historial
        
        // --- DETECCIÓN DE CAMBIO DE MODO (Reinicia la ventana) ---
        static int prevShadingMode = shadingMode;
        if (shadingMode != prevShadingMode) {
            frameTimeWindow.clear();
            windowTimeSum = 0.0f;
            prevShadingMode = shadingMode;
        }
        
        // 1. Añadir el frame actual a la ventana
        frameTimeWindow.push_back(deltaTime);
        windowTimeSum += deltaTime;
        
        // 2. Eliminar los frames más viejos si pasamos de 5 segundos
        while (windowTimeSum > WINDOW_SIZE && !frameTimeWindow.empty()) {
            windowTimeSum -= frameTimeWindow.front();
            frameTimeWindow.pop_front();
        }
        
        // 3. Calcular FPS "Instantáneo" (cada medio segundo)
        fpsAccumulator += deltaTime;
        frameCount++;
        if (fpsAccumulator >= 0.5f) { 
            fps = frameCount / fpsAccumulator;
            frameTimeMs = (fpsAccumulator / frameCount) * 1000.0f;
            frameCount = 0;
            fpsAccumulator = 0.0f;
        }
        
        // 4. Calcular el promedio EXACTO de los últimos 5 segundos
        float avgFps = (windowTimeSum > 0.0f) ? (frameTimeWindow.size() / windowTimeSum) : 0.0f;
        // --- INTERFAZ IMGUI ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::SetNextWindowSize(ImVec2(320, 350), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        
        ImGui::Begin("Analisis de Iluminacion Local - Cap 3");

        // SECCIÓN 3.4: Métricas de Rendimiento
        ImGui::TextColored(ImVec4(0, 1, 0, 1), ">>> METRICAS DE RENDIMIENTO <<<");
        ImGui::Text("FPS Actual: %.1f", fps);
        ImGui::Text("FPS Promedio (5s): %.1f", avgFps); // <--- ACTUALIZADO
        ImGui::Text("Tiempo/frame: %.3f ms", frameTimeMs);
        ImGui::Separator();

        ImGui::TextColored(ImVec4(0.5f, 0.5f, 1, 1), "3.3 ALGORITMOS DE INTERPOLACION");
        const char* modes[] = { "Flat Shading", "Gouraud Shading", "Phong Shading (Pixel)" };
        ImGui::Combo("Modo de Sombreado", &shadingMode, modes, IM_ARRAYSIZE(modes));
        ImGui::Separator();

        ImGui::TextColored(ImVec4(0.5f, 0.5f, 1, 1), "3.2 MODELOS DE REFLECTANCIA");
        if (shadingMode != 1) { 
            ImGui::Checkbox("Usar Blinn-Phong (Vector H)", &useBlinn);
            if (!useBlinn) {
                ImGui::TextColored(ImVec4(1,1,0,1), "  -> Phong Clasico (Vector R)");
            } else {
                ImGui::TextColored(ImVec4(0,1,0,1), "  -> Blinn-Phong Activo");
            }
        } else {
            ImGui::TextDisabled("Gouraud usa Phong por defecto en VS");
        }
        ImGui::Separator();

        ImGui::Text("Coeficientes (Ecuacion de Reflexion):");
        ImGui::SliderFloat("Ambiental (ka)", &ka, 0.0f, 1.0f);
        ImGui::SliderFloat("Difusa (kd)", &kd, 0.0f, 1.0f);
        ImGui::SliderFloat("Especular (ks)", &ks, 0.0f, 1.0f);
        ImGui::SliderFloat("Exponente (n)", &shininess, 1.0f, 256.0f);

        ImGui::End();

        // --- DIBUJAR ESCENA ---
        ourShader.use();
        
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        float aspectRatio = (float)screenWidth / (float)screenHeight;

        glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);
        glm::mat4 view = camera.GetViewMatrix();
        
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        ourShader.setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.8f));
        ourShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
        ourShader.setVec3("viewPos", camera.Position);

        // Enviar variables de ImGui al Shader
        ourShader.setInt("shadingMode", shadingMode);
        ourShader.setFloat("ka", ka);
        ourShader.setFloat("kd", kd);
        ourShader.setFloat("ks", ks);
        ourShader.setFloat("n", shininess);
        ourShader.setInt("useBlinn", useBlinn); 

        // BUCLE DE ESTRES PARA MEDIR FPS (Sección 3.4)
        glBindVertexArray(VAO);
        for(int i = 0; i < 20; i++) { 
            glm::mat4 model = glm::mat4(1.0f);
            // Posiciona las esferas en diagonal
            model = glm::translate(model, glm::vec3(i * 2.5f, 0.0f, -i * 2.5f)); 
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, totalVerticesToDraw); 
        }

        // --- RENDERIZAR IMGUI ENCIMA ---
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpieza
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}