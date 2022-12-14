#version 330 core

uniform float time;
uniform mat4 V;

out vec4 finalcolor;

in vec3 interpolatedColor;


void main() {
	// finalcolor = vec4(1.0, 0.5, 0.3, 1.0);
	// And in its main() function, set the output color like this:

	finalcolor = vec4(interpolatedColor, 1.0); // sin(time)
}