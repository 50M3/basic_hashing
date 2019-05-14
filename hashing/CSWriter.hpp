/*
 * Name:  CSVWriter.hpp
 * Autor: Frederik Plate, Patrick Schauer
 * Datum: 15.04.2019
 * Beschreibung:
 *  Diese Datei stellt eine Klasse zum Arbeiten mit CSV-Dateien bereit.
 */

#pragma once

#ifndef _CSV_WRITER_H_
#define _CSV_WRITER_H_

#include <locale>

#define MAX_FILE_NAME_LENGTH 16

class CSVWriter {

public:
	CSVWriter(char* fileName, int rows, int columns);
	~CSVWriter();

	void load();
	void save();

	double get(int row, int column);
	void set(int row, int column, double data);

private:
	char* fileName;
	int columns, rows;
	double** data;
	bool** changed;

	struct comma_separator : std::numpunct<char> {
		virtual char do_decimal_point() const override { return ','; }
	};
};

#endif