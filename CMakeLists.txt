cmake_minimum_required(VERSION 3.10)

# Nombre de tu proyecto
project(SistemaRenderizado_PBR)

# Forzar el estándar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Evitar advertencias del sistema de OpenGL en Linux
set(OpenGL_GL_PREFERENCE GLVND)

# ---------------------------------------------------------
# 1. BUSCAR LIBRERÍAS INSTALADAS EN EL CODESPACE (APT-GET)
# ---------------------------------------------------------
find_package(OpenGL REQUIRED)
# Assimp nos permite cargar modelos .obj y .gltf
find_package(assimp REQUIRED)

# Nota sobre GLM: Como es "header-only", se instala en /usr/include 
# y el compilador de GCC lo encuentra automáticamente. No necesita find_package.

# ---------------------------------------------------------
# 2. RECOPILAR ARCHIVOS FUENTE
# ---------------------------------------------------------
# Busca automáticamente todos los archivos .cpp en tu carpeta src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Recopilar los archivos de ImGui (La interfaz gráfica)
# ImGui no se instala, se compila directamente junto con tu proyecto.
file(GLOB IMGUI_SOURCES 
    "vendor/imgui/*.cpp"
    "vendor/imgui/backends/imgui_impl_glfw.cpp"
    "vendor/imgui/backends/imgui_impl_opengl3.cpp"
)

# ---------------------------------------------------------
# 3. CREAR EL EJECUTABLE
# ---------------------------------------------------------
# Combina tus clases y las de ImGui en un solo programa llamado "app"
add_executable(app ${SOURCES} ${IMGUI_SOURCES})

# ---------------------------------------------------------
# 4. CONFIGURAR LAS RUTAS DE LOS ENCABEZADOS (.h)
# ---------------------------------------------------------
# Le decimos al compilador dónde ir a buscar cuando uses #include "..."
target_include_directories(app PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/imgui/backends
)

# ---------------------------------------------------------
# 5. ENLAZAR LIBRERÍAS DINÁMICAS (LINKER)
# ---------------------------------------------------------
# Conectamos las librerías gráficas y matemáticas al ejecutable final
target_link_libraries(app 
    PRIVATE 
    glfw        # Ventanas modernas
    GL          # OpenGL Core
    GLEW        # Extensiones de OpenGL
    glut        # FreeGLUT (por si usan primitivas)
    assimp      # Carga de modelos 3D
)