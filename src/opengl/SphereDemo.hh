#pragma once

/*
	Generate a unit sphere with resx resolution around the equator
	and resy around the poles.
 */
class Sphere : public Shape {
public:
	Sphere(int reslon, int reslat)	{
		GLfloat vertices[(resx*resy+2)*3]; // 3 floats per point
		double chord = 2 * sin(PI / min(longRes, latRes));
		double rMin = sqrt(1 - chord * chord / 4);
		reslat *= 0.5;

		int c = 0;
		// first point is the south pole
		vertices[c++] = 0;
		vertices[c++] = 0;
		vertices[c++] = -1;

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
