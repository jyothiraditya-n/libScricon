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

#include <stddef.h>
#include <stdint.h>

#include <LSC_buffer.h>

#ifndef LSC_TRIANGLES_H
#define LSC_TRIANGLES_H 1

extern void LSCt_set(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	size_t x3, size_t y3, char chr);

extern void LSCt_setcol(LSCb_t *buf, size_t x1, size_t y1, size_t x2,
	size_t y2, size_t x3, size_t y3, uint8_t fg, uint8_t bg);

extern void LSCt_setfg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	size_t x3, size_t y3, uint8_t fg);

extern void LSCt_setbg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	size_t x3, size_t y3, uint8_t bg);

extern void LSCt_setall(LSCb_t *buf, size_t x1, size_t y1, size_t x2,
	size_t y2, size_t x3, size_t y3, char chr, uint8_t fg, uint8_t bg);

#endif