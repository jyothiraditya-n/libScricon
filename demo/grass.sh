#! /bin/bash

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

fgs="34 70 106 142 178 214 40 76 112 148 184 46 82 118 154 190 226"

case "$1" in
"-about")
	echo ""
	echo "  libScricon: The Simple Graphical Console Library"
	echo "  Copyright (C) 2021-2022 Jyothiraditya Nellakra"
	echo "  Waving Grass Demonstration"
	echo ""
	echo "  This program is free software: you can redistribute it and/or modify"
	echo "  it under the terms of the GNU General Public License as published by"
	echo "  the Free Software Foundation, either version 3 of the License, or"
	echo "  (at your option) any later version."
	echo ""
	echo "  This program is distributed in the hope that it will be useful,"
	echo "  but WITHOUT ANY WARRANTY; without even the implied warranty of"
	echo "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
	echo "  GNU General Public License for more details."
	echo ""
	echo "  You should have received a copy of the GNU General Public License"
	echo "  along with this program.  If not, see <https://www.gnu.org/licenses/>."
	echo ""
;;

"-help")
	echo ""
	echo "  Usage: $0 [OPTION] [DELAY]"
	echo ""
	echo "  Valid options are:"
	echo "    -about   display the about dialogue."
	echo "    -colour  enable colour support."
	echo "    -help    display this help dialogue."
	echo ""
	echo "  Note: [DELAY] is the unsigned integer number of milliseconds to delay"
	echo "        printing."
	echo ""
	echo "  Happy coding! :)"
	echo ""
;;

"-colour")
	if [[ -z "$2" ]]; then
		delay="0"

	elif [ "$2" -eq "$2" ] 2>/dev/null; then
		delay="$2"

	else
		echo "$0: Error: Incorrect arguments."
		echo ""
		"$0" -help
		exit 1
	fi

	make rand_chars
	./rand_chars -Cc "_.,-/" -f $fgs -b 22 -d "$delay"
;;

*)
	if [[ -z "$1" ]]; then
		delay="0"

	elif [ "$1" -eq "$1" ] 2>/dev/null; then
		delay="$1"

	else
		echo "$0: Error: Incorrect arguments."
		echo ""
		"$0" -help
		exit 1
	fi

	make rand_chars
	./rand_chars -c "_.,-/" -d "$delay"
;;
esac