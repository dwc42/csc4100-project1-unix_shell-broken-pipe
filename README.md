to compile
gcc $(find . -path './archive' -prune -o -name "*.c") -o out/wish