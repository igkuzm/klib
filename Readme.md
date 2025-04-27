# Library of functions and macros for plain C

1. alloc.h - macros for save malloc and realloc
1. array.h - dynamic array
1. base64.h - base64 encode/decode
1. bitmap.h - image bitmap editing
5. enum.h - converting C enums to strings
5. errcodes.h - describe errors in header and get it's value 
   in code with function ``const char *errcode(int code)``
6. fm.h - filemanager functions (copy file, copy directory recursive, etc.)
1. fstrfind - find strings in file stream
1. fstrrep - find and replace string in file stream
8. getbundle.h - for Windows to EXEC, for MacOS to BUNDLE/Resources, for Linux - ../share/$progname
2. ini.h - parse ini config file
10. list.h - list support
11. log.h - logs in Android/iOS and other platforms
1. memread.h - FILE-like functions to read from memory
1. mimetype - get extension from mimetype and mimetype from extension
12. openfile.h - open file for Windows/MacOS/Linux with default application
13. printbit.h - print bit functions
14. reachability.h - check reachability of ip address
1. rtf.h - generate RTF string
1. str.h - dynamic string
1. strfilter.h - filter string
18. strptime.cc - implimation of strptime for windows
1. strrep - find and replace string
17. strrf.h - write file contents to allocated string
19. strtok_foreach.h - split string with delim
19. strsubstr.h - substring
20. translit.h - convert KYRILIC utf8 string to transliteral LATIN ANSI string
21. utf.h - utf8 to utf32 converter
22. uuid.h - generate uuid4
