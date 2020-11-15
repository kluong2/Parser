#include "Parser.h"



int main()
{
	//Here is a string of all the textfiles that came with the project. I also added on my own "testing" textfile to test other conditions.
	string filename[] = { "ab.txt", "ab2.txt", "bad_assignment.txt", "bad_decl.txt", "bad_exp.txt", "bad_exp2.txt","bad_exp3.txt","bad_exp4.txt","bad_exp5.txt","bad_exp6.txt",
						"bad_exp7.txt", "bad_exp8.txt", "bad_scoping.txt", "bad_scoping2.txt", "euclid.txt", "hiding.txt", "if.txt", "print.txt", "testing.txt" };

	//string file = filename[0];//Change this to whatever you want. If you are using multiple instances of classes, you might want to use this so you don't have to go through and change every argument.

	//for (int i = 0; i < 19; i++)//For-loop to check all files.
	//{
		//cout << i << endl;
		Parser parse(filename[0]);

		//parse.analyze();//Runs a lexical analysis that stores all the tokens(in order as seen in the file itself) in an array before parsing. 

		parse.program();//This here does the actual parsing. The functions were designed by Professor Saeedloei. This program runs analyze() automatically
		//for convenience.
	//}
	return 0;

}

