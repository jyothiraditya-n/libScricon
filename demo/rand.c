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

#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <libClame/args.h>
#include <libClame/vars.h>

#include <libScricon/buffer.h>
#include <libScricon/error.h>
#include <libScricon/scrolling.h>

static const char *name;
static LSCb_t buf;

static size_t len_chrs = 256;
static char *chrs = " !\"#$%&'()*+,-./"
	"0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWX"
	"YZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

static size_t len_fgs = 16;
static uint8_t fgs[64] = {
	30, 31, 32, 33, 34, 35, 36, 37,
	90, 91, 92, 93, 94, 95, 96, 97
};

static size_t len_bgs = 16;
static uint8_t bgs[64] = {
	40, 41, 42, 43, 44, 45, 46, 47,
	100, 101, 102, 103, 104, 105, 106, 107
};

static bool colour = true;
static bool fullwidth = false;
static bool fullheight = false;

static size_t height = 0;
static size_t width = 0;

static size_t delay = 1;
static double change = 0.1;

static double scroll;
static intmax_t scroll_rows = 0;
static size_t scroll_delay = 0;

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

	exit(0);
}

static void refill() {
	intmax_t lines = imaxabs(scroll_rows);

	if(scroll_rows < 0)
		for(size_t i = buf.height - lines; i < buf.height; i++)
		for(size_t j = 0; j < buf.width; j++)
	{
		if(buf.use_colour) LSCb_setall(&buf, j, i,
			chrs[rand() % len_chrs],
			fgs[rand() % len_fgs],
			bgs[rand() % len_bgs]);

		else LSCb_set(&buf, j, i, chrs[rand() % len_chrs]);
	}

	else if(scroll_rows > 0)
		for(intmax_t i = 0; i < lines; i++)
		for(size_t j = 0; j < buf.width; j++)
	{
		if(buf.use_colour) LSCb_setall(&buf, j, i,
			chrs[rand() % len_chrs],
			fgs[rand() % len_fgs],
			bgs[rand() % len_bgs]);

		else LSCb_set(&buf, j, i, chrs[rand() % len_chrs]);
	}
}

static void mkdelay() {
	struct timespec ts;
	int ret;

	ts.tv_sec = delay / 1000;
	ts.tv_nsec = (delay % 1000) * 1000000;

	do ret = nanosleep(&ts, &ts);
	while (ret && errno == EINTR);
}

static void engine() {
	LSCb_init(&buf);
	buf.fullwidth = fullwidth;
	buf.use_colour = colour;

	if(fullheight && height) height--;
	if(height) buf.height = height;
	if(width) buf.width = width;

	LSCb_alloc(&buf);
	srand(time(NULL));

	for(size_t i = 0; i < buf.width; i++)
		for(size_t j = 0; j < buf.height; j++)
	{
		if(buf.use_colour) LSCb_setall(&buf, i, j,
			chrs[rand() % len_chrs],
			fgs[rand() % len_fgs],
			bgs[rand() % len_bgs]);

		else LSCb_set(&buf, i, j, chrs[rand() % len_chrs]);
	}

	size_t step = 0;
	size_t count = fabs(buf.width * buf.height * change);
	scroll_rows = buf.height * scroll;

	while(1) {
		LSCb_print(&buf);
		mkdelay();


		for(size_t i = 0; i < count; i++) {
			if(len_fgs > 1) LSCb_setfg(&buf,
				rand() % buf.width,
				rand() % buf.height,
				fgs[rand() % len_fgs]);

			if(len_bgs > 1) LSCb_setbg(&buf,
				rand() % buf.width,
				rand() % buf.height,
				bgs[rand() % len_bgs]);

			if(len_chrs > 1) LSCb_set(&buf,
				rand() % buf.width,
				rand() % buf.height,
				chrs[rand() % len_chrs]);
		}

		if(step == scroll_delay) {
			LSC_scrollv(&buf, scroll_rows);
			refill();
			step = 0;
		}

		else step++;
	}
}

static void help() {
	printf("Usage: %s [OPTIONS]\n\n", name);

	printf("Valid options are:\n");
	printf("-about: Print the about dialogue.\n");
	printf("-help:  Print this help dialogue.\n\n");

	printf("-chars CHARS: Select random characters from CHARS "
		"(one string argument).\n");

	printf("-fgs FGS: Select random foreground colours from FGS. "
		"(Sequence of up to 16 unsigned integer arguments).\n");

	printf("-fgs BGS: Select random background colours from BGS. "
		"(Sequence of up to 16 unsigned integer arguments).\n\n");

	printf("-colour: Enable colour output.\n");
	printf("-no-colour: Disable colour output.\n\n");

	printf("-fullwidth: Don't print newlines after each line.\n");
	printf("-no-fullwidth: Print newlines after each line. \n\n");

	printf("NOTE: You'll want the former if WIDTH is also the width of "
		"your console window. Otherwise, use the latter.\n\n");

	printf("-fullheight: Automatically subtract one from HEIGHT.\n");
	printf("-no-fullheight: Don't automatically subtract one from "
		"HEIGHT.\n\n");

	printf("NOTE: You'll want the former if HEIGHT is also the height of "
		"your console window. Otherwise, use the latter.\n\n");

	printf("-height HEIGHT: Set height to unsigned integer HEIGHT.\n");
	printf("-height WIDTH: Set width to unsigned integer WIDTH.\n\n");

	printf("-delay DELAY: The number of milliseconds to delay printing. "
		"(unsigned integer number)\n\n");

	printf("-change VALUE: The number of randomisations to make to "
		"screen data as a fraction of the total number of "
		"charcaters. (double between 0.0 and 1.0)");

	printf("-scroll-rows ROWS: The number of rows to scroll up (if ROWS "
		"is negative) or down (if ROWS is positive) whenever the "
		"screen is scrolled as a percentage of the number of rows.\n\n");

	printf("-scroll-delay DELAY: The number of frames during which the "
		"screen is not scrolled (between the frames during which it "
		"is scrolled). (unsigned integer number)\n\n");

	printf("Happy coding! :)\n");
	return;
}

