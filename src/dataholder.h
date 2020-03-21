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

//zwraca wszystkie nazwy wpisów w [dataHolder] w porządku rosnącym
char *dataHolderGetAllEntryNamesOrdered(DataHolder dataHolder);

void dataHolderPrintAllEntriesOrdered(DataHolder dataHolder);