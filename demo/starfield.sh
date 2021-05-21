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

chars=".,+*\`                                                                ";

case "$1" in
"--about")
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

	exit
;;

"--help")
	echo "Usage: $0 [OPTION]"
	echo ""
	echo "Valid values for OPTION are:"
	echo "--about: display the about dialogue."
	echo "--help: display this help dialogue."
	echo ""
	echo "Happy coding! :)"

	exit
;;
esac

make random_fullwidth
./random_fullwidth $(tput cols) $(tput lines) "$chars"

exit