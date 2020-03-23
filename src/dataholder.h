//Definicja typu DataHolder
// służy przechowywaniu elementów postaci ciągu znaków

struct DataEntry;

typedef struct DataEntry *DataHolder;

void dataHolderCreate(DataHolder *dataHolderPtr, char *name);

void dataHolderDestroy(DataHolder dataHolder);

//Jeśli istnieje już dataHolder o nazwie [entryName]
//to go zwraca, a jeśli nie to tworzy nowy i go zwraca

DataHolder dataHolderAddEntry(DataHolder dataHolder, char *entryName);

void dataHolderRemoveEntry(DataHolder dataHolder, char *entryName);

void dataHolderRemoveAllEntries(DataHolder dataHolder);

//Zwraca DataHolder o nazwie [entryName] albo NULL jeśli takiego nie ma
DataHolder dataHolderFindEntry(DataHolder dataHolder, char *entryName);

//powoduje, że [out] wskazuje na tablicę ze wszystkimi wpisami w [dataHolder]
//zwracana wartość to wielkość tej tablicy
//po użyciu tej funkcji trzeba zdealokować [out]

unsigned int dataHolderGetAllEntries
        (DataHolder dataHolder, DataHolder **outPtr);

//zwraca wszystkie nazwy wpisów w [dataHolder] w porządku rosnącym
char *dataHolderGetAllEntryNamesOrdered(DataHolder dataHolder);

void dataHolderPrintAllEntriesOrdered(DataHolder dataHolder);