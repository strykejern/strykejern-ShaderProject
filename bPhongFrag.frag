
// Blinn-Phong lighting fragment shader

varying vec3 surfaceNormal;
varying vec3 towardsEye;
varying vec3 towardsLight;

varying float lightDistance;

void main(){
	vec4 lightspec = vec4(1.0, 1.0, 1.0, 1.0);

	vec3 eyeLightNormal = normalize(towardsLight) + normalize(towardsEye);

	float specIntensity = pow(max(0.0, dot(normalize(eyeLightNormal), normalize(surfaceNormal))), 16.0);

	float diffuseIntensity = max(0.0, dot(normalize(towardsLight), normalize(surfaceNormal)));

	vec4 diffuseColor = diffuseIntensity * gl_Color;

	vec4 specColor = specIntensity * lightspec;

	gl_FragColor = diffuseColor + specColor;
}//