static void error(const char *msg) {
	printf("%s: Error: %s\n", name, msg);
	help();
	exit(1);
}

int main(int argc, char **argv) {
	name = argv[0];

	for(int i = 1; i < argc; i++) {
		if(!strcmp(argv[i], "-about")) about();

		if(!strcmp(argv[i], "-help")) {
			help();
			exit(0);
		}

		if(!strcmp(argv[i], "-chars")) {
			if(i == argc - 1) error("Option -chars requires the "
				"list of characters after it.");

			i++;
			chrs = argv[i];
			len_chrs = strlen(chrs);
		}

		else if(!strcmp(argv[i], "-fgs")) {
			if(i == argc - 1) error("Option -fgs requires at "
				"least one valid colour code after it.");

			i++;
			int ret = sscanf(argv[i], "%" SCNu8, &fgs[0]);

			if(ret != 1) error("Option -fgs requires at "
				"least one valid colour code after it.");

			int j = i;
			for(i++; i < argc && i - j < 64; i++) {
				ret = sscanf(argv[i], "%" SCNu8, &fgs[i - j]);
				if(ret != 1) break;
			}

			len_fgs = i - j;
			i--;
		}

		else if(!strcmp(argv[i], "-bgs")) {
			if(i == argc - 1) error("Option -bgs requires at "
				"least one valid colour code after it.");

			i++;
			int ret = sscanf(argv[i], "%" SCNu8, &bgs[0]);

			if(ret != 1) error("Option -bgs requires at "
				"least one valid colour code after it.");

			int j = i;
			
			for(i++; i < argc && i - j < 64; i++) {
				ret = sscanf(argv[i], "%" SCNu8, &bgs[i - j]);
				if(ret != 1) break;
			}

			len_bgs = i - j;
			i--;
		}

		else if(!strcmp(argv[i], "-colour")) colour = true;
		else if(!strcmp(argv[i], "-no-colour")) colour = false;

		else if(!strcmp(argv[i], "-fullwidth")) fullwidth = true;
		else if(!strcmp(argv[i], "-no-fullwidth")) fullwidth = false;

		else if(!strcmp(argv[i], "-fullheight")) fullheight = true;
		else if(!strcmp(argv[i], "-no-fullheight")) fullheight = false;

		else if(!strcmp(argv[i], "-height")) {
			if(i == argc - 1) error("Option -height requires the "
				"height of the screen after it.");

			i++;
			int ret = sscanf(argv[i], "%zu", &height);

			if(ret != 1) error("Option -height requires the "
				"height of the screen after it.");
		}

		else if(!strcmp(argv[i], "-width")) {
			if(i == argc - 1) error("Option -width requires the "
				"width of the screen after it.");

			i++;
			int ret = sscanf(argv[i], "%zu", &width);

			if(ret != 1) error("Option -width requires the "
				"width of the screen after it.");
		}

		else if(!strcmp(argv[i], "-delay")) {
			if(i == argc - 1) error("Option -delay requires an "
				"unsigned integer after it.");

			i++;
			int ret = sscanf(argv[i], "%zu", &delay);

			if(ret != 1) error("Option -delay requires an "
				"unsigned integer after it.");

			if(!delay) error("0 is not a valid value of DELAY.");
		}

		else if(!strcmp(argv[i], "-change")) {
			if(i == argc - 1) error("Option -change requires a "
				"decimal number after it.");

			i++;
			int ret = sscanf(argv[i], "%lf", &change);

			if(ret != 1) error("Option -width requires a "
				"decimal number after it.");
		}

		else if(!strcmp(argv[i], "-scroll-rows")) {
			if(i == argc - 1) error("Option -scroll-rows "
				"requires a decimal number after it.");

			i++;
			int ret = sscanf(argv[i], "%lf", &scroll);

			if(ret != 1) error("Option -scroll-rows requires a "
				"decimal number after it.");
		}

		else if(!strcmp(argv[i], "-scroll-delay")) {
			if(i == argc - 1) error("Option -scroll-delay "
				"requires an unsigned integer after it.");

			i++;
			int ret = sscanf(argv[i], "%zu", &scroll_delay);

			if(ret != 1) error("Option -scroll-delay requires an "
				"unsigned integer after it.");
		}

		else if(!strlen(argv[i])) continue;

		else {
			char msg[32 + strlen(argv[i])];
			sprintf(msg, "Unknown option '%s'.", argv[i]);
			error(msg);
		}
	}

	engine();
}