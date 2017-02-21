//
//  shaders.cpp
//  Bedrock
//
//  Created by Quinn on 11/2/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "shaders.h"

// shader sources //
#include "fog.cpp"
////////////////////

extern const std::string	fogVertSrc, fogFragSrc,		// defined in fog.cpp
							plainVertSrc, plainFragSrc,
							orthoVertSrc, orthoFragSrc;

void shaders::load() {
	printf("COMPILING SHADERS...\n");
	
	fog.newProgram(fogVertSrc, fogFragSrc);
	if (!fog.isCompiled())
		printf("SHADER COMPILATION FAILED: fog\n");
	fog_fogCenterLoc = glGetUniformLocation(fog.progObj, "fogCenter");
	fog_fogEndLoc = glGetUniformLocation(fog.progObj, "fogEnd");
	fog_viewMatrixLoc = glGetUniformLocation(fog.progObj, "viewMatrix");
	fog_grassWaveLoc = glGetUniformLocation(fog.progObj, "grassWave");
	fog_plainLoc = glGetUniformLocation(fog.progObj, "plain");
	fog_debugLoc = glGetUniformLocation(fog.progObj, "debug");
	fog_terrainLoc = glGetUniformLocation(fog.progObj, "terrain");
	fog_texturesLoc = glGetUniformLocation(fog.progObj, "textures");
	fog_fogOnLoc = glGetUniformLocation(fog.progObj, "fogOn");
	fog_selectLoc = glGetUniformLocation(fog.progObj, "select");
	fog_flowerPetalsLoc = glGetUniformLocation(fog.progObj, "flowerPetals");
	fog_flowerCenterLoc = glGetUniformLocation(fog.progObj, "flowerCenter");
	fog_facePositionLoc = glGetAttribLocation(fog.progObj, "facePosition");
	fog_particleColorSizeLoc = glGetAttribLocation(fog.progObj, "particleColorSize");
	
	plain.newProgram(plainVertSrc, plainFragSrc);
	if (!plain.isCompiled())
		printf("SHADER COMPILATION FAILED: plain\n");
	plain_viewMatrixLoc = glGetUniformLocation(plain.progObj, "viewMatrix");
	
	ortho.newProgram(orthoVertSrc, orthoFragSrc);
	if (!ortho.isCompiled())
		printf("SHADER COMPILATION FAILED: ortho\n");
	
	printf("DONE\n\n");
}

ShaderProgram shaders::fog, shaders::plain, shaders::ortho;
GLint shaders::fog_terrainLoc, shaders::fog_texturesLoc, shaders::fog_fogOnLoc, shaders::fog_selectLoc, shaders::fog_flowerCenterLoc, shaders::fog_flowerPetalsLoc, shaders::fog_fogCenterLoc, shaders::fog_fogEndLoc, shaders::fog_viewMatrixLoc, shaders::fog_grassWaveLoc, shaders::fog_plainLoc, shaders::fog_debugLoc, shaders::fog_facePositionLoc, shaders::fog_particleColorSizeLoc,
	  shaders::plain_viewMatrixLoc;

ShaderProgram * shaders::curProg = NULL;


//// SHADER SOURCE ////

const std::string plainVertSrc = "\
\
uniform mat4 viewMatrix;\
\
void main(void)\
{\
	gl_TexCoord[0]	= gl_MultiTexCoord0;\
	gl_Position		= gl_ProjectionMatrix * viewMatrix * vec4(vec3(gl_ModelViewMatrix * gl_Vertex), 1.0);\
	gl_FrontColor	= gl_Color;\
}\
";

const std::string plainFragSrc = "\
\
uniform sampler2D colorMap;\
\
void main(void)\
{\
	gl_FragColor = vec4(1,1,1,1);\
}\
";



const std::string orthoVertSrc = "\
\
uniform sampler2D colorMap;\
\
void main(void)\
{\
	gl_TexCoord[0]	= gl_MultiTexCoord0;\
	gl_Position		= gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;\
	gl_FrontColor	= gl_Color;\
}\
";

const std::string orthoFragSrc = "\
\
uniform sampler2D colorMap;\
\
void main(void)\
{\
	gl_FragColor = texture2D(colorMap, gl_TexCoord[0].st) * gl_Color;\
}\
";
