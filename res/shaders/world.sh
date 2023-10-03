#version 110
varying vec3 color_;
varying vec3 norm;
varying vec3 vert;
varying float bl;

#vert
void main(){
	vec3 sunDir = gl_LightSource[0].position.xyz;
	float sunFactor = max(dot(gl_Normal, sunDir), 0.0);
	color_ = gl_FrontLightProduct[0].diffuse.rgb * sunFactor * gl_Color.r;
	
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;
	bl = gl_MultiTexCoord0.x - 1.0;
	gl_Position = ftransform();
	norm = gl_Normal;
	vert = gl_Vertex.xyz;
}

#frag
uniform sampler2D texture;

void main(){
	//gl_FragColor = vec4(1, 0, 0, 1);
	
	float b = ceil(bl);
	
	vec3 pWeight = abs(norm);
	pWeight /= pWeight.x + pWeight.y + pWeight.z;
	
	vec3 xP = texture2D(texture, mod(vert.yz / 4.0, 0.25) +  (b) * 0.25 ).rgb * pWeight.x;
	vec3 yP = texture2D(texture, mod(vert.xz / 4.0, 0.25) +  (b) * 0.25 ).rgb * pWeight.y;
	vec3 zP = texture2D(texture, mod(vert.xy / 4.0, 0.25) +  (b) * 0.25).rgb * pWeight.z;
	vec3 fP = xP + yP + zP;
	gl_FragColor = vec4(fP * color_, 1);
}

