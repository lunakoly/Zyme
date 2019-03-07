#version 330 core
precision lowp float;

layout(location = 0) in vec2 iTexture;
layout(location = 0) out vec4 oColor;

uniform struct {
	vec4 color;
	sampler2D diffuse;
} uMaterial;

void main(void) {
	vec4 diffuseColor = texture(uMaterial.diffuse, iTexture);
	
	vec3 color = 
			uMaterial.color.xyz * (1.0 - diffuseColor.a) + 
			   diffuseColor.xyz *        diffuseColor.a;

	oColor = vec4(color * 0.15, 1.0);
	//oColor.rgb = pow(oColor.rgb, vec3(1 / 2.2));
}