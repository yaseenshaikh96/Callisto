#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in float a_TexIndex;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in vec2 a_TexScale;

uniform mat4 u_ViewProjection;

out float v_TexIndex;
out vec2 v_TexCoord;
out vec2 v_TexScale;
out vec4 v_Color;

void main()
{
	v_TexScale = a_TexScale;
	v_TexIndex = a_TexIndex;
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}	

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out int outID;

in float v_TexIndex;
in vec2 v_TexCoord;
in vec2 v_TexScale;
in vec4 v_Color;

uniform sampler2D u_Textures[16];			
uniform vec2 u_TexScale;

void main()
{
	color = texture(u_Textures[int(v_TexIndex)], vec2(v_TexCoord.x * v_TexScale.x, v_TexCoord.y * v_TexScale.y ) ) * v_Color;
	//color = v_Color;

	outID = 50;
}	
