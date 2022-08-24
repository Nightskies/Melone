#type vertex
#version 450 core

layout(location = 0) in vec3 aWorldPosition;
layout(location = 1) in vec3 aLocalPosition;
layout(location = 2) in vec4 aColor;
layout(location = 3) in float aThickness;
layout(location = 4) in float aFade;
layout(location = 5) in int aEntityID;

layout(std140) uniform Camera
{
	mat4 uViewProjection;
};

out vec3 vLocalPosition;
out vec4 vColor;
out float vThickness;
out float vFade;
out flat int vEntityID;

void main()
{
	vLocalPosition = aLocalPosition;
	vColor = aColor;
	vThickness = aThickness;
	vFade = aFade;
	vEntityID = aEntityID;
	gl_Position = uViewProjection * vec4(aWorldPosition, 1.0f);	
};

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int entityID;

in vec3 vLocalPosition;
in vec4 vColor;
in float vThickness;
in float vFade;
in flat int vEntityID;

void main()
{
	// Calculate distance and fill circle with white
    float distance = 1.0 - length(vLocalPosition);
    float circle = smoothstep(0.0, vFade, distance);
    circle *= smoothstep(vThickness + vFade, vThickness, distance);

	if (circle == 0.0)
		discard;

    // Set output color
    color = vColor;
	color.a *= circle;

	entityID = vEntityID;
} 