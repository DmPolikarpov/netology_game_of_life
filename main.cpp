#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <Windows.h>

int** getInitialState(int* rows, int* columns);
int** createNewArray(int rows, int columns);
void drawAllCells(int** arr, int rows, int columns);
void drawCell(int** arr, int row, int column);
bool calculateNewGeneration(int** arr, int rows, int columns);
int getAliveCellNumber(int** arr, int rows, int columns);
int getNumberOfAliveNeighbours(int** arr, int i, int j, int rows, int columns);
int** copyArray(int** arr, int rows, int columns);
void mainGameLoop(int** arr, int rows, int columns);
void showGeneration(int** arr, int rows, int columns, int generation);
void clearArr(int** arr, int rows);

int main()
{
	int rows{}, columns{};

	int** allCells = getInitialState(&rows, &columns);

	mainGameLoop(allCells, rows, columns);

	clearArr(allCells, rows);

	return EXIT_SUCCESS;
}

// read source text file and create an array with initial data (first generation)
// the array has values of 0 and 1 only
// 1 - cell is alive
// 0 - cell is dead
int** getInitialState(int* rows, int* columns)
{

	std::ifstream iText("data.txt");

	// case if the text file is opened without errors
	if (iText.is_open())
	{
		// fill rows and columns numbers
		iText >> *rows;
		iText >> *columns;
		// create a two dimentional array
		int** arr = createNewArray(*rows, *columns);

		std::string data{};

		int row{}, column{};
		// fill the array
		while (iText >> data)
		{
			row = std::stoi(data);
			iText >> column;

			arr[row][column] = 1;
		}

		iText.close();

		return arr;

	}
	// handle error case
	else
	{
		std::cout << "some error happened!" << std::endl;
		// create and return a new array anyway
		return createNewArray(*rows, *columns);
	}

}

// main game loop function:
// - show each generation
// - calculate new generations
// - check conditions to end game
// - end game and show result message to user
void mainGameLoop(int** arr, int rows, int columns)
{
	bool isRunnig{ true }; // to check if array were modified in new generation or not (to continue or stop the game)
	int generation{ 1 }; // generation counter

	while (isRunnig)
	{
		// display the current generation in the console
		showGeneration(arr, rows, columns, generation);
		// calculate a new generation and find out if it is different compared to previous one
		isRunnig = calculateNewGeneration(arr, rows, columns);

		generation++;
		Sleep(2000);

		// if a new generation is not different and there are alive cells. It means that the world is stagnated.
		if (!isRunnig && getAliveCellNumber(arr, rows, columns))
		{
			showGeneration(arr, rows, columns, generation);
			std::cout << "The world is stagnated. Game over." << std::endl;
		}
		// if a new generation is not different and there are not any alive cells. It means that all cell are dead.
		else if (!isRunnig)
		{
			showGeneration(arr, rows, columns, generation);
			std::cout << "All cells are dead. Game over." << std::endl;
		}
	}
}

/********** DRAWING FUNCTIONS **********/

// display the current generation in the console
// show the current generation statistic
void showGeneration(int** arr, int rows, int columns, int generation)
{
	std::system("cls");

	drawAllCells(arr, rows, columns);

	int aliveCellsNumber = getAliveCellNumber(arr, rows, columns);

	std::cout << "Generation #" << generation << ". Alive cells: " << aliveCellsNumber << std::endl;
}

// draw the whole game board
void drawAllCells(int** arr, int rows, int columns)
{
	for (int i{}; i < rows; i++)
	{
		for (int j{}; j < columns; j++)
		{
			drawCell(arr, i, j);
		}
		std::cout << std::endl;
	}
}

// draw a single cell
void drawCell(int** arr, int row, int column)
{
	arr[row][column] == 0 ?
		std::cout << "- " :
		std::cout << "* ";
}

/********** END DRAWING FUNCTIONS **********/

/*********** UTILS ***********/

