#version 330 core

uniform float time;

uniform mat4 V;

layout(location = 0) in vec3 Position;

// --- Add this to the declarations in the vertex shader
layout(location = 1) in vec3 Color;
out vec3 interpolatedColor;

void main() {
	gl_Position = V * vec4(Position * 0.5, 2.0); //sin(time)
	
	interpolatedColor = Color; //sin(time) 
}