/*

filelist.nsh

Lists of files to include in the installer

*/

!define FILE '${COMMAND} "${DIRECTORY}'

!macro FileListLyXBin COMMAND DIRECTORY

  ${FILE}lyx.exe"
  ${FILE}tex2lyx.exe"

!macroend

!macro FileListLyXLauncher COMMAND DIRECTORY

  ${FILE}LyXLauncher.exe"

!macroend

!macro FileListQtBin COMMAND DIRECTORY

  ${FILE}QtCore4.dll"
  ${FILE}QtGui4.dll"  

!macroend

!macro FileListMSVC COMMAND DIRECTORY

  # Manifest not needed for 2010
  ${FILE}msvcr100.dll"
  ${FILE}msvcp100.dll"
  
!macroend

!macro FileListDll COMMAND DIRECTORY

  ${FILE}iconv.dll"
  ${FILE}intl.dll"
  ${FILE}libhunspell.dll"
  ${FILE}zlibwapi.dll"

!macroend

!macro FileListNetpbmBin COMMAND DIRECTORY

  ${FILE}libnetpbm10.dll"
  ${FILE}pnmcrop.exe"
  
!macroend

!macro FileListDTLBin COMMAND DIRECTORY

  ${FILE}dt2dv.exe"
  ${FILE}dv2dt.exe"
  
!macroend

!macro FileListDvipostBin COMMAND DIRECTORY

  ${FILE}pplatex.exe"
  
!macroend

!macro FileListPDFToolsBin COMMAND DIRECTORY

  ${FILE}pdfopen.exe"
  ${FILE}pdfclose.exe"
  
!macroend

!macro FileListPDFViewBin COMMAND DIRECTORY

  ${FILE}pdfview.exe"
  ${FILE}System.dll"
  ${FILE}Console.dll"

!macroend

!macro FileListFonts COMMAND DIRECTORY

  ${FILE}cmex10.ttf"
  ${FILE}cmmi10.ttf"
  ${FILE}cmr10.ttf"
  ${FILE}cmsy10.ttf"
  ${FILE}eufm10.ttf"
  ${FILE}msam10.ttf"
  ${FILE}msbm10.ttf"
  ${FILE}wasy10.ttf"
  
!macroend

!macro FileListPythonBin COMMAND DIRECTORY

  ${FILE}python.exe"
  ${FILE}python26.dll"
  ${FILE}unicodedata.pyd"

!macroend

!macro FileListPythonLib COMMAND DIRECTORY

  ${FILE}__future__.py"
  ${FILE}_abcoll.py"  
  ${FILE}UserDict.py"
  ${FILE}abc.py"  
  ${FILE}codecs.py"
  ${FILE}compileall.py"  
  ${FILE}copy_reg.py"
  ${FILE}csv.py"
  ${FILE}functools.py"  
  ${FILE}fnmatch.py"
  ${FILE}genericpath.py"  
  ${FILE}getopt.py"
  ${FILE}gettext.py"
  ${FILE}glob.py"
  ${FILE}gzip.py"
  ${FILE}linecache.py"
  ${FILE}locale.py"
  ${FILE}ntpath.py"
  ${FILE}os.py"
  ${FILE}optparse.py"
  ${FILE}pipes.py"
  ${FILE}posixpath.py"
  ${FILE}py_compile.py"  
  ${FILE}random.py"
  ${FILE}re.py"
  ${FILE}shutil.py"
  ${FILE}site.py"
  ${FILE}sre.py"
  ${FILE}sre_compile.py"
  ${FILE}sre_constants.py"
  ${FILE}sre_parse.py"
  ${FILE}stat.py"
  ${FILE}string.py"
  ${FILE}struct.py"  
  ${FILE}tempfile.py"
  ${FILE}tempfile.py"
  ${FILE}textwrap.py"
  ${FILE}traceback.py"  
  ${FILE}types.py"
  ${FILE}warnings.py"

!macroend

