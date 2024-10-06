If you use the terminal, compile and run 'main.c' as follows: 

//To compile (linux/mac): gcc cbmp.c functions.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile test (linux/mac): gcc cbmp.c functions.c test_main.c -o test_main.out -std=c99
// To run test (linux/mac): ./test_main.out

//To compile (win): gcc cbmp.c functions.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

// To compile test (win): gcc cbmp.c functions.c test_main.c -o test_main.exe -std=c99
// To run test (win): test_main.exe

The folder test_erosion should, after running the program on an image, show every step of the erosion.
