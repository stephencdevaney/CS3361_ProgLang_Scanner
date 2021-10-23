# CS3361 DFA-Scanner
Link to repository: https://github.com/stephencdevaney/CS3361_ProgLang_Scanner.git



## How to compile and run the file from the command line:
### (Step 1) check to see if you have gcc:
gcc -v
if don't down load a gcc complier

### (Step 2) check to make sure you are in the correct file location:
Windows:
cd // changes directory
dir // list files in the current directory


### (Step 3) Compliling:
gcc -o scanner scanner.c queue.c -lm

### (Step 4) Executing:
#### Executing with included test cases:
scanner tokenFile1.txt
scanner tokenFile2.txt
scanner tokenFile3.txt
scanner tokenFile4.txt
scanner tokenFile5.txt


#### Executing with custom testcases:
scanner [tokens file name]
