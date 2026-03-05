gcc delete.c insert.c misc.c open.c query.c save.c showall.c showsummary.c snapshot.c sort.c update.c main.c -o cms.exe
gcc delete.c insert.c misc.c open.c query.c save.c showall.c showsummary.c snapshot.c sort.c update.c test_cms.c -o test_cms.exe
./test_cms.exe
bash run_integration_tests.sh

./run_tests.ps1
