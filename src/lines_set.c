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
	intmax_t d, dx = x2 - x1, dy;

	if(x1 > x2) {
		algorithm(call, x2, y2, x1, y1);
		return;
	}

	if(x1 == x2) {
		if(y1 > y2) {
			x = x2;
			for(y = y2; y <= y1; ++y)
				call.function(call.buf, x, y, call.data);
			return;
		}

		if(y1 == y2) {
			x = x1; y = y1;
			call.function(call.buf, x, y, call.data);
			return;
		}

		x = x1;
		for(y = y1; y <= y2; ++y)
			call.function(call.buf, x, y, call.data);
		return;
	}

	if(y1 > y2) {
		dy = y1 - y2;

		if(dx > dy) {
			d = (2 * dy) - dx; y = y2;
			for(x = x2; x >= x1; --x) {
				call.function(call.buf, x, y, call.data);

				if(d > 0) { ++y; d -= 2 * dx; }
				d += 2 * dy;
			}

			return;
		}

		if(dx == dy) {
			y = y1;
			for(x = x1; x <= x2; ++x) {
				call.function(call.buf, x, y, call.data);
				--y;
			}

			return;
		}

		d = (2 * dx) - dy; x = x2;
		for(y = y2; y <= y1; ++y) {
			call.function(call.buf, x, y, call.data);

			if(d > 0) { --x; d -= 2 * dy; }
			d += 2 * dx;
		}

		return;
	}

	if(y1 == y2) {
		y = y1;
		for(x = x1; x <= x2; ++x)
			call.function(call.buf, x, y, call.data);
		return;
	}

	dy = y2 - y1;

	if(dx > dy) {
		d = (2 * dy) - dx; y = y1;
		for(x = x1; x <= x2; ++x) {
			call.function(call.buf, x, y, call.data);

			if(d > 0) { ++y; d -= 2 * dx; }
			d += 2 * dy;
		}

		return;
	}

	if(dx == dy) {
		y = y1;
		for(x = x1; x <= x2; ++x) {
			call.function(call.buf, x, y, call.data);
			++y;
		}

		return;
	}

	d = (2 * dx) - dy; x = x2;
	for(y = y2; y >= y1; --y) {
		call.function(call.buf, x, y, call.data);

		if(d > 0) { --x; d -= 2 * dy; }
		d += 2 * dx;
	}
}

void LSCl_set(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	char chr)
{
	bool validate = buf -> validate == LSCB_VALIDATE_CHAR;

	if(buf -> validate == LSCB_VALIDATE_SHAPE) {
		if(x1 >= buf -> width || x2 >= buf -> width
			|| y1 >= buf -> height || y2 >= buf -> height)
		{ return; }
	}

	char data[2] = {chr, ' '};
	call_t call = {validate? LSCb_setsv: LSCb_sets, buf, " "};
	call.data = data;

	algorithm(call, x1, y1, x2, y2);
}

void LSCl_setcol(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	uint8_t fg, uint8_t bg)
{
	bool validate = buf -> validate == LSCB_VALIDATE_CHAR;

	if(buf -> validate == LSCB_VALIDATE_SHAPE) {
		if(x1 >= buf -> width || x2 >= buf -> width
			|| y1 >= buf -> height || y2 >= buf -> height)
		{ return; }
	}

	char data[23];
	call_t call = {validate? LSCb_setcolsv: LSCb_setcols, buf, " "};

	sprintf(data, "\033[48;5;%03um\033[38;5;%03um", bg, fg);
	call.data = data;

	algorithm(call, x1, y1, x2, y2);
}

void LSCl_setfg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	uint8_t fg)
{
	bool validate = buf -> validate == LSCB_VALIDATE_CHAR;

	if(buf -> validate == LSCB_VALIDATE_SHAPE) {
		if(x1 >= buf -> width || x2 >= buf -> width
			|| y1 >= buf -> height || y2 >= buf -> height)
		{ return; }
	}

	char data[12];
	call_t call = {validate? LSCb_setfgsv: LSCb_setfgs, buf, " "};

	sprintf(data, "\033[38;5;%03um", fg);
	call.data = data;

	algorithm(call, x1, y1, x2, y2);
}

void LSCl_setbg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	uint8_t bg)
{
	bool validate = buf -> validate == LSCB_VALIDATE_CHAR;

	if(buf -> validate == LSCB_VALIDATE_SHAPE) {
		if(x1 >= buf -> width || x2 >= buf -> width
			|| y1 >= buf -> height || y2 >= buf -> height)
		{ return; }
	}

	char data[12];
	call_t call = {validate? LSCb_setbgsv: LSCb_setbgs, buf, " "};

	sprintf(data, "\033[48;5;%03um", bg);
	call.data = data;

	algorithm(call, x1, y1, x2, y2);
}

void LSCl_setall(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	char chr, uint8_t fg, uint8_t bg)
{
	bool validate = buf -> validate == LSCB_VALIDATE_CHAR;

	if(buf -> validate == LSCB_VALIDATE_SHAPE) {
		if(x1 >= buf -> width || x2 >= buf -> width
			|| y1 >= buf -> height || y2 >= buf -> height)
		{ return; }
	}

	char data[24];
	call_t call = {validate? LSCb_setallsv: LSCb_setalls, buf, " "};

	sprintf(data, "\033[48;5;%03um\033[38;5;%03um%c", bg, fg, chr);
	call.data = data;

	algorithm(call, x1, y1, x2, y2);
}