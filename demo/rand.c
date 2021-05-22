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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <LSC_buffer.h>
#include <LSC_error.h>

static char all_chrs[256];
static char all_fgs[16];
static char all_bgs[16];

static LSCb_t buf;
static bool no_colour = false;
static size_t slowdown = 10;

static void about() {
	printf("  libScricon: The Simple Graphical Console Library\n");
	printf("  Copyright (C) 2021 Jyothiraditya Nellakra\n\n");

	printf("  This program is free software: you can redistribute it and/or modify\n");
	printf("  it under the terms of the GNU General Public License as published by\n");
	printf("  the Free Software Foundation, either version 3 of the License, or\n");
	printf("  (at your option) any later version.\n\n");

	printf("  This program is distributed in the hope that it will be useful,\n");
	printf("  but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	printf("  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n");
	printf("  GNU General Public License for more details.\n\n");

	printf("  You should have received a copy of the GNU General Public License\n");
	printf("  along with this program. If not, see <https://www.gnu.org/licenses/>.\n\n");
	return;
}

static void engine(const char *chrs, const char *fgs, const char *bgs,
	size_t len_chrs, size_t len_fgs, size_t len_bgs)
{
	LSCb_init(&buf);
	buf.use_colour = !no_colour;

	LSCb_alloc(&buf);
	srand(time(NULL));

	for(size_t i = 0; i < buf.width; i++)
		for(size_t j = 0; j < buf.height; j++)
	{
		LSCb_setc(&buf, i, j,
			(int) fgs[rand() % len_fgs],
			(int) bgs[rand() % len_bgs]);

		LSCb_set(&buf, i, j, chrs[rand() % len_chrs]);
	}

	size_t steps = (buf.width * buf.height) / slowdown;

	while(1) {
		for(size_t i = 0; i < steps; i++) {
			if(len_fgs > 1) LSCb_setfg(&buf, rand() % buf.width,
				rand() % buf.height,
				(int) fgs[rand() % len_fgs]);

			if(len_bgs > 1) LSCb_setbg(&buf, rand() % buf.width,
				rand() % buf.height,
				(int) bgs[rand() % len_bgs]);

			if(len_chrs > 1) LSCb_set(&buf, rand() % buf.width,
				rand() % buf.height,
				chrs[rand() % len_chrs]);
		}

		LSCb_print(&buf);
	}
}

static void help(const char *name) {
	printf("Usage: %s [-no-colour] [CHARS] [FG_COLS] [BG_COLS] [SLOWDOWN]\n", name);
	printf("       %s OPTION\n\n", name);

	printf("[CHARS] is the optional list of characters to pick from "
		"when writing characters randomly to the screen. If not "
		"specified, the program will pick from all legal "
		"characters.\n\n");

	printf("NOTE: The optional list of characters is still subject to "
		"charset limitations, and some characters may not "
		"printed.\n\n");

	printf("[*_COLOURS] are the optional lists of colours to pick from "
		"when writing colours randomly to the screen. If not "
		"specified, the program will pick from all legal "
		"colours.\n\n");

	printf("NOTE 1: The optional list of colours is still subject to "
		"ANSI limitations, and some colours may not printed.\n\n");

	printf("NOTE 2: The colours are passed a a string of characters, and "
		"the ASCII values of the characters are used to set the ANSI "
		"colour code.\n\n");

	printf("[SLOWDOWN] is an optional integer to change the speed at "
		"which the screen will update. Specifically, the number of "
		"updates per draw call is the number of characters divided by "
		"SLOWDOWN, so the default SLOWDOWN value of 10 means 10%% of "
		"the characters and colours are updated per draw call.\n\n");

	printf("Valid values for OPTION are:\n");
	printf("-about: Print the about dialogue.\n");
	printf("-help:  Print this help dialogue.\n\n");

	printf("Happy coding :)\n");
	return;
}

static void inp_err(const char *name) {
	fprintf(stderr, "%s: Error: Incorrect arguments.\n\n", name);
	help(name);
	exit(1);
}

int main(int argc, char **argv) {
	if(argc == 1) {
		for(size_t i = 0; i < 256; i++) all_chrs[i] = i;

		for(size_t i = 0; i < 8; i++) all_fgs[i] = 30 + i;
		for(size_t i = 0; i < 8; i++) all_fgs[i + 8] = 90 + i;

		for(size_t i = 0; i < 8; i++) all_bgs[i] = 40 + i;
		for(size_t i = 0; i < 8; i++) all_bgs[i + 8] = 100 + i;

		engine(all_chrs, all_fgs, all_bgs, 256, 16, 16);
	}

	if(!strcmp(argv[1], "-about")) {
		about();
		exit(0);
	}

	if(!strcmp(argv[1], "-help")) {
		help(argv[0]);
		exit(0);
	}

	if(!strcmp(argv[1], "-no-colour")) {
		no_colour = true;
	}

	const char *chrs = argv[1 + (int) no_colour];
	size_t len_chrs = strlen(chrs);

	if(!len_chrs) {
		for(size_t i = 0; i < 256; i++) all_chrs[i] = i;

		chrs = all_chrs;
		len_chrs = 256;
	}

	if(argc == 2 + (int) no_colour) {
		for(size_t i = 0; i < 8; i++) all_fgs[i] = 30 + i;
		for(size_t i = 0; i < 8; i++) all_fgs[i + 8] = 90 + i;

		for(size_t i = 0; i < 8; i++) all_bgs[i] = 40 + i;
		for(size_t i = 0; i < 8; i++) all_bgs[i + 8] = 100 + i;

		engine(chrs, all_fgs, all_bgs, len_chrs, 16, 16);
	}

	const char *fgs = argv[2 + (int) no_colour];
	size_t len_fgs = strlen(fgs);

	if(!len_fgs) {
		for(size_t i = 0; i < 8; i++) all_fgs[i] = 30 + i;
		for(size_t i = 0; i < 8; i++) all_fgs[i + 8] = 90 + i;

		fgs = all_fgs;
		len_fgs = 16;
	}

	if(argc == 3 + (int) no_colour) {
		for(size_t i = 0; i < 8; i++) all_bgs[i] = 40 + i;
		for(size_t i = 0; i < 8; i++) all_bgs[i + 8] = 100 + i;

		engine(chrs, fgs, all_bgs, len_chrs, len_fgs, 16);
	}

	const char *bgs = argv[3 + (int) no_colour];
	size_t len_bgs = strlen(bgs);

	if(!len_bgs) {
		for(size_t i = 0; i < 8; i++) all_bgs[i] = 40 + i;
		for(size_t i = 0; i < 8; i++) all_bgs[i + 8] = 100 + i;

		bgs = all_bgs;
		len_bgs = 16;
	}

	if(argc > 4 + (int) no_colour) {
		int ret = sscanf(argv[4 + (int) no_colour], "%zu", &slowdown);
		if(ret != 1) inp_err(argv[0]);
	}

	engine(chrs, fgs, bgs, len_chrs, len_fgs, len_bgs);
}