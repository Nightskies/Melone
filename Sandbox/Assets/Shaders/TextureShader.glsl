#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexture;

out vec4 vColor;
out vec2 vTexCoord;

uniform mat4 uViewProjection;

void main()
{
	vColor = aColor;
	vTexCoord = aTexture;
	gl_Position = uViewProjection * vec4(aPosition, 1.0f);	
};

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 vColor;
in vec2 vTexCoord;

uniform vec4 uColor;
uniform float uTilingFactor;
uniform sampler2D uTexture;

void main()
{
	//color = texture(uTexture, vTexCoord * uTilingFactor) * uColor;
	color = vColor;
};