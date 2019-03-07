#version 330 core
precision lowp float;

layout(location = 0) in vec3 iPosition;

uniform mat4 uMVPMatrix;

void main(void) {	
	gl_Position = uMVPMatrix * vec4(iPosition, 1.0);
}