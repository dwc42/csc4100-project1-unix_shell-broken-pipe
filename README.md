to compile
gcc $(find . -path './archive' -prune -o -name "*.c" -type f -print) -o out/wish