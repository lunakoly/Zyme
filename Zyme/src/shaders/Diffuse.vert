#version 330 core
precision lowp float;

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec2 iTexture;
layout(location = 2) in vec3 iNormal;
layout(location = 3) in vec3 iTangent;
layout(location = 0) out vec3 oNormal;
layout(location = 1) out vec2 oTexture;
layout(location = 2) out vec4 oInLightPosition;
layout(location = 3) out vec3 oWorldPosition;
layout(location = 4) out mat3 oTBNMatrix;

uniform mat4 uMVPMatrix;
uniform mat4 uMLPMatrix;
uniform mat4 uModelMatrix;
uniform float uTime;

void main(void) {	
	gl_Position		 = uMVPMatrix * vec4(iPosition, 1.0);
	oInLightPosition = uMLPMatrix * vec4(iPosition, 1.0);
	
	oNormal = normalize(uModelMatrix * vec4(iNormal, 0.0)).xyz;
	oTexture = iTexture;
	oTexture.y = 1.0 - oTexture.y;

	oWorldPosition = (uModelMatrix * vec4(iPosition, 1.0)).xyz;

	vec3 normal    = normalize(iNormal);
	vec3 tangent   = normalize(iTangent);
	vec3 bitangent = normalize(cross(normal, tangent));
	oTBNMatrix = mat3(tangent, bitangent, normal);
}