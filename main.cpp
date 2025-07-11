#include <iostream>
#include <cstdlib>
#include <Windows.h>

int** createNewArray(int rows, int columns);
void fillAllCells(int** allCells);
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
	int rows{20}, columns{30};

	int** allCells = createNewArray(rows, columns);

	fillAllCells(allCells);

	mainGameLoop(allCells, rows, columns);

	clearArr(allCells, rows);

	return EXIT_SUCCESS;
}

void mainGameLoop(int** arr, int rows, int columns)
{
	bool isRunnig{ true };
	int generation{1};
	
	while (isRunnig && generation < 20)
	{

		showGeneration(arr, rows, columns, generation);

		isRunnig = calculateNewGeneration(arr, rows, columns);

		generation++;

		if (isRunnig)
		{
			Sleep(2000);
		} 
		else if (getAliveCellNumber(arr, rows, columns))
		{
			showGeneration(arr, rows, columns, generation);
			std::cout << "The world is stagnated. Game over." << std::endl;
		}
		else 
		{
			showGeneration(arr, rows, columns, generation);
			std::cout << "All cells are dead. Game over." << std::endl;
		}
	}
}

void showGeneration(int** arr, int rows, int columns, int generation)
{
	std::system("cls");

	drawAllCells(arr, rows, columns);

	int aliveCellsNumber = getAliveCellNumber(arr, rows, columns);

	std::cout << "Generation #" << generation << ". Alive cells: " << aliveCellsNumber << std::endl;
}

int** createNewArray(int rows, int columns)
{
	int** arr = new int* [rows] {};

	for (int index{}; index < rows; index++)
	{
		arr[index] = new int[columns] {};
	}

	return arr;
}

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

void fillAllCells(int** arr)
{
	arr[2][3] = 1;
	arr[2][4] = 1;
	arr[3][4] = 1;
	arr[3][5] = 1;
	arr[3][6] = 1;
	arr[3][7] = 1;
	arr[5][0] = 1;
	arr[5][1] = 1;
	arr[5][2] = 1;
	arr[6][2] = 1;
}

bool calculateNewGeneration(int** arr, int rows, int columns)
{
	bool isChanged{ false };

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
			} else if (arr[i][j] != 0 && (aliveCells < 2 || aliveCells > 3)) {
				arr[i][j] = 0;
				isChanged = true;
			}
		}
	}

	clearArr(currGenArr, rows);

	return isChanged;
}

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

void drawCell(int** arr, int row, int column)
{
	arr[row][column] == 0 ?
		std::cout << "- " :
		std::cout << "* ";
}

void clearArr(int** arr, int rows)
{
	for (int index{}; index < rows; index++)
	{
		delete[] arr[index];
	}

	delete[] arr;
}