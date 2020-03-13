//Definicja typu DataHolder
// służy przechowywaniu elementów postaci ciągu znaków

#define DATAHOLDER_ERR_SUCCESS 0
#define DATAHOLDER_ERR_FAILURE 1
#define DATAHOLDER_ERR_PTR_NOT_NULL 2
#define DATAHOLDER_ERR_PTR_IS_NULL 3
#define DATAHOLDER_ERR_MEMORY_FULL 4

struct DataEntry;
typedef struct DataEntry* DataHolder;

typedef int DataHolderError;

DataHolderError dataHolderCreate(DataHolder *dataHolderPtr);

DataHolderError dataHolderDestroy(DataHolder dataHolder);

DataHolderError dataHolderAddEntry(DataHolder dataHolder, char *entryName);

DataHolderError dataHolderRemoveEntry(DataHolder dataHolder, char *entryName);

DataHolderError dataHolderFindEntry(DataHolder dataHolder,
                                    char* entryName,
                                    DataHolder out);

int getLength(DataHolder dataHolder);


