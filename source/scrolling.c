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
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <libScricon/buffer.h>

static void scroll_up(LSCb_t *buf, intmax_t lines) {
	for(size_t i = 0; i < buf -> height - lines; i++)
		for(size_t j = 0; j < buf -> width_eff; j++)
	{
		buf -> data[j + i * buf -> width_eff]
		= buf -> data[j + (i + lines) * buf -> width_eff];
	}

	for(size_t i = buf -> height - lines; i < buf -> height; i++)
		for(size_t j = 0; j < buf -> width; j++)
	{
		if(buf -> use_colour) LSCb_setall(buf, j, i, ' ', 37, 40);
		else LSCb_set(buf, j, i, ' ');
	}
}

void LSC_scrollv(LSCb_t *buf, intmax_t lines) {
	if(lines == 0) return;

	if(lines < 0) {
		scroll_up(buf, imaxabs(lines));
		return;
	}

	lines = imaxabs(lines);
	for(intmax_t i = buf -> height - 1; i >= lines; i--)
		for(size_t j = 0; j < buf -> width_eff; j++)
	{
		buf -> data[j + i * buf -> width_eff]
		= buf -> data[j + (i - lines) * buf -> width_eff];
	}

	for(intmax_t i = 0; i < lines; i++)
		for(size_t j = 0; j < buf -> width; j++)
	{
		if(buf -> use_colour) LSCb_setall(buf, j, i, ' ', 37, 40);
		else LSCb_set(buf, j, i, ' ');
	}

	return;
}