// create a new two dimentional array
int** createNewArray(int rows, int columns)
{
	int** arr = new int* [rows] {};

	for (int index{}; index < rows; index++)
	{
		arr[index] = new int[columns] {};
	}

	return arr;
}

// make and return a copy of a two dimentional array
int** copyArray(int** arr, int rows, int columns)
{
	int** newArr = createNewArray(rows, columns);

	for (int i{}; i < rows; i++)
	{
		for (int j{}; j < columns; j++)
		{
			newArr[i][j] = arr[i][j];
		}
	}

	return newArr;
}

// calculate a new generation and return true or false 
// if the new generation in different compared to previous one
bool calculateNewGeneration(int** arr, int rows, int columns)
{
	bool isChanged{ false };

	// create a new service temp array as a copy of the current generation
	// to calculate neighbour cell number correctly, because common array
	// will be changed gradually
	int** currGenArr = copyArray(arr, rows, columns);

	for (int i{}; i < rows; i++)
	{
		for (int j{}; j < columns; j++)
		{
			int aliveCells = getNumberOfAliveNeighbours(currGenArr, i, j, rows, columns);
			if (aliveCells == 3 && arr[i][j] == 0) {
				arr[i][j] = 1;
				isChanged = true;
			}
			if (arr[i][j] == 1 && (aliveCells == 2 || aliveCells == 3)) {
				continue;
			}
			else if (arr[i][j] != 0 && (aliveCells < 2 || aliveCells > 3)) {
				arr[i][j] = 0;
				isChanged = true;
			}
		}
	}
	// remove temp array
	clearArr(currGenArr, rows);

	return isChanged;
}

// calculate alive cells number (1 - a cell is alive, 0 - a cell is dead)
int getAliveCellNumber(int** arr, int rows, int columns)
{
	int result{ };
	for (int i{}; i < rows; i++)
	{
		for (int j{}; j < columns; j++)
		{
			result += arr[i][j];
		}
	}
	return result;
}

// calculate a number of alive neighbour cells
int getNumberOfAliveNeighbours(int** arr, int i, int j, int rows, int columns)
{
	int result{};

	if (i == 0 && j == 0)
		result = arr[i + 1][j] + arr[i][j + 1] + arr[i + 1][j + 1];
	else if (i == 0 && j == columns - 1)
		result = arr[i][j - 1] + arr[i + 1][j - 1] + arr[i + 1][j];
	else if (i == rows - 1 && j == 0)
		result = arr[i - 1][j] + arr[i - 1][j + 1] + arr[i][j + 1];
	else if (i == rows - 1 && j == columns - 1)
		result = arr[i - 1][j - 1] + arr[i][j - 1] + arr[i - 1][j];
	else if (i == 0 && j != 0 && j != columns - 1)
		result = arr[i][j - 1] + arr[i + 1][j - 1] + arr[i + 1][j] + arr[i][j + 1] + arr[i + 1][j + 1];
	else if (i != 0 && j == 0 && i != rows - 1)
		result = arr[i - 1][j] + arr[i + 1][j] + arr[i - 1][j + 1] + arr[i][j + 1] + arr[i + 1][j + 1];
	else if (i == rows - 1 && j != columns - 1)
		result = arr[i - 1][j - 1] + arr[i][j - 1] + arr[i - 1][j] + arr[i - 1][j + 1] + arr[i][j + 1];
	else if (i != rows - 1 && j == columns - 1)
		result = arr[i - 1][j - 1] + arr[i][j - 1] + arr[i + 1][j - 1] + arr[i - 1][j] + arr[i + 1][j];
	else
		result = arr[i - 1][j - 1] + arr[i][j - 1] + arr[i + 1][j - 1] + arr[i - 1][j] + arr[i + 1][j] + arr[i - 1][j + 1] + arr[i][j + 1] + arr[i + 1][j + 1];

	return result;
}

// remove a two dimentional array
void clearArr(int** arr, int rows)
{
	for (int index{}; index < rows; index++)
	{
		delete[] arr[index];
	}

	delete[] arr;
}

/********** END UTILS **********/