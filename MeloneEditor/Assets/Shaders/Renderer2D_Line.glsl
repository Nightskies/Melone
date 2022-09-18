#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in int aEntityID;

layout(std140) uniform Camera
{
	mat4 uViewProjection;
};

out vec4 vColor;
out flat int vEntityID;

void main()
{
	vColor = aColor;
	vEntityID = aEntityID;
	gl_Position = uViewProjection * vec4(aPosition, 1.0f);	
};

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int entityID;

in vec4 vColor;
in flat int vEntityID;

void main()
{
	color = vColor;
	entityID = vEntityID;
} 