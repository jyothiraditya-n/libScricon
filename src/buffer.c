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

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <LSC_buffer.h>
#include <LSC_error.h>

const char *LSCb_cch = "\033[48;5;000m\033[38;5;015m ";

void LSCb_init(LSCb_t *buf) {
	buf -> colour = false;

	buf -> height = 23;
	buf -> width = 80;
	buf -> depth = -1.0;

	buf -> data = NULL;
	buf -> zdata = NULL;
}

int LSCb_alloc(LSCb_t *buf) {
	buf -> chwidth = buf -> colour? strlen(LSCb_cch): 1;

	buf -> data = malloc(buf -> chwidth * buf -> width * buf -> height + 1);
	if(!buf -> data) {
		if(LSCe_auto) perror("stdlib");
		LSC_errno = LSC_MALLOC_ERR;
		return LSCE_NOOP;
	}

	buf -> zdata = malloc(sizeof(double) * buf -> width * buf -> height);
	if(!buf -> zdata) {
		if(LSCe_auto) perror("stdlib");
		LSC_errno = LSC_MALLOC_ERR;

		free(buf -> data);
		buf -> data = NULL;

		return LSCE_NOOP;
	}

	if(buf -> colour)
		for(size_t i = 0; i < buf -> width * buf -> height; i++)
			memcpy(&(buf -> data[i * buf -> chwidth]),
				LSCb_cch, buf -> chwidth);

	else memset(buf -> data, ' ', buf -> width * buf -> height);

	for(size_t i = 0; i < buf -> width * buf -> height; i++)
		buf -> zdata[i] = -buf -> depth;

	buf -> data[buf -> chwidth * buf -> width * buf -> height] = 0;
	return LSCE_OK;
}

void LSCb_clear(LSCb_t *buf) {
	if(buf -> colour)
		for(size_t i = 0; i < buf -> width * buf -> height; i++)
			memcpy(&(buf -> data[i * buf -> chwidth]),
				LSCb_cch, buf -> chwidth);

	else memset(buf -> data, ' ', buf -> width * buf -> height);

	for(size_t i = 0; i < buf -> width * buf -> height; i++)
		buf -> zdata[i] = 0.0;
}

void LSCb_free(LSCb_t *buf) {
	free(buf -> data);
	buf -> data = NULL;

	free(buf -> zdata);
	buf -> zdata = NULL;
	return;
}

void LSCb_set(LSCb_t *buf, size_t x, size_t y, char chr) {
	buf -> data[buf -> chwidth * (x + y * buf -> width + 1) - 1] = chr;
}

int LSCb_print(LSCb_t *buf, size_t y) {
	int ret = printf("\033[%zu;1H%s", y, buf -> data);
	if(ret < 0) {
		if(LSCe_auto) perror("stdlib");
		LSC_errno = LSC_PRINTF_ERR;
		return LSCE_NOOP;
	}

	return LSCE_OK;
}

void LSCb_setcol(LSCb_t *buf, size_t x, size_t y, uint8_t fg, uint8_t bg) {
	char chr = buf -> data[23 * (x + y * buf -> width) + 22];

	sprintf(buf -> data + 23 * (x + y * buf -> width),
		"\033[48;5;%03um\033[38;5;%03um", bg, fg);

	buf -> data[23 * (x + y * buf -> width) + 22] = chr;
}

void LSCb_setfg(LSCb_t *buf, size_t x, size_t y, uint8_t fg) {
	char chr = buf -> data[23 * (x + y * buf -> width) + 22];

	sprintf(buf -> data + 23 * (x + y * buf -> width) + 11,
		"\033[38;5;%03um", fg);

	buf -> data[23 * (x + y * buf -> width) + 22] = chr;
}

void LSCb_setbg(LSCb_t *buf, size_t x, size_t y, uint8_t bg) {
	sprintf(buf -> data + 23 * (x + y * buf -> width),
		"\033[48;5;%03um", bg);

	buf -> data[23 * (x + y * buf -> width) + 11] = '\033';
}

void LSCb_setall(LSCb_t *buf, size_t x, size_t y,
	char chr, uint8_t fg, uint8_t bg)
{
	sprintf(buf -> data + 23 * (x + y * buf -> width),
		"\033[48;5;%03um\033[38;5;%03um", bg, fg);

	buf -> data[23 * (x + y * buf -> width) + 22] = chr;
}

