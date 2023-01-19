man stncpy:
No manual entry for stncpy
By google:
Copies the first num characters of source to destination.

man printf:
Print ARGUMENT(s) according to FORMAT, or execute

man gcc:
When you invoke GCC, it normally does preprocessing, compilation, assembly and linking. 

Answer for -g flag in README from Section 3:
The '-g' option passed to GCC adds debugging information to the executable file.

Error for assembly:
0x555555555183 <main+26>        mov    %rdx,(%rax)

Error for c code:
7	strncpy(buffer, "Hello, world!", 14);
Line 7 is where the program gets its error;

Error description:
Program received signal SIGSEGV, Segmentation fault.
0x0000555555555183 in main () at test.c:7

Answer for gdb line and likely error in README from Section 4:
The reason why the error happens might be that the program was trying to put a string into a place where it is read only (no permission for program).
