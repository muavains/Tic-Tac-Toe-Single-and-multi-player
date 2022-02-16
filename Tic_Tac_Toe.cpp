#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
char game_board[3][3];
int match_drawn_1[50], matchdrawn1 = 0, match_drawn_2[50], matchdrawn2, moves = 0;
struct pos
{
	int ro;
	int co;
	void con(int n)
	{
		ro = (n - 1) / 3;
		co = (n - 1) % 3;
	}
	int con_num()
	{
		int num = ((co % 3) + 1) + (3 * ro);
		return num;
	}
	char con_chr()
	{
		char chr = '1';
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if ((i == ro) && (j == co))
					return chr;
				else
					chr++;
			}
		}
	}
};
struct users
{
	string name;
	pos num;
	char symbol;
	int win_games[50], wins = 0;
	bool operator==(users user)
	{
		return (name == user.name);
	}
};
users *current_player;
users user0, user2, computer;
void initialize_game()
{
	char c = '1';
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++, c++)
			game_board[i][j] = c;
}
void draw_game()
{
	cout << "\n\n\n\n\n";
	for (int i = 0; i < 3; i++)
	{
		cout << "\t\t\t\t\t";
		for (int j = 0; j < 3; j++)
		{
			if (j < 2)
				cout << game_board[i][j] << "  |  ";
			else
				cout << game_board[i][j] << "\n";
		}
		cout << "\t\t\t\t\t_____________\n";
	}
	cout << endl;
}
bool check(pos che)
{
	if (game_board[che.ro][che.co] == che.con_chr())
	{
		moves++;
		return 1;
	}
	else
		return 0;
}
pos input_user()
{
	int x;
	pos user1;
	cout << "\nEnter the position to take : ";
	while (1)
	{
		cin >> x;
		user1.con(x);
		if (check(user1))
			return user1;
		else
			cout << "\nPostion Already occupied!!!\nPlease Input a valid Position : ";
	}
}
char win()
{
	for (int i = 0; i < 3; i++)
    {
        if ((game_board[i][0] == game_board[i][1]) && (game_board[i][0] == game_board[i][2]))
            return game_board[i][0];
        if ((game_board[0][i] == game_board[1][i]) && (game_board[0][i] == game_board[2][i]))
            return game_board[0][i];
    }
    if ((game_board[0][0] == game_board[1][1]) && (game_board[0][0] == game_board[2][2]))
        return game_board[0][0];
    else if ((game_board[0][2] == game_board[1][1]) && (game_board[0][2] == game_board[2][0]))
        return game_board[0][2];
    int openSpot = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (!(game_board[i][j] == 'X' || game_board[i][j] == 'O'))
                openSpot += 1;
    if (openSpot == 0)
        return 'N';
    else
        return 'T';
}
int minmax(int depth, int alpha, int beta, bool isMaximizing)
{
    pos check;
    char checkWin = win();
    
    if (checkWin != 'T')
    {
        if (checkWin == user0.symbol)
            return (-10 - depth);
        else if (checkWin == computer.symbol)
            return (10 - depth);
        else if (checkWin == 'N')
            return 0;
    }
    if (isMaximizing)
    {
        int bestScore = -99, score;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                check.ro = i;
                check.co = j;
                if (game_board[check.ro][check.co] == check.con_chr())
                {
                    game_board[i][j] = computer.symbol;
                    score = minmax(depth + 1, alpha, beta, false);
                    game_board[i][j] = check.con_chr();
                    if (score > bestScore)
                        bestScore = score;
                    if (score > alpha)
                        alpha = score;
                    if (beta <= alpha)
                        break;
                }
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = 99, score;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                check.ro = i;
                check.co = j;
                if (game_board[check.ro][check.co] == check.con_chr())
                {
                    game_board[i][j] = user0.symbol;
                    score = minmax(depth + 1, alpha, beta, true);
                    game_board[i][j] = check.con_chr();
                    if (score < bestScore)
                        bestScore = score;
                    if (score < beta)
                        beta = score;
                    if (beta <= alpha)
                        break;
                }
            }
        }
        return bestScore;
    }
}
void computer_turn(int n)
{
	int bestScore = -99;
    int score, x, y;
    pos check;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            check.ro = i;
            check.co = j;
            if (game_board[check.ro][check.co] == check.con_chr())
            {
                game_board[i][j] = computer.symbol;
                score = minmax(0, -99, 99, false);
                game_board[i][j] = check.con_chr();
                if (score > bestScore)
                {
                    bestScore = score;
                    x = i;
                    y = j;
                }
            }
        }
    }
	moves++;
    computer.num.ro = x;
    computer.num.co = y;
}
void toogle_player(users &user0, users &user2)
{
	if (*current_player == user0)
		current_player = &user2;
	else
		current_player = &user0;
}

