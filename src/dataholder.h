//Definicja typu DataHolder
// służy przechowywaniu elementów postaci ciągu znaków

struct DataEntry;

typedef struct DataEntry* DataHolder;

void dataHolderCreate(DataHolder *dataHolderPtr);

void dataHolderDestroy(DataHolder dataHolder);

//Zwraca nowy dodany DataHolder
DataHolder dataHolderAddEntry(DataHolder dataHolder, char *entryName);

void dataHolderRemoveEntry(DataHolder dataHolder, char *entryName);

void dataHolderRemoveAllEntries(DataHolder dataHolder);

//Zwraca DataHolder o nazwie [entryName] albo NULL jeśli takiego nie ma
DataHolder dataHolderFindEntry(DataHolder dataHolder, char* entryName);

DataHolder dataHolderFindOrAddEntry(DataHolder dataHolder, char *entryName);

void dataHolderPrintEntryName(DataHolder dataHolder);

void dataHolderPrintAllEntries(DataHolder dataHolder);