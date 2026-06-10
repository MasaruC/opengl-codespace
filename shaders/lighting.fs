#version 330 core
out vec4 FragColor;


// Variables Interpoladas: Al pasar por la Etapa 3 (Rasterización),
// la GPU interpoló estas variables. Si el triángulo tenía un vértice rojo
// y otro azul, el fragmento en el medio recibe un tono morado.
in vec3 FragPos;
in vec3 Normal;
in vec3 GouraudColor;

// Variables Globales: Son los datos enviados directamente desde el código
// C++ (la CPU) que se mantienen constantes para toda la malla durante 
// este fotograma: la posición de la cámara (viewPos), la luz, los coeficientes
// de los materiales ($k_a$, $k_d$, $k_s$), el brillo o "shininess" ($n$), y los selectores de modo.
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform int shadingMode;
uniform float ka, kd, ks, n;
uniform bool useBlinn;

void main()
{
    // MODO GOURAUD: El color ya vino calculado desde el Vertex Shader
    // GouraudColor: Contiene el color de iluminación calculado en el Vertex Shader
    // objectColor: Es un color uniforme del material o textura del objeto
    // GouraudColor * objectColor: Representa la multiplicación del color de la luz por el color del objeto
    if(shadingMode == 1) {
        // FragColor = vec4(R, G, B, A);
        FragColor = vec4(GouraudColor * objectColor, 1.0);
        // La multiplicación representa cómo la luz (GouraudColor)
        // interactúa con el material del objeto (objectColor).
        return;
    }

    // Calcular la normal adecuada
    vec3 norm;
    if(shadingMode == 0) {
        // MODO FLAT SHADING: Recalcula la normal usando derivadas de pantalla
        // Esto ignora las normales suavizadas del modelo y crea caras planas
        vec3 dx = dFdx(FragPos);
        vec3 dy = dFdy(FragPos);
        norm = normalize(cross(dx, dy));// el producto cruz resulta en vectores completamente perpendiculares
    } else {
        // MODO PHONG: Usa la normal interpolada píxel a píxel y la normaliza
        norm = normalize(Normal);
    }

    // Vectores de Iluminación
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 1. Componente Ambiental
    // ka: Coeficiente de refleccion ambiental, es una propiedad del material que dicta
    // que tanta luz ambiental un objeto es capaz de reflejar (entre 0 y 1)
    vec3 ambient = ka * lightColor;

    // 2. Componente Difusa
    // Determina qué tan directamente impacta la luz sobre la superficie.
    float diff = max(dot(norm, lightDir), 0.0);
    // kd: Define el color base del objeto y el procentaje de luz difusa que refleja
    vec3 diffuse = kd * diff * lightColor;

    // 3. Componente Especular (Ecuación de Blinn-Phong / Phong clásico)
    float spec = 0.0;
    if(useBlinn) {
        // Vector Medio (H) de Blinn-Phong
        // Soluciona el problema de corte de Phong. En lugar de calcular el rebote,
        // calcula el Vector Medio ($H$), que se sitúa exactamente a la mitad entre 
        // la dirección de la luz y la visión ($H = normalize(L + V)$). Luego compara 
        // la normal $N$ con este vector $H$. Es computacionalmente más rápido y 
        // produce brillos más suaves en ángulos extremos.
        // $I_{specular} = \max(N \cdot H, 0)^n$
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(norm, halfwayDir), 0.0), n);
    } else {
        // Vector de Reflexión (R) de Phong clásico
        // Calcula el vector de reflexión $R$ usando la luz rebotando en la normal.
        // Luego compara $R$ con la dirección de visión $V$. El problema de Phong 
        // es que si el ángulo entre $V$ y $R$ supera los 90 grados, el producto punto
        // se vuelve negativo y el brillo se corta de forma abrupta.
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), n);
    }
    vec3 specular = ks * spec * lightColor;

    // Resultado final
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