int main()
{
	int no_games_1 = 1, no_games_2 = 1, level, test;
	char c, game_mode, chr;
	bool flag, exit = 0;
	srand(time(0));
	cout << "\n\n\n\n\n\t\t\t\t\t______________________________\n\t\t\t\t\tWelcome to Tic Tac Toe Game!!!\n\t\t\t\t\t______________________________\n\n\t\t\t    \"By Shaheer Ahmed, Muhammad Uzair Arshad and Umar Khayyam\"\n\n\n\n\n\t\t\t\t\t";
	system("pause");
	system("cls");
main_menu:
	system("cls");
	cout << "\n\n\n\n\t\t--------Main Menu--------\n" 
		<< "\t\t1-Single Player (vs. Computer)\n\t\t2-Double Player\n\t\t3-Statics\n\t\t4-Exit\n\t\t-------------------------\n\t\tChoose Game Mode : ";
	cin >> game_mode;
	flag = 1;
	system("cls");
	switch (game_mode)
	{
	case '1':
		
		system("cls");
		goto single_player;
		break;
	case '2':
		system("cls");
		goto double_player;
	case '3':
		system("cls");
		goto statics;
		break;
	case '4':
		system("cls");
		goto exit;
		break;
	default:
		cout << "Wrong Input!!!: Try again!!!\n";
		goto main_menu;
	}
single_player:
	char choise;
	system("cls");
	cout << "Enter your First name : ";
	cin >> user0.name;
	difficulty_level:
	cout << "\n1-Easy\n2-Normal\n3-Hard\nEnter the level of Difficulty : ";
	cin >> level;
	computer.name = "Computer";
	system("cls");
	cout << "'" << user0.name << "' choose your character (X / O): ";
	while (flag)
	{
		cin >> chr;
		switch (chr)
		{
		case 'X':
		case 'x':
			user0.symbol = 'X';
			computer.symbol = 'O';
			flag = 0;
			break;
		case 'O':
		case 'o':
			computer.symbol = 'X';
			user0.symbol = 'O';
			flag = 0;
			break;
		default:
			cout << "Invalid Character Entered!!!\nPlease input a valid character : ";
			flag = 1;
		}
	}
play_again_1:
	test  = rand() % 2;
	if (test == 0)
		current_player = &user0;
	else
		current_player = &computer;
	cout << (*current_player).name << " won the Toss and its his first turn." << endl;
	initialize_game();
	while (moves <= 9)
	{
		draw_game();
		cout << "Player '" << (*current_player).name << "' turn : \n";
		if (*current_player == user0)
			(*current_player).num = input_user();
		else
		{
			computer_turn(level);
			cout << "Computer took position " << computer.num.con_num() << endl;
		}
		
		game_board[(*current_player).num.ro][(*current_player).num.co] = (*current_player).symbol;
		if (win() == (*current_player).symbol)
		{
			draw_game();
			cout << "Player '" << (*current_player).name << "' Wins!!!\n";
			(*current_player).win_games[(*current_player).wins] = no_games_1;
			(*current_player).wins++;
			cout << "Total Wins:\t\t\t" << user0.name << " = " << user0.wins << "\t\t\t" << computer.name << " = " << computer.wins << "\t\t\tDrawn Matches = " << matchdrawn1 << endl;
			break;
		}
		
		draw_game();
		system("pause");
		system("cls");
		toogle_player(user0, computer);
		if (moves == 9)
			break;
	}
	if (moves == 9)
	{
		cout << "Match Draw!!!\n";
		match_drawn_1[matchdrawn1] = no_games_1;
		matchdrawn1++;
	}
	no_games_1++;
	moves = 0; 
	cout << "Do you want to Play again(Y/N) : ";
	cin >> c;
	if ((c == 'n') || (c == 'N'))
	{
		cout << "Where you want to go : \n1-Main menu.\n2-Difficulty level.\n3-Exit\nEnter your choise : ";
		cin >> choise;
		switch (choise)
		{
		case '1':
			goto main_menu;
			break;
		case '2':
			goto difficulty_level;
			break;
		case '3':
			goto exit;
		}
	}
	else
		goto play_again_1;
double_player:
	system("cls");
	cout << "Enter first name of first player : ";
	cin >> user0.name;
	cout << "Welcome '" << user0.name << "'!!!. To the Game.\n";
	cout << "'" << user0.name << "' choose your character (X / O): ";
	while (flag)
	{
		cin >> chr;
		switch (chr)
		{
		case 'X':
		case 'x':
			user0.symbol = 'X';
			user2.symbol = 'O';
			flag = 0;
			break;
		case 'O':
		case 'o':
			user2.symbol = 'X';
			user0.symbol = 'O';
			flag = 0;
			break;
		default:
		{
			cout << "Invalid Character Entered!!!\nPlease input a valid character : ";
			flag = 1; 
		}
		}
	}
	cout << "Enter first name of Second player : ";
	cin >> user2.name;
	cout << "Welcome '" << user2.name << "'!!!. To the Game.\n";
play_again_2:
	test  = rand() % 2;
	if (test == 0)
		current_player = &user0;
	else
		current_player = &user2;
	initialize_game();
	do
	{
		draw_game();
		cout << "Player '" << (*current_player).name << "' turn : \n";
		(*current_player).num = input_user();
		game_board[(*current_player).num.ro][(*current_player).num.co] = (*current_player).symbol;
		if (win() == (*current_player).symbol)
		{
			draw_game();
			cout << "Player '" << (*current_player).name << "' Wins!!!\n";
			(*current_player).win_games[(*current_player).wins] = no_games_2;
			(*current_player).wins++;
			cout << "Total Wins:\t\t\t" << user0.name << " = " << user0.wins << "\t\t\t" << user2.name << " = " << user2.wins << "\t\t\tDrawn Matches = " << matchdrawn2 << endl;
			break;
		}
		toogle_player(user0, user2);
		system("cls");
	} while ((moves) <= 9);
	if ((moves) == 9)
	{
		cout << "\nMatch Draw!!!";
		match_drawn_2[matchdrawn2] = no_games_2;
		matchdrawn2++;
	}
	no_games_2++;
	moves = 0;
	cout << "Do you want to Play again(Y/N) : ";
	cin >> c;
	if ((c == 'n') || (c == 'N'))
	{
		cout << "Where you want to go : \n1-Main menu.\n2-Exit\nEnter your choise : ";
		cin >> choise;
		switch (choise)
		{
		case '1':
			goto main_menu;
			break;
		case '2':
			goto exit;
		}
	}
	else
		goto play_again_2;
statics:
	system("cls");
	char ch;
	cout << "1-Single Player (vs. Computer)\n2-Double Player\nChoose Mode of game : ";
	cin >> ch;
	switch (ch)
	{
	case '2':
		if (no_games_2 > 1)
		{
			cout << "Double Player Mode\n__________________\nNumber of matches Drawn : " << matchdrawn2;
			if (matchdrawn2 >= 1)
			{
				cout << "\nMatches Drawn : ";
				for (int i = 0; i < (matchdrawn2 - 1); i++)
				{
					if (i < (matchdrawn2 - 2))
						cout << match_drawn_2[i] << ", ";
					else
						cout << match_drawn_2[i] << ".\n";
				}
			}
			cout << "\nPlayer '" << user0.name << "' : \nNumber of Games won = " << user0.wins;
			if (user0.wins >= 1)
			{
				cout << "\nGames Won : ";
				for (int i = 0; i < user0.wins; i++)
				{
					if (i < (user0.wins - 1))
						cout << user0.win_games[i] << ", ";
					else
						cout << user0.win_games[i] << ".\n";
				}
			}
			cout << "Player '" << user2.name << "' : \nNumber of Games won = " << user2.wins;
			if (user2.wins >= 1)
			{
				cout << "Games Won : ";
				for (int i = 0; i < user2.wins; i++)
				{
					if (i < (user2.wins - 1))
						cout << user2.win_games[i] << ", ";
					else
						cout << user2.win_games[i] << ".\n";
				}
			}
		}
		else
			cout << "\nNo, Wins.!!!\n";
		system("pause");
		system("cls");
		break;
	case '1':
		if (no_games_1 > 1)
		{
			cout << "Single Player Mode\n__________________\nNumber of matches Drawn : " << matchdrawn1;
			if (matchdrawn1 >= 1)
			{
				cout << "\nMatches Drawn : ";
				for (int i = 0; i < (matchdrawn1 - 1); i++)
				{
					if (i < (matchdrawn1 - 2))
						cout << match_drawn_1[i] << ", ";
					else
						cout << match_drawn_1[i] << ".\n";
				}
			}
			cout << "\nPlayer '" << user0.name << "' : \nNumber of Games won = " << user0.wins;
			if (user0.wins >= 1)
			{
				cout << "\nGames Won : ";
				for (int i = 0; i < user0.wins; i++)
				{
					if (i < (user0.wins - 1))
						cout << user0.win_games[i] << ", ";
					else
						cout << user0.win_games[i] << ".\n";
				}
			}
			cout << "\n__________________\nPlayer '" << computer.name << "' : \nNumber of Games won = " << computer.wins;
			if (computer.wins >= 1)
			{
				cout << "\nGames Won : ";
				for (int i = 0; i < computer.wins; i++)
				{
					if (i < (computer.wins - 1))
						cout << computer.win_games[i] << ", ";
					else
						cout << computer.win_games[i] << ".\n";
				}
			}
		}
		else
			cout << "\nNo, Wins.!!!\n";
		system("pause");
		system("cls");
		break;
	}
	goto main_menu;
exit:
	system("pause");
	system("exit");
	cout << endl;
	return 0;
}