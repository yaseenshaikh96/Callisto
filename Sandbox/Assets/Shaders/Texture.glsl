#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;

void main()
{
	v_TexIndex = a_TexIndex;
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}	

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;

uniform sampler2D u_Textures[16];			
uniform vec2 u_TexScale;

void main()
{
	int postTexIndex = 0;
	if(v_TexIndex == 0.0) postTexIndex = 0;
	if(v_TexIndex == 1.0) postTexIndex = 1;
	if(v_TexIndex == 2.0) postTexIndex = 2;

	//if(v_TexIndex == 0.0)
	//	color = vec4(0.0, 1.0, 0.0, 1.0);
	//else vec2(v_TexCoord.x * u_TexScale.x, v_TexCoord.y * u_TexScale.y)
	//	color = vec4(1.0, 0.0, 0.0, 1.0);

	color = texture(u_Textures[postTexIndex], v_TexCoord ) ;// * v_Color;
	//color = v_Color;
}	
