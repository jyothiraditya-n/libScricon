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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <LSC_buffer.h>
#include <LSC_lines.h>

typedef struct {
	void (*function)(LSCb_t *buf, size_t x, size_t y, double z,
		const char *data);

	LSCb_t *buf;
	const char *data;

} call_t;

static void algorithm(call_t call, intmax_t x1, intmax_t y1, double z1,
	intmax_t x2, intmax_t y2, double z2)
{
	intmax_t x, y;
	intmax_t d, dx = x2 - x1, dy;

	void (*setter)(LSCb_t *buf, size_t x, size_t y, double z, char chr)
		= call.buf -> validate? LSCb_setzv: LSCb_setz;

	double z, dz = z2 - z1;
	char chr;

	if(x1 > x2) {
		algorithm(call, x2, y2, z2, x1, y1, z1);
		return;
	}

	if(x1 == x2) {
		if(y1 > y2) {
			x = x2; chr = '|';
			z = z2; dz /= (y1 - y2);

			for(y = y2; y <= y1; ++y) {
				call.function(call.buf, x, y, z, call.data);
				setter(call.buf, x, y, z, chr);

				z -= dz;
			}

			return;
		}

		if(y1 == y2) {
			x = x1; y = y1; z = z1; chr = '+';

			call.function(call.buf, x, y, z, call.data);
			setter(call.buf, x, y, z, chr);

			return;
		}

		x = x1; chr = '|';
		z = z1; dz /= (y2 - y1);

		for(y = y1; y <= y2; ++y) {
			call.function(call.buf, x, y, z, call.data);
			setter(call.buf, x, y, z, chr);

			z += dz;
		}

		return;
	}

	if(y1 > y2) {
		dy = y1 - y2;

		if(dx > dy) {
			d = (2 * dy) - dx; y = y2; chr = '_';
			dz /= dx; z = z2;

			for(x = x2; x >= x1; --x) {
				call.function(call.buf, x, y, z, call.data);
				setter(call.buf, x, y, z, chr);

				if(d > 0) { ++y; d -= 2 * dx; chr = '/'; }
				else chr = '_';

				d += 2 * dy;
				z -= dz;
			}

			return;
		}

		if(dx == dy) {
			y = y1; chr = '/';
			dz /= dx; z = z1;

			for(x = x1; x <= x2; ++x) {
				call.function(call.buf, x, y, z, call.data);
				setter(call.buf, x, y, z, chr);

				z += dz;
				--y;
			}

			return;
		}

		d = (2 * dx) - dy; x = x2; chr = '|';
		dz /= dy; z = z2;

		for(y = y2; y <= y1; ++y) {
			call.function(call.buf, x, y, z, call.data);
			setter(call.buf, x, y, z, chr);

			if(d > 0) { --x; d -= 2 * dy; chr = '/'; }
			else chr = '|';

			d += 2 * dx;
			z -= dz;
		}

		return;
	}

	if(y1 == y2) {
		y = y1; chr = '_';
		dz /= dx; z = z1;

		for(x = x1; x <= x2; ++x) {
			call.function(call.buf, x, y, z, call.data);
			setter(call.buf, x, y, z, chr);

			z += dz;
		}

		return;
	}

	dy = y2 - y1;

	if(dx > dy) {
		d = (2 * dy) - dx; y = y1; chr = '_';
		z = z1; dz /= dx;

		for(x = x1; x <= x2; ++x) {
			call.function(call.buf, x, y, z, call.data);
			setter(call.buf, x, y, z, chr);

			if(d > 0) { ++y; d -= 2 * dx; chr = '\\'; }
			else chr = '_';

			d += 2 * dy;
			z += dz;
		}

		return;
	}

	if(dx == dy) {
		y = y1; chr = '\\';
		z = z1; dz /= dx;

		for(x = x1; x <= x2; ++x) {
			call.function(call.buf, x, y, z, call.data);
			setter(call.buf, x, y, z, chr);

			z += dz;
			++y;
		}

		return;
	}

	d = (2 * dx) - dy; x = x2; chr = '|';
	dz /= dy; z = z2;

	for(y = y2; y >= y1; --y) {
		call.function(call.buf, x, y, z, call.data);
		setter(call.buf, x, y, z, chr);

		if(d > 0) { --x; d -= 2 * dy; chr = '\\'; }
		else chr = '|';

		d += 2 * dx;
		z -= dz;
	}
}

void LSCl_drawz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2)
{
	call_t call = {buf -> validate? LSCb_setszv: LSCb_setsz, buf, " "};
	algorithm(call, x1, y1, z1, x2, y2, z2);
}

void LSCl_drawcolz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, uint8_t fg, uint8_t bg)
{
	char data[23];
	call_t call = {buf -> validate? LSCb_setcolszv: LSCb_setcolsz,
		buf, " "};

	sprintf(data, "\033[48;5;%03um\033[38;5;%03um", bg, fg);
	call.data = data;

	algorithm(call, x1, y1, z1, x2, y2, z2);
}

void LSCl_drawfgz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, uint8_t fg)
{
	char data[12];
	call_t call = {buf -> validate? LSCb_setfgszv: LSCb_setfgsz, buf, " "};

	sprintf(data, "\033[38;5;%03um", fg);
	call.data = data;

	algorithm(call, x1, y1, z1, x2, y2, z2);
}

void LSCl_drawbgz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, uint8_t bg)
{
	char data[12];
	call_t call = {buf -> validate? LSCb_setbgszv: LSCb_setbgsz, buf, " "};

	sprintf(data, "\033[48;5;%03um", bg);
	call.data = data;

	algorithm(call, x1, y1, z1, x2, y2, z2);
}