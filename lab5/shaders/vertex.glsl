#version 330 core

uniform float time;
uniform mat4 T;

uniform mat4 MV;
uniform mat4 P;

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoord;

// --- Add this to the declarations in the vertex shader
out vec3 interpolatedNormal;
out vec2 st;

void main() {
	vec3 transformedNormal = mat3(MV) *  Normal; //mat3(MV) * 
	interpolatedNormal = normalize(transformedNormal);
	// P * MV
	gl_Position = P * MV * vec4(Position, 1.0); // Special, required output // MV * 
	st = TexCoord; // Will also be interpolated across the triangle
}