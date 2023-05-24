# 0x16. C- Simple Shell
The "Simple Shell" project is about writing a simple Unix command interpreter using the C language.
## Description
This shell is based on the Thompson shell (V6 shell); it reads commands and executes them.
### *Usage*

To run "This shell" compile using:
```
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```
Or: 
```
gcc *.c -o hsh
```
### *Modes*

After compilation, the shell could work in interactive mode:
```
$ ./hsh
($) /bin/ls
hsh main.c shell.c
($) exit
$
```
Also in non-interactive mode :
``` 
$ echo "/bin/ls" | ./hsh
hsh main.c shell.c test_ls_2
$
```
###

