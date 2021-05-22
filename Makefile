# libScricon: The Simple Graphical Console Library
# Copyright (C) 2021 Jyothiraditya Nellakra
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <https://www.gnu.org/licenses/>.

objs = $(patsubst %.c,%.o,$(wildcard source/*.c))
demo_objs = $(patsubst %.c,%.o,$(wildcard demo/*.c))

headers = $(wildcard include/*.h)
demos = $(patsubst demo/%.c,%,$(wildcard demo/*.c))
demo_shs += $(patsubst demo/%.sh,%,$(wildcard demo/*.sh))

files = $(foreach file,$(objs),$(wildcard $(file)))
files += $(foreach file,$(demo_objs),$(wildcard $(file)))
files += $(foreach file,$(demos),$(wildcard $(file)))
files += $(foreach file,$(demo_shs),$(wildcard $(file)))
files += $(wildcard *.a)

CLEAN = $(foreach file,$(files),rm $(file);)

CC ?= gcc
CFLAGS ?= -L.
CPPFLAGS ?= -std=c17 -Wall -Wextra -Werror -O3 -I include/

AR ?= ar

LIBS ?= -lScricon

$(objs) : %.o : %.c $(headers)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(demo_objs) : %.o : %.c $(headers)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(demos) : % : demo/%.o libScricon.a
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

$(demo_shs) : % : demo/%.sh
	cp $< $@; chmod +x $@

libScricon.a : $(objs)
	$(AR) -r libScricon.a $(objs)

.DEFAULT_GOAL = all
.PHONY : all clean

all : libScricon.a $(demos) $(demo_shs)

clean :
	$(CLEAN)