#!/bin/bash

#skrypt do testowania programu [prog]

#przyjmowane argumenty: [prog] i [dir]

#pliki danych wejściowych (*.in) oraz
#prawidłowych danych wyjściowych (*.out)
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
  realOut="${filename%in}pout"
  testName=$(basename "${targetOut%.out}")

  echo ""
  echo "Running test: $testName"
  echo ""

  valgrind --error-exitcode=15 --leak-check=full \
    --show-leak-kinds=all --errors-for-leak-kinds=all -s \
    ./"$prog" <"$filename" >"$realOut"

  if [ "$(diff "$targetOut" "$realOut")" == "" ]; then
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