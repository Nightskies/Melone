#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexture;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aTilingFactor;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;
out float vTilingFactor;

uniform mat4 uViewProjection;

void main()
{
	vColor = aColor;
	vTexCoord = aTexture;
	vTexIndex = aTexIndex;
	vTilingFactor = aTilingFactor;
	gl_Position = uViewProjection * vec4(aPosition, 1.0f);	
};

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;
in float vTilingFactor;

uniform sampler2D uTextures[32];

void main()
{
	color = texture(uTextures[int(vTexIndex)], vTexCoord * vTilingFactor) * vColor;
};