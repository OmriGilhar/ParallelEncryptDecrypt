
# Final project in Parallel Computing (MPI, OpenMP)

## About
---   
This program porpuse is to break an enctyption with 4-bit key by running a brute force method.
In a nutshell,  there are 2 process, the Master & the Slave, the master process parse all the arguments, read the files, and send it to the slave process, both of them get an range of binary numbers to try to crack the cipher text,

Master - 0000 -> 0111   
Slave - 1000 -> 1111   

looping over every possible binary number in thier range useing OpenMP threads (every process have 4 threads).
trying to match every hash decrypted data to an hash-table from the words (input or default).

at the end the thread with the most matches words "win" and gets to print its summay (decrypted text).

This program was created Omri Gilhar.

## Usage
---   

**Important Note**: This program is using [GLib](https://en.wikipedia.org/wiki/GLib) for the hash table (speeding up the comparison process), therefore, You will need to run the ```make link``` command to link the glibconfig.h to your ```/usr/include/glib-2.0/``` folder.

To run the program please use this command:
```
make link
make build
```
then run this (without words file):
```
make run length=4 file="./exampleEncrypted.e"
```
or run this (with words file):
```
make run length=4 file="./exampleEncrypted.e" words="./words"
```
when finished, clean your env with:
```
make clean
```


Makefile targers:
| target     | description                                                                         | param1 | param2      | param3                |
|------------|-------------------------------------------------------------------------------------|--------|-------------|-----------------------|
| test       | Uses links, build, run_test, clean targets (for debuging or fast usage).            |        |             |                       |
| links      | Running an soft link command to link the GLib config to your usr environment.       |        |             |                       |
| build      | Building the program C files.                                                       |        |             |                       |
| clean      | Cleaning all Object files.                                                          |        |             |                       |
| run_test   | Running a pre-made command (4 bit key, cipherText.e as the cipher text to encrypt). |        |             |                       |
| change_mod | Changing all files' permissions in directory to 755                                 |        |             |                       |
| run        | The main target, runs the encryption program.                                       | length | cipher file | [OPTIONAL] words file |




## Flow Chard (in a nutshell)
---  
Blue = Masters' process   
Red = Slaves' process   
White = Both   
![WorkflowDiagram](https://i.ibb.co/Wxk0Dgy/asd.png) 
