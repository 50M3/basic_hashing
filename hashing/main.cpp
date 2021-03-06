//  ***********************************************************************************************************
//  Name:       main.cpp
//  Author:     Florian Liehr
//  Datum:      07.05.2019
//  Uhrzeit:    21:14
//  Beschreib.: Implementiert wichtige Funktionen für Hashing
//  ***********************************************************************************************************
#include <iostream>
#include <iomanip>
#include <cmath>
#include "CSWriter.hpp"

#define MAX_HASH 50
#define TABLE_SIZE 100
#define FREI 0
#define BELEGT -1
#define ENTFERNT -2


int insert_key(int data);
int insert_key_quad(int data);
int search_key(int data);
int search_key_quad(int data);
int biggestClusterSize();
int clusterAmount();
void delete_key(int data);
void delete_key_quad(int data);
void print_hashtable();

int getHash(int data);

using namespace std;


/* 
 * typedef struct anzahl
 *  Erstellt eine Struktur anzahl
 *  welche die erfolglosen und erfolgreichen Suchen speichert!
 *
 * */
typedef struct anzahl {
	int64_t erfolg = 0;
	int64_t erfolglos = 0;
}anzahl;

int hashtable[TABLE_SIZE];
anzahl zaehler;


int main() {
	for (int i = 0; i < TABLE_SIZE; i++) {
		hashtable[i] = FREI;
	}

	int* randomValues;
	randomValues = new int[MAX_HASH];

	try {
		while (true) {

			if (system("CLS")) system("clear");

			cout << "************************************" << endl;
			//print_hashtable();
			cout << "0 -> Element in Hashtabelle einfuegen" << endl;
			cout << "1 -> Element in Hashtabelle suchen" << endl;
			cout << "2 -> Element in Hashtabelle QUAD einfuegen" << endl;
			cout << "4 -> Testroutine Quad ausfuehren!" << endl;
			cout << "5 -> Testroutine ausfuehren!" << endl;
			cout << "3 -> Programm schliessen" << endl;
			cout << "************************************" << endl;
			cout << "Tablesize: " << TABLE_SIZE << endl;
			cout << "Anzahl der Schluessel " << MAX_HASH << endl;
			cout << "Anzahl der erfolgreichen Suchen: " << zaehler.erfolg << endl;
			cout << "Anzahl der erfolglosen Suchen: " << zaehler.erfolglos << endl;
			cout << "Durchschnittliche Vergleiche erfolgreich: " << (double)zaehler.erfolg / MAX_HASH << endl;
			cout << "Durchschnittliche Vergleiche erfolglos: " << (double)zaehler.erfolglos / TABLE_SIZE << endl;
			double alpha = (double)MAX_HASH / (double)TABLE_SIZE;
			cout << "Alpha: " << alpha << endl;
			
			//cout << "Lin: Anzahl der Cluster: " << clusterAmount() << endl;
			//cout << "Lin: Groestes Cluster: " << biggestClusterSize() << endl;

			int eingabe = 0;
			int select = 0;

			cin >> select;

			switch (select) {

			case 0:
				cout << "-Element in Hashtabelle einfuegen" << endl;
				cin >> eingabe;
				insert_key(eingabe);
				break;

			case 1:
				cout << "-Element in Hashtabelle suchen" << endl;
				cin >> eingabe;
				search_key(eingabe);
				break;

			case 2:
				cout << "-Element in Hashtabelle QUAD einfuegen" << endl;
				cin >> eingabe;
				insert_key_quad(eingabe);
				break;
			case 3:
				cout << "-Programm schliessen" << endl;
				exit(EXIT_SUCCESS);

			case 4:
				cout << "-Testroutine für Quad wird ausgefuehrt!" << endl;
				

				for (int i = 0; i < MAX_HASH; i++) {
					randomValues[i] = (rand() << 15) + rand();
					insert_key_quad(randomValues[i]);
				}

				for (int j = 0; j < MAX_HASH; j++) {
					zaehler.erfolg += search_key_quad(randomValues[j]);
				}

				for (int j = 0; j < TABLE_SIZE; j++) {
					zaehler.erfolglos += search_key_quad(j * -1);
				}

				for (int j = 0; j < MAX_HASH; j++) {
					delete_key_quad(randomValues[j]);
				}
				break;

			case 5:

				srand(time(nullptr));

				cout << "-Testroutine wird ausgefuehrt!" << endl;

				for (int i = 0; i < MAX_HASH; i++) {
					randomValues[i] = (rand() << 15) + rand();
					insert_key(randomValues[i]);
					
				}

				for (int j = 0; j < MAX_HASH; j++) {
					zaehler.erfolg += search_key(randomValues[j]);
				}

				for (int j = 0; j < TABLE_SIZE; j++) {
					zaehler.erfolglos += search_key(j * -1);
				}

				/*for (int j = 0; j < MAX_HASH; j++) {
					delete_key(randomValues[j]);
				}*/
				
				char file[16] = "pdf.csv";
				CSVWriter writer(file, 1, 3);
				writer.set(0, 0, alpha);
				writer.set(0, 1, (double)zaehler.erfolg / MAX_HASH);
				writer.set(0, 2, (double)zaehler.erfolglos / TABLE_SIZE);
				writer.save();
				cout << "Datei erfolgreich geschrieben!" << endl;

				break;
			}

		}
	}
	catch (const char* msg) {
		cerr << msg << endl;
	}


}

