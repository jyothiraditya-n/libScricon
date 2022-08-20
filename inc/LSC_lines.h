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

#ifndef LSC_LINES_H
#define LSC_LINES_H 1

extern void LSCl_set(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	char chr);

extern void LSCl_setcol(LSCb_t *buf, size_t x1, size_t y1, size_t x2,
	size_t y2, uint8_t fg, uint8_t bg);

extern void LSCl_setfg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	uint8_t fg);

extern void LSCl_setbg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	uint8_t bg);

extern void LSCl_setall(LSCb_t *buf, size_t x1, size_t y1, size_t x2,
	size_t y2, char chr, uint8_t fg, uint8_t bg);

extern void LSCl_draw(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2);

extern void LSCl_drawcol(LSCb_t *buf, size_t x1, size_t y1, size_t x2,
	size_t y2, uint8_t fg, uint8_t bg);

extern void LSCl_drawfg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	uint8_t fg);

extern void LSCl_drawbg(LSCb_t *buf, size_t x1, size_t y1, size_t x2, size_t y2,
	uint8_t bg);

extern void LSCl_setz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, char chr);

extern void LSCl_setcolz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, uint8_t fg, uint8_t bg);

extern void LSCl_setfgz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, uint8_t fg);

extern void LSCl_setbgz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, uint8_t bg);

extern void LSCl_setallz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, char chr, uint8_t fg, uint8_t bg);

extern void LSCl_drawz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2);

extern void LSCl_drawcolz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, uint8_t fg, uint8_t bg);

extern void LSCl_drawfgz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, uint8_t fg);

extern void LSCl_drawbgz(LSCb_t *buf, size_t x1, size_t y1, double z1,
	size_t x2, size_t y2, double z2, uint8_t bg);

#endif