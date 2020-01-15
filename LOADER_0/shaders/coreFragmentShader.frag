#version 330 core

out vec4 color;

in vec3 ec_pos;

void main()
{
	vec3 ec_normal = normalize(cross(dFdx(ec_pos), dFdy(ec_pos)));
    ec_normal = normalize(ec_normal);

	vec3 base00 = vec3(0.1f, 0.59f, 0.92f);
	vec3 base2 = vec3(0.2f, 0.7f, 0.96f);
	vec3 base3 = vec3(0.1f, 0.6f, 0.9f);

	float alpha = 1.0f;

    float a = dot(ec_normal, vec3(0.3, 0.3, 1.5));
    float b = dot(ec_normal, vec3(-0.97, -0.57, 0.57));

    color = vec4((a*base2 + (a)*base00)*0.30 +
                        (b*base3 + (b)*base00)*0.22, alpha);
}