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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LSC_buffer.h>
#include <LSC_error.h>

void LSCb_init(LSCb_t *buf) {
	buf -> charset = LSCB_CHARSET;
	buf -> endl = LSCB_ENDL;
	buf -> fullwidth = false;
	buf -> use_ansi = LSCB_USE_ANSI;

	buf -> height = LSCB_HEIGHT;
	buf -> width = LSCB_WIDTH;
	buf -> data = NULL;
}

int LSCb_alloc(LSCb_t *buf) {
	size_t endl_len = buf -> fullwidth ? 0 : strlen(buf -> endl);
	size_t width_eff = buf -> width + endl_len;

	buf -> data = malloc(buf -> height * width_eff + 1);

	if(!buf -> data) {
		if(LSCe_auto) perror("stdlib");
		LSC_errno = LSC_MALLOC_ERR;
		return LSCE_NOOP;
	}

	char *data = buf -> data;

	for(size_t i = 0; i < buf -> height; i++) {
		for(size_t j = 0; j < buf -> width; j++) {
			*data = ' ';
			data++;
		}

		strcpy(data, buf -> endl);
		data += endl_len;
	}

	buf -> width_eff = width_eff;
	return LSCE_OK;
}

void LSCb_clear(LSCb_t *buf) {
	free(buf -> data);
	buf -> data = 0;
	return;
}

static bool islegal(char chr, int set) {
	if(set == LSCB_NOCHECKS) return true;

	else if(set == LSCB_ASCII) {
		if(chr < LSCB_ASCII_MIN) return false;
		if(chr > LSCB_ASCII_MAX) return false;
	}

	else if(set == LSCB_IBM437) {
		if(chr < LSCB_IBM437_MIN) return false;
		if(strchr(LSCB_IBM437_ILL, chr)) return false;
	}

	return true;
}

int LSCb_set(LSCb_t *buf, size_t x, size_t y, char chr) {
	if(x >= buf -> width || y >= buf -> height) return LSCE_ILLEGAL;
	if(!islegal(chr, buf -> charset)) return LSCE_ILLEGAL;

	buf -> data[x + y * buf -> width_eff] = chr;
	return LSCE_OK;
}

int LSCb_print(LSCb_t *buf) {
	int ret;

	if(buf -> use_ansi) ret = printf("\033[;f%s", buf -> data);
	else ret = printf("%s", buf -> data);

	if(ret < 0) {
		if(LSCe_auto) perror("stdlib");
		LSC_errno = LSC_PRINTF_ERR;
		return LSCE_NOOP;
	}

	return LSCE_OK;
}
