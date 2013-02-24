#version 120

varying vec3 normal, lightDir;		// varying variables are passed on to the fragment shader (interpolated)
varying float lightDistance;
varying float specular_intensity;
varying float diffuse_intensity;
varying vec3 light_refl;
varying vec4 vertex_position_camera;

uniform float waveDelta;

void main()
{

    vec4 wave = gl_Vertex;

    float xAmp = 0.3;
    float zAmp = 0.4;

    float xPer = 0.8;
    float zPer = 0.7;

    float timeDelta = waveDelta * 0.001;

    wave.y = sin(xPer * wave.x + timeDelta) * xAmp + sin(zPer * wave.z + timeDelta) * zAmp;

    lightDir = vec3(normalize(gl_LightSource[0].position - (gl_ModelViewMatrix * wave)));	// unit vector in the direction of GL_LIGHT0 (light zero)
	
	//normal = gl_NormalMatrix * vec3(-cos(xPer * wave.x + timeDelta) * xAmp, 2.0, -cos(zPer * wave.z + timeDelta) * zAmp); //tangentx + tangentz;

	//normal = normalize(normalx + normalz);
	//normal = vec3(-tangent.y, sqrt(2.0), -tangent.y);
	

	// GL_TEXTURE0 texture coordinates passed in as a vertex attribute, and it is passed on 
	// by means of the built-in variable gl_TexCoord[] (varying variable; thus interpolated to fragment shader)
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	
	gl_FrontColor = gl_Color;
	normal = gl_NormalMatrix * gl_Normal;										
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * wave;				// Compute vertex position in clip coordinates (positional invariance)
	gl_Position = ftransform();
	lightDistance = pow(length(gl_LightSource[0].position - (gl_ModelViewMatrix * wave)), 1);
	
	vertex_position_camera = gl_ModelViewMatrix * wave;
	vec3 normal_camera = normalize(normal);
	vec4 light_position_camera = gl_LightSource[0].position;

	vec3 light_vert = normalize(vec3(light_position_camera - vertex_position_camera));
	light_refl = normalize(reflect(light_vert, normal_camera));

	// specular light
	//specular_intensity = max(0.0, dot(light_refl, normalize(vec3(vertex_position_camera))));
	//specular_intensity = pow(specular_intensity, 8.0);
	//specular_intensity = 0.0;
}//