extern size_t LSCb_getx(LSCb_t *buf, double x) {
	const double width = buf -> width % 2? buf -> width: buf -> width - 1;
	return (width / 2) + x * (width / 2);
}

extern size_t LSCb_gety(LSCb_t *buf, double y) {
	const double height = buf -> height % 2?
		buf -> height: buf -> height - 1;

	return (height / 2) + y * (height / 2);
}

extern void LSCb_sets(LSCb_t *buf, size_t x, size_t y, const char *data) {
	buf -> data[buf -> chwidth * (x + y * buf -> width + 1) - 1] = data[0];
}

void LSCb_setcols(LSCb_t *buf, size_t x, size_t y, const char *data) {
	memcpy(buf -> data + 23 * (x + y * buf -> width), data, 22);
}

void LSCb_setfgs(LSCb_t *buf, size_t x, size_t y, const char *data) {
	memcpy(buf -> data + 23 * (x + y * buf -> width) + 11, data, 11);
}

void LSCb_setbgs(LSCb_t *buf, size_t x, size_t y, const char *data) {
	memcpy(buf -> data + 23 * (x + y * buf -> width), data, 11);
}

void LSCb_setalls(LSCb_t *buf, size_t x, size_t y, const char *data) {
	memcpy(buf -> data + 23 * (x + y * buf -> width), data, 23);
}

void LSCb_setz(LSCb_t *buf, size_t x, size_t y, double z, char chr) {
	if(buf -> zdata[x + y * buf -> width] > z) return;
	LSCb_set(buf, x, y, chr);
}

void LSCb_setcolz(LSCb_t *buf, size_t x, size_t y, double z,
	uint8_t fg, uint8_t bg)
{
	if(buf -> zdata[x + y * buf -> width] > z) return;
	LSCb_setcol(buf, x, y, fg, bg);
}

void LSCb_setfgz(LSCb_t *buf, size_t x, size_t y, double z, uint8_t fg) {
	if(buf -> zdata[x + y * buf -> width] > z) return;
	LSCb_setfg(buf, x, y, fg);
}

void LSCb_setbgz(LSCb_t *buf, size_t x, size_t y, double z, uint8_t bg) {
	if(buf -> zdata[x + y * buf -> width] > z) return;
	LSCb_setbg(buf, x, y, bg);
}

void LSCb_setallz(LSCb_t *buf, size_t x, size_t y, double z,
	char chr, uint8_t fg, uint8_t bg)
{
	if(buf -> zdata[x + y * buf -> width] > z) return;
	LSCb_setall(buf, x, y, chr, fg, bg);
}

size_t LSCb_getxz(LSCb_t *buf, double x, double z) {
	return LSCb_getx(buf, x * -buf -> depth / z);
}

size_t LSCb_getyz(LSCb_t *buf, double y, double z) {
	return LSCb_gety(buf, y * -buf -> depth / z);
}

void LSCb_setsz(LSCb_t *buf, size_t x, size_t y, double z, const char *data) {
	if(buf -> zdata[x + y * buf -> width] > z) return;
	LSCb_sets(buf, x, y, data);
	buf -> zdata[x + y * buf -> width] = z;
}

void LSCb_setcolsz(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data)
{
	if(buf -> zdata[x + y * buf -> width] > z) return;
	LSCb_setcols(buf, x, y, data);
	buf -> zdata[x + y * buf -> width] = z;
}

void LSCb_setfgsz(LSCb_t *buf, size_t x, size_t y, double z, const char *data) {
	if(buf -> zdata[x + y * buf -> width] > z) return;
	LSCb_setfgs(buf, x, y, data);
	buf -> zdata[x + y * buf -> width] = z;
}

void LSCb_setbgsz(LSCb_t *buf, size_t x, size_t y, double z, const char *data) {
	if(buf -> zdata[x + y * buf -> width] > z) return;
	LSCb_setbgs(buf, x, y, data);
	buf -> zdata[x + y * buf -> width] = z;
}

void LSCb_setallsz(LSCb_t *buf, size_t x, size_t y, double z,
	const char *data)
{
	if(buf -> zdata[x + y * buf -> width] > z) return;
	LSCb_setalls(buf, x, y, data);
	buf -> zdata[x + y * buf -> width] = z;
}