build triangle :
    cd triangle
    cc -DTRILIBRARY -O -c triangle.c

build test :
    cd test
    cc -O -o ./test1 ./test1.c ../triangle/triangle.o -I../triangle -lm