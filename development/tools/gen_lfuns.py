#! /usr/bin/env python
# -*- coding: utf-8 -*-

# file gen_lfuns.py
# This file is part of LyX, the document processor.
# Licence details can be found in the file COPYING.

# author Ewan Davies

# Full author contact details are available in file CREDITS

# Usage:
# gen_lfuns.py <path/to/LyXAction.cpp> <where/to/save/LFUNs.lyx>

import sys
import os.path
from datetime import date

def error(message):
    sys.stderr.write(message + '\n')
    sys.exit(1)

def usage(prog_name):
    return "Usage: %s <path/to/LyXAction.cpp> <where/to/save/LFUNs.lyx>" % prog_name

DOXYGEN_START = "/*!"
DOXYGEN_END = "*/"

LYX_NEWLINE = "\n\\begin_inset Newline newline\n\\end_inset\n\n"
LYX_BACKSLASH = "\n\\backslash\n"

HTMLONLY_START = "\\htmlonly"
HTMLONLY_END = "\\endhtmlonly"
LFUN_NAME_ID = "\\var lyx::FuncCode lyx::"
LFUN_ACTION_ID = "\\li Action: "
LFUN_NOTION_ID = "\\li Notion: "
LFUN_SYNTAX_ID = "\\li Syntax: "
LFUN_PARAMS_ID = "\\li Params: "
LFUN_SAMPLE_ID = "\\li Sample: "
LFUN_ORIGIN_ID = "\\li Origin: "
LFUN_ENDVAR = "\\endvar"

ID_DICT = dict(name=LFUN_NAME_ID, action=LFUN_ACTION_ID, notion=LFUN_NOTION_ID, 
                syntax=LFUN_SYNTAX_ID, params=LFUN_PARAMS_ID, sample=LFUN_SAMPLE_ID, origin=LFUN_ORIGIN_ID)

LFUNS_HEADER = """# gen_lfuns.py generated this file. For more info see http://www.lyx.org/
\\lyxformat 345
\\begin_document
\\begin_header
\\textclass amsart
\\use_default_options false
\\begin_modules
theorems-ams
\\end_modules
\\language english
\\inputencoding auto
\\font_roman default
\\font_sans default
\\font_typewriter default
\\font_default_family default
\\font_sc false
\\font_osf false
\\font_sf_scale 100
\\font_tt_scale 100

\\graphics default
\\paperfontsize default
\\use_hyperref false
\\papersize default
\\use_geometry true
\\use_amsmath 1
\\use_esint 1
\\cite_engine basic
\\use_bibtopic false
\\paperorientation portrait
\\leftmargin 2.5cm
\\topmargin 2cm
\\rightmargin 3cm
\\bottommargin 1cm
\\secnumdepth 3
\\tocdepth 3
\\paragraph_separation indent
\\defskip medskip
\\quotes_language english
\\papercolumns 1
\\papersides 1
\\paperpagestyle default
\\tracking_changes false
\\output_changes false
\\author "" 
\\author "" 
\\end_header

\\begin_body

\\begin_layout Section*""" + "\nLFUNs documentation automatically generated " + str(date.today()) + """
\\end_layout

\\begin_layout Standard
\\begin_inset ERT
status collapsed

\\begin_layout Plain Layout


\\backslash
thispagestyle{empty}
\\end_layout

\\end_inset


\\begin_inset VSpace 1cm
\\end_inset


\\end_layout
"""
LFUNS_FOOTER = """\\end_body
\\end_document
"""

