/*
 * Name:  CSVWriter.cpp
 * Autor: Frederik Plate, Patrick Schauer
 * Datum: 15.04.2019
 * Beschreibung:
 *  Diese Datei implementiert Methoden zum Arbeiten mit CSV-Dateien.
 */

#define _CRT_SECURE_NO_WARNINGS

#include "CSWriter.hpp"
#include <stdlib.h>
#include <cstring>
#include <stdexcept>
#include <fstream>

 /*
  * CSVWriter(char *fileName, int )
  *  <function_description>
  *
  * Parameterliste:
  *  char *fileName: <param_description>
  *  int rows: <param_description>
  *  int columns: <param_description>
  *
  * Exceptions:
  *  invalid_argument("file name too long"): <exception_description>
  *
  * R�ckgabeparameter:
  *  -
  */
CSVWriter::CSVWriter(char* fileName, int rows, int columns) {
	int fileNameLength = strlen(fileName);

	if (fileNameLength > MAX_FILE_NAME_LENGTH) {
		throw std::invalid_argument("file name too long");
	}

	this->fileName = new char[fileNameLength + 1];
	strcpy(this->fileName, fileName);

	this->rows = rows;
	this->columns = columns;

	this->data = new double* [this->rows];
	this->changed = new bool* [this->rows];
	for (int i = 0; i < this->rows; i++) {
		this->data[i] = new double[this->columns];
		this->changed[i] = new bool[this->columns];
	}

	// initialize changed table
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->columns; j++) {
			this->changed[i][j] = false;
		}
	}
}

/*
 * ~CSVWriter()
 *  <function_description>
 *
 * Parameterliste:
 *  -
 *
 * R�ckgabeparameter:
 *  -
 */
CSVWriter::~CSVWriter() {
	for (int i = 0; i < this->rows; i++) {
		delete[] this->data[i];
		delete[] this->changed[i];
	}

	delete[] this->data;
	delete[] this->changed;
	delete[] this->fileName;
}

/*
 * void load()
 *  <function_description>
 *
 * Parameterliste:
 *  -
 *
 * R�ckgabeparameter:
 *  -
 */
void CSVWriter::load() {
	std::fstream file;
	file.open(this->fileName, std::ios::in);

	/*
	while (!file.eof())
	{
		// read line for line from file
	}
	*/

	file.close();
}

/*
 * void save()
 *  <function_description>
 *
 * Parameterliste:
 *  -
 *
 * R�ckgabeparameter:
 *  -
 */
void CSVWriter::save() {
	std::fstream file;
	file.open(this->fileName, std::ios::app);
	file.imbue(std::locale(file.getloc(), new comma_separator));

	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->columns; j++) {
			if (this->changed[i][j]) {
				// write data to file
				file << data[i][j];
			}

			// write semicolon to file
			file << ";";
		}

		// write new line to file
		file << std::endl;
	}

	file.close();
}

/*
 * double get(int row, int column)
 *  <function_description>
 *
 * Parameterliste:
 *  int row: <param_description>
 *  int column: <param_description>
 *
 * Exceptions:
 *  invalid_argument("invalid row"): <exception_description>
 *  invalid_argument("invalid column"): <exception_description>
 *
 * R�ckgabeparameter:
 *  double: <param_description>
 */
double CSVWriter::get(int row, int column) {
	if (row < 0 || row >= this->rows) {
		throw std::invalid_argument("invalid row");
	}

	if (column < 0 || column >= this->columns) {
		throw std::invalid_argument("invalid column");
	}

	if (!this->changed[row][column]) {
		throw std::invalid_argument("invalid entry");
	}

	return this->data[row][column];
}

/*
 * void set(int row, int column, double data)
 *  <function_description>
 *
 * Parameterliste:
 *  int row: <param_description>
 *  int column: <param_description>
 *  double data: <param_description>
 *
 * Exceptions:
 *  invalid_argument("invalid row"): <exception_description>
 *  invalid_argument("invalid column"): <exception_description>
 *
 * R�ckgabeparameter:
 *  -
 */
void CSVWriter::set(int row, int column, double data) {
	if (row < 0 || row >= this->rows) {
		throw std::invalid_argument("invalid row");
	}

	if (column < 0 || column >= this->columns) {
		throw std::invalid_argument("invalid column");
	}

	this->changed[row][column] = true;
	this->data[row][column] = data;
}