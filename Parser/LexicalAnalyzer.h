#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <array>
using namespace std;

class LexicalAnalyzer
{

public:

	int y, x, z, l, i, m, n, s;

	string filename, line, lines, token; //Constructor will take the file's name as an argument. "lines" is the entire text file with line feeds. "line"
	//stores each individual line in order to add it to the overall file string(lines). "token" will store the found token as a string
	bool start, state, valid;

	int count;

	array<string, 20> symbols = { ":=", ";", ":", "+", "-", "*", "<", "=<", "=", "!=", ">=", ">", "/", "|", "{", "}", "[", "]", "(", ")" };
	array<string, 17> keywords = { "program", "end", "bool", "int", "if", "then", "else", "fi", "while", "do", "od", "print", "or", "and", "not", "false", "true" };
	array<char, 17> symbol = { ';', ':', '+', '-', '*', '<', '=', '>', '/', '|', '{', '}', '[', ']', '(', ')', '!' };//We're gonna use this to check for invalid characters, so symbols with length > 1 were taken out while
	//the ones that were in the "symbols" array but not a single character by themselves were added in.
	string tokens[5] = { "ID", "NUM", "symbol", "keyword", "end-of-state" };

	ifstream grammar;//Creates an instance of ifstream



	LexicalAnalyzer(string filename)//Constructor that takes a string value as an argument
	{

		this->filename = filename;//Initializes this instance's filename to the constructor's argument

		grammar.open(this->filename);//Calls the function in the "grammar" instance to open a file. The function is called in here so the function "next()" can work in a loop without opening the same file repeatedly

		if (grammar.fail())
		{
			cout << "File failed to open!" << endl;
			abort();
		}

		y = 1;//Line position. Textfiles start at line 1

		x = 1;//Column position. Textfiles start at line 1

		z = 0;//Initialized as the position of the starting character of a token when one is found

		i = 0;//Keeps track of where you are in the string that contains the entire textfile

		m = symbols.size();//Used to limit the loop that checks the token against every valid symbol 

		n = keywords.size();//Used to limit the loop that checks the token against every valid keyword

		start = false;//I put the entire textfile into a string(so it's easier to analyze character-by-character) before I set this to "true."

		state = false;//Gets set to "true" when a token is found

		valid = false;//Used in aiding when an invalid character is found within a token

		//function declarations

		void next();

		string kind();

		string value();

		string position();

		count = 0;
	
	}








