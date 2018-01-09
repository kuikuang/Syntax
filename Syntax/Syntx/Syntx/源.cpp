#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<istream>
#include <queue>

using namespace std;
bool IsLetter(char ch)//判断是否为字母
{
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		return true;
	return false;
}

bool IsNum(char ch)//判断是否为数字
{
	if (ch >= '0'&&ch <= '9')
		return true;
	return false;
}

int IsKeyword(string str)//判断是否为关键词
{
	if (str == "int" || str == "double")
		return  1;
	else if (str == "if")
		return 9;
	else if (str == "else")
		return 10;
	else return -1;

}

void tokenProcess(string phrase, queue<int>&input, queue<string>&output)//词法处理
{
	int i = 0;
	char ch;
	while (i < phrase.length())
	{
		ch = phrase[i];
		if (ch == ' ')
		{
			i++;
		}
		else if (ch == '+')
		{
			input.push(3);
			i++;
			output.push("+");
		}
		else if (ch == '-')
		{
			input.push(4);
			i++;
			output.push("-");
		}
		else if (ch == '*')
		{
			input.push(5);
			i++;
			output.push("*");
		}
		else if (ch == '/')
		{
			input.push(6);
			i++;
			output.push("/");
		}
		else if (ch == '(')
		{
			input.push(7);
			i++;
			output.push("(");
		}
		else if (ch == ')')
		{
			input.push(8);
			i++;
			output.push(")");
		}
		else if (ch == '=')
		{
			if (phrase[i + 1] == '=')
			{
				input.push(13);
				i += 2;
				output.push("==");
			}
			else
			{
				input.push(11);
				i++;
				output.push("=");
			}

		}
		else if (ch == ';')
		{
			input.push(12);
			i++;
			output.push(";");
		}
		else if (ch == '>' || ch == '<' || ch == '!' || ch == '|' || ch == '&')
		{
			string str;
			str = str + ch;
			input.push(13);
			if (phrase[i + 1] == '=' || ch == '|' || ch == '&')
			{
				i += 2;
				str += phrase[i + 1];
			}
			else
			{
				i++;
			}
			output.push(str);
		}
		else if (IsNum(ch))
		{
			string str;
			str = str + ch;
			input.push(0);
			i++;
			while (IsNum(phrase[i]) && i < phrase.size() - 1)
			{
				str += phrase[i];
				i++;
			}
			output.push(str);
		}
		else if (IsLetter(ch))
		{
			string str;
			str = str + ch;
			i++;
			while (IsLetter(phrase[i]) && i < phrase.size() - 1)
			{
				str = str + phrase[i];
				i++;
			}
			if (IsKeyword(str) != -1)
			{
				input.push(IsKeyword(str));
			}
			else
				input.push(2);
			output.push(str);
		}


	}
	input.push(-1);//整个语句结束标志

}
void outputAll(string symbol[26], vector<int> stack, vector<int>serial, queue<string>output)//输出栈内内容,产生式标号
{
	queue<string>temp;
	temp = output;
	cout << "目前识别语句:";
	while (!temp.empty())
	{
		cout << temp.front() << " ";
		temp.pop();
	}
	cout << endl;
	cout << "语法栈： ";
	if (!stack.empty())
	{
		for (int i = 0; i < stack.size(); i++)
		{
			if (i < stack.size() - 1)
			{
				cout << symbol[stack.at(i)] << ",";
			}
			else
			{
				cout << symbol[stack.at(i)];
			}

		}
	}
	cout << endl;
	cout << "产生式标号： ";
	if (!serial.empty())
	{
		for (int i = 0; i < serial.size(); i++)
		{
			int at = serial.at(i);
			if (i < serial.size() - 1)
			{

				cout << at << ",";
			}
			else
			{
				cout << at;
			}

		}
	}
	cout << endl;
}

