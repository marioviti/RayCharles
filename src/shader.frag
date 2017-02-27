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

const vec3 inputLightPosition = vec3 (5.0, 5.0, 5.0);
const vec4 inputLightMaterial = vec4 (1.0, 1.0, 1.0 , 1.0); // white light
const bool lightIsInCamSpace = true;

const vec4 inputObjectAmbiantMaterial = vec4 (0.1, 0.2, 0.1 , 1.0); // green-ish
const vec4 inputObjectDiffuseMaterial = vec4 (0.3, 0.3, 0.4 , 1.0); // blue-ish
const vec4 inputObjectSpecularMaterial = vec4 (1.0, 1.0, 1.0 , 1.0); // white
const float inputObjectShininess = 50.0;

varying vec4 P; // Interpolated fragment-wise position
varying vec3 N; // Interpolated fragment-wise normal
varying vec4 C; // Interpolated fragment-wise color

void main (void) {
    // PUT EVERY QUANTITY IN CAMERA SPACE!
    vec4 PModelview = gl_ModelViewMatrix * P;
    vec3 position = vec3 (PModelview.xyz / PModelview.w);
    vec3 normal = normalize(gl_NormalMatrix * N);
    vec3 directionToCamera = normalize(-position);
    vec3 lightPosition = inputLightPosition;
    if( !lightIsInCamSpace ) {
        vec4 LModelview = gl_ModelViewMatrix * vec4(lightPosition.xyz,1.0);
        lightPosition = vec3 (LModelview.xyz) / LModelview.w;
    }
    vec3 directionToLight = normalize(lightPosition - position);
    vec3 reflectedRayDirection = normalize( - directionToLight + 2.0*dot(directionToLight , normal) * normal );

    // ---------- Code to change -------------
    //vec4 color = C;
    /*
    // This may be usefullo to check movements and camera/model relative coords
    float r = PModelview.x;
    float g = PModelview.y;
    float b = PModelview.z;
    vec4 color = vec4 (r, g, b, 1.0); // notice blue on the z axis so it's never visible (must invert)
    */
    // Coutput = C frag ∗ (Clight ∗ Cambient)
    // Coutput = C frag ∗ Clight * ( Cambient + Cdiffus max(0, n · l))
    float theta;
    vec4 color;

    theta = max(0,dot(normal,directionToLight));
    color = inputObjectAmbiantMaterial + inputObjectDiffuseMaterial*theta;
    color *= inputLightMaterial * C;
    // ----------------------------------------
    gl_FragColor = color;
}
