<<<<<<< HEAD
first make 'out' folder to compile first
gcc $(find . -path './archive' -prune -o -name "*.c") -o out/wish
=======
to compile
gcc $(find . -path './archive' -prune -or -name "*.c" -print) -o out/wish
>>>>>>> 1c21c2fb7e13cfa8fdf177b6469e05f1e52fbfa3
