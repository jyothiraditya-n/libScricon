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

#ifndef LSC_VECTOR_H
#define LSC_VECTOR_H 1

typedef struct {
	double x, y, z;

} LSCv_t;

extern LSCv_t LSCv_x, LSCv_y, LSCv_z;

extern double LSCv_len(LSCv_t v);
extern LSCv_t LSCv_cap(LSCv_t v);
extern LSCv_t LSCv_rev(LSCv_t v);

extern LSCv_t LSCv_add(LSCv_t a, LSCv_t b);
extern LSCv_t LSCv_sub(LSCv_t a, LSCv_t b);

extern LSCv_t LSCv_mul(LSCv_t v, double d);
extern LSCv_t LSCv_div(LSCv_t v, double d);

extern double LSCv_dot(LSCv_t a, LSCv_t b);
extern LSCv_t LSCv_cross(LSCv_t a, LSCv_t b);

#endif