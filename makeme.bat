cd assets\stages
call makeme.bat
cd..
cd..

set ROOT=c:\ultra
set gccdir=%ROOT%\gcc
set PATH=%ROOT%\gcc\mipse\bin;%ROOT%\usr\sbin
set gccsw=-mips3 -mgp32 -mfp32 -funsigned-char -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__
set n64align=on
set GCC_CELF=ON
make

::Clean up out files::
del *.o
del *.out

:: Del old ROM and place new one ::
del G:\first.n64
move first.n64 G:\