	void next()//Puts entire textfile into a string before it can be looped in the main code to analyze token-by-token
	{


		//The file was already opened in the constructor. Do NOT open it again. It is unnecessary and could lead to problems.

		if (start == false)//I have found it easier to just put the entire textfile(even matching where the newlines should be) in my own string for analysis. 
		{
			while (getline(grammar, line))//I tried analyzing the file itself character-by-character, but the "char" variables used in C++ have a frustrating feature built-in: They take two characters that are next to each other instead of just one.
			{


				//Check for invalid characters
				for (int r = 0; r < line.length(); r++)
				{

					valid = false;

					if ((r < line.length() - 2) && line.at(r) == '/' && line.at(r++) == '/')//attempting to read lines.at(i++) when lines.at(i) is the last character will generate an error
					{
						x = 1;//when at a new line, reset the column position to one
						break;
					}

					for (int s = 0; s < symbol.size(); s++)
					{
						if (line.at(r) == symbol[s] || isalnum(line.at(r)) || line.at(r) == '\n' || line.at(r) == 13 || line.at(r) == ' ' || line.at(r) == '_')
						{
							valid = true;
							break;
						}
					}
					if (valid == false)
					{
						cout << "Invalid character at line " << y << ", position " << x << endl;
						abort();
					}

					x++;
				}

				//This while loop reads line-by-line, adding the lines onto each other before adding newlines
				lines.append(line);
				lines.append("\n");
				y++;
				x = 1;

			}

			grammar.close(); //We have everything we need in string "lines." Close the file.

			//Checks if file is empty
			int length = 0;
			for (int g = 0; g < lines.length(); g++)
			{
				if (lines.at(g) != '\n' && lines.at(g) != 13 && lines.at(g) != ' ')
				{

					length++;
				}
			}
			if (length == 0)
			{
				cout << "Empty File!" << endl;
				abort();
			}

			x = 1;
			y = 1;
			valid = false;

			start = true;//We are ready to start analyzing in the "else" statement.



		}
		else
		{
			state = false;
			//The number "13" is the int-value representing the carriage-return.
			//The while-loop right below keeps going until a token is found
			while (state == false && (i <= (lines.length() - 1)) && (lines.length() != 0))
			{

				if ((i < lines.length() - 2) && lines.at(i) == '/' && lines.at(i++) == '/')//Attempting to read lines.at(i++) when lines.at(i) is the last character will generate an error
				{
					//The while-loop right below checks for comments in textfiles and skips them until it sees a line feed or carriage-return.
					while (lines.at(i) != '\n' && lines.at(i) != 13)
					{

						x++;//Technically don't need to increment, since seeing a "//" means to ignore everything until a new line and reset to 1. I'll just leave it here in case it's needed.
						i++;//Since string "lines" is literally the entire textfile, i needs to be incremented as you count through 
					}

					x = 1;//When at a new line, reset the column position to one
				}


				//If the code sees a space, it keeps going while incrementing x and i until it doesn't
				else if (lines.at(i) == ' ')
				{
					while (lines.at(i) == ' ')
					{
						x++;
						i++;
					}
				}

				//If the code sees a newline, set the column number back to "1" and increment "y" and "i"
				else if (lines.at(i) == '\n' || lines.at(i) == 13)
				{

					i++;
					y++;
					x = 1;
				}

				//This is to check if a digit is seen first, in which case it keeps incrementing until it doesn't see a digit. This digit will then be counted as a literal integer value.
				//For this grammar we are implementing, literal integer values do NOT have to be separated from keywords, symbols, or identifiers by a space. "2while" is the token "2" and keyword "while." 
				else if (isdigit(lines.at(i)) && (lines.at(i) != ' '))
				{
					z = i;
					s = x;
					while (isdigit(lines.at(i)) && (lines.at(i) != ' '))
					{
						x++;
						i++;
						l++;
					}

					token = lines.substr(z, l);//Makes string starting line.at(z) to line.at(z+l)
					
					state = true;

					l = 0;


				}

				//When an alphanumeric character is found, increment set z = i to mark the beginning of the token . Set s = x for starting character of token's position.
				//"s" will be used to return the position of the token's first character while z is used to create the substring "token" from the string "lines."
				//I believe you can use either one for both tasks, but I'm gonna leave it like this in-case changes have to be made.
				else if ((isalnum(lines.at(i)) || lines.at(i) == '_') && (lines.at(i) != ' '))
				{

					z = i;
					s = x;

					//Underscore after an alphanumeric character is fine. An underscore FIRST, however, is not OK for this language I'm analyzing. 
					//This will keep going until you find a ' ' or a symbol.
					while ((isalnum(lines.at(i)) || lines.at(i) == '_') && (lines.at(i) != ' '))
					{


						x++;
						i++;
						l++;

					}

					token = lines.substr(z, l);//Makes string starting line.at(z) to line.at(z+l)



					l = 0;

					state = true;

				}

				//Similar logic to the "else if" statement right above, but for symbols.
				else if (!isalnum(lines.at(i)) && (lines.at(i) != ' ') && (lines.at(i) != '\n') && (lines.at(i) != 13))
				{

					s = x;
					z = i;

					while (!isalnum(lines.at(i)) && (lines.at(i) != ' ') && (lines.at(i) != '\n') && (lines.at(i) != 13))
					{

						if ((i < lines.length() - 2) && lines.at(i) == '/' && lines.at(i++) == '/')
						{
							break;
						}

						x++;
						i++;
						l++;

					}

					token = lines.substr(z, l);

					l = 0;
					state = true;
				}


			}
		}
		count++;
	}











