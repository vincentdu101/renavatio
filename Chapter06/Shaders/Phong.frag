// request GLSL 3.3
#version 330

// inputs from vertex shader
// tex coord
in vec2 fragTexCoord;

// normal (in world space)
in vec3 fragNormal;

// position in world space
in vec3 fragWorldPos;

// corresponds to the output color to the color buffer
out vec4 outColor;

// this is used for the texture sampling
uniform sampler2D uTexture;

// create a struct for directional light
struct DirectionalLight
{
    // direction of light
    vec3 mDirection;
    
    // diffuse color
    vec3 mDiffuseColor;
    
    // specular color
    vec3 mSpecColor;
};

// create a struct for point light
struct PointLight
{
    // direction of light
    vec3 mDirection;
    
    // diffuse color
    vec3 mDiffuseColor;
    
    // specular color
    vec3 mSpecColor;
    
    // position of light
    vec3 mPosition;
};

// uniforms for lighting
// camera position in world space
uniform vec3 uCameraPos;

// specular power for this surface
uniform float uSpecPower;

// ambient light level
uniform vec3 uAmbientLight;

// directional light
uniform DirectionalLight uDirLight;

// point lights
uniform PointLight uPointLights[4];

uniform float uPointLightsRadius;

void main()
{
    // surface normal
    vec3 N = normalize(fragNormal);
    // vector from surface to light
    vec3 L = normalize(-uDirLight.mDirection);
    // vector from surface to camera
    vec3 V = normalize(uCameraPos - fragWorldPos);
    // reflection of -L and N
    vec3 R = normalize(reflect(-L, N));
    
    // compute phong reflection
    vec3 Phong = uAmbientLight;
    float NdotL = dot(N, L);
    float specPower = pow(max(0.0, dot(R, V)), uSpecPower);
    
    if (NdotL > 0) {
        vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
        vec3 Specular = uDirLight.mSpecColor * specPower;
        Phong += Diffuse + Specular;
    }
    
    // compute point light effect if the object is in the radius of the
    // light
    for (int i = 0; i < uPointLights.length(); i++) {
        PointLight light = uPointLights[i];
        bool xClose = light.mPosition.x - fragNormal.x < uPointLightsRadius;
        bool yClose = light.mPosition.y - fragNormal.y < uPointLightsRadius;
        bool zClose = light.mPosition.z - fragNormal.z < uPointLightsRadius;

        if (xClose && yClose && zClose) {
            vec3 PL = normalize(-light.mDirection);
            vec3 PR = normalize(reflect(-PL, N));
            float NdotPL = dot(N, PL);
            
            if (NdotPL > 0) {
                vec3 pointDiffuse = light.mDiffuseColor * NdotPL;
                vec3 pointSpecular = light.mSpecColor * specPower;
                Phong += pointDiffuse + pointSpecular;
            }
        }
    }
    
    // final color is texture color times phong light (alpha = 1)
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
