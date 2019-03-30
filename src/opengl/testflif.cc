#include <flif.h>
#include <GL/gl.h>
#include "GLWin.hh"

#include <iostream>
using namespace std;

#pragma pack(push,1)
typedef struct RGBA
{
    uint8_t r,g,b,a;
} RGBA;
#pragma pack(pop)

uint32_t GLWin::loadImage(const char filename[]) {
	FLIF_DECODER* d = flif_create_decoder();
	if(d == nullptr)
		throw "alloc failed";
	
	flif_decoder_set_quality(d, 100);
	flif_decoder_set_scale(d, 1);
	if (!flif_decoder_decode_file(d, filename))
		throw "decoding file";
	FLIF_IMAGE* im = flif_decoder_get_image(d, 0);
	if(im == nullptr) {
		throw "No_decoded_image";
	}
	uint32_t w = flif_image_get_width(im);
	uint32_t h = flif_image_get_height(im);
	RGBA* buf = new RGBA[w * h];
	for(uint32_t y = 0, pos = 0; y < h; ++y, pos += w) {
		flif_image_read_row_RGBA8(im, y, &buf[pos], w * sizeof(RGBA));
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0,
							 GL_BGRA, GL_UNSIGNED_BYTE, buf);

	// OpenGL has now copied the data. Free our own version
	delete [] buf;
	flif_destroy_image(im);

	// Poor filtering, or ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

#if 0
	// ... nice trilinear filtering ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);	// requires mipmaps. Generate them automatically.
#endif
	
	// Return the ID of the texture we just created
	return textureID;
}
