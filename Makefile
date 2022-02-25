

# -fshort-enums is critial, otherwise enums will be integer sized

all:
	gcc -Wno-unused-variable -fshort-enums -DWORD_ALIGN -O3 -Wall ./main.c -g -o ./build/array
	./build/array



packed:
	gcc -Wno-unused-variable -O3 -Wall ./main.c -g -o ./build/array
	./build/array



clang:
	clang -O0 -Wall ./main.c -g -o ./build/array
	./build/array


gdb:
	gcc -O0 -Wall ./main.c -g -o ./build/array
	gdb ./build/array