/*
 * int insert_key(int data)
 *  Fuegt ein Element nach linearem Sondieren in die Liste ein!
 *
 * Parameterliste:
 *  int data: Der Key der eingefügt werden soll.
 *
 * Rückgabeparameter:
 *  @hash = Der gehashte Key
 *
 * */
int insert_key(int data) {
	int hash = getHash(data);

	// BUCKET LEER, FÜGE EIN
	if (hashtable[hash] == FREI) {
		hashtable[hash] = data;
	}

	else {

		// SUCHE LEEREN BUCKET
		int i = 1;

		// OVERFLOW FIX
		if (hash + i >= TABLE_SIZE)
			i = hash * -1;
		while (hashtable[hash + i] != FREI && i != 0) {
			i++;

			// OVERFLOW FIX
			if (hash + i >= TABLE_SIZE)
				i = hash * -1;
		}

		// BUCKET IST LEER, FÜGE EIN
		if (hashtable[hash + i] == FREI) {
			hashtable[hash + i] = data;
		}

		// ALLE SLOTS SIND VOLL!
		else {
			throw "Hashtabelle ist voll! [insert_key]";
		}
	}
	return hash;
}

/*
 * int insert_key_quad(int data)
 *  Fuegt ein Element nach quadratischem Sondieren in die Liste ein!
 *
 * Parameterliste:
 *  int data: Der Key der eingefügt werden soll.
 *
 * Rückgabeparameter:
 *  @hash = Der gehashte Key
 *
 * */

// VERALTET! Muss neugeschrieben werden.

int insert_key_quad(int data) {
	int hash = getHash(data);

	// SLOT FREI, FÜGE EIN
	if (hashtable[hash] == FREI) {
		hashtable[hash] = data;
	}

	else {
		int i = 2;

		// GENERIERE EINEN NEUEN BUCKET
		int new_position = getHash(hash + pow(i / 2, 2) * pow(-1, i));

		// ÜBERPRÜFE OB NEUER BUCKET BELEGT
		while (hashtable[new_position] != FREI && i < TABLE_SIZE) {
			// NEUEN BUCKET SUCHEN
			i++;
			new_position = getHash(hash + pow(i / 2, 2) * pow(-1, i));
		}
		
		// NEUER BUCKET IST FREI, FÜGE EIN
		if (hashtable[new_position] == FREI) {
			hashtable[new_position] = data;
		}
		// NEUER BUCKET IST FREI, WAR ABER VORHER GEFÜLLT
		else if(hashtable[new_position] == ENTFERNT){
			hashtable[new_position] = data;
		}

		// INSERT AN EINER STELLE DIE ES NICHT GIBT
		else if (i >= TABLE_SIZE) {
			throw "Insert nicht in Reichweite [insert_key_quad]";
		}
		// HASHTABELLE IST VOLL!
		else {
			throw "Hashtabelle ist voll! [insert_key_quad]";
		}
		return hash;

	}
}
/*
 * int search_key(int data)
 *  Sucht einen Key in der Liste und vergleich in jedem Behälter die Schlüssel!
 *
 * Parameterliste:
 *  int data: Der Key der eingefügt werden soll.
 *
 * Rückgabeparameter:
 *  @count = RÜCKGABE DER GEZÄHLTEN VERGLEICHE
 *
 * */

