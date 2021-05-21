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
static LSCb_t buf;

static size_t height;
static size_t width;

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

static void engine(const char *chrs, size_t len) {
	LSCb_init(&buf);
	buf.height = height;
	buf.width = width;
	buf.fullwidth = true;

	LSCb_alloc(&buf);
	srand(time(NULL));

	for(size_t i = 0; i < buf.width; i++)
		for(size_t j = 0; j < buf.height; j++)
	{
		LSCb_set(&buf, i, j, chrs[rand() % len]);
	}

	while(1) {
		LSCb_set(&buf, rand() % buf.width,
			rand() % buf.height,
			chrs[rand() % len]);

		LSCb_print(&buf);
	}
}

static void help(const char *name) {
	printf("Usage: %s COLS ROWS [CHARS]\n", name);
	printf("       %s OPTION\n\n", name);

	printf("COLS is a mandatory argument specifying the width of the "
		"physical display in characters. ROWS is a mandatory argument "
		"specifying the height of the physical display in "
		"characters.\n\n");

	printf("[CHARS] is the optional list of characters to pick from "
		"when writing characters randomly to the screen. If not "
		"specified, the program will pick from all legal "
		"characters.\n\n");

	printf("NOTE: The optional list of characters is still subject to "
		"charset limitations, and some characters may not "
		"printed.\n\n");

	printf("Valid values for OPTION are:\n");
	printf("--about: Print the about dialogue.\n");
	printf("--help:  Print this help dialogue.\n\n");

	printf("Happy coding :)\n");
	return;
}

static void inp_err(const char *name) {
	fprintf(stderr, "%s: Error: Incorrect arguments.\n\n", name);
	help(name);
	exit(1);
}

int main(int argc, char **argv) {
	if(argc > 4) inp_err(argv[0]);

	if(argc == 2) {
		if(!strcmp(argv[1], "--about")) {
			about();
			exit(0);
		}

		if(!strcmp(argv[1], "--help")) {
			help(argv[0]);
			exit(0);
		}

		else inp_err(argv[0]);
	}

	int ret = sscanf(argv[1], "%zu", &width);
	if(ret != 1) inp_err(argv[0]);

	ret = sscanf(argv[2], "%zu", &height);
	if(ret != 1) inp_err(argv[0]);
	height--;

	if(argc == 3) {
		for(size_t i = 0; i < 256; i++) all_chrs[i] = i;
		engine(all_chrs, 256);
	}

	const char *chrs = argv[3];
	size_t len = strlen(chrs);

	if(!len) {
		for(size_t i = 0; i < 256; i++) all_chrs[i] = i;

		chrs = all_chrs;
		len = 256;
	}

	engine(chrs, len);
}