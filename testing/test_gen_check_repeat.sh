./compile.sh

for ((i=0; i<"$1"; i+=1)); do
  python3 testing/TestGenerator.py
  ./testing/test.sh bin/forests testing/generatedTests/ "$2"
  rm testing/generatedTests/*.in
  rm testing/generatedTests/*.out
  rm testing/generatedTests/*.pout
done