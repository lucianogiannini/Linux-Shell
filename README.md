# UNIX-Shell
This program is a recreation of a UNIX Shell. It has a lot of the processes that the original UNIX Shell would have. 

There were three main parts to this project (A,B,C):  

## Problem A
Write a C++ program that will act as a shell command line interpreter for the Linux kernel. Your shell
program should use the same style as the bash shell for running programs. In particular, when the user
types a line such as:  

$ **command [parameter1] .... [parameterN]**  

The shell should parse the command line to build argv. It should search the directory system (in the order
specified by the PATH environment variable) for a file with the same name as the first identifier (which may
be a relative filename or a full pathname). If the file is found, then it should be executed with the optional
parameter list, as is done with bash. If the file is not found, then an error should be printed.  

Use the execvp() system call to execute the file that contains the command. You will also need to
become familiar with the Linux fork() and wait() functions.  

When the command has completed executing, your shell should prompt the user for another command.  

Here’s an example of the shell running a simple hello world program followed by the “ls” command:  

**Bash O' Rama> ./main**  
**Hello World!**  
**Bash O' Rama> ls**  
**main.cpp main.o main makefile**  
**Bash O'Rama>** 

## Problem B  
Add functionality to the shell from Problem A so that a user can use the "&" operator as a command
terminator. A command terminated with "&" should be executed concurrently with the shell (rather than the
shell's waiting for the child to terminate before it prompts the user for another command).  


## Problem C  
Modify your shell program so that the user can redirect the stdin or stdout file descriptors by using the "<"
and ">" characters as filename pre-fixes.  

For example: 

**Bash O'Rama> ./main > out.txt**  
**Bash O'Rama> cat out.txt**  
**Hello World!**  
**Bash O'Rama>**  

This will place the output of the “Hello World” program into the file “out.txt” rather than sending it to the
screen (which is stdout).    
Also, allow your user to use the pipe operator, "|", to execute two processes concurrently, with stdout from
the first process being redirected as the stdin for the second process.  

For example: 

**Bash O'Rama> cat out.txt**  
**Hello World!**  
**Bash O'Rama> cat out.txt | wc –l**  
**1**  
**Bash O'Rama>**  

The cat command sends the contents of the “out.txt” file to stdout. The “wc –l” command counts the
number of characters, words, and lines typed into stdin.  
The two commands can be strung together using the pipe operator “|” so that the output of cat (sent to stdout) is connected to the input of wc
(received from stdin).  

 
