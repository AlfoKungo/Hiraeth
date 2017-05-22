#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 colour;

in DATA
{
	vec4 position;
	vec2 uv;
	vec4 color; 
} fs_in;

uniform sampler2D tex;

void main()
{
	//float intensity = 1.0 / length(fs_in.position.xy - light_pos);
	float intensity = 1.0;
	vec4 texColor = texture(tex, fs_in.uv);
	
	color = texColor * intensity;


}
