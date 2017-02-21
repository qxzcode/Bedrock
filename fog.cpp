//
//  fog.cpp
//  Bedrock
//
//  Created by Quinn on 1/3/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#define STRINGIFY(x) #x
#define STRINGIFY_VAL(macro) STRINGIFY(macro)

#include "Chunk.h" // also includes World.h
#define CHUNK_SIZE_STR STRINGIFY_VAL(CHUNK_SIZE)
#define WORLD_X_STR "float(" STRINGIFY_VAL(WORLD_X) ")"
#define WORLD_Y_STR "float(" STRINGIFY_VAL(WORLD_Y) ")"
#define WORLD_Z_STR "float(" STRINGIFY_VAL(WORLD_Z) ")"

static const std::string fogVertSrc = "\
\
uniform vec3 fogCenter;\
uniform float grassWave;\
uniform bool plain;\
uniform bool debug;\
uniform bool terrain;\
uniform bool textures;\
uniform mat4 viewMatrix;\
uniform sampler2D colorMap;\
\
attribute vec3 facePosition;\
attribute vec4 particleColorSize;\
\
varying float grassAttr;\
varying vec3 vertexToPlayer;\
\
void main(void)\
{\
	vec3 vertexPos;\
	if (!textures) {\
		vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);\
		vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);\
		vertexPos = facePosition + cameraRight*gl_Vertex.x*particleColorSize.w + cameraUp*gl_Vertex.y*particleColorSize.w;\
	} else {\
		vertexPos = vec3( gl_ModelViewMatrix * gl_Vertex );\
	}\
	if (!plain) {\
		/*vec3 fogCenter2 = fogCenter;\
		if (fogCenter.x-vertexPos.x > " WORLD_X_STR "/2.0) fogCenter2.x -= " WORLD_X_STR ";\
		if (fogCenter.x-vertexPos.x < " WORLD_X_STR "/-2.0) fogCenter2.x += " WORLD_X_STR ";\
		if (fogCenter.y-vertexPos.y > " WORLD_Y_STR "/2.0) fogCenter2.y -= " WORLD_Y_STR ";\
		if (fogCenter.y-vertexPos.y < " WORLD_Y_STR "/-2.0) fogCenter2.y += " WORLD_Y_STR ";\
		if (fogCenter.z-vertexPos.z > " WORLD_Z_STR "/2.0) fogCenter2.z -= " WORLD_Z_STR ";\
		if (fogCenter.z-vertexPos.z < " WORLD_Z_STR "/-2.0) fogCenter2.z += " WORLD_Z_STR ";*/\
		\
		if (terrain) {\
			grassAttr = gl_Color.g;\
			if (grassAttr == 1.0) {\
				float wave = sin( grassWave+((vertexPos.x+vertexPos.z)/10.0) )/12.0 - 0.1;\
				vertexPos.x += wave;\
				vertexPos.z += wave;\
			}\
		}\
		\
		vertexToPlayer = abs(fogCenter - vertexPos);\
	}\
	\
	gl_TexCoord[0]	= gl_MultiTexCoord0;\
	gl_Position		= gl_ProjectionMatrix * viewMatrix * vec4(vertexPos, 1.0);\
	gl_FrontColor	= !textures? (debug?vec4(0,0,1,1):vec4(particleColorSize.rgb, 1.0)) : terrain? gl_Color.rrra : gl_Color;\
}\
";

/* FOR BLACK HOLE EFFECT: INSERT AFTER THE IF STATEMENTS
 \
 vec3 playerToVertex = vertexPos - fogCenter2;\
 \
 const float radius = 10.0;\
 float len = length(playerToVertex);\
 if (len < radius) {\
 float factor = radius - len;\
 len = clamp(len - (factor/3.0), 0.0, radius);\
 playerToVertex = normalize(playerToVertex) * len;\
 vertexPos = playerToVertex + fogCenter2;\
 \
 }\
 */

static const std::string fogFragSrc = "\
\
uniform float fogRadius;\
uniform float fogEnd;\
uniform vec4 fogColor;\
\
uniform vec3 flowerPetals;\
uniform vec3 flowerCenter;\
\
uniform bool plain;\
uniform bool debug;\
uniform bool terrain;\
uniform bool textures;\
uniform bool fogOn;\
uniform bool select;/*used to whiten selected entities*/\
uniform sampler2D sampler;\
\
varying float grassAttr;\
varying vec3 vertexToPlayer;\
\
void setFragColor(vec4 color)\
{\
	if (select)\
		gl_FragColor = (color+color+vec4(1,1,1,1))/3.0;\
	else\
		gl_FragColor = color;\
}\
\
void main(void)\
{\
	if (plain) {\
		gl_FragColor = gl_Color;\
	} else if (debug) {\
		gl_FragColor = !terrain? gl_Color : grassAttr > 0.0? vec4(0,1,0,1) : vec4(0,0,0,1);\
	} else {\
		vec4 diffuseColor = textures?texture2D(sampler, gl_TexCoord[0].st)*gl_Color:gl_Color;\
		if (diffuseColor.a < 0.5) discard;\
		else diffuseColor.a = 1.0;\
		\
		if (terrain && grassAttr > 0.0) {\
			if (diffuseColor.rgb == vec3(0, 0, 0))\
				diffuseColor.rgb = flowerPetals; /* flower petals */\
			if (length(diffuseColor.rgb-vec3(1, 1, 1)) < 0.01)\
				diffuseColor.rgb = flowerCenter; /* flower center */\
		}\
		\
		float dist = length(vertexToPlayer);\
		if (dist > fogEnd) discard;\
		float fogStart = fogEnd - fogRadius;\
		if (!fogOn || dist < fogStart) {setFragColor(diffuseColor);return;}\
		float attenuation = clamp((fogEnd - dist) / fogRadius, 0.0, 1.0);\
		attenuation = (1.0 - cos(attenuation*3.1415927)) * 0.5;\
		\
		vec4 diffuseTerm  = diffuseColor * attenuation + (fogColor * (1.0-attenuation));\
		diffuseTerm.a = diffuseColor.a; /* preserve alpha */\
		\
		setFragColor(diffuseTerm);\
	}\
}\
\
";