#version 330

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec3 vertexNormal;

out vec2 outTexCoord;
out vec3 mvVertexNormal;
out vec3 mvVertexPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
  mat4 modelViewMatrix = viewMatrix * modelMatrix;
  vec4 mvPos = modelViewMatrix * vec4(position, 1.0);
  gl_Position = projectionMatrix * mvPos;
  outTexCoord = texCoord;
  mvVertexNormal = normalize(modelViewMatrix * vec4(vertexNormal, 0.0)).xyz;
  mvVertexPos = mvPos.xyz;
}
