#version 330 core
precision lowp float;

layout(location = 0) in vec4 iPosition;
layout(location = 0) out vec2 oPosition;

void main(void) {
	gl_Position = iPosition;
	oPosition = (iPosition.xy + 1.0) / 2.0;
}