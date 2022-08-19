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
	void (*function)(LSCb_t *buf, size_t x, size_t y, const char *data);
	LSCb_t *buf;
	const char *data;

} call_t;

static void algorithm(call_t call, intmax_t x1, intmax_t y1,
	intmax_t x2, intmax_t y2)
{
	intmax_t x, y;
	intmax_t d, dx, dy;
	char chr;

	if(x1 > x2) {
		algorithm(call, x2, y2, x1, y1);
		return;
	}

	if(x1 == x2) {
		if(y1 > y2) {
			x = x1; chr = '|';
			for(y = y2; y <= y1; ++y) {
				call.function(call.buf, x, y, call.data);
				LSCb_set(call.buf, x, y, chr);
			}

			return;
		}

		if(y1 == y2) {
			x = x1; y = y1; chr = '+';

			call.function(call.buf, x, y, call.data);
			LSCb_set(call.buf, x, y, chr);

			return;
		}

		x = x1; chr = '|';
		for(y = y1; y <= y2; ++y) {
			call.function(call.buf, x, y, call.data);
			LSCb_set(call.buf, x, y, chr);
		}

		return;
	}

	if(y1 > y2) {
		dx = x2 - x1; dy = y1 - y2;

		if(dx > dy) {
			d = (2 * dy) - dx; y = y2; chr = '_';
			for(x = x2; x >= x1; --x) {
				call.function(call.buf, x, y, call.data);
				LSCb_set(call.buf, x, y, chr);

				if(d > 0) { ++y; d -= 2 * dx; chr = '/'; }
				else chr = '_';
				d += 2 * dy;
			}

			return;
		}

		if(dx == dy) {
			y = y1; chr = '/';
			for(x = x1; x <= x2; ++x) {
				call.function(call.buf, x, y, call.data);
				LSCb_set(call.buf, x, y, chr);
				--y;
			}

			return;
		}

		d = (2 * dx) - dy; x = x2; chr = '|';
		for(y = y2; y <= y1; ++y) {
			call.function(call.buf, x, y, call.data);
			LSCb_set(call.buf, x, y, chr);

			if(d > 0) { --x; d -= 2 * dy; chr = '/'; }
			else chr = '|';

			d += 2 * dx;
		}

		return;
	}

	if(y1 == y2) {
		y = y1; chr = '_';
		for(x = x1; x <= x2; ++x) {
			call.function(call.buf, x, y, call.data);
			LSCb_set(call.buf, x, y, chr);
		}

		return;
	}

	dx = x2 - x1; dy = y2 - y1;

	if(dx > dy) {
		d = (2 * dy) - dx; y = y1; chr = '_';
		for(x = x1; x <= x2; ++x) {
			call.function(call.buf, x, y, call.data);
			LSCb_set(call.buf, x, y, chr);

			if(d > 0) { ++y; d -= 2 * dx; chr = '\\'; }
			else chr = '_';

			d += 2 * dy;
		}

		return;
	}

	if(dx == dy) {
		y = y1; chr = '\\';
		for(x = x1; x <= x2; ++x) {
			call.function(call.buf, x, y, call.data);
			LSCb_set(call.buf, x, y, chr);
			++y;
		}

		return;
	}

	d = (2 * dx) - dy; x = x2; chr = '|';
	for(y = y2; y >= y1; --y) {
		call.function(call.buf, x, y, call.data);
		LSCb_set(call.buf, x, y, chr);

		if(d > 0) { --x; d -= 2 * dy; chr = '\\'; }
		else chr = '|';

		d += 2 * dx;
	}
}

void LSCl_draw(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2) {
	call_t call = {LSCb_sets, buf, " "};
	algorithm(call, x1, y1, x2, y2);
}

void LSCl_drawcol(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	uint8_t fg, uint8_t bg)
{
	char data[23];
	call_t call = {LSCb_setcols, buf, " "};

	sprintf(data, "\033[48;5;%03um\033[38;5;%03um", bg, fg);
	call.data = data;

	algorithm(call, x1, y1, x2, y2);
}

void LSCl_drawfg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	uint8_t fg)
{
	char data[12];
	call_t call = {LSCb_setfgs, buf, " "};

	sprintf(data, "\033[38;5;%03um", fg);
	call.data = data;

	algorithm(call, x1, y1, x2, y2);
}

void LSCl_drawbg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	uint8_t bg)
{
	char data[12];
	call_t call = {LSCb_setbgs, buf, " "};

	sprintf(data, "\033[48;5;%03um", bg);
	call.data = data;

	algorithm(call, x1, y1, x2, y2);
}