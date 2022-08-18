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

#include <threads.h>
#include <stdbool.h>
#include <stdio.h>

#include <LSC_error.h>

#define MAX_ERR 7

bool LSCe_auto = true;
thread_local int LSC_errno;

static const char *errors[MAX_ERR + 1] = {
	"Unknown error.",
	"Memory allocation error.",
	"Mutex initialisation error.",
	"Mutex locking error.",
	"Mutex unlocking error.",
	"Thread creation error.",
	"Thread joining error.",
	"Error printing data."
};

const char *LSC_strerror(int err) {
	if(err < 0 || err > MAX_ERR) err = 0;
	return errors[err];
}

void LSC_perror() {
	fprintf(stderr, "libScricon: %s", LSC_strerror(LSC_errno));
	return;
}