#version 330 core
precision lowp float;

layout(location = 0) in vec3 iNormal;
layout(location = 1) in vec2 iTexture;
layout(location = 2) in vec4 iInLightPosition;
layout(location = 3) in vec3 iWorldPosition;
layout(location = 4) in mat3 iTBNMatrix;
layout(location = 0) out vec4 oColor;

uniform struct {
	vec4 color;
	float shininess;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
} uMaterial;

uniform mat4 uModelMatrix;
uniform vec4 uCameraPosition;
uniform vec4 uLightDirection;
uniform sampler2D uLightDepthTexture;

void main(void) {
	vec4 diffuseColor = texture(uMaterial.diffuse, iTexture);
	
	vec3 blendedDiffuseColor = 
			uMaterial.color.xyz * (1.0 - diffuseColor.a) + 
			   diffuseColor.xyz *        diffuseColor.a;


	vec4 specularColor = texture(uMaterial.specular, iTexture);

	vec3 blendedSpecularColor = 
			vec3(1.0, 1.0, 1.0) * (1.0 - specularColor.a) + 
			  specularColor.xyz *        specularColor.a;


	vec3 rawNormal = texture(uMaterial.normal, iTexture).xyz * 2.0 - 1.0;
	vec3 normal = normalize(mat3(uModelMatrix) * iTBNMatrix * rawNormal);


	vec3 position = iInLightPosition.xyz / iInLightPosition.w;
	position = position * 0.5 + 0.5;


	// pcf filtering
	float shadowCoef = 0.0;
	vec2 texelSize = 1.0 / textureSize(uLightDepthTexture, 0);

	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (
				texture(uLightDepthTexture, position.xy + vec2(dx, dy) * texelSize).r > position.z
			) shadowCoef += 1.0;
		}
	}

	shadowCoef /= 9.0;

	vec3 toLightDirection = -1 * normalize(uLightDirection.xyz);
	float diffuseCoef = max(0.0, dot(toLightDirection, normal));
	oColor = vec4(blendedDiffuseColor * diffuseCoef, 1.0);

	vec3 toCameraDirection = (uCameraPosition.xyz - iWorldPosition.xyz);
	vec3 halfWay = (toCameraDirection + toLightDirection) 
		   / length(toCameraDirection + toLightDirection);

	float specularCoef = pow(max(0.0, dot(halfWay, normal)), uMaterial.shininess);
	oColor += vec4(blendedSpecularColor * specularCoef, 1.0);

	oColor *= shadowCoef;
	// oColor.rgb = pow(oColor.rgb, vec3(1 / 2.2));
}