def parse_lfun(str):
    """Takes a comment block (str) and parses it for fields describing the LFUN. Returns a dict containing the fields."""
    
    lfun = dict(name="", action="", notion="", syntax="", params="", sample="", origin="")
    field = ""
    lines = str.splitlines()
    # strip leading whitespace and * from the lines of the comment to get 
    # rid of unimportant characters
    for i in range(0, len(lines)):
        lines[i] = lines[i].strip(" *")
    
    for i in range(0, len(lines) - 1):
        # work out what field is being read if none of these is found, the line will be added
        #     to the last field edited
        # since the field identifier is not included skip it out if it's found, otherwise skip
        #     nothing as an existing field is being added to
        # if a field id is found, then its the first line of the field so set the pre_space to ""
        #     so that the first line isn't prespaced
        if lines[i].startswith(LFUN_NAME_ID):
            field = "name"
            pre_space = ""
            skip = len(ID_DICT[field])
        elif lines[i].startswith(LFUN_ACTION_ID):
            field = "action"
            pre_space = ""
            skip = len(ID_DICT[field])
        elif lines[i].startswith(LFUN_NOTION_ID):
            field = "notion"
            pre_space = ""
            skip = len(ID_DICT[field])
        elif lines[i].startswith(LFUN_SYNTAX_ID):
            field = "syntax"
            pre_space = ""
            skip = len(ID_DICT[field])
        elif lines[i].startswith(LFUN_PARAMS_ID):
            field = "params"
            pre_space = ""
            skip = len(ID_DICT[field])
        elif lines[i].startswith(LFUN_SAMPLE_ID):
            field = "sample"
            pre_space = ""
            skip = len(ID_DICT[field])
        elif lines[i].startswith(LFUN_ORIGIN_ID):
            field = "origin"
            pre_space = ""
            skip = len(ID_DICT[field])
        elif lines[i].startswith(LFUN_ENDVAR):
            break
        else:
            skip = 0
            # if a manual line break was found last line, don't prespace this line
            if i > 1 and lines[i-1].endswith("\\n"):
                pre_space = ""
            else:
                pre_space = " "
        
        # add the line to the field, processing it for \ characters and \n
        # which, if occurring at the end of a line, must become a LYX_NEWLINE
        line = lines[i][skip:]
        
        # deal with \htmlonly
        # TODO: convert chars found in htmlonly to unicode
        start = line.find(HTMLONLY_START)
        if start > 0:
            # if removing the htmlonly element leaves a double space, go back one to remove it
            if line[start-1] == " ":
                start = start - 1
            end = line.find(HTMLONLY_END)
            if end > start:
                end = line.find(HTMLONLY_END) + len(HTMLONLY_END)
                line = line[:start] + line[end:]
            #else:
            # TODO: if HTMLONLY_END is not found, look on the next line
            # TODO: in the current LyXAction.cpp there are no htmlonly fields which go over a line break
        
        # deal with \ but leave \n if at the end of the line
        slash_idx = line.find("\\")
        while slash_idx >= 0:
            if slash_idx < len(line)-2 \
            or slash_idx == len(line)-1:
                # true when the \ is not the last or second last char
                #      or when the slash is the last char of the line
                
                # slash must be interpreted literaly so swap it for a LYX_BACKSLASH
                line = line[:slash_idx] + LYX_BACKSLASH + line[slash_idx+1:]
                # skip the index ahead beyond the added text
                slash_idx = slash_idx + len(LYX_BACKSLASH)
            elif line[slash_idx+1] != "n": # only evaluated if the line ends "\x" where 'x' != 'n'
                line = line[:slash_idx] + LYX_BACKSLASH + line[slash_idx+1:]
                # skip the index ahead beyond the added text
                slash_idx = slash_idx + len(LYX_BACKSLASH) 
            # look for the next \
            slash_idx = line.find("\\", slash_idx+1)
            
        # \n at the end of lines will not be processed by the above while loop
        # so sort those out now
        # sometime lines end " \n" so chop the space if its there
        if line.endswith(" \\n"):
            line = line[:len(line)-3] + LYX_NEWLINE
        elif line.endswith("\\n"):
            line = line[:len(line)-2] + LYX_NEWLINE
        
        # any references to other LFUNs need the # removing
        # TODO: actually insert a cross-reference here
        line = line.replace("#LFUN", "LFUN")
        
        # the first line might not have a field in it in which
        # case the variable field won't have a value, so check
        # to avoid an error
        if field != "":
            lfun[field] = lfun[field] + pre_space + line
        
        # TODO: sort out chopping lines of more that 80 chars in length
        
    return lfun

