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
find_package(assimp REQUIRED)
find_package(GLEW REQUIRED) # Se añade para usar el target moderno

# Nota sobre GLM: Al ser header-only, GCC lo encuentra automáticamente en /usr/include.

# ---------------------------------------------------------
# 2. RECOPILAR ARCHIVOS FUENTE
# ---------------------------------------------------------
# Busca automáticamente todos los archivos .cpp en tu carpeta src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Recopilar los archivos de ImGui (La interfaz gráfica)
file(GLOB IMGUI_SOURCES 
    "vendor/imgui/*.cpp"
    "vendor/imgui/backends/imgui_impl_glfw.cpp"
    "vendor/imgui/backends/imgui_impl_opengl3.cpp"
)

# ---------------------------------------------------------
# 3. CREAR EL EJECUTABLE
# ---------------------------------------------------------
add_executable(app ${SOURCES} ${IMGUI_SOURCES})

# ---------------------------------------------------------
# 4. CONFIGURAR LAS RUTAS DE LOS ENCABEZADOS (.h)
# ---------------------------------------------------------
target_include_directories(app PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/imgui/backends
)

# ---------------------------------------------------------
# 5. ENLAZAR LIBRERÍAS DINÁMICAS (LINKER) - CORREGIDO
# ---------------------------------------------------------
# Usamos los nombres de "Target" modernos de CMake para evitar errores de linker
target_link_libraries(app 
    PRIVATE 
    glfw              # Ventanas
    GLEW::GLEW        # Extensiones de OpenGL (Target moderno)
    OpenGL::GL        # OpenGL Core (Target moderno desde find_package)
    assimp::assimp    # Carga de modelos 3D (Target moderno)
)