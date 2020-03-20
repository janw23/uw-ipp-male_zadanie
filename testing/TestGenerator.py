# generuje parę plików test#<numer>.in oraz test#<numer>.out
# z wejściem do programu i docelowym wyjściem

# generowane nazwy mają jedynie 2 znaki. Jest ich tak mało, że nie trzeba
# specjalnie generowac przypadków z istniejącymi już nazwami elementów,
# bo takie trafią się same przy odpowiednio dużej liczbie wygenerowanych przypadków

from random import randint
import random
import string
import pathlib

# ścieżka do folderu, gdzie zapisane zostaną wygenerowane pliki
saveResultDir = "{0}/generatedTests/".format(pathlib.Path(__file__).parent.absolute())

# numer, który zostanie umieszczony w nazwie wygenerowanych plików
testID = randint(0, 2 ** 10)

# tworzenie i otwieranie plików, w których zapisywane będa wygenerowane testy
generatedInputTargetFile = open(saveResultDir + "test#" + str(testID) + ".in", "w")
generatedOutputTargetFile = open(saveResultDir + "test#" + str(testID) + ".out", "w")

# główny słownik; (przechowuje poziom lasu. Kolejne poziomy to drzewa i zwierzęta
# nazwa elementu na danym poziomie to klucz, a wartość to słownik z kolejnym poziomem
dataHolder = {}

# znane znaki białe
whitespaces = " "  # " \t\v\f\r"

#nieprawdiłowe znaki (które nie powinny być przyjmowane przez program)
wrongCharacters = ""
#for ch in range(30, 33):
#    if ch != 10:    #pomijanie \n
#        wrongCharacters += chr(ch)


# źródło https://stackoverflow.com/questions/2257441/random-string-generation-with-upper-case-letters-and-digits
def string_gen(size=2, chars=string.ascii_lowercase + string.ascii_uppercase + string.digits + "#"):
    return ''.join(random.choice(chars) for _ in range(size))


def printDebug(msg):
    print(msg)
    #pass


def addWhitespacesToCommand(cmd):
    for i in range(len(cmd)):
        cmd[i] = string_gen(chars=whitespaces) + cmd[i] + string_gen(chars=whitespaces)


# zapisywanie komendy w pliku z poleceniami wejściowymi
def writeCommandToInput(cmd):
    addWhitespacesToCommand(cmd)

    for name in cmd:
        generatedInputTargetFile.write(name + " ")

    generatedInputTargetFile.write('\n')


def writeResultToOutput(result):
    generatedOutputTargetFile.write(result)

def genCommandArgs(cmd, argsCount):
    for i in range(argsCount):
        cmd.append(string_gen())

def genWrongCommandArgs(cmd, argsCount):
    for i in range(argsCount):
        cmd.append(string_gen(
            chars=string.ascii_lowercase + string.ascii_uppercase + string.digits + "#" + wrongCharacters))

def genTestCmdAddCorrect():
    printDebug("Generating CORRECT ADD command test")

    cmd = ["ADD"]

    argsCount = randint(1, 3)
    genCommandArgs(cmd, argsCount)

    currentHolder = dataHolder

    # tworzenie elementów w dataHolderze, których jeszcze nie ma i wchodzenie głębiej
    for i in range(argsCount):
        arg = cmd[i + 1]

        if arg not in currentHolder:
            currentHolder[arg] = {}

        currentHolder = currentHolder[arg]

    writeCommandToInput(cmd)
    writeResultToOutput("OK\n")


def genTestCmdAddWrong():
    printDebug("Generating WRONG ADD command test")

    cmd = ["ADD"]

    # losowanie, czy błąd ma polegać na zbyt małej,
    # czy zbyt dużej liczbie argumentów
    argsCount = 0
    if randint(0, 1) == 1:
        argsCount = randint(4, 100)

    genWrongCommandArgs(cmd, argsCount)

    writeCommandToInput(cmd)
    writeResultToOutput("ERROR\n")


def genTestCmdDelCorrect():
    printDebug("Generating CORRECT DEL command test")

    global dataHolder

    cmd = ["DEL"]

    argsCount = randint(0, 3)
    genCommandArgs(cmd, argsCount)

    currentHolder = dataHolder

    if argsCount == 0:
        currentHolder.clear()
    else:
        for name in cmd[1:-1]:
            if name in currentHolder:
                currentHolder = currentHolder[name]
                if name == cmd[-1]:
                    currentHolder.clear()
            else:
                break

    writeCommandToInput(cmd)
    writeResultToOutput("OK\n")


def genTestCmdDelWrong():
    printDebug("Generating WRONG DEL command test")

    global dataHolder

    cmd = ["DEL"]

    argsCount = randint(4, 100)
    genWrongCommandArgs(cmd, argsCount)

    writeCommandToInput(cmd)
    writeResultToOutput("ERROR\n")


def genTestCmdPrintCorrect():
    pass


def genTestCmdPrintWrong():
    pass


def genTestCmdCheckCorrect():
    printDebug("Generating CORRECT CHECK command test")

    cmd = ["CHECK"]

    argsCount = randint(1, 3)
    genCommandArgs(cmd, argsCount)

    currentHolder = dataHolder

    for name in cmd[1:-1]: #może powodować błað bo ostatni wpis może być taki, jak pierwszy; w DEL to samo
        if name in currentHolder:
            currentHolder = currentHolder[name]
            if name

    writeCommandToInput(cmd)
    writeResultToOutput("OK\n")


def genTestCmdCheckWrong():
    pass


def genTestWhitespaceOnly():
    pass


def genTestLineToIgnore():
    pass


def genTestWrongCmdName():
    pass


def genNextTest():
    genTestOption = randint(0, 1)  # test którego z dostępnych poleceń ma zostać wygenerowany
    genTestCorrect = randint(0, 1)  # czy ma zostać wygenerowana poprawna, czy błędna wersja testu

    if genTestOption == 0:
        if genTestCorrect == 1:
            genTestCmdAddCorrect()
        else:
            genTestCmdAddWrong()

    elif genTestOption == 1:
        if genTestCorrect == 1:
            genTestCmdDelCorrect()
        else:
            genTestCmdDelWrong()

    elif genTestOption == 2:
        if genTestCorrect == 1:
            genTestCmdPrintCorrect()
        else:
            genTestCmdPrintWrong()

    elif genTestOption == 3:
        if genTestCorrect == 1:
            genTestCmdCheckCorrect()
        else:
            genTestCmdCheckWrong()

    elif genTestOption == 4:
        genTestWhitespaceOnly()

    elif genTestOption == 5:
        genTestLineToIgnore()

    elif genTestOption == 6:
        genTestWrongCmdName()

printDebug("Generating test package number #" + str(testID))

generateTestsCount = 2000#randint(1, 20000)  # 2 ** 10)

for testNumber in range(generateTestsCount):
    genNextTest()

# ostatnie znak nowej linii powinien zakończyć program
generatedInputTargetFile.write("\n")

generatedInputTargetFile.close()
generatedOutputTargetFile.close()
