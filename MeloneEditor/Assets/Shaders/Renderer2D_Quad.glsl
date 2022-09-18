#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexture;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aTilingFactor;
layout(location = 5) in int aEntityID;

layout(std140) uniform Camera
{
	mat4 uViewProjection;
};

out vec4 vColor;
out vec2 vTexCoord;
out flat float vTexIndex;
out float vTilingFactor;
out flat int vEntityID;

void main()
{
	vColor = aColor;
	vTexCoord = aTexture;
	vTexIndex = aTexIndex;
	vTilingFactor = aTilingFactor;
	vEntityID = aEntityID;
	gl_Position = uViewProjection * vec4(aPosition, 1.0f);	
};

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int entityID;

in vec4 vColor;
in vec2 vTexCoord;
in flat float vTexIndex;
in float vTilingFactor;
in flat int vEntityID;

layout(binding = 0) uniform sampler2D uTextures[32];

void main()
{
	color = texture(uTextures[int(vTexIndex)], vTexCoord * vTilingFactor) * vColor;

	entityID = vEntityID;;
} 