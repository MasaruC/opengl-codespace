#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 GouraudColor;

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
    if(shadingMode == 1) {
        FragColor = vec4(GouraudColor * objectColor, 1.0);
        return;
    }

    // Calcular la normal adecuada
    vec3 norm;
    if(shadingMode == 0) {
        // MODO FLAT SHADING: Recalcula la normal usando derivadas de pantalla
        // Esto ignora las normales suavizadas del modelo y crea caras planas
        vec3 dx = dFdx(FragPos);
        vec3 dy = dFdy(FragPos);
        norm = normalize(cross(dx, dy));
    } else {
        // MODO PHONG: Usa la normal interpolada píxel a píxel
        norm = normalize(Normal);
    }

    // Vectores de Iluminación
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 1. Componente Ambiental
    vec3 ambient = ka * lightColor;

    // 2. Componente Difusa
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = kd * diff * lightColor;

    // 3. Componente Especular (Ecuación de Blinn-Phong / Phong clásico)
    float spec = 0.0;
    if(useBlinn) {
        // Vector Medio (H) de Blinn-Phong
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(norm, halfwayDir), 0.0), n);
    } else {
        // Vector de Reflexión (R) de Phong clásico
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), n);
    }
    vec3 specular = ks * spec * lightColor;

    // Resultado final
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}