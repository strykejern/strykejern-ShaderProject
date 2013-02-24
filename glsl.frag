// ******************************************************************
// * TGP4162 Project 2: Visualization using OpenGL shader language *
// * Stein Dale/Thorvald Natvig, IPT, NTNU							*
// ******************************************************************
#version 120
// Required uniform variables to solve Project.
// Remember texture values are within the domain [0.0, 1.0]

// The framework vertex shader conveys the following varying attributes which can be used if you want. 
varying vec3 normal;		// interpolated surface normal
varying vec3 lightDir;		// interpolated vector pointing towards light source
varying float lightDistance;
varying vec4 gl_Color;
//varying float specular_intensity;
varying float diffuse_intensity;

varying vec4 vertex_position_camera;
varying vec3 light_vert;
varying vec3 normal_camera;

varying vec4 ShadowCoord;
//smooth in vec3 position;

uniform vec3 lightPos;
uniform vec4 color;

void main()
{
	vec4 lightspec = vec4(1.0, 1.0, 1.0, 1.0);

	vec3 light_refl = normalize(reflect(normalize(light_vert), normalize(normal_camera)));

	float specular_intensity = max(0.0, dot(normalize(light_refl), normalize(vec3(vertex_position_camera))));
	specular_intensity = pow(specular_intensity, 16.0);
    
	vec4 specular = lightspec * specular_intensity;// / pow(lightDistance, 1);
    
	float diffuseLightIntensity = max(0.0,dot(normalize(normal),lightDir));

	float ambientLight = 0.01;

    vec4 fragcolor = max(diffuseLightIntensity, ambientLight) * gl_Color;// / (lightDistance / 16.0);

	gl_FragColor = fragcolor * 1.0 + specular;
}//