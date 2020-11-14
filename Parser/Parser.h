#pragma once
#include "LexicalAnalyzer.h"
#include <assert.h>
class Parser : public LexicalAnalyzer
{
//Due to the way my lexical analyzer works, using next() and kind() won't be helpful here. I've decided to do store the tokens in their respective order in a dynamic array. 
//The function analyze() should be called before you start parsing, as it calls the lexical analyzer to create this "helper" array.



public: 

	

	using LexicalAnalyzer::LexicalAnalyzer;//Inherit the constructor of "LexicalAnalyzer" class
	int c = 0;
	string* tokens;
	int count = 0;


	bool program()
	{
		//cout << i << endl;
		//cout << tokens[c] << endl;
		match("program");
		match("ID");
		match(":");
		body();
		match("end-of-state");
		return true;
	}






	bool body()
	{
		if (tokens[c] == "bool" || tokens[c] == "int")
		{
			declarations();
		}
		statements();
		return true;
	}






	bool declarations()
	{
		declaration();

		while (tokens[c] == "int" || tokens[c] == "bool")
		{
			declaration();
		}
		return true;
	}


	bool declaration()
	{
		assert(tokens[c] == "int" || tokens[c] == "bool");
		c++;
		match("ID");
		match(";");
		return true;
	}



	bool statements()
	{
		statement();
		while (tokens[c] == ";")
		{
			c++;
			statement();
		}
		return true;
	}



	bool statement()
	{
		if (tokens[c] == "ID")
		{
			assignmentstatement();
		}
		else if (tokens[c] == "if")
		{
			conditionalstatement();
		}
		else if (tokens[c] == "while")
		{
			iterativestatement();
		}
		else if (tokens[c] == "print")
		{
			printstatement();
		}
		else
		{
			string symbols[] = { "ID", "if", "while", "print", "\0"};
			expected(symbols);
		}
		return true;
	}




	bool assignmentstatement()
	{
		assert(tokens[c] == "ID");
		match("ID");
		match(":=");
		expression();
		return true;
	}


	bool conditionalstatement()
	{
		assert(tokens[c] == "if");
		match("if");
		
		expression();
		match("then");
		body();
		if (tokens[c] == "else")
		{
			c++;
			body();
		}
		match("fi");
		return true;
	}


	bool iterativestatement()
	{
		assert(tokens[c] == "while");
		match("while");
		expression();
		match("do");
		body();
		match("od");
		return true;
	}


	bool printstatement()
	{
		assert(tokens[c] == "print");
		match("print");
		expression();
		return true;
	}


	bool expression()
	{
		simpleexpression();
		if (tokens[c] == "<" || tokens[c] == "=<" || tokens[c] == "=" || tokens[c] == "!=" || tokens[c] == ">=" || tokens[c] == ">")
		{
			c++;
			simpleexpression();
		}
		return true;
	}


	bool simpleexpression()
	{
		terminal();
		while (tokens[c] == "+" || tokens[c] == "-" || tokens[c] == "or")
		{
			c++;
			terminal();
		}
		return true;
	}


	bool terminal()
	{
		factor();
		while (tokens[c] == "*" || tokens[c] == "/" || tokens[c] == "and")
		{
			c++;
			factor();
		}
		return true;
	}

	bool factor()
	{
		if (tokens[c] == "-" || tokens[c] == "not")
		{
			c++;
		}
		if (tokens[c] == "false" || tokens[c] == "true" || tokens[c] == "NUM")
		{
			literal();
		}
		else if (tokens[c] == "ID")
		{
			c++;
		}
		else if (tokens[c] == "(")
		{
			c++;
			expression();
			match(")");
		}
		else
		{
			string symbols[] = { "true", "false", "NUM", "ID", "(", "\0" };
			expected(symbols);
		}
		return true;
	}

	bool literal()
	{
		assert(tokens[c] == "false" || tokens[c] == "true" || tokens[c] == "NUM");
			if (tokens[c] == "NUM")
			{
				c++;
			}
			else
			{
				booleanliteral();
			}
			return true;
	}

	bool booleanliteral()
	{
		assert(tokens[c] == "true" || tokens[c] == "false");
		c++;
		return true;
	}


	bool match(string symbol)
	{
		if (tokens[c] == symbol)
		{
			c++;
			return true;
		}
		else
		{
			//cout << "ERROR on line " << y << "!" << " Seeing " << tokens[c] << ", expecting " << symbol << endl;
			cout << "ERROR! Seeing " << tokens[c] << ", expecting " << symbol << endl;
			cout << c << endl;
			return false;
		}
	}



	bool expected(string symbols[])
	{
		int i = 0;
		while(symbols[i] != "\0")
		{
			if (tokens[c] == symbols[i])
			{
				return true;
			}
			i++;
		}
		cout << "ERROR: seeing " << tokens[c] << " but expecting ";
		i = 0;
		while (symbols[i] != "\0")
		{
			if (symbols[i + 1] != "\0")
			{
				cout << symbols[i] << ", ";
			}
			else
			{
				cout << "or " << symbols[i];
			}
			i++;
		}
		cout << endl;
		return false;
	}



//Using an instance of class "LexicalAnalyzer," It scans and outputs all the tokens while keeping count of how many tokens there are. A dynamic array is then declared to fit these tokens.
//Due to the way "LexicalAnalyzer" is designed and the fact that it's impossible to know how many tokens there are before you count, I tell the parser to run its own lexical analysis(it inherits from class "LexicalAnalyzer") so it can go back through
//and store the tokens in this dynamic array. Specifically, it's quite hard to get an instance of "LexicalAnalyzer" to go back to the beginning of a file and reset all the variables. It's not worth the trouble, and you're really
//not supposed to change the base class anyway. I know of more efficient ways to go about doing this, but it can be confusing and a bit of a mess. This way is simpler.
	void analyze()
	{

		LexicalAnalyzer analyze(filename);

		analyze.next();
		while (analyze.kind() != "end-of-state")
		{

			analyze.next();
			cout << analyze.position() << " " << analyze.kind() << " " << analyze.value() << endl;
			count++;

		}

		int i = 0;

		//LexicalAnalyzer store(filename);
		next();
		this->tokens = new string[count];
		while (kind() != "end-of-state")
		{
			next();
			//cout << "hi" << endl;
			this->tokens[i] = kind();
			i++;
		}

		//for (int i = 0; i < count; i++)
		//{
		//	cout << this->tokens[i] << endl;
		//}
		
		
	}

};

