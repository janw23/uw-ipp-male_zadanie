//Definicja typu DataHolder
// służy przechowywaniu elementów postaci ciągu znaków


struct DataEntry;
typedef struct DataEntry* DataHolder;

void dataHolderCreate(DataHolder *dataHolderPtr);

void dataHolderDestroy(DataHolder dataHolder);

void dataHolderAddEntry(DataHolder dataHolder, char *entryName);

void dataHolderRemoveEntry(DataHolder dataHolder, char *entryName);

DataHolder dataHolderFindEntry(DataHolder dataHolder, char* entryName);

int getLength(DataHolder dataHolder);


