#version 330 core
precision lowp float;

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec2 iTexture;
layout(location = 0) out vec2 oTexture;

uniform mat4 uMVPMatrix;

void main(void) {	
	gl_Position = uMVPMatrix * vec4(iPosition, 1.0);
	oTexture = iTexture;
	oTexture.y = 1.0 - oTexture.y;
}