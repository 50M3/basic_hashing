#include <iostream>

#define MAX_HASH 50
#define TABLE_SIZE 100
#define FREI 0
#define BELEGT -1
#define ENTFERNT -2


/*
 * typedef struct bucket
 *  Erstellt die Struktur bucket
 *	welche aus der Data und dem State besteht
 *
 * */
typedef struct bucket {
	int data;
	int state; // 0 = frei, -1 = entfernt, 1 = belegt
}bucket;

typedef struct anzahl {
	int64_t erfolg = 0;
	int64_t erfolglos = 0;
	int vergleiche = 0;
}anzahl;

bucket* hashtable[TABLE_SIZE];
bucket* hashtable_quad[TABLE_SIZE];
anzahl zaehler;

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

	// Slot ist leer, füg ein Element ein
	if (hashtable[hash]->state == FREI) {
		hashtable[hash]->state = BELEGT;
		hashtable[hash]->data = data;
	}

	else {

		// Slot ist nicht leer, such nach freiem Slot
		int i = 1;
		if (hash + i >= TABLE_SIZE)
			i = hash * -1;
		while (hashtable[hash + i]->state != FREI && i != 0) {
			i++;
			if (hash + i >= TABLE_SIZE)
				i = hash * -1;
		}

		// Slot ist leer, füg ein Element ein
		if (hashtable[hash + i]->state == FREI) {
			hashtable[hash + i]->state = BELEGT;
			hashtable[hash + i]->data = data;
		}

		// Alle Slots sind voll, overflow!
		else {
			throw "Invalid Argument!";
		}
	}
	return hash;
}

