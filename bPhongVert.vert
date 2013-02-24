
// Blinn-Phong lighting vertex shader

varying vec3 surfaceNormal;
varying vec3 towardsEye;
varying vec3 towardsLight;

varying float lightDistance;

uniform float waveDelta;

void main(){
	vec4 wave = gl_Vertex;

    float xAmp = 2.0;
    float zAmp = 1.5;

    float xPer = 0.2;
    float zPer = 0.14;

    float timeDelta = waveDelta * 0.0025;

    wave.y = sin(xPer * wave.x + timeDelta) * xAmp + sin(zPer * wave.z + timeDelta) * zAmp;

    towardsLight = vec3(normalize(gl_LightSource[0].position - (gl_ModelViewMatrix * wave)));	// unit vector in the direction of GL_LIGHT0 (light zero)

    towardsEye = -vec3(gl_ModelViewMatrix * wave);
	
	surfaceNormal = gl_NormalMatrix * vec3(-cos(xPer * wave.x + timeDelta) * xAmp, xAmp + zAmp, -cos(zPer * wave.z + timeDelta) * zAmp); //tangentx + tangentz;
	
	gl_FrontColor = gl_Color;
											
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * wave; // Compute vertex position in clip coordinates (positional invariance)

	lightDistance = pow(length(gl_LightSource[0].position - (gl_ModelViewMatrix * wave)), 1.0);
}//