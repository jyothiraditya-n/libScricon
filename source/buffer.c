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

#include <inttypes.h>
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
	buf -> use_colour = false;

	buf -> height = LSCB_HEIGHT;
	buf -> width = LSCB_WIDTH;
	buf -> data = NULL;
}

int LSCb_alloc(LSCb_t *buf) {
	if(!buf -> height) buf -> height = LSCB_HEIGHT;
	if(!buf -> width) buf -> width = LSCB_WIDTH;

	size_t endl_len = buf -> fullwidth ? 0 : strlen(buf -> endl);
	size_t width_eff;

	if(buf -> use_colour) width_eff = 10 * buf -> width + endl_len;
	else width_eff = buf -> width + endl_len;

	buf -> data = malloc(buf -> height * width_eff + 1);

	if(!buf -> data) {
		if(LSCe_auto) perror("stdlib");
		LSC_errno = LSC_MALLOC_ERR;
		return LSCE_NOOP;
	}

	char *data = buf -> data;

	for(size_t i = 0; i < buf -> height; i++) {
		for(size_t j = 0; j < buf -> width; j++) {
			if(buf -> use_colour) {
				strcpy(data, "\033[37;040m");
				data += 9;
			}

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

static bool islegalchr(char chr, int set) {
	if(set == LSCB_NOCHECKS) return true;

	else if(set == LSCB_ASCII) {
		if(chr < ' ') return false;
		if(chr > '~') return false;
	}

	else if(set == LSCB_IBM437) {
		if(chr < ' ') return false;
		if(chr == 0x7f) return false;
	}

	return true;
}

int LSCb_set(LSCb_t *buf, size_t x, size_t y, char chr) {
	if(x >= buf -> width || y >= buf -> height) return LSCE_ILLEGAL;
	if(!islegalchr(chr, buf -> charset)) return LSCE_ILLEGAL;

	if(buf -> use_colour) {
		buf -> data[9 + 10 * x + y * buf -> width_eff] = chr;
	}

	else buf -> data[x + y * buf -> width_eff] = chr;
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

static bool islegalfg(uint8_t fg) {
	if(30 <= fg && fg <= 37) return true;
	if(90 <= fg && fg <= 97) return true;
	return false;
}

static bool islegalbg(uint8_t bg) {
	if(40 <= bg && bg <= 47) return true;
	if(100 <= bg && bg <= 107) return true;
	return false;
}

int LSCb_setcol(LSCb_t *buf, size_t x, size_t y, uint8_t fg, uint8_t bg) {
	if(!buf -> use_colour) return LSCE_ILLEGAL;
	if(x >= buf -> width || y >= buf -> height) return LSCE_ILLEGAL;
	if(!islegalfg(fg)) return LSCE_ILLEGAL;
	if(!islegalbg(bg)) return LSCE_ILLEGAL;

	char chr = buf -> data[9 + 10 * x + y * buf -> width_eff];

	if(bg < 100) sprintf(buf -> data + 10 * x + y * buf -> width_eff,
		"\033[%" PRIu8 ";0%" PRIu8 "m", fg, bg);

	else sprintf(buf -> data + 10 * x + y * buf -> width_eff,
		"\033[%d;%dm", fg, bg);

	buf -> data[9 + 10 * x + y * buf -> width_eff] = chr;
	return LSCE_OK;
}

extern int LSCb_setfg(LSCb_t *buf, size_t x, size_t y, uint8_t fg) {
	if(!buf -> use_colour) return LSCE_ILLEGAL;
	if(x >= buf -> width || y >= buf -> height) return LSCE_ILLEGAL;
	if(!islegalfg(fg)) return LSCE_ILLEGAL;

	sprintf(buf -> data + 10 * x + y * buf -> width_eff,
		"\033[%" PRIu8,
		fg);

	buf -> data[4 + 10 * x + y * buf -> width_eff] = ';';
	return LSCE_OK;
}

extern int LSCb_setbg(LSCb_t *buf, size_t x, size_t y, uint8_t bg) {
	if(!buf -> use_colour) return LSCE_ILLEGAL;
	if(x >= buf -> width || y >= buf -> height) return LSCE_ILLEGAL;
	if(!islegalbg(bg)) return LSCE_ILLEGAL;

	char chr = buf -> data[9 + 10 * x + y * buf -> width_eff];

	if(bg < 100) sprintf(buf -> data + 5 + 10 * x + y * buf -> width_eff,
			"0%" PRIu8 "m", bg);

	else sprintf(buf -> data + 5 + 10 * x + y * buf -> width_eff,
			"%" PRIu8 "m", bg);

	buf -> data[9 + 10 * x + y * buf -> width_eff] = chr;
	return LSCE_OK;
}

int LSCb_setall(LSCb_t *buf, size_t x, size_t y,
	char chr, uint8_t fg, uint8_t bg)
{
	if(!buf -> use_colour) return LSCE_ILLEGAL;
	if(x >= buf -> width || y >= buf -> height) return LSCE_ILLEGAL;
	if(!islegalchr(chr, buf -> charset)) return LSCE_ILLEGAL;
	if(!islegalfg(fg)) return LSCE_ILLEGAL;
	if(!islegalbg(bg)) return LSCE_ILLEGAL;

	if(bg < 100) sprintf(buf -> data + 10 * x + y * buf -> width_eff,
		"\033[%" PRIu8 ";0%" PRIu8 "m", fg, bg);

	else sprintf(buf -> data + 10 * x + y * buf -> width_eff,
		"\033[%d;%dm", fg, bg);

	buf -> data[9 + 10 * x + y * buf -> width_eff] = chr;
	return LSCE_OK;
}