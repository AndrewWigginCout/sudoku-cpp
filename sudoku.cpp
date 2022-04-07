#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void printasints(char *n,int size)			//debug
{
	for (int i=0;i<size;i++)
		cout<<int(n[i])<<"-";
	cout<<endl;
}

bool onetonine(char c)
{
	if ( '1' <= c and c <= '9') return 1;
	else return 0;
}

class Sudoku
{
public:
Sudoku(): g(), b(), unsolved()
{
	for (int j=0;j<9;j++)
		for (int i=0;i<81;i++)
			b[j][i/9][i%9]=0;
}

Sudoku(char n[81]): g(), b(), unsolved()
{
switch (n[0])
{
case'^':		//input from console
{
	string n;
	for (int y=0;y<9;y++)
	{
		getline(cin,n);
		for (int x=0;x<9;x++)
			if (onetonine(n[x])) g[y][x]=n[x]; else g[y][x]=32;
	}
break;
}
case '#':		//open from inputfile square format
{
	//cout<<"case#";
	ifstream in;
	string line;
	in.open(&n[1]);
	for (int y=0;y<9;y++)
	{
		getline(in,line);
		for (int x=0;x<9;x++)
		{
			g[y][x]=line[x];
			//cout<<g[y][x]<<endl;
			if (g[y][x] < 49 or g[y][x] > 57) g[y][x]=32;
		}	
	}
	in.close();
break;
}


case '@':		//short form string
{
	int cursor=1;
	int i=0;
	while (i<81)
	{
		if (onetonine(n[cursor])) g[i/9][i++%9] = n[cursor++];
		else
			{
			for (int j=0;j<n[cursor]-96;j++)	g[i/9][i++%9]=32;
			cursor++;
			}
	}
break;
}

case '-':		//short form file
{
	ifstream in;
	string line;
	in.open(&n[1]);
	getline(in,line);
	in.close();
	
	int cursor=0;
	int i=0;
	while (i<81)
	{
		if (onetonine(line[cursor])) g[i/9][i++%9] = line[cursor++];
		else
			{
			for (int j=0;j<line[cursor]-96;j++)	g[i/9][i++%9]=32;
			cursor++;
			}
	}
break;
}

default:		//long form string
{
	for (int i=0;i<81;i++)
	{	
		g[i/9][i%9]=n[i];
		if (g[i/9][i%9]<49 or g[i/9][i%9]>57) g[i/9][i%9]=32;
	}


break;			
}
}
	for (int j=0;j<9;j++)
		for (int i=0;i<81;i++)
			b[j][i/9][i%9]=0;
	for (int i=0;i<81;i++)
		if (!isdigit(g[i/9][i%9])) unsolved++;
}

void xxport(char *out)
{
	for (int y=0;y<9;y++)
		for (int x=0;x<9;x++)
			{out[y*9+x]=g[y][x];}
	out[81]=0;
}

void showbooleans()			//debug to make sure the fucking bools are set to 0!
{
	for (int i=0;i<81;i++)
		cout<<g[i/9][i%9];
	cout<<endl<<endl;
	for (int j=0;j<9;j++)
		for (int i=0;i<81;i++)
		cout<<b[j][i/9][i%9];
	cout<<endl<<endl;
	cout<<unsolved;
}
		
int showbooleans2()			//debug to make sure the fucking bools are set to 0!
{
	int a=0;
	for (int j=0;j<9;j++)
		for (int i=0;i<81;i++)
		if(b[j][i/9][i%9]) a++;
	return a;
}
		
int countbools(int y, int x)
{
	int sum=0;
	for (int i=0;i<9;i++)
		if (b[i][y][x]) sum++;
	return sum;
}

void printboolsyx(int y, int x)		//debug
{
	for (int i=0;i<9;i++)	
	cout<<b[i][y][x];
	cout<<endl;
}

void print()
{
	cout<<"unsolved= "<<unsolved<<endl;
	for (int i=0;i<9;i++)
	{
		for (int j=0;j<9;j++)
			cout<<g[i][j];
		cout<<endl;
	}
	cout<<endl;	
}

void printoneline()
{
	for (int i=0;i<9;i++)
	{
		for (int j=0;j<9;j++)
			cout<<g[i][j];
	}
	cout<<endl;	
}

void populatebools()
{
	for (int y=0;y<9;y++)
		for (int x=0;x<9;x++)
			{
				if (isdigit(g[y][x]))	eliminate(y,x);		//doesn't check in case g[y][x] is '0'
			}
}

bool checkforcontradictions()
{
	for (int i=0;i<81;i++)
		if (countbools(i/9,i%9)==9)	return 1;
	return 0;
}

bool updategrid()
{
	char c;
	for (int y=0;y<9;y++)
		for (int x=0;x<9;x++)
			{
				if (!isdigit(g[y][x])) 
				{
					c=mustbe(y,x);//must be will be space if there are less than 8 bools set
					if (isdigit(c))
						{
							unsolved--;
							g[y][x]=c;
						}
				}
			}
	return checkforcontradictions();
}

bool logic()
{
	populatebools();
	return updategrid();	
}

bool logicexhaust()
{
	int i;
	do
	{
		i=unsolved;
		if (logic()) return 1;
	}
	while (unsolved!=i);
	return 0;
}

char mustbe(int y,int x)		//checks each square to see if 8 out of 9 booleans are set
{
	if ((countbools(y,x)) == 8 )
		for (int i=0;i<9;i++)
			if (b[i][y][x] == 0) return i+49;
	return 32;
}

void eliminate(int y, int x)
{
	for (int i=0;i<x;i++) //elim across
		b[g[y][x]-49][y][i]=1;
	for (int i=x+1;i<9;i++)
		b[g[y][x]-49][y][i]=1;
				
	for (int i=0;i<y;i++) //elim up down
		b[g[y][x]-49][i][x]=1;
	for (int i=y+1;i<9;i++)
		b[g[y][x]-49][i][x]=1;
	
	for (int i=(y/3)*3;i<(y/3)*3+3;i++) // elim square
		for (int j=(x/3)*3;j<(x/3)*3+3;j++)
			{if (i==y and j==x) continue;	b[g[y][x]-49][i][j]=1;}
	
	for (int i=0;i<g[y][x]-49;i++)		// populate bools for self necessary for checking for contradictions
		b[i][y][x]=1;
	for (int i=g[y][x]-48;i<9;i++)
		b[i][y][x]=1;
}

void printcountbools()				//debug function
{
	cout<<"printcountbools output"<<endl;
	for (int y=0;y<9;y++)
		{for (int x=0;x<9;x++)
			cout<<countbools(y,x);
		cout<<endl;}
		cout<<endl;
}

char possibility(int y, int x, int w) // w starts at 0
{
	int j = -1;
	for (int i=0;i<9;i++)
	{
		if (!b[i][y][x])
		{
			j++;
			if (j==w) return i+49;
		}
	}
	return 65;  //unecessary
}


void listguesses(char *n)
{
	int j=0;
	for (int y=0;y<9;y++)
		for (int x=0;x<9;x++)
			if (countbools(y,x) == 7)
			{				//I'm shifting the values so I can print the character array for debugging
				n[j++]=y+48;
				n[j++]=x+48;
				n[j++]=possibility(y,x,0);
				n[j++]=y+48;
				n[j++]=x+48;
				n[j++]=possibility(y,x,1);
			}
	n[j]=0;
}

int findhighest()			//and less than 8
{
	int y=0;
	int x=0;
	int highestsofar = 0;
	for (int i=0;i<9;i++)
		for (int j=0;j<9;j++)
			if ((countbools(i,j) != 8)
				and (countbools(i,j) > highestsofar)) {y=i; x=j;highestsofar=countbools(y,x);}
	return y*9+x;
}

//int findhighestx()
//{
//	//int numbranches=0;
//	int y=0;
//	int x=0;
//	for (int i=0;i<9;i++)
//		for (int j=0;j<9;j++)
//			if (countbools(i,j) > countbools(y,x)) {y=i; x=j;}
////	y=yh;
////	x=xh;
//	//return 9-countbools(yh,xh);
//	return x;
//}

int listguesses2(char *n)
{
	int y=findhighest()/9;
	int x=findhighest()%9;
	int j=0;	
	//findhighest(int &y, int &x);
	int numbranches = 9-countbools(y,x);
	n[j++]=y+48;
	n[j++]=x+48;
	for (int i=0;i<numbranches;i++)
	{
		n[j++]=possibility(y,x,i);
	}
	n[j++]=0;	//terminate n
	return numbranches;
}

void setentry(int y, int x, char c)
{
	g[y][x] = c;
}

int explorebranch(int y, int x, char c)
{
	char s2[81];
	xxport(s2);
	Sudoku b(s2);
	b.setentry(y, x, c);
	return b.solve();
}

int solve()		//returns number of solutions
//returns 2 if solveable, 1 if unsolvable because of contradiction,
				//4 if unsolvable because both branches of first binary choice lead to contradiction
{
	if (logicexhaust()) return 0;
	if (unsolved ==0) {return 1;}
	char gu[243] = {0};
	
	
	int numbranches=listguesses2(gu);			//cout<<"numberbranches= "<<numbranches<<endl;
	if (numbranches>2) {print();cout<<"NUMBRANCHES > 2!!!!!!!!!!!!!!!!!!\n";}
	int sol=0;
	
	for (int i=0;i<numbranches;i++)
	{
		sol += explorebranch(gu[0]-48,gu[1]-48,gu[i+2]);		
	}
	return sol;
}


