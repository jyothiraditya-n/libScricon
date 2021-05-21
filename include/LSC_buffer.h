/* libScricon: The Simple Graphical Console Library
 * Copyright (C) 2021 Jyothiraditya Nellakra
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

#ifndef LSC_BUFFER_H
#define LSC_BUFFER_H 1

#define LSCB_NOCHECKS 0
#define LSCB_ASCII 1
#define LSCB_IBM437 2

#define LSCB_LF "\n"
#define LSCB_CRLF "\r\n"

#ifdef _WIN32
	#define LSCB_CHARSET LSCB_IBM437
	#define LSCB_ENDL LSCB_CRLF

#else
	#define LSCB_CHARSET LSCB_ASCII
	#define LSCB_ENDL LSCB_LF

#endif

#define LSCB_USE_ANSI true

#define LSCB_HEIGHT 18
#define LSCB_WIDTH 60

typedef struct {
	int charset;
	const char *endl;
	bool fullwidth;
	bool use_ansi;
	bool use_colour;

	size_t height;
	size_t width;
	char *data;

	size_t width_eff;

} LSCb_t;

extern void LSCb_init(LSCb_t *buf);
extern int LSCb_alloc(LSCb_t *buf);
extern void LSCb_clear(LSCb_t *buf);

extern int LSCb_set(LSCb_t *buf, size_t x, size_t y, char chr);
extern int LSCb_setc(LSCb_t *buf, size_t x, size_t y, int fg, int bg);
extern int LSCb_print(LSCb_t *buf);

#endif