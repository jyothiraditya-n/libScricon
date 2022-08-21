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
	double screen;

	bool validate;
	const char *cchs;
	char cch;

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

extern size_t LSCb_getx(LSCb_t *buf, double x);
extern size_t LSCb_gety(LSCb_t *buf, double y);

extern void LSCb_sets(LSCb_t *buf, size_t x, size_t y, const char *data);
extern void LSCb_setcols(LSCb_t *buf, size_t x, size_t y, const char *data);
extern void LSCb_setfgs(LSCb_t *buf, size_t x, size_t y, const char *data);
extern void LSCb_setbgs(LSCb_t *buf, size_t x, size_t y, const char *data);
extern void LSCb_setalls(LSCb_t *buf, size_t x, size_t y, const char *data);

extern void LSCb_setz(LSCb_t *buf, size_t x, size_t y, double z, char chr);

extern void LSCb_setcolz(LSCb_t *buf, size_t x, size_t y, double z,
	uint8_t fg, uint8_t bg);

extern void LSCb_setfgz(LSCb_t *buf, size_t x, size_t y, double z, uint8_t fg);
extern void LSCb_setbgz(LSCb_t *buf, size_t x, size_t y, double z, uint8_t bg);

extern void LSCb_setallz(LSCb_t *buf, size_t x, size_t y, double z,
	char chr, uint8_t fg, uint8_t bg);

extern size_t LSCb_getxz(LSCb_t *buf, double x, double z);
extern size_t LSCb_getyz(LSCb_t *buf, double y, double z);

extern void LSCb_setsz(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data);

extern void LSCb_setcolsz(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data);

extern void LSCb_setfgsz(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data);

extern void LSCb_setbgsz(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data);

extern void LSCb_setallsz(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data);

extern void LSCb_setv(LSCb_t *buf, size_t x, size_t y, char chr);

extern void LSCb_setcolv(LSCb_t *buf, size_t x, size_t y,
	uint8_t fg, uint8_t bg);

extern void LSCb_setfgv(LSCb_t *buf, size_t x, size_t y, uint8_t fg);
extern void LSCb_setbgv(LSCb_t *buf, size_t x, size_t y, uint8_t bg);

extern void LSCb_setallv(LSCb_t *buf, size_t x, size_t y,
	char chr, uint8_t fg, uint8_t bg);

extern void LSCb_setsv(LSCb_t *buf, size_t x, size_t y, const char *data);
extern void LSCb_setcolsv(LSCb_t *buf, size_t x, size_t y, const char *data);
extern void LSCb_setfgsv(LSCb_t *buf, size_t x, size_t y, const char *data);
extern void LSCb_setbgsv(LSCb_t *buf, size_t x, size_t y, const char *data);
extern void LSCb_setallsv(LSCb_t *buf, size_t x, size_t y, const char *data);

extern void LSCb_setzv(LSCb_t *buf, size_t x, size_t y, double z, char chr);

extern void LSCb_setcolzv(LSCb_t *buf, size_t x, size_t y, double z,
	uint8_t fg, uint8_t bg);

extern void LSCb_setfgzv(LSCb_t *buf, size_t x, size_t y, double z, uint8_t fg);
extern void LSCb_setbgzv(LSCb_t *buf, size_t x, size_t y, double z, uint8_t bg);

extern void LSCb_setallzv(LSCb_t *buf, size_t x, size_t y, double z,
	char chr, uint8_t fg, uint8_t bg);

extern void LSCb_setszv(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data);

extern void LSCb_setcolszv(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data);

extern void LSCb_setfgszv(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data);

extern void LSCb_setbgszv(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data);

extern void LSCb_setallszv(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data);

#endif