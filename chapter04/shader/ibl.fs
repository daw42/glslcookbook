#version 400

in vec3 ViewDir;   // View direction in world coordinates
in vec3 Normal;    // Normal in world coordinates

uniform samplerCube CubeMapTex;  // The env. map
uniform samplerCube DiffuseMap;  // The diffuse env. map
uniform samplerCube SpecMap;     // The specular env. map

uniform bool DrawSkyBox;

struct MaterialInfo {
    vec3 BaseColor;
    float DiffuseFactor;
    float SpecFactor;
};
uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

void main() {

    // Access the irradiance maps
    vec4 diffuseIrr = texture(DiffuseMap, Normal);
    vec4 specIrr = texture(SpecMap, ViewDir);

    if( DrawSkyBox )
        FragColor = texture(CubeMapTex, normalize( reflect(-ViewDir,Normal) ) );
    else {

        // Compute the diffuse component by mixing with the base color
        vec3 color = mix( Material.BaseColor,
                          Material.BaseColor * diffuseIrr,
                          Material.DiffuseFactor );

        // The specular component is added to the diffuse color
        color = color + specIrr * Material.SpecFactor;

        FragColor = vec4(color, 1.0);
    }
}
