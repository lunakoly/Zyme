#version 330 core
precision lowp float;

layout(location = 0) in vec2 iPosition;
layout(location = 0) out vec4 oColor;

uniform sampler2D uTexture;

void main(void) {
	float depth = texture(uTexture, iPosition).r;
	oColor = vec4(depth, depth, depth, 1.0);
}