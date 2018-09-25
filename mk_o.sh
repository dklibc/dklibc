tcc -fnostdlib -c -I./include $1.c && ar rcs dklibc.a $1.o
 