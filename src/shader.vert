// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

varying vec4 P; // Per-vertex position
varying vec3 N; // Per-vertex normal
varying vec4 C; // Per-vertex color

// varying vec2 vTexCoord;

void main(void) {
    P = gl_Vertex;
    N = gl_Normal;
    C = gl_Color;
    gl_Position = ftransform();
    /*
    Pass to the fragmente shader the UV
    vTexCoord = gl_MultiTexCoord0;
    glTexCoord[0] = gl_MultiTexCoord0;
    */

}
