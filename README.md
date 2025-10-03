to compile
gcc $(find . -path './archive' -prune -or -name "*.c" -print) -o out/wish