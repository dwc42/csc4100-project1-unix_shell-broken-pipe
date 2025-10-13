# Command Shell 
## Built-in Commands
 - cd: changes current directory
 - path: changes command search path
 - exit: exits the program

runs exes/commands as well.  
& for runing commands parallel
\> to store the output in a file ie redirection.
## Running the Project
to run  
```bash
out/wish
```
or to run a bash like file
```bash
out/wish <path to file>
```
To compile and find memory leaks.  
```bash
gcc -fsanitize=address -g $(find . -path './archive' -prune -o -name "*.c" -type f -print) -o out/wish
```
To just compile  
```bash
gcc -g $(find . -path './archive' -prune -o -name "*.c" -type f -print) -o out/wish
```
