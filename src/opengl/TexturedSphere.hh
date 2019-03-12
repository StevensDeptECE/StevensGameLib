#pragma once

#include <glut.h>
/*
	Generate a unit sphere with resx resolution around the equator
	and resy around the poles.
 */
class TexturedSphere {
private:
	uint32_t texID;
public:
	TexturedSphere(int reslon, int reslat, const char filename[])	{
		glActiveTexture(GL_TEXTURE0);
		texID = SOIL_load_OGL_texture
			(filename, SOIL_LOAD_AUTO,
			 SOIL_CREATE_NEW_ID,
			 SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS
			 );

		
		// next compute a broad band around the sphere excluding the poles
		final double dphi = PI / latRes; // vertically only +90 to -90 degrees
		double phi = -PI / 2 + dphi;

		for (int j = 0; j < latRes - 1; ++j, phi += dphi) {
			double z = sin(phi);
			double r2 = cos(phi);

			double theta = 0;
			final double dtheta = 2 * PI / reslon;


			for (int i = 0; i < reslon; ++i, theta += dtheta) {
				double x = r2 * cos(theta), y = r2 * sin(theta);
				vertices[c++] = x;
				vertices[c++] = y;
				vertices[c++] = z;
			}
		}
#if 0
		// then create quads with all the points created
		for (int i = 0; i < verts.size() - reslon; i++) {
			addSquare(i + reslon,
								i, (i % reslon == reslon - 1
										? -reslon : 0) + i + 1, //bottom right
								(i % reslon == reslon - 1
								 ? -longRes : 0) + i + reslon + 1,//top right 
								verts);
		}
#endif
		// north pole
		vertices[c++] = 0;
		vertices[c++] = 0;
		vertices[c++] = 1;

		init(vertices, len);
	}

};
