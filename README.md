first make 'out' folder to compile first
gcc $(find . -path './archive' -prune -o -name "*.c") -o out/wish