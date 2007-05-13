#!/usr/bin/env python
# -*- coding: utf-8 -*-

# file lyx_pot.py
# This file is part of LyX, the document processor.
# Licence details can be found in the file COPYING.
#
# \author Bo Peng
#
# Full author contact details are available in file CREDITS

# Usage: use
#     lyx_pot.py -h
# to get usage message

# This script will extract translatable strings from input files and write
# to output in gettext .pot format.
#
import sys, os, re, getopt

def relativePath(path, base):
    '''return relative path from top source dir'''
    # full pathname of path
    path1 = os.path.normpath(os.path.realpath(path)).split(os.sep)
    path2 = os.path.normpath(os.path.realpath(base)).split(os.sep)
    if path1[:len(path2)] != path2:
        print "Path %s is not under top source directory" % path
    path3 = os.path.join(*path1[len(path2):]);
    # replace all \ by / such that we get the same comments on Windows and *nix
    path3 = path3.replace('\\', '/')
    return path3


def ui_l10n(input_files, output, base):
    '''Generate pot file from lib/ui/*'''
    output = open(output, 'w')
    Submenu = re.compile(r'^[^#]*Submenu\s+"([^"]*)"')
    Popupmenu = re.compile(r'^[^#]*PopupMenu\s+"[^"]+"\s+"([^"]*)"')
    Toolbar = re.compile(r'^[^#]*Toolbar\s+"[^"]+"\s+"([^"]*)"')
    Item = re.compile(r'[^#]*Item\s+"([^"]*)"')
    for src in input_files:
        input = open(src)
        for lineno, line in enumerate(input.readlines()):
            if Submenu.match(line):
                (string,) = Submenu.match(line).groups()
                string = string.replace('_', ' ')
            elif Popupmenu.match(line):
                (string,) = Popupmenu.match(line).groups()
            elif Toolbar.match(line):
                (string,) = Toolbar.match(line).groups()
            elif Item.match(line):
                (string,) = Item.match(line).groups()
            else:
                continue
            string = string.replace('"', '')
            if string != "":
                print >> output, '#: %s:%d\nmsgid "%s"\nmsgstr ""\n' % \
                    (relativePath(src, base), lineno+1, string)
        input.close()
    output.close()


def layouts_l10n(input_files, output, base):
    '''Generate pot file from lib/layouts/*.layout and *.inc'''
    output = open(output, 'w')
    Style = re.compile(r'^Style\s+(.*)')
    # include ???LabelString???, but exclude comment lines
    LabelString = re.compile(r'^[^#]*LabelString\S*\s+(.*)')
    GuiName = re.compile(r'\s*GuiName\s+(.*)')
    ListName = re.compile(r'\s*ListName\s+(.*)')
    for src in input_files:
        input = open(src)
        for lineno, line in enumerate(input.readlines()):
            if Style.match(line):
                (string,) = Style.match(line).groups()
                string = string.replace('_', ' ')
            elif LabelString.match(line):
                (string,) = LabelString.match(line).groups()
            elif GuiName.match(line):
                (string,) = GuiName.match(line).groups()
            elif ListName.match(line):
                (string,) = ListName.match(line).groups()
            else:
                continue
            string = string.replace('\\', '\\\\').replace('"', '')
            if string != "":
                print >> output, '#: %s:%d\nmsgid "%s"\nmsgstr ""\n' % \
                    (relativePath(src, base), lineno+1, string)
        input.close()
    output.close()


def qt4_l10n(input_files, output, base):
    '''Generate pot file from src/frontends/qt4/ui/*.ui'''
    output = open(output, 'w')
    pat = re.compile(r'\s*<string>(.*)</string>')
    prop = re.compile(r'\s*<property.*name.*=.*shortcut')
    for src in input_files:
        input = open(src)
        skipNextLine = False
        for lineno, line in enumerate(input.readlines()):
            # skip the line after <property name=shortcut>
            if skipNextLine:
                skipNextLine = False
                continue
            if prop.match(line):
                skipNextLine = True
                continue
            # get lines that match <string>...</string>
            if pat.match(line):
                (string,) = pat.match(line).groups()
                string = string.replace('&amp;', '&').replace('&lt;', '<').replace('&gt;', '>').replace('"', r'\"')
                print >> output, '#: %s:%d\nmsgid "%s"\nmsgstr ""\n' % \
                    (relativePath(src, base), lineno+1, string) 
        input.close()
    output.close()


def languages_l10n(input_files, output, base):
    '''Generate pot file from lib/language'''
    output = open(output, 'w')
    # assuming only one language file
    input = open(input_files[0])
    for lineno, line in enumerate(input.readlines()):
        if line[0] == '#':
            continue
        items = line.split()
        # empty lines?
        if len(items) < 3:
            continue
        # From:
        #   afrikaans   afrikaans	"Afrikaans"	false  iso8859-15 af_ZA	 ""
        # To:
        #   #: lib/languages:2
        #   msgid "Afrikaans"
        #   msgstr ""
        # I do not care extra "s like "af_ZA"
        print >> output, '#: %s:%d\nmsgid "%s"\nmsgstr ""\n' % (relativePath(input_files[0], base), lineno+1, items[2].strip('"'))
    input.close()
    output.close()


Usage = '''
lyx_pot.py [-b|--base top_src_dir] [-o|--output output_file] [-h|--help] -t|--type input_type input_files

where 
    --base:
        path to the top source directory. default to '.'
    --output:
        output pot file, default to './lyx.pot'
    --input_type can be
        ui: lib/ui/*
        layouts: lib/layouts/*
        qt4: qt4 ui files
        languages: file lib/languages
'''

if __name__ == '__main__':
    input_type = None
    output = 'lyx.pot'
    base = '.'
    #
    optlist, args = getopt.getopt(sys.argv[1:], 'ht:o:b:',
        ['help', 'type=', 'output=', 'base='])
    for (opt, value) in optlist:
        if opt in ['-h', '--help']:
            print Usage
            sys.exit(0)
        elif opt in ['-o', '--output']:
            output = value
        elif opt in ['-b', '--base']:
            base = value
        elif opt in ['-t', '--type']:
            input_type = value
    if input_type not in ['ui', 'layouts', 'qt4', 'languages'] or output is None:
        print 'Wrong input type or output filename.'
        sys.exit(1)
    if input_type == 'ui':
        ui_l10n(args, output, base)
    elif input_type == 'layouts':
        layouts_l10n(args, output, base)
    elif input_type == 'qt4':
        qt4_l10n(args, output, base)
    else:
        languages_l10n(args, output, base)

