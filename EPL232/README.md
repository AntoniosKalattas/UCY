Periodic Table Formula Processor

This project process chemical formulas using a periodic table. Calculates the total number of protons and it will fully expand it. 


It can do the following:

Parentheses Verification: Checks if parentheses are balanced in chemical formulas.

Formula Expansion: Expands the given formulas into a full expanded version.

Proton Calculation: Calculates the total number of protons in a chemical formula.

Command-Line Options
The program has three modes, which are given by the command-line:

Parentheses check (-v):
Checks for balanced parentheses in the input file.
Example: ./parseFormula periodic_table.txt -v input_file.txt

Formula Expansion (-ext):
Expands formulas. H2O becomes HHO.
Output is written to the given output file.
Example: ./parseFormula periodic_table.txt -ext input_file.txt output_file.txt

Proton Calculation (-pn):
Calculates the total number of protons for each formula in the input file.
example: ./parseFormula periodic_table.txt -pn input_file output_file.txt

The compilation can be done through the make file.

To compile all : make all

To compile the debugging executables:   make stackDebug
                                        make calcProtonsDebug
To clean: make clean
