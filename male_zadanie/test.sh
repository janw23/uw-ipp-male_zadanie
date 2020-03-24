#!/bin/bash

#skrypt do testowania programu [prog]
#przyjmowane argumenty: [prog] i [dir]

#pliki danych wejściowych (*.in) oraz
#prawidłowych danych wyjściowych (*.out, *.err)
#znajduje w folderze o ścieżce [dir]

prog=$1
dir=$2

#tablice z nazwami plików, które zakończyły
#się odpowiednio sukcesem albo porażką
testsPassed=()
testsFailed=()

for filename in "${dir}"/*.in; do
  [ -e "$filename" ] || continue

  targetOut="${filename%in}out"
  progOut="${filename%in}pout"
  targetErrOut="${filename%in}err"
  progErrOut="${filename%in}perr"
  testName=$(basename "${targetOut%.out}")

  echo "Running test: $testName"

  valgrind --log-file="${dir}"/"${testName}"-memcheck.log \
      --error-exitcode=15 --leak-check=full \
      --show-leak-kinds=all --errors-for-leak-kinds=all -s \
      ./"$prog" <"$filename" >"$progOut" 2>"$progErrOut"

  if [ "$(diff "$targetOut" "$progOut")" == "" ] && \
     [ "$(diff "$targetErrOut" "$progErrOut")" == "" ]; then
    testsPassed+=("$testName")
  else
    testsFailed+=("$testName")
  fi
done

echo ""

if [ ${#testsPassed[@]} == 0 ]; then
  echo "No tests passed :("
else
  echo "Tests passed:"
  for t in "${testsPassed[@]}"; do
    printf "\t%s\n" "$t"
  done
fi

echo ""

if [ ${#testsFailed[@]} == 0 ]; then
  echo "No tests failed :)"
else
  echo "Tests Failed:"
  for t in "${testsFailed[@]}"; do
    printf "\t%s\n" "$t"
  done
fi