	private:
		char g[9][9];
		bool b[9][9][9];
		int unsolved;
	
};

int main(int argc, char* argv[])
{
		if (argc == 1)
		{
			cout<<"This is a sudoku solver. Usage: Takes one cmd line argument."		<<endl
				<<"You can give the puzzle as the argument as a sequence of 81"		<<endl
				<<"digits where 0 is unsolved. Or you can do #inputfilename.txt"		<<endl
				<<"Where the file contains the puzzle in a 9x9 square"		<<endl
				<<"Or -inputfilename of a shortened form file"		<<endl
				<<"Or @shortformpuzzlestring. You can now enter a puzzle"		<<endl
				<<"In 9x9 form"		<<endl;
				
			char n[]="^";
			Sudoku a(n);
			cout<<"Given: "<<endl;
			a.print();
			cout<<"Number of solutions = "<<a.solve();
		}
		if (argc == 2)
		{	
			Sudoku a(argv[1]);
			cout<<"Given: "<<endl;
			a.print();
			cout<<"Number of solutions = "<<a.solve();
		}
		if (argc ==3)
		{
			Sudoku a(argv[1]);
			cout<<"Given: "<<endl;
			a.print();
			cout<<"After logicexhaust()\n";
			a.logicexhaust();
			a.print();
			a.printcountbools();
			cout<<"Number of solutions = "<<a.solve();
		}
		if (argc ==4)
		{
			Sudoku a(argv[1]);
			cout<<"Number of solutions = "<<a.solve();
		}
		cin.ignore();
}