	//Checks what kind of token "token" is.
	//Do NOT change the orderings of these conditionals, or you could risk breaking the code.
	//They are ordered in such a way that they "cover" each other.
	string kind()
	{

		//Checks if "token" is equal to any of the valid symbols
		for (int i = 0; i < m; i++)
		{
			if (token == symbols[i])
			{
				return symbols[i];
			}

		}

		//Checks if "token" is equal to any of the keywords
		for (int i = 0; i < n; i++)
		{
			if (token == "end")
			{
				return tokens[4];
			}
			if (token == keywords[i])
			{
				count++;
				return keywords[i];
			}

		}

		//Checks if "token" is an identifier
		if (token.length() > 0 && (isalpha(token.at(0)) || token.at(0) == '_'))
		{
			for (int i = 1; i < token.length(); i++)
			{
				if (!isalnum(token.at(i)) && token.at(i) != '_')
				{
					return "invalid";
				}
			}
			return tokens[0];

		}

		//Checks if "token" is a literal integer
		if (token.length() > 0 && isdigit(token.at(0)))
		{
			for (int i = 1; i < token.length(); i++)
			{
				if (!isdigit(token.at(i)))
				{
					return "invalid";
				}
			}
			return tokens[1];

		}



		//WARNING: Long explanation for why my validation check is so long

		//By design, the above conditional statements in this function have already checked for EVERY valid token possible. 

		//If the code actually gets to this point, there is clearly an invalid token.

		//Unfortunately, using any of the variables I increment in this class is pointless. 

		//The incremented variables do NOT stop when an invalid token is found among a "group" of symbols. They increment until the ENTIRE token is put within "token."

		//The validation check is done in this function, meaning that "x" has already been incremented to the end of the token, making it
		//useless for checking the position of the invalid character within the line. "z," "s," "l," and "i" are also useless to find 
		//the position of the character because they don't mark when an invalid character is found.


		//It is also useless to attempt checking from the very beginning because dev comments may contain forbidden characters that aren't really part of the code and so shouldn't generate errors

		//After careful thought and deliberation, I decided it was easier to just start over again to get the position of the invalid character

		//The below conditional finds the invalid character in "token," counts up variable "v" until it equals "y(the line position of invalid character)," 
		//sets "v" back to "1," and increments "v" until it gets to the invalid character
		if (token.length() > 0 && valid == false)
		{

			cout << "Invalid token at line " << y << ", position ";

			char invalid = '0';

			//The for-loop right below scans token's characters until it finds the invalid one
			for (int i = 0; i < token.length(); i++)
			{
				valid = false;
				for (int r = 0; r < symbol.size(); r++)
				{
					if (token.at(i) == symbol[r])
					{
						valid = true;
						break;
					}
				}
				if (valid == false)
				{
					invalid = token.at(i);
				}
			}


			int v = 1;

			//The reason why you have to go to the specific line before you start scanning for the invalid characters is because dev comments with invalid characters might get in the way
			for (int i = 0; i < lines.length(); i++)
			{


				if (lines.at(i) == '\n' || lines.at(i) == 13)
				{
					v++;

				}

				if (v == y)
				{
					v = 1;
					while (lines.at(i) != invalid)
					{
						i++;
						v++;
						if (lines.at(i) == '\n')//Stops once a line feed is found. This means that the symbols are valid, but the token is not due to incorrect spacing. ">==" makes no sense.
						{
							break;
						}
					}

					if (valid == true)//In the case that the characters are valid but the token is not or makes no sense. ">==," for example, doesn't make sense.
					{
						cout << s << endl;

					}
					else//If there are invalid characters, this will run instead. 
					{
						cout << v - 1 << endl;
					}
				}

			}

			abort();//Aborts the program once the invalid token's position is found 

		}



		return " ";
	}











	//Returns value of token if it's an identifier or integer
	string value()
	{

		if (token.length() > 0 && (kind() == tokens[0] || kind() == tokens[1]))
		{
			return token;
		}
		else
		{
			return " ";
		}
	}












	//Returns line and first character of token's position
	string position()
	{
		return "line " + to_string(y) + ", position " + to_string(s);
	}



};

