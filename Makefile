# libScricon: The Simple Graphical Console Library
# Copyright (C) 2021-2022 Jyothiraditya Nellakra
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

headers = $(wildcard include/*.h)
objs = $(patsubst %.c,%.o,$(wildcard src/*.c))

demos = $(patsubst demo/%.c,%,$(wildcard demo/*.c))
demo_objs = $(patsubst %.c,%.o,$(wildcard demo/*.c))
demo_shs += $(patsubst demo/%.sh,%,$(wildcard demo/*.sh))

files = $(foreach file,$(objs) $(demo_objs),$(wildcard $(file)))
files += $(foreach file,$(demos) $(demo_shs),$(wildcard $(file)))
files += $(wildcard *.a)

CLEAN = $(foreach file,$(files),rm $(file);)

CPPFLAGS += -std=gnu17 -Wall -Wextra -Wpedantic -O0 -g
CPPFLAGS += -I inc/ -I libClame/inc/

CFLAGS += -std=gnu17 -O0 -g

libs = libClame/libClame.a libScricon.a
LD_LIBS ?= -L. -lScricon -lm -L libClame -lClame

$(objs) : %.o : %.c $(headers)
	$(CC) $(CPPFLAGS) -c $< -o $@

libScricon.a : $(objs)
	$(AR) -r libScricon.a $(objs)

$(demo_objs) : %.o : %.c $(headers)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(demos) : % : demo/%.o $(libs)
	$(CC) $(CFLAGS) $< -o $@ $(LD_LIBS)

$(demo_shs) : % : demo/%.sh
	cp $< $@; chmod +x $@

libClame/libClame.a : libClame
	+cd libClame; make libClame.a; cd ..

.DEFAULT_GOAL = all
.PHONY : all clean

all : libScricon.a $(demos) $(demo_shs)

clean :
	$(CLEAN)
	+cd libClame; make clean; cd ..