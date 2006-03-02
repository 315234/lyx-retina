;--------------------------------

!macro GetFileExtProg ProgPath AppExe Extension Subentry

 ReadRegStr ${AppExe} HKCU \

"Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\${Extension}\OpenWithList" \
 "${Subentry}"
 ${if} ${AppExe} != ""
  ReadRegStr ${ProgPath} HKLM \
             "Software\Microsoft\Windows\CurrentVersion\App Paths\${AppExe}" "Path"
  ;remove the "\" at the end
  StrCpy $0 ${ProgPath} "" -1
  ${if} $0 == "\"
   StrCpy ${ProgPath} ${ProgPath} -1
  ${endif}
 ${endif}

!macroend

;--------------------------------

!macro FileCheck Result FileName FilePath
 Push $0
 Push $1
 StrCpy $0 ""
 StrCpy $1 ""

 FileOpen $0 "${Filepath}\${FileName}" r

 ${if} $0 = ""
  StrCpy $1 "False"
 ${Else}
  StrCpy $1 "True"
 ${endif}

 FileClose $0
 StrCpy ${Result} $1

 Pop $1
 Pop $0
!macroend


!macro TranslateLangCode LangNme LangISOCode

  ${if} ${LangISOCode} = 1030
   StrCpy ${LangNme} "Dansk"
  ${endif}

  ${if} ${LangISOCode} = 1031
   StrCpy ${LangNme} "Deutsch"
  ${endif}

  ${if} ${LangISOCode} = 1033
   StrCpy ${LangNme} "English"
  ${endif}

  ${if} ${LangISOCode} = 1034
   StrCpy ${LangNme} "Espa�ol"
  ${endif}

  ${if} ${LangISOCode} = 1036
   StrCpy ${LangNme} "Fran�ais"
  ${endif}

  ${if} ${LangISOCode} = 1040
   StrCpy ${LangNme} "Italiano"
  ${endif}

  ${if} ${LangISOCode} = 1043
   StrCpy ${LangNme} "Nederlands"
  ${endif}

  ${if} ${LangISOCode} = 1045
   StrCpy ${LangNme} "Polski"
  ${endif}

  ${if} ${LangISOCode} = 1053
   StrCpy ${LangNme} "Svenska"
  ${endif}

!macroend


!macro GetLangCode LangCde Name
 
  ${if} ${Name} == "Dansk"
   StrCpy ${LangCde} "da_DK"
  ${endif}

  ${if} ${Name} == "Deutsch"
   StrCpy ${LangCde} "de_DE"
  ${endif}

  ${if} ${Name} == "English"
   StrCpy ${LangCde} "en_EN"
  ${endif}

  ${if} ${Name} == "Espa�ol"
   StrCpy ${LangCde} "es_ES"
  ${endif}

  ${if} ${Name} == "Euskara"
   StrCpy ${LangCde} "eu_EU"
  ${endif}

  ${if} ${Name} == "Fran�ais"
   StrCpy ${LangCde} "fr_FR"
  ${endif}

  ${if} ${Name} == "Italiano"
   StrCpy ${LangCde} "it_IT"
  ${endif}

  ${if} ${Name} == "Magyar nyelv"
   StrCpy ${LangCde} "hu_HU"
  ${endif}  

  ${if} ${Name} == "Nederlands"
   StrCpy ${LangCde} "nl_NL"
  ${endif}

  ${if} ${Name} == "Norsk"
   StrCpy ${LangCde} "no_NO"
  ${endif}

  ${if} ${Name} == "Nynorsk"
   StrCpy ${LangCde} "no_NY"
  ${endif}

  ${if} ${Name} == "Polski"
   StrCpy ${LangCde} "pl_PL"
  ${endif}

  ${if} ${Name} == "Rom�na"
   StrCpy ${LangCde} "ro_RO"
  ${endif}

  ${if} ${Name} == "Russian"
   StrCpy ${LangCde} "ru_RU"
  ${endif}

  ${if} ${Name} == "Slovensk�"
   StrCpy ${LangCde} "sk_SK"
  ${endif}

  ${if} ${Name} == "Sloven�cina"
   StrCpy ${LangCde} "sl_SI"
  ${endif}

  ${if} ${Name} == "Suomi"
   StrCpy ${LangCde} "fi_FI"
  ${endif}

  ${if} ${Name} == "T�rk�e"
   StrCpy ${LangCde} "tr_TR"
  ${endif}

!macroend
