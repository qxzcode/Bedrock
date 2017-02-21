//
//  textures.cpp
//  Bedrock
//
//  Created by Quinn on 9/20/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "textures.h"

#include "SOIL.h"
#include <string>

GLuint textures::blocks, textures::unit, textures::bear;
GLuint textures::items::rock;
GLuint textures::hud::font, textures::hud::menus;


/*
To generate image data headers:

cd "/Users/quinn/Documents/Voxel game imgs"
xxd -i img.png "data headers/img_png.h"

*/


//#define FILE_IMAGES // if defined, load images from file

#ifdef FILE_IMAGES
#warning Images from file is on!
static GLuint loadTexture(std::string path) {
	// load an image file directly as a new OpenGL texture
	GLuint tex_2d = SOIL_load_OGL_texture
	(
		path.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		0
	);
#else
static GLuint loadTexture(unsigned char* data, unsigned int len) {
	// load image data directly as a new OpenGL texture
	GLuint tex_2d = SOIL_load_OGL_texture_from_memory
	(
		data,
		len,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		0
	);
#endif
	
	// check for an error during the load process
	if( !tex_2d ) {
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Disable Texture Blur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	return tex_2d;
}

#ifdef FILE_IMAGES
	#define IMAGE_PATH "/Users/quinn/Documents/Voxel game imgs/"
	#define BLOCKS_PNG_DATA	IMAGE_PATH"blocks.png"
	#define UNIT_PNG_DATA	IMAGE_PATH"unit.png"
	#define FONT_PNG_DATA	IMAGE_PATH"font.png"
	#define MENUS_PNG_DATA	IMAGE_PATH"menus.png"
	#define BEAR_PNG_DATA	IMAGE_PATH"bear.png"
	#define ROCK_PNG_DATA	IMAGE_PATH"rock.png"
#else
	#include "blocks_png.h"
	#include "unit_png.h"
	#include "font_png.h"
	#include "menus_png.h"
	#include "bear_png.h"
	#include "rock_png.h"
	#define BLOCKS_PNG_DATA	blocks_png, blocks_png_len
	#define UNIT_PNG_DATA	unit_png, unit_png_len
	#define FONT_PNG_DATA	font_png, font_png_len
	#define MENUS_PNG_DATA	menus_png, menus_png_len
	#define BEAR_PNG_DATA	bear_png, bear_png_len
	#define ROCK_PNG_DATA	rock_png, rock_png_len
#endif

void textures::load() {
	// terrain
	textures::blocks = loadTexture(BLOCKS_PNG_DATA);
	// entities
	textures::unit = loadTexture(UNIT_PNG_DATA);
	textures::bear = loadTexture(BEAR_PNG_DATA);
	// items
	textures::items::rock = loadTexture(ROCK_PNG_DATA);
	// HUD
	textures::hud::font = loadTexture(FONT_PNG_DATA);
	textures::hud::menus = loadTexture(MENUS_PNG_DATA);
}
