To compile and find memory leaks.
gcc -fsanitize=address -g $(find . -path './archive' -prune -o -name "*.c" -type f -print) -o out/wish