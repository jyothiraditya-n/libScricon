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
#include <stdint.h>
#include <stddef.h>

#ifndef LSC_BUFFER_H
#define LSC_BUFFER_H 1

typedef struct {
	bool colour;
	size_t chwidth;

	size_t height;
	size_t width;

	char *data;
	double *zdata;
} LSCb_t;

extern const char *LSCb_cch;

extern void LSCb_init(LSCb_t *buf);
extern int LSCb_alloc(LSCb_t *buf);

extern void LSCb_clear(LSCb_t *buf);
extern void LSCb_free(LSCb_t *buf);

extern void LSCb_set(LSCb_t *buf, size_t x, size_t y, char chr);
extern int LSCb_print(LSCb_t *buf, size_t y);

extern void LSCb_setcol(LSCb_t *buf, size_t x, size_t y,
	uint8_t fg, uint8_t bg);

extern void LSCb_setfg(LSCb_t *buf, size_t x, size_t y, uint8_t fg);
extern void LSCb_setbg(LSCb_t *buf, size_t x, size_t y, uint8_t bg);

extern void LSCb_setall(LSCb_t *buf, size_t x, size_t y,
	char chr, uint8_t fg, uint8_t bg);

#endif