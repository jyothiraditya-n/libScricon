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

#include <threads.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef LSC_ERROR
#define LSC_ERROR 1

#define LSCE_ILLEGAL -1
#define LSCE_OK 0
#define LSCE_NOOP 1
#define LSCE_REC 2
#define LSCE_NO_REC 3

#define LSC_MALLOC_ERR 1
#define LSC_MTX_INIT_ERR 2
#define LSC_MTX_LOCK_ERR 3
#define LSC_MTX_UNLOCK_ERR 4
#define LSC_THREAD_CREATE_ERR 5
#define LSC_THREAD_JOIN_ERR 6
#define LSC_PRINTF_ERR 7

extern bool LSCe_auto;

extern thread_local int LSC_errno;
extern const char *LSC_strerror(int err);
extern void LSC_perror();

#endif