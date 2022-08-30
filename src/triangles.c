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
#include <LSC_triangles.h>

typedef struct {
	void (*function)(LSCb_t *buf, size_t x, size_t y, const char *data);
	LSCb_t *buf;
	const char *data;

} call_t;

static intmax_t max(intmax_t a, intmax_t b, intmax_t c) {
	return a > b? (a > c? a: c): (b > c? b: c);
}

static intmax_t min(intmax_t a, intmax_t b, intmax_t c) {
	return a < b? (a < c? a: c): (b < c? b: c);
}

static void algorithm(call_t call, intmax_t x1, intmax_t y1, intmax_t x2,
	intmax_t y2, intmax_t x3, intmax_t y3)
{
	intmax_t dx12 = x1 - x2, dx23 = x2 - x3, dx31 = x3 - x1;
	intmax_t dy12 = y1 - y2, dy23 = y2 - y3, dy31 = y3 - y1;

	intmax_t minx = min(x1, x2, x3), maxx = max(x1, x2, x3);
	intmax_t miny = min(y1, y2, y3), maxy = max(y1, y2, y3);

	intmax_t oy = miny;

	double c1 = dy12 * x1 - dx12 * y1;
	double c2 = dy23 * x2 - dx23 * y2;
	double c3 = dy31 * x3 - dx31 * y3;

	double cy1 = c1 + dx12 * miny - dy12 * minx;
	double cy2 = c2 + dx23 * miny - dy23 * minx;
	double cy3 = c3 + dx31 * miny - dy31 * minx;

	for(intmax_t y = miny; y < maxy; y++) {
		double cx1 = cy1, cx2 = cy2, cx3 = cy3;

		for(intmax_t x = minx; x < maxx; x++) {
			if(cx1 > 0 && cx2 > 0 && cx3 > 0)
				call.function(call.buf, x, oy, call.data);

			cx1 -= dy12; cx2 -= dy23;
			cx3 -= dy31;
		}

		cy1 += dx12; cy2 += dx23;
		cy3 += dx31; oy++;
	}
}

void LSCt_set(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	size_t x3, size_t y3, char chr)
{
	bool validate = buf -> validate == LSCB_VALIDATE_CHAR;

	if(buf -> validate == LSCB_VALIDATE_SHAPE) {
		if(x1 >= buf -> width || y1 >= buf -> height
			|| x2 >= buf -> width || y2 >= buf -> height
			|| x3 >= buf -> width || y3 >= buf -> height)
		{ return; }
	}

	char data[2] = {chr, ' '};
	call_t call = {validate? LSCb_setsv: LSCb_sets, buf, " "};
	call.data = data;

	algorithm(call, x1, y1, x2, y2, x3, y3);
}

void LSCt_setcol(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	size_t x3, size_t y3, uint8_t fg, uint8_t bg)
{
	bool validate = buf -> validate == LSCB_VALIDATE_CHAR;

	if(buf -> validate == LSCB_VALIDATE_SHAPE) {
		if(x1 >= buf -> width || y1 >= buf -> height
			|| x2 >= buf -> width || y2 >= buf -> height
			|| x3 >= buf -> width || y3 >= buf -> height)
		{ return; }
	}

	char data[23];
	call_t call = {validate? LSCb_setcolsv: LSCb_setcols, buf, " "};

	sprintf(data, "\033[48;5;%03um\033[38;5;%03um", bg, fg);
	call.data = data;

	algorithm(call, x1, y1, x2, y2, x3, y3);
}

void LSCt_setfg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	size_t x3, size_t y3, uint8_t fg)
{
	bool validate = buf -> validate == LSCB_VALIDATE_CHAR;

	if(buf -> validate == LSCB_VALIDATE_SHAPE) {
		if(x1 >= buf -> width || y1 >= buf -> height
			|| x2 >= buf -> width || y2 >= buf -> height
			|| x3 >= buf -> width || y3 >= buf -> height)
		{ return; }
	}

	char data[12];
	call_t call = {validate? LSCb_setfgsv: LSCb_setfgs, buf, " "};

	sprintf(data, "\033[38;5;%03um", fg);
	call.data = data;

	algorithm(call, x1, y1, x2, y2, x3, y3);
}

void LSCt_setbg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	size_t x3, size_t y3, uint8_t bg)
{
	bool validate = buf -> validate == LSCB_VALIDATE_CHAR;

	if(buf -> validate == LSCB_VALIDATE_SHAPE) {
		if(x1 >= buf -> width || y1 >= buf -> height
			|| x2 >= buf -> width || y2 >= buf -> height
			|| x3 >= buf -> width || y3 >= buf -> height)
		{ return; }
	}

	char data[12];
	call_t call = {validate? LSCb_setbgsv: LSCb_setbgs, buf, " "};

	sprintf(data, "\033[48;5;%03um", bg);
	call.data = data;

	algorithm(call, x1, y1, x2, y2, x3, y3);
}

void LSCt_setall(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	size_t x3, size_t y3, char chr, uint8_t fg, uint8_t bg)
{
	bool validate = buf -> validate == LSCB_VALIDATE_CHAR;

	if(buf -> validate == LSCB_VALIDATE_SHAPE) {
		if(x1 >= buf -> width || y1 >= buf -> height
			|| x2 >= buf -> width || y2 >= buf -> height
			|| x3 >= buf -> width || y3 >= buf -> height)
		{ return; }
	}

	char data[24];
	call_t call = {validate? LSCb_setallsv: LSCb_setalls, buf, " "};

	sprintf(data, "\033[48;5;%03um\033[38;5;%03um%c", bg, fg, chr);
	call.data = data;

	algorithm(call, x1, y1, x2, y2, x3, y3);
}