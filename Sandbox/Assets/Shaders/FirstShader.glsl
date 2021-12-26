#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexture;

out vec2 vTexCoord;

uniform mat4 uViewProjection;

void main()
{
	vTexCoord = aTexture;
	gl_Position = uViewProjection * vec4(aPosition, 1.0f);	
};

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 vTexCoord;

uniform sampler2D uTexture;

void main()
{
	color = texture(uTexture, vTexCoord);
};