int search_key(int data) {
	int i = 1, hash = getHash(data), count = 1;


	// BUCKET FREI
	if (hashtable[hash] == FREI) {
		return 0;
	}

	// BUCKET INHALT = GESUCHTER DATA
	if (hashtable[hash] == data) {
		return count;
	}

	// PASSENDEN BUCKET SUCHEN
	else {
		count++;

		// OVERFLOW FIX
		if (hash + i >= TABLE_SIZE) {
			i = hash * -1;

		}

		// SOLANGE SUCHEN BIS BUCKET DATA = GESUCHTER DATA
		while (hashtable[hash + i] != data && i != 0 && hashtable[hash + i] != FREI) {
			i++;
			count++;

			// OVERFLOW FIX
			if (hash + i >= TABLE_SIZE) {
				i = hash * -1;

			}
		}
		return count;
	}
}

// MUSS NOCH GETESTET WERDEN!

int search_key_quad(int data) {
	int count = 1, hash = getHash(data), i = 1;

	if (hashtable[hash] == FREI) {
		return 0;
	}

	if (hashtable[hash] == data) {
		return count;
	}
	else {
		count++;
		int new_position = getHash(hash + pow(i / 2, 2) * pow(-1, i));
		while (hashtable[new_position] != data && i < TABLE_SIZE && hashtable[new_position] != FREI) {
			i++;
			count++;
			new_position = getHash(hash + pow(i / 2, 2) * pow(-1, i));
		}

		if (hashtable[new_position] == data) {
			return count;
		}
		else {
			return count;
		}
	}
}


/*
 * void delete_key(int data)
 *  Sucht und löscht das Element data
 *
 * Parameterliste:
 *  int data: Die zu suchende und löschende Data
 *
 * Rückgabeparameter:
 *  void
 *
 * */
void delete_key(int data) {
	int hash = getHash(data);

	// BUCKET DATA GEFUNDEN, LÖSCHE DATA, SETZE STATE
	if (hashtable[hash] == data) {
		hashtable[hash] = ENTFERNT;
	}

	else {
		// BUCKET DATA NICHT GEFUNDEN, SUCHE DATA
		int i = 1;
		while (hashtable[hash+i] != data && i != 0) {
			i++;

			// OVERFLOW FIX
			if (hash + i >= TABLE_SIZE) {
				i = hash * -1;
			}
		}

		// DATA GEFUNDEN, LÖSCHE DATA, SETZE STATE
		if (hashtable[hash + i] == data) {
			hashtable[hash + i] = ENTFERNT;
		}
	}
}

void delete_key_quad(int data) {
	int hash = getHash(data);

	if (hashtable[hash] == data) {
		hashtable[hash] = ENTFERNT;
	}

	else {
		int i = 2;
		int new_position = getHash(hash + pow(i / 2, 2) * pow(-1, i));
		while (hashtable[new_position] != data && i != 0) {
			i++;
			new_position = getHash(hash + pow(i / 2, 2) * pow(-1, i));
		}
		if (hashtable[new_position] == data) {
			hashtable[new_position] = ENTFERNT;
		}
	}
}
	

/*
 * int biggestClusterSize()
 *  Wie groß das größte Cluster ist!
 *
 * Rückgabeparameter:
 *  @ count > max = return count
 *  @ max = return max;
 *
 * */
int biggestClusterSize() {
	int count = 0, max = 0;

	for (int i = 0; i < TABLE_SIZE; i++) {
		if (hashtable[i] != FREI && hashtable[i] != ENTFERNT) {
			count++;
		}
		else {
			if (count > max) {
				max = count;
			}

			count = 0;
		}
	}
	return (count > max) ? count : max;
}

/*
 * int clusterAmount()
 *  Zählt die Cluster in der Hashtabelle!
 *
 * Rückgabeparameter:
 *  @count = Anzahl der Cluster!
 *
 * */
int clusterAmount() {
	int count = 0;
	bool found = false;

	for (int i = 0; i < TABLE_SIZE; i++) {
		if (hashtable[i] != FREI && hashtable[i] != ENTFERNT) {
			if (!found) {
				found = true;
				count++;
			}
			else {
				found = false;
			}
		}
	}
	return count;
}


/*
 * int getHash(int data)
 *  Gehashter Key mit Modulo von Wikipedia für -a mod m!
 *
 * Parameterliste:
 *  int data: Der Key der gehasht wird.
 *
 * Rückgabeparameter:
 *  @calc Der gehashte Schlüssel 
 *
 * */
int getHash(int data) {
	int calc = data % TABLE_SIZE;
	if (calc < 0)
		calc += TABLE_SIZE;
	return calc;
}


void print_hashtable() {
	cout << "------------------------------------" << endl;
	for (int i = 0; i < TABLE_SIZE; i++) {
		cout << setw(3) << i << " Memory " << hashtable[i]
			<< endl;
	}
	cout << "------------------------------------" << endl;
}
