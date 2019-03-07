#version 330 core
precision lowp float;

layout(location = 0) in vec2 iPosition;
layout(location = 0) out vec4 oColor;

uniform sampler2D uTexture;

void main(void) {
	oColor = vec4(0.6, 0.6, 0.6, 1.0);
	
	bool isInSquareX = mod(gl_FragCoord.x, 20.0) > 10.0;
	bool isInSquareY = mod(gl_FragCoord.y, 20.0) > 10.0;
	
	if (
		 isInSquareX  &&  isInSquareY  ||
		!isInSquareX  && !isInSquareY
	) oColor.rgb -= vec3(0.2, 0.2, 0.2);

	vec4 textureColor = texture(uTexture, iPosition);
	oColor = oColor * (1.0 - textureColor.a) + textureColor * textureColor.a;
}