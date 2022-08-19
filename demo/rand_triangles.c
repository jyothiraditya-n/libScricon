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
#include <LSC_triangles.h>

const char *name;
LSCb_t buffer;

struct termios cooked, raw;
size_t height, width;

size_t len_chrs;
char chrs[256];

size_t len_fgs;
uint8_t fgs[256];

size_t len_bgs;
uint8_t bgs[256];

bool colour;
bool running = true;

size_t count = 1;
size_t delay = 1;

void init(int argc, char **argv);
void on_interrupt(int signum);

void mkdelay() {
	struct timespec ts;
	int ret;

	ts.tv_sec = delay / 1000;
	ts.tv_nsec = (delay % 1000) * 1000000;

	do ret = nanosleep(&ts, &ts);
	while (ret && errno == EINTR);
}

void engine() {
	LSCb_print(&buffer, 1);
	mkdelay();

	for(size_t i = 0; i < count; i++) {
		size_t x1 = rand() % buffer.width, y1 = rand() % buffer.height;
		size_t x2 = rand() % buffer.width, y2 = rand() % buffer.height;
		size_t x3 = rand() % buffer.width, y3 = rand() % buffer.height;

		if(colour && len_fgs) LSCt_setfg(&buffer, x1, y1, x2, y2, x3, y3,
			fgs[rand() % len_fgs]);

		if(colour && len_bgs) LSCt_setbg(&buffer, x1, y1, x2, y2, x3, y3,
			bgs[rand() % len_bgs]);

		if(len_chrs) LSCt_set(&buffer, x1, y1, x2, y2, x3, y3,
			chrs[rand() % len_chrs]);

		else LSCt_set(&buffer, x1, y1, x2, y2, x3, y3, ' ');
	}
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
	while(running) engine();

	tcsetattr(STDIN_FILENO, TCSANOW, &cooked);
	printf("\033[?25h");
	return 0;
}

void about() {
	putchar('\n');
	puts("  libScricon: The Simple Graphical Console Library");
	puts("  Copyright (C) 2021-2022 Jyothiraditya Nellakra");
	puts("  Random Triangle Generator\n");

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

void help(int ret) {
	putchar('\n');
	printf("  Usage: %s [OPTIONS]\n\n", name);

	puts("  Valid options are:");
	puts("    -a, --about             print the about dialogue");
	puts("    -h, --help              print this help dialogue\n");

	puts("    -c, --chrs CHRS         set the characters to use");
	puts("    -f, --fgs FGS           set the foreground colours to use");
	puts("    -b, --bgs BGS           set the background colours to use\n");

	puts("    -n, --count             set the number of triangles per frame");
	puts("    -C, --colour            enable colour output\n");
	
	puts("  Note: CHRS is a single-string argument. FGS and BGS are a list of integers");
	puts("        separated by spaces. FRAC and ROWS are doubles between 0.0 and 1.0.\n");

	puts("  Happy coding! :)\n");
	exit(ret);
}

void help_flag() {
	help(0);
}

void init(int argc, char **argv) {
	LCa_t *arg = LCa_new(); arg -> long_flag = "about";
	arg -> short_flag = 'a'; arg -> pre = about;
	arg = LCa_new(); arg -> long_flag = "help";
	arg -> short_flag = 'h'; arg -> pre = help_flag;

	LCv_t *var = LCv_new(); var -> id = "chrs";
	var -> fmt = "%95c"; var -> data = chrs;
	arg = LCa_new(); arg -> long_flag = "chrs";
	arg -> short_flag = 'c'; arg -> var = var;

	var = LCv_new(); var -> id = "fgs"; var -> fmt = "%" SCNu8;
	var -> data = fgs; var -> len = &len_fgs; var -> min_len = 0;
	var -> max_len = 256; var -> size = sizeof(uint8_t);
	arg = LCa_new(); arg -> long_flag = "fgs";
	arg -> short_flag = 'f'; arg -> var = var;

	var = LCv_new(); var -> id = "bgs"; var -> fmt = "%" SCNu8;
	var -> data = bgs; var -> len = &len_bgs; var -> min_len = 0;
	var -> max_len = 256; var -> size = sizeof(uint8_t);
	arg = LCa_new(); arg -> long_flag = "bgs";
	arg -> short_flag = 'b'; arg -> var = var;

	var = LCv_new(); var -> id = "colour"; var -> data = &colour;
	arg = LCa_new(); arg -> long_flag = "colour"; arg -> short_flag = 'C';
	arg -> var = var; arg -> value = true;

	var = LCv_new(); var -> id = "delay";
	var -> fmt = "%zu"; var -> data = &delay;
	arg = LCa_new(); arg -> long_flag = "delay";
	arg -> short_flag = 'd'; arg -> var = var;

	var = LCv_new(); var -> id = "count";
	var -> fmt = "%zu"; var -> data = &count;
	arg = LCa_new(); arg -> long_flag = "count";
	arg -> short_flag = 'n'; arg -> var = var;

	int ret = LCa_read(argc, argv);
	if(ret != LCA_OK) help(1);

	len_chrs = strlen(chrs);
	if(!colour && !len_chrs) {
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

void on_interrupt(int signum) {
	if(signum != SIGINT) {
		signal(signum, SIG_DFL);
		return;
	}

	running = false;
}