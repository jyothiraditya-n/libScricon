#! /bin/bash

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

chrs=" .,/"
fgs="    !\\\\\\\\]"
bgs="("

case "$1" in
"-about")
	echo "  libScricon: The Simple Graphical Console Library"
	echo "  Copyright (C) 2021 Jyothiraditya Nellakra"
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
;;

"-help")
	echo "Usage: $0 [OPTION] [SLOWDOWN]"
	echo ""
	echo "Valid values for OPTION are:"
	echo "-about: display the about dialogue."
	echo "-colour: enable colour support."
	echo "-help: display this help dialogue."
	echo ""
	echo -n "[SLOWDOWN] is an optional integer to change the speed at "
	echo -n "which the screen will update. Specifically, the number of "
	echo -n "updates per draw call is the number of characters divided "
	echo -n "by SLOWDOWN, so the default SLOWDOWN value of 10 means 10% "
	echo    "of the characters and colours are updated per draw call."
	echo ""
	echo "Happy coding! :)"
;;

"-colour")
	if [[ -z "$2" ]]; then
		slowdown="10"

	elif [ "$2" -eq "$2" ] 2>/dev/null; then
		slowdown="$2"

	else
		echo "$0: Error: Incorrect arguments."
		echo ""
		"$0" -help
		exit 1
	fi

	make randfw
	set -x; ./randfw $(tput cols) $(tput lines) "$chrs" "$fgs" "$bgs" "$slowdown"
;;

*)
	if [[ -z "$1" ]]; then
		slowdown="10"

	elif [ "$1" -eq "$1" ] 2>/dev/null; then
		slowdown="$1"

	else
		echo "$0: Error: Incorrect arguments."
		echo ""
		"$0" -help
		exit 1
	fi

	make randfw
	set -x; ./randfw -no-colour $(tput cols) $(tput lines) "$chrs" "$fgs" "$bgs" "$slowdown"
;;
esac