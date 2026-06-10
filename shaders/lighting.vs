#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 GouraudColor; // Para pasar el color si es modo Gouraud

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int shadingMode; // 0=Flat, 1=Gouraud, 2=Phong

// Uniforms de luz y material (necesarios si calculamos Gouraud aquí)
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform float ka, kd, ks, n;
uniform bool useBlinn;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    
    // LA MATRIZ NORMAL: Transpuesta de la inversa del modelo.
    // Evita que los escalados no uniformes deformen las normales.
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // --- CÁLCULO GOURAUD (Si aplica) ---
    if(shadingMode == 1) {
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        vec3 viewDir = normalize(viewPos - FragPos);
        
        // Ambiental
        vec3 ambient = ka * lightColor;
        // Difusa
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = kd * diff * lightColor;
        // Especular (Blinn o Phong clásico)
        vec3 halfwayDir = normalize(lightDir + viewDir);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = useBlinn ? pow(max(dot(norm, halfwayDir), 0.0), n) 
                              : pow(max(dot(viewDir, reflectDir), 0.0), n);
        vec3 specular = ks * spec * lightColor;

        GouraudColor = ambient + diffuse + specular;
    }

    gl_Position = projection * view * worldPos;
}