#version 110
varying vec3 color_;
varying vec3 norm;
varying vec3 vert;

varying float blv;

varying vec3 blocks;

#vert
void main(){
	blocks = gl_Color.rgb;

	vec3 sunDir = gl_LightSource[0].position.xyz;
	float lightF = max( dot(gl_Normal, sunDir), 0.0);
	float sunFactor = min(lightF + 0.4, 1.0);
	color_ = gl_FrontLightProduct[0].diffuse.rgb * sunFactor;
	
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;
	
	blv = gl_MultiTexCoord0.x;
	
	gl_Position = ftransform();
	norm = gl_Normal;
	vert = gl_Vertex.xyz;
}

#frag
uniform sampler2D texture;

float closestOf3(float x, float a, float b, float c);

void main(){
	float trueBlock = closestOf3(blv, blocks.r, blocks.g, blocks.b);
	//float trueBlock = blv;

	//gl_FragColor = vec4(1, 0, 0, 1);
	
	float b = (trueBlock);
	
	vec3 pWeight = abs(norm);
	pWeight /= pWeight.x + pWeight.y + pWeight.z;
	
	float K = 8.0;
	
	vec3 xP = texture2D(texture, mod(vert.yz / K, 0.125) + b * 0.125).rgb * pWeight.x;
	vec3 yP = texture2D(texture, mod(vert.xz / K, 0.125) + b * 0.125).rgb * pWeight.y;
	vec3 zP = texture2D(texture, mod(vert.xy / K, 0.125) + b * 0.125).rgb * pWeight.z;
	vec3 fP = xP + yP + zP;
	gl_FragColor = vec4(fP * color_, 1);
}

float closestOf3(float x, float a, float b, float c) {
	float d1 = abs(x - a);
	float d2 = abs(x - b);
	float d3 = abs(x - c);
	
	if (d1 <= d2 && d1 <= d3) {
		return a;
	}
	if (d2 <= d1 && d2 <= d3) {
		return b;
	}
	
	return c;
}