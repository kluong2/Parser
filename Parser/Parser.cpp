#include "Parser.h"



int main()
{
	//Here is a string of all the textfiles that came with the project. I also added on my own "testing" textfile to test other conditions.
	string filename[] = { "ab.txt", "ab2.txt", "bad_assignment.txt", "bad_decl.txt", "bad_exp.txt", "bad_exp2.txt","bad_exp3.txt","bad_exp4.txt","bad_exp5.txt","bad_exp6.txt",
						"bad_exp7.txt", "bad_exp8.txt", "bad_scoping1.txt", "bad_scoping2.txt", "euclid.txt", "hiding.txt", "if.txt", "print.txt", "testing.txt" };

	//string file = filename[0];//Change this to whatever you want. If you are using multiple instances of classes, you might want to use this so you don't have to go through and change every argument.

	Parser parse(filename[0]);



	parse.analyze();//Runs a lexical analysis before parsing. 
	


	parse.program();//This here does the actual parsing. The functions were designed by Professor Saeedloei. 

	return 0;

}

