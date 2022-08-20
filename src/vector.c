/* libScricon: The Simple Graphical Console Library
 * Copyright (C) 2021-2022 Jyothiraditya Nellakra
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>. */

#include <math.h>

#include <LSC_vector.h>

LSCv_t x_v = {1.0, 0.0, 0.0};
LSCv_t y_v = {0.0, 1.0, 0.0};
LSCv_t z_v = {0.0, 0.0, 1.0};

double LSCv_len(LSCv_t v) {
	return pow(pow(v.x, 2.0) + pow(v.y, 2.0) + pow(v.z, 2.0), 0.5);
}

LSCv_t LSCv_cap(LSCv_t v) {
	double length = LSCv_len(v);
	if(length != 0.0) { v.x /= length; v.y /= length; v.z /= length; }
	return v;
}

LSCv_t LSCv_rev(LSCv_t v) {
	v.x = -v.x; v.y = -v.y; v.z = -v.z;
	return v;
}

LSCv_t LSCv_add(LSCv_t a, LSCv_t b) {
	a.x += b.x; a.y += b.y; a.z += b.z;
	return a;
}

LSCv_t LSCv_sub(LSCv_t a, LSCv_t b) {
	a.x -= b.x; a.y -= b.y; a.z -= b.z;
	return a;
}

LSCv_t LSCv_mul(LSCv_t v, double d) {
	v.x *= d; v.y *= d; v.z *= d;
	return v;
}

LSCv_t LSCv_div(LSCv_t v, double d) {
	v.x /= d; v.y /= d; v.z /= d;
	return v;
}

double LSCv_dot(LSCv_t a, LSCv_t b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

LSCv_t LSCv_cross(LSCv_t a, LSCv_t b) {
	LSCv_t ret = {
		(a.y * b.z) - (b.y * a.z),
		(b.x * a.z) - (a.x * b.z),
		(a.x * b.y) - (b.x * a.y)
	};

	return ret;
}
