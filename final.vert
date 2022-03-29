#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTangent;
layout (location = 3) in vec2 vertexUV;
layout (location = 4) in vec4 vertexColor;

out vec3 worldPosition;
out vec2 uv;
out vec3 normal;
out vec3 tangent;

uniform mat4 cameraMatrix = mat4(1.0);
uniform mat4 objectRotationMatrix = mat4(1.0);
uniform mat4 objectTranslateMatrix = mat4(1.0);

void main() {
	worldPosition = (/*objectTranslateMatrix * */ /*objectRotationMatrix * */vec4(vertexPosition, 1.0)).xyz;
	uv = vertexUV;
	normal = (/*objectRotationMatrix * */vec4(vertexNormal, 1.0)).xyz;
	tangent = (/*objectRotationMatrix * */vec4(vertexTangent, 1.0)).xyz;
	gl_Position = cameraMatrix * vec4(worldPosition, 1.0);
}