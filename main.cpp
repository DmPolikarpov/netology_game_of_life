#include <iostream>
#include <cstdlib>
#include <Windows.h>

int** createAllCells(int rows, int columns);
void fillAllCells(int** allCells);
void drawAllCells(int** arr, int rows, int columns);
void drawCell(int** arr, int row, int column);
bool calculateNewGeneration(int** arr, int rows, int columns);
int getNumberOfAliveNeighbours(int** arr, int i, int j, int rows, int columns);
void mainGameLoop(int** arr, int rows, int columns);
void clearArr(int** arr, int rows);

int main()
{
	int rows{10}, columns{10};

	int** allCells = createAllCells(rows, columns);

	fillAllCells(allCells);

	mainGameLoop(allCells, rows, columns);

	clearArr(allCells, rows);

	return EXIT_SUCCESS;
}

void mainGameLoop(int** arr, int rows, int columns)
{
	bool isRunnig{ true };
	int generation{};
	
	while (isRunnig && generation < 7)
	{
		generation++;

		std::system("cls");

		drawAllCells(arr, rows, columns);

		std::cout << "Generation #" << generation << ". ";

		isRunnig = calculateNewGeneration(arr, rows, columns);

		Sleep(3000);
	}
}

int** createAllCells(int rows, int columns)
{
	int** arr = new int* [rows] {};

	for (int index{}; index < rows; index++)
	{
		arr[index] = new int[columns] {};
	}

	return arr;
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
	for (int i{}; i < rows; i++)
	{
		for (int j{}; j < columns; j++)
		{
			// calculate number of alive neighbour cells
			int aliveCells = getNumberOfAliveNeighbours(arr, i, j, rows, columns);
			// if number of alive neighbors equals 2
			// life status of the cell won't be changed
			if (aliveCells == 2) {
				continue;
				// if number of alive neighbors equals 3
				// the cell will become alive
			}
			else if (aliveCells == 3 && arr[i][j] != 1) {
				arr[i][j] = 1;
				isChanged = true;
				// otherwise it will die
			} else if (arr[i][j] != 0) {
				arr[i][j] = 0;
				isChanged = true;
			}
		}
	}
	return isChanged;
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