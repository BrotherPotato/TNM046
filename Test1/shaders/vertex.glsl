#version 330 core

layout(location = 0) in vec3 Position;

// --- Add this to the declarations in the vertex shader
layout(location = 1) in vec3 Color;
out vec3 interpolatedColor;

void main() {
	gl_Position = vec4(Position, 1.0);
	// And somewhere in its main() function, add this:
	interpolatedColor = Color; 
}