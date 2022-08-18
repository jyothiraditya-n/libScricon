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

#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <signal.h>
#include <termios.h>
#include <unistd.h>

#include <LC_args.h>
#include <LC_vars.h>

#include <LSC_buffer.h>
#include <LSC_error.h>
#include <LSC_scrolling.h>

static const char *name;
static LSCb_t buffer;

static struct termios cooked, raw;
static size_t height, width;

static size_t len_chrs;
static char chrs[256];

static size_t len_fgs;
static uint8_t fgs[256];

static size_t len_bgs;
static uint8_t bgs[256];

static bool colour;
static bool running = true;

static size_t delay = 1;
static double change = 0.1;

static double scroll;
static intmax_t scroll_rows;
static size_t scroll_delay;

static void init(int argc, char **argv);
static void on_interrupt(int signum);

static void refill() {
	intmax_t lines = imaxabs(scroll_rows);

	if(scroll_rows < 0)
		for(size_t i = buffer.height - lines; i < buffer.height; i++)
		for(size_t j = 0; j < buffer.width; j++)
	{
		if(colour) LSCb_setall(&buffer, j, i, chrs[rand() % len_chrs],
			fgs[rand() % len_fgs], bgs[rand() % len_bgs]);

		else LSCb_set(&buffer, j, i, chrs[rand() % len_chrs]);
	}

	else if(scroll_rows > 0)
		for(intmax_t i = 0; i < lines; i++)
		for(size_t j = 0; j < buffer.width; j++)
	{
		if(colour) LSCb_setall(&buffer, j, i, chrs[rand() % len_chrs],
			fgs[rand() % len_fgs], bgs[rand() % len_bgs]);

		else LSCb_set(&buffer, j, i, chrs[rand() % len_chrs]);
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
	static size_t step = 0;
	size_t count = fabs(buffer.width * buffer.height * change);

	LSCb_print(&buffer, 1);
	mkdelay();

	for(size_t i = 0; i < count; i++) {
		if(colour && len_fgs > 1) LSCb_setfg(&buffer,
			rand() % buffer.width, rand() % buffer.height,
			fgs[rand() % len_fgs]);

		if(colour && len_bgs > 1) LSCb_setbg(&buffer,
			rand() % buffer.width, rand() % buffer.height,
			bgs[rand() % len_bgs]);

		if(len_chrs > 1) LSCb_set(&buffer,
			rand() % buffer.width, rand() % buffer.height,
			chrs[rand() % len_chrs]);
	}

	if(step == scroll_delay) {
		LSC_scrollv(&buffer, scroll_rows);
		refill();
		step = 0;
	}

	else step++;
}

int main(int argc, char **argv) {
	name = argv[0];
	init(argc, argv);

	signal(SIGINT, on_interrupt);

	int ret = tcgetattr(STDIN_FILENO, &cooked);
	if(ret == -1) {
		puts("Error getting terminal properties with tcgetattr().");
		exit(1);
	}

	raw = cooked;
	raw.c_lflag &= ~(ICANON | ECHO);

	ret = tcsetattr(STDIN_FILENO, TCSANOW, &raw);
	if(ret == -1) {
		puts("Error setting terminal properties with tcsetattr().");
		exit(2);
	}

	printf("\033[999;999H\033[6n");
	while(getchar() != '\033');

	ret = scanf("[%zu;%zuR", &height, &width);
	if(ret != 2) {
		tcsetattr(STDIN_FILENO, TCSANOW, &cooked);
		puts("Error getting screen size with ANSI escape codes.");
		exit(3);
	}

	printf("\033[?25l");
	LSCb_init(&buffer);

	buffer.colour = colour;
	buffer.height = height;
	buffer.width = width;

	ret = LSCb_alloc(&buffer);
	if(ret != LSCE_OK) {
		tcsetattr(STDIN_FILENO, TCSANOW, &cooked);
		puts("Error initialising libScricon.");
		printf("\033[?25h");
		exit(4);
	}

	srand(time(NULL));

	for(size_t i = 0; i < buffer.width; i++)
		for(size_t j = 0; j < buffer.height; j++)
	{
		if(colour) LSCb_setall(&buffer, i, j,
			chrs[rand() % len_chrs],
			fgs[rand() % len_fgs],
			bgs[rand() % len_bgs]);

		else LSCb_set(&buffer, i, j, chrs[rand() % len_chrs]);
	}

	scroll_rows = buffer.height * scroll;
	while(running) engine();

	tcsetattr(STDIN_FILENO, TCSANOW, &cooked);
	printf("\033[?25h");
	return 0;	
}

static void about() {
	putchar('\n');
	puts("  libScricon: The Simple Graphical Console Library");
	puts("  Copyright (C) 2021-2022 Jyothiraditya Nellakra");
	puts("  Random Noise Generator\n");

	puts("  This program is free software: you can redistribute it and/or modify");
	puts("  it under the terms of the GNU General Public License as published by");
	puts("  the Free Software Foundation, either version 3 of the License, or");
	puts("  (at your option) any later version.\n");

	puts("  This program is distributed in the hope that it will be useful,");
	puts("  but WITHOUT ANY WARRANTY; without even the implied warranty of");
	puts("  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the");
	puts("  GNU General Public License for more details.\n");

	puts("  You should have received a copy of the GNU General Public License");
	puts("  along with this program. If not, see <https://www.gnu.org/licenses/>.\n");

	exit(0);
}

static void help(int ret) {
	putchar('\n');
	printf("  Usage: %s [OPTIONS]\n\n", name);

	puts("  Valid options are:");
	puts("    -a, --about             print the about dialogue");
	puts("    -h, --help              print this help dialogue\n");

	puts("    -c, --chrs CHRS         set the characters to use");
	puts("    -f, --fgs FGS           set the foreground colours to use");
	puts("    -b, --bgs BGS           set the background colours to use\n");

	puts("    -C, --colour            enable colour output");
	puts("    -d, --delay MILLISECS   the length of time to delay printing");
	puts("    -x, --change FRAC       the number of changes to make as a fraction of the");
	puts("                            total number of charcaters.\n");

	puts("    -s, --scroll-rows ROWS  number of rows to scroll up (if negative) or down");
	puts("                            (if positive) as a fraction of the screen height.");
	puts("    -S, --scroll-delay DLY  number of frames to wait to scroll the screen.\n");

	puts("  Note: CHRS is a single-string argument. FGS and BGS are a list of integers");
	puts("        separated by spaces. FRAC and ROWS are doubles between 0.0 and 1.0.\n");

	puts("  Happy coding! :)\n");
	exit(ret);
}

static void help_flag() {
	help(0);
}

static void init(int argc, char **argv) {
	LCa_t *arg = LCa_new();
	arg -> long_flag = "about";
	arg -> short_flag = 'a';
	arg -> pre = about;

	arg = LCa_new();
	arg -> long_flag = "help";
	arg -> short_flag = 'h';
	arg -> pre = help_flag;

	LCv_t *var = LCv_new();
	var -> id = "chrs";
	var -> fmt = "%95c";
	var -> data = chrs;

	arg = LCa_new();
	arg -> long_flag = "chrs";
	arg -> short_flag = 'c';
	arg -> var = var;

	var = LCv_new();
	var -> id = "fgs";
	var -> fmt = "%" SCNu8;
	var -> data = fgs;
	var -> len = &len_fgs;
	var -> min_len = 0;
	var -> max_len = 256;
	var -> size = sizeof(uint8_t);

	arg = LCa_new();
	arg -> long_flag = "fgs";
	arg -> short_flag = 'f';
	arg -> var = var;

	var = LCv_new();
	var -> id = "bgs";
	var -> fmt = "%" SCNu8;
	var -> data = bgs;
	var -> len = &len_bgs;
	var -> min_len = 0;
	var -> max_len = 256;
	var -> size = sizeof(uint8_t);

	arg = LCa_new();
	arg -> long_flag = "bgs";
	arg -> short_flag = 'b';
	arg -> var = var;

	var = LCv_new();
	var -> id = "colour";
	var -> data = &colour;

	arg = LCa_new();
	arg -> long_flag = "colour";
	arg -> short_flag = 'C';
	arg -> var = var;
	arg -> value = true;

	var = LCv_new();
	var -> id = "delay";
	var -> fmt = "%zu";
	var -> data = &delay;

	arg = LCa_new();
	arg -> long_flag = "delay";
	arg -> short_flag = 'd';
	arg -> var = var;

	var = LCv_new();
	var -> id = "change";
	var -> fmt = "%lf";
	var -> data = &change;

	arg = LCa_new();
	arg -> long_flag = "change";
	arg -> short_flag = 'x';
	arg -> var = var;

	var = LCv_new();
	var -> id = "scroll_rows";
	var -> fmt = "%lf";
	var -> data = &scroll;

	arg = LCa_new();
	arg -> long_flag = "scroll-rows";
	arg -> short_flag = 's';
	arg -> var = var;

	var = LCv_new();
	var -> id = "scroll_delay";
	var -> fmt = "%zu";
	var -> data = &scroll_delay;

	arg = LCa_new();
	arg -> long_flag = "scroll-delay";
	arg -> short_flag = 'S';
	arg -> var = var;

	int ret = LCa_read(argc, argv);
	if(ret != LCA_OK) help(1);

	len_chrs = strlen(chrs);
	if(!len_chrs) {
		for(uint8_t i = ' '; i <= '~'; i++) chrs[i - ' '] = i;
		len_chrs = '~' - ' ' + 1;
	}

	if(!len_fgs) {
		for(size_t i = 0; i < 256; i++) fgs[i] = i;
		len_fgs = 256;
	}

	if(!len_bgs) {
		for(size_t i = 0; i < 256; i++) bgs[i] = i;
		len_bgs = 256;
	}
}

static void on_interrupt(int signum) {
	if(signum != SIGINT) {
		signal(signum, SIG_DFL);
		return;
	}

	running = false;
}