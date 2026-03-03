gcc *.c -o cms.exe
gcc *.c test_cms.c -o test_cms.exe
./test_cms.exe
bash run_integration_tests.sh

./run_tests.ps1