void syntax(queue<int>&input, int LL1[12][14], string symbol[26], int grammar[21][4], queue<string>output)
{
	vector<int>stack;//语法栈
	vector<int>serial;//记录所用产生式标号
	stack.push_back(0);//开始符号 V 
	int top = 0;//栈内第一个符号
	while (input.front() != -1)//我们规定 -1是整个代码段识别完毕标志
	{
		while (top - 12 >= 0)    //如果是终结符 出栈
		{
			input.pop();
			output.pop();
			stack.pop_back();
			outputAll(symbol, stack, serial, output);
			top = *(stack.end() - 1);

		}

		serial.push_back(LL1[top][input.front()]);//压入所用产生式

		if (top == 0 && input.front() == 12)//某一语句识别完毕
		{
			input.pop();
			output.pop();
		}
		if (input.front() == -1)   //代码段识别完毕
			break;

		stack.pop_back();//top出栈
		for (int i = 3; i >= 0; i--)
		{
			int temp = grammar[LL1[top][input.front()] - 1][i];

			if (temp != -1)   //产生式入栈
			{
				stack.push_back(temp);

			}

		}
		outputAll(symbol, stack, serial, output);//入栈完毕 输出
		if (!stack.empty())
		{
			top = *(stack.end() - 1);
		}
		else  //某一语句识别完毕
		{
			top = 0;
			stack.push_back(0);
			outputAll(symbol, stack, serial, output);

		}

	}
	cout << "代码段识别完毕" << endl;

}





void createLL1(int LL1[12][14])
{
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 14; j++)
			LL1[i][j] = 0;           //初始化

	LL1[0][1] = 1;   LL1[0][2] = 1;   //构造LL1表
	LL1[0][9] = 16;  LL1[0][10] = 1;
	LL1[0][12] = 1;  LL1[1][1] = 2;
	LL1[1][2] = 3;   LL1[2][2] = 4;
	LL1[3][0] = 5;   LL1[3][2] = 5;
	LL1[3][7] = 5;   LL1[4][3] = 6;
	LL1[4][4] = 7;   LL1[4][8] = 8;
	LL1[4][10] = 8;  LL1[4][12] = 8;
	LL1[4][13] = 8;  LL1[5][0] = 9;
	LL1[5][2] = 9;   LL1[5][7] = 9;
	LL1[6][3] = 12;  LL1[6][4] = 12;
	LL1[6][5] = 10;  LL1[6][6] = 11;
	LL1[6][8] = 12;  LL1[6][10] = 12;
	LL1[6][12] = 12; LL1[6][13] = 12;
	LL1[7][0] = 15;  LL1[7][2] = 14;
	LL1[7][7] = 13;  LL1[8][10] = 17;
	LL1[8][12] = 18; LL1[9][1] = 20;
	LL1[9][2] = 20;  LL1[9][9] = 20;
	LL1[10][7] = 19; LL1[11][0] = 21;
	LL1[11][2] = 21; LL1[11][7] = 21;

}

void createGrammar(int grammar[21][4])
{
	int	temp[21][4] = { { 1,2,23,3 },//二维数组记录语法产生式，-1表示空。
	{ 13,-1,-1,-1 },
	{ -1,-1,-1,-1 },
	{ 14,-1,-1,-1 },
	{ 5,4,-1,-1 },
	{ 15,5,4,-1 },
	{ 16,5,4,-1 },
	{ -1,-1,-1,-1 },
	{ 7,6,-1,-1 },
	{ 17,7,6,-1 },
	{ 18,7,6,-1 },
	{ -1,-1,-1,-1 },
	{ 19,3,20,-1 },
	{ 14,-1,-1,-1 },
	{ 12,-1,-1,-1 },
	{ 21, 10, 9, 8 },
	{ 22,9,-1,-1 },
	{ -1,-1,-1,-1 },
	{ 19,11,20,-1 },
	{ 0,-1,-1,-1 },
	{ 3,25,3,-1 }
	};
	for (int i = 0; i < 21; i++)
		for (int j = 0; j < 4; j++)
			grammar[i][j] = temp[i][j];

}
int main()
{
	int LL1[12][14];//记录LL1表
	createLL1(LL1); //创建LL1表

	int grammar[21][4];
	createGrammar(grammar);

	string symbol[26] = { "V","Ty","Id","E","E'","T","T'","F","S'","S","B","C","num","type","id","+","-","*","/","(",")","if","else","=","#","op" };
	ifstream file("test.txt");//打开文件
	queue<int>input;//创建队列，将词法分析后的结果存入该队列
	queue<string>output;//保存处理的语句
	string line, str;


	while (getline(file, line))
	{
		str = str + line;
	}//获得输入字符串

	tokenProcess(str, input, output);//词法分析

	cout << str << endl;

	syntax(input, LL1, symbol, grammar, output);//语法分析

	return 0;
}