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

chrs=".,+*\`'                                                               ";
fgs="232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 \
     250 251 252 253 254 255 16 52 88 124 160 196 202 208 214 220 226 227 228 \
     229 230 231 195 159 123 87 51 45 39 33 27 21 20 19 18 17 16"

case "$1" in
"-about")
	echo ""
	echo "  libScricon: The Simple Graphical Console Library"
	echo "  Copyright (C) 2021-2022 Jyothiraditya Nellakra"
	echo "  Twinkling Stars Demonstration"
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

	make rand
	./rand -Cc "$chrs" -f $fgs -b 0 -d "$delay" -x 0.01
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

	make rand
	./rand -c "$chrs" -d "$delay" -x 0.01
;;
esac