!macro FileListPythonLibEncodings COMMAND DIRECTORY

  ${FILE}aliases.py"
  ${FILE}ascii.py"
  ${FILE}base64_codec.py"
  ${FILE}big5.py"
  ${FILE}big5hkscs.py"
  ${FILE}bz2_codec.py"
  ${FILE}charmap.py"
  ${FILE}cp037.py"
  ${FILE}cp1006.py"
  ${FILE}cp1026.py"
  ${FILE}cp1140.py"
  ${FILE}cp1250.py"
  ${FILE}cp1251.py"
  ${FILE}cp1252.py"
  ${FILE}cp1253.py"
  ${FILE}cp1254.py"
  ${FILE}cp1255.py"
  ${FILE}cp1256.py"
  ${FILE}cp1257.py"
  ${FILE}cp1258.py"
  ${FILE}cp424.py"
  ${FILE}cp437.py"
  ${FILE}cp500.py"
  ${FILE}cp737.py"
  ${FILE}cp775.py"
  ${FILE}cp850.py"
  ${FILE}cp852.py"
  ${FILE}cp855.py"
  ${FILE}cp856.py"
  ${FILE}cp857.py"
  ${FILE}cp860.py"
  ${FILE}cp861.py"
  ${FILE}cp862.py"
  ${FILE}cp863.py"
  ${FILE}cp864.py"
  ${FILE}cp865.py"
  ${FILE}cp866.py"
  ${FILE}cp869.py"
  ${FILE}cp874.py"
  ${FILE}cp875.py"
  ${FILE}cp932.py"
  ${FILE}cp949.py"
  ${FILE}cp950.py"
  ${FILE}euc_jisx0213.py"
  ${FILE}euc_jis_2004.py"
  ${FILE}euc_jp.py"
  ${FILE}euc_kr.py"
  ${FILE}gb18030.py"
  ${FILE}gb2312.py"
  ${FILE}gbk.py"
  ${FILE}hex_codec.py"
  ${FILE}hp_roman8.py"
  ${FILE}hz.py"
  ${FILE}idna.py"
  ${FILE}iso2022_jp.py"
  ${FILE}iso2022_jp_1.py"
  ${FILE}iso2022_jp_2.py"
  ${FILE}iso2022_jp_2004.py"
  ${FILE}iso2022_jp_3.py"
  ${FILE}iso2022_jp_ext.py"
  ${FILE}iso2022_kr.py"
  ${FILE}iso8859_1.py"
  ${FILE}iso8859_10.py"
  ${FILE}iso8859_11.py"
  ${FILE}iso8859_13.py"
  ${FILE}iso8859_14.py"
  ${FILE}iso8859_15.py"
  ${FILE}iso8859_16.py"
  ${FILE}iso8859_2.py"
  ${FILE}iso8859_3.py"
  ${FILE}iso8859_4.py"
  ${FILE}iso8859_5.py"
  ${FILE}iso8859_6.py"
  ${FILE}iso8859_7.py"
  ${FILE}iso8859_8.py"
  ${FILE}iso8859_9.py"
  ${FILE}johab.py"
  ${FILE}koi8_r.py"
  ${FILE}koi8_u.py"
  ${FILE}latin_1.py"
  ${FILE}mac_arabic.py"
  ${FILE}mac_centeuro.py"
  ${FILE}mac_croatian.py"
  ${FILE}mac_cyrillic.py"
  ${FILE}mac_farsi.py"
  ${FILE}mac_greek.py"
  ${FILE}mac_iceland.py"
  ${FILE}mac_latin2.py"
  ${FILE}mac_roman.py"
  ${FILE}mac_romanian.py"
  ${FILE}mac_turkish.py"
  ${FILE}mbcs.py"
  ${FILE}palmos.py"
  ${FILE}ptcp154.py"
  ${FILE}punycode.py"
  ${FILE}quopri_codec.py"
  ${FILE}raw_unicode_escape.py"
  ${FILE}rot_13.py"
  ${FILE}shift_jis.py"
  ${FILE}shift_jisx0213.py"
  ${FILE}shift_jis_2004.py"
  ${FILE}string_escape.py"
  ${FILE}tis_620.py"
  ${FILE}undefined.py"
  ${FILE}unicode_escape.py"
  ${FILE}unicode_internal.py"
  ${FILE}utf_16.py"
  ${FILE}utf_16_be.py"
  ${FILE}utf_16_le.py"
  ${FILE}utf_7.py"
  ${FILE}utf_8.py"
  ${FILE}utf_8_sig.py"
  ${FILE}uu_codec.py"
  ${FILE}zlib_codec.py"
  ${FILE}__init__.py"
  
!macroend

!macro FileListPythonLibLogging COMMAND DIRECTORY

  ${FILE}__init__.py"
  ${FILE}config.py"
  ${FILE}handlers.py"
  
!macroend

!macro FileListMetaFile2EPS COMMAND DIRECTORY

  ${FILE}metafile2eps.exe"
  ${FILE}metafile2eps.dat"  

!macroend

!macro FileListImageMagick COMMAND DIRECTORY

  ${FILE}coder.xml"
  ${FILE}colors.xml"
  ${FILE}configure.xml"
  ${FILE}convert.exe"
  ${FILE}delegates.xml"
  ${FILE}english.xml"
  ${FILE}ImageMagick.rdf"
  ${FILE}locale.xml"
  ${FILE}log.xml"
  ${FILE}magic.xml"
  ${FILE}policy.xml"
  ${FILE}sRGB.icm"
  ${FILE}thresholds.xml"
  ${FILE}type-ghostscript.xml"
  ${FILE}type.xml"
  ${FILE}X11.dll"
  ${FILE}Xext.dll"

!macroend

!macro FileListGhostscript COMMAND DIRECTORY

  ${FILE}gswin32.exe"
  ${FILE}gswin32c.exe"
  ${FILE}gsdll32.dll"

!macroend