def write_fields(file, lfun):
    """Writes the LFUN contained in the dict lfun to the file. Does not write a the file header or footer"""
    # add lfun to LFUNs.lyx
    file.write("\\begin_layout Subsection*\n")
    file.write(lfun["name"] + "\n")
    file.write("\\end_layout\n")
    #file.write("\n")
    if lfun["action"] != "":
        file.write("\\begin_layout Description\n")
        file.write("Action " + lfun["action"] + "\n")
        #file.write("\n")
        file.write("\\end_layout\n")
        #file.write("\n")
    if lfun["notion"] != "":
        file.write("\\begin_layout Description\n")
        file.write("Notion " + lfun["notion"] + "\n")
        file.write("\\end_layout\n")
        #file.write("\n")
    if lfun["syntax"] != "":
        file.write("\\begin_layout Description\n")
        file.write("Syntax " + lfun["syntax"] + "\n")
        file.write("\\end_layout\n")
        #file.write("\n")
    if lfun["params"] != "":
        file.write("\\begin_layout Description\n")
        file.write("Params " + lfun["params"] + "\n")
        file.write("\\end_layout\n")
        #file.write("\n")
    if lfun["sample"] != "":
        file.write("\\begin_layout Description\n")
        file.write("Sample " + lfun["sample"] + "\n")
        file.write("\\end_layout\n")
        #file.write("\n")
    if lfun["origin"] != "":
        file.write("\\begin_layout Description\n")
        file.write("Origin " + lfun["origin"] + "\n")
        file.write("\\end_layout\n")
        #file.write("\n")
    file.write("\n")        
    
def main(argv):
    # parse command line arguments   
    script_path, script_name = os.path.split(argv[0])
    if len(argv) != 3:
        error(usage(script_name))
    # input file
    lyxaction_path = argv[1]
    if not os.path.exists(lyxaction_path):
        error(script_name + ": %s is not a valid path" % lyxaction_path, usage(argv[0]))

    # output file
    lfuns_path = argv[2]
    if os.path.isdir(lfuns_path):
        lfuns_path = lfuns_path + "LFUNs.lyx"
    elif os.path.exists(lfuns_path):
        error(script_name + ": %s already exists, delete it and rerun the script" % lfuns_path)

    print(script_name + ": Start processing " + argv[1])
    # Read the input file and write the output file
    lyxaction_file = open(lyxaction_path, 'rb')
    lfuns_file = open(lfuns_path, 'wb')
	
    lyxaction_text = lyxaction_file.read()
	
    # TODO: format a proper header for LFUNs.lyx
    lfuns_file.write(LFUNS_HEADER)
	
	# seek to the important bit of LyXAction.cpp
    try:
        start = lyxaction_text.index("ev_item const items[] = {")
    except ValueError:
        lyxaction_file.close()
        lfuns_file.close()
        error(script_name + ": LFUNs not found in " + lyxaction_file)

    done = count = 0

    while done == 0:
        # look for a doxygen comment
        start = lyxaction_text.find(DOXYGEN_START, start)
        end = lyxaction_text.find(DOXYGEN_END, start) + len(DOXYGEN_END)
        # parse the lfun if it is found
        if start > 0:
            count = count + 1
            lfun = parse_lfun(lyxaction_text[start:end])
            # write the lfun to the file
            write_fields(lfuns_file, lfun)
            # done adding current lfun to LFUNs.lyx so get the next one
            start = end
        else:
            # if no more lfuns are found, EOF reached
            done = 1
            
    print(script_name + ": Created documentation for " + str(count) + " LFUNs")
    
    # write the last part of LFUNs.lyx
    lfuns_file.write(LFUNS_FOOTER)
    
    lyxaction_file.close()
    lfuns_file.close()
    
    print(script_name + ": Finished")
    
if __name__ == "__main__":
    main(sys.argv)
