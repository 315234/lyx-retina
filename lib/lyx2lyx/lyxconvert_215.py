# This file is part of lyx2lyx
# -*- coding: iso-8859-1 -*-
# Copyright (C) 2002 Jos� Matos <jamatos@lyx.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

import re
from parser_tools import *

layout_exp = re.compile(r"\\layout (\S*)")

math_env = ["\\[","\\begin{eqnarray*}","\\begin{eqnarray}","\\begin{equation}"]

def replace_protected_separator(lines):
    i=0
    while 1:
        i = find_token(lines, "\\protected_separator", i)
        if i == -1:
            break
        j = find_token_backwards(lines, "\\layout", i)
        #if j == -1: print error
        layout = layout_exp.match(lines[j]).group(1)

        if layout == "LyX-Code":
            result = ""
            while lines[i] == "\\protected_separator ":
                result = result + " "
                del lines[i]

            lines[i-1] = lines[i-1] + result + lines[i]
        else:
            lines[i-1] = lines[i-1]+ "\\SpecialChar ~"

        del lines[i]

def merge_formula_inset(lines):
    i=0
    while 1:
        i = find_token(lines, "\\begin_inset Formula", i)
        if i == -1: break
        if lines[i+1] in math_env:
            lines[i] = lines[i] + lines[i+1]
            del lines[i+1]
        i = i + 1

# Update from tabular format 4 to 5 if necessary
def update_tabular(lines):
    lyxtable_re = re.compile(r".*\\LyXTable$")
    i=0
    while 1:
        i = find_re(lines, lyxtable_re, i)
        if i == -1:
            break
        i = i + 1
        format = lines[i][8]
        if format != '4':
            continue

        lines[i]='multicol5'
        i = i + 1
        rows = int(string.split(lines[i])[0])
        columns = int(string.split(lines[i])[1])

        i = i + rows + 1
        for j in range(columns):
            col_info = string.split(lines[i])
            if len(col_info) == 3:
                lines[i] = lines[i] + '"" ""'
            else:
                lines[i] = string.join(col_info[:3]) + ' "%s" ""' % col_info[3]
            i = i + 1

        while lines[i]:
            lines[i] = lines[i] + ' "" ""'
            i = i + 1

def update_toc(lines):
    i = 0
    while 1:
        i = find_token(lines, '\\begin_inset LatexCommand \\tableofcontents', i)
        if i == -1:
            break
        lines[i] = lines[i] + '{}'
        i = i + 1

def remove_cursor(lines):
    i = find_token(lines, '\\cursor', 0)
    if i != -1:
        del lines[i]

def remove_vcid(lines):
    i = find_token(lines, '\\lyxvcid', 0)
    if i != -1:
        del lines[i]
    i = find_token(lines, '\\lyxrcsid', 0)
    if i != -1:
        del lines[i]

def first_layout(lines):
    while (lines[0] == ""):
        del lines[0]
    if lines[0][:7] != "\\layout":
        lines[:0] = ["\\layout Standard"]

def remove_space_in_units(lines):
    margins = ["\\topmargin","\\rightmargin",
               "\\leftmargin","\\bottommargin"]

    unit_rexp = re.compile(r'[^ ]* (.*) (.*)')

    begin_preamble = find_token(lines,"\\begin_preamble", 0)
    end_preamble = find_token(lines, "\\end_preamble", 0)
    for margin in margins:
        i = 0
        while 1:
            i = find_token(lines, margin, i)
            if i == -1:
                break

            if i > begin_preamble and i < end_preamble:
                i = i + 1
                continue

            result = unit_rexp.search(lines[i])
            if result:
                lines[i] = margin + " " + result.group(1) + result.group(2)
            i = i + 1

def convert(header,body):
    first_layout(body)
    remove_vcid(header)
    remove_cursor(body)
    update_toc(body)
    replace_protected_separator(body)
    merge_formula_inset(body)
    update_tabular(body)
    remove_space_in_units(header)

if __name__ == "__main__":
    pass

