// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

// Add here all the values you need to describe the lights or the materials.
// At first used const values, eventually stored in structures.
// Then, use uniform variables and set them from your CPU program using
// the GLProgram methods.

uniform int lightIsInCamSpace;
uniform vec3 diffuse_color;
uniform vec3 ambient_color;
uniform vec3 specular_color;
uniform vec3 light_color;
uniform vec3 inputLightPosition;
uniform float specular_intensity;
uniform int has_texture;

vec4 inputLightMaterial = vec4 (light_color.xyz , 1.0);
vec4 inputObjectAmbientMaterial = vec4 (ambient_color.xyz , 1.0);
vec4 inputObjectDiffuseMaterial = vec4 (diffuse_color.xyz, 1.0);
vec4 inputObjectSpecularMaterial = vec4 (specular_color.xyz, 1.0);

varying vec4 P; // Interpolated fragment-wise position
varying vec3 N; // Interpolated fragment-wise normal
varying vec4 C; // Interpolated fragment-wise color

//TEXTURE
uniform sampler2D uTextureColor;

void main (void) {

    //TEXTURE
    vec4 texture_color;
    vec2 fragUV;
    vec4 color1;

    // PUT EVERY QUANTITY IN CAMERA SPACE!
    vec4 PModelview = gl_ModelViewMatrix * P;
    vec3 position = vec3 (PModelview.xyz / PModelview.w);
    vec3 normal = normalize(gl_NormalMatrix * N);
    vec3 directionToCamera = normalize(-position); // on est deja en espace camera
    vec3 lightPosition = inputLightPosition;
    if( lightIsInCamSpace != 1) {
        vec4 LModelview = gl_ModelViewMatrix * vec4(lightPosition.xyz,1.0);
        lightPosition = vec3 (LModelview.xyz)/LModelview.w;
    }
    vec3 directionToLight = normalize(lightPosition - position);
    vec3 reflectedRayDirection = normalize( - directionToLight + 2.0*dot(directionToLight , normal) * normal );

    // RENDERING EQUATION
    // Coutput = C frag ∗ (Clight ∗ Cambient)
    // Coutput = C frag ∗ Clight * ( Cambient + Cdiffus * max(0, n · l))
    // Coutput = C frag ∗ Clight ∗ ( Cambient + Cdiffus * max(0, n · l) + Cspecular.max(0, r · v)^{s}) )
    float theta,sigma;
    vec4 color;
    theta = max(0,dot(normal,directionToLight));
    sigma = pow(max(0,dot(reflectedRayDirection,directionToCamera)),specular_intensity);
    if(has_texture == 1) {
      fragUV = gl_TexCoord[0].xy;
      color1 = texture2D(uTextureColor,fragUV);
      color = C * inputLightMaterial * (inputObjectAmbientMaterial + color1*theta + inputObjectSpecularMaterial*sigma);
    }
    else
      color = C * inputLightMaterial * (inputObjectAmbientMaterial + inputObjectDiffuseMaterial*theta + inputObjectSpecularMaterial*sigma);

    // ----------------------------------------
    gl_FragColor = color;
}
