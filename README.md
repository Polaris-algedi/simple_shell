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
### *Command lines*
You can type simple command lines or ones with arguments : 
```
$ pwd
 /home/user
$ echo this is a simple shell
this is a simple shell
$ ls -l
total 140
-rwxr-xr-x 1 root root 26904 May 23 14:47 a.out
-rw-r--r-- 1 root root   139 May 24 09:58 AUTHORS
-rw-r--r-- 1 root root 16968 May 20 16:18 FILE.c
### *Exit*
to exit the shell, type `exit`  
```
$ exit
thank you for using our shell ^_^
