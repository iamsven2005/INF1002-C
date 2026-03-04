#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "headers.h"

#define TOTAL_TESTS 20
#define TEST_DB "test_cms.txt"

int passed_tests = 0;
int failed_tests = 0;

void print_result(int test_num, const char *test_name, int passed) {
    if (passed) {
        printf("PASS Test %d: %s\n", test_num, test_name);
        passed_tests++;
    } else {
        printf("FAIL Test %d: %s\n", test_num, test_name);
        failed_tests++;
    }
}

void create_test_database() {
    FILE *file = fopen(TEST_DB, "w");
    fprintf(file, "2301234,Joshua Chen,Software Engineering,70.5\n");
    fprintf(file, "2201234,Isaac Teo,Computer Science,63.4\n");
    fprintf(file, "2304567,John Levoy,Digital Supply Chain,85.9\n");
    fclose(file);
}

void cleanup_test_database() {
    remove(TEST_DB);
}

int test_open_file() {
    struct Record *records = NULL;
    int result = execute_on_file(&records, TEST_DB);
    
    int passed = (result == 3 && records != NULL && 
                  records[0].id == 2301234 &&
                  strcmp(records[0].name, "Joshua Chen") == 0);
    
    print_result(1, "Open file and read 3 records", passed);
    free(records);
    return passed;
}

int test_find_existing_record() {
    struct Record records[3];
    records[0].id = 2301234;
    records[1].id = 2201234;
    records[2].id = 2304567;
    
    int index = find_record_index(records, 3, 2301234);
    int passed = (index == 0);
    
    print_result(2, "Find existing record by ID", passed);
    return passed;
}

int test_find_nonexistent_record() {
    struct Record records[3];
    records[0].id = 2301234;
    records[1].id = 2201234;
    records[2].id = 2304567;
    
    int index = find_record_index(records, 3, 2501234);
    int passed = (index == -1);
    
    print_result(3, "Find nonexistent record returns -1", passed);
    return passed;
}

int test_validate_id_valid() {
    int id = 0;
    int passed = isValidID("2301234", &id) && id == 2301234;
    
    print_result(4, "Validate valid ID string", passed);
    return passed;
}

int test_validate_id_invalid() {
    int id = 0;
    int passed = !isValidID("abc123", &id);
    
    print_result(5, "Reject invalid ID string", passed);
    return passed;
}

int test_validate_marks_valid() {
    float mark = 0;
    int passed = isValidMarks("75.5", &mark) && mark == 75.5f;
    
    print_result(6, "Validate valid mark value", passed);
    return passed;
}

int test_validate_marks_invalid() {
    float mark = 0;
    int passed = !isValidMarks("invalid", &mark);
    
    print_result(7, "Reject invalid mark value", passed);
    return passed;
}

int test_validate_marks_range() {
    float mark = 0;
    int passed = isValidMarks("150", &mark) && mark == 150;
    
    print_result(8, "Parse mark value correctly", passed);
    return passed;
}

int test_validate_name_valid() {
    int passed = isValidName("John Smith") == 1;
    
    print_result(9, "Validate valid name", passed);
    return passed;
}

int test_validate_name_invalid() {
    int passed = isValidName("John123") == 0;
    
    print_result(10, "Reject name with numbers", passed);
    return passed;
}

int test_strcasecmp_equal() {
    int result = strcasecmp_ci("JOHN", "john");
    int passed = (result == 0);
    
    print_result(11, "Case-insensitive string comparison (equal)", passed);
    return passed;
}

int test_strcasecmp_different() {
    int result = strcasecmp_ci("JOHN", "JANE");
    int passed = (result != 0);
    
    print_result(12, "Case-insensitive string comparison (different)", passed);
    return passed;
}

int test_find_id_value() {
    const char *input = "ID=2301234";
    char *value = find_id_value(input);
    int passed = (value != NULL && strncmp(value, "2301234", 7) == 0);
    
    print_result(13, "Extract ID value from input", passed);
    return passed;
}

int test_find_id_value_lowercase() {
    const char *input = "id=2401234";
    char *value = find_id_value(input);
    int passed = (value != NULL && strncmp(value, "2401234", 7) == 0);
    
    print_result(14, "Extract ID value (lowercase)", passed);
    return passed;
}

int test_find_id_value_missing() {
    const char *input = "NAME=John";
    char *value = find_id_value(input);
    int passed = (value == NULL);
    
    print_result(15, "Handle missing ID value", passed);
    return passed;
}

int test_save_file() {
    struct Record records[2];
    records[0].id = 2301234;
    strcpy(records[0].name, "Joshua Chen");
    strcpy(records[0].prog, "Software Engineering");
    records[0].marks = 70.5f;
    
    records[1].id = 2201234;
    strcpy(records[1].name, "Isaac Teo");
    strcpy(records[1].prog, "Computer Science");
    records[1].marks = 63.4f;
    
    int result = save(records, 2, "test_save.txt");
    int passed = (result == 0);
    
    FILE *file = fopen("test_save.txt", "r");
    passed = passed && (file != NULL);
    if (file) fclose(file);
    remove("test_save.txt");
    
    print_result(16, "Save records to file", passed);
    return passed;
}

int test_empty_records() {
    struct Record records[1];
    int index = find_record_index(records, 0, 2301234);
    int passed = (index == -1);
    
    print_result(17, "Handle empty record list", passed);
    return passed;
}

int test_duplicate_id() {
    struct Record records[2];
    records[0].id = 2301234;
    records[1].id = 2301234;
    
    int index = find_record_index(records, 2, 2301234);
    int passed = (index == 0);
    
    print_result(18, "Find first occurrence of duplicate ID", passed);
    return passed;
}

int test_boundary_id() {
    int id = 0;
    int passed = isValidID("0", &id) && id == 0;
    
    print_result(19, "Validate boundary ID (0)", passed);
    return passed;
}

int test_large_id() {
    int id = 0;
    int passed = isValidID("9999999", &id) && id == 9999999;
    
    print_result(20, "Validate large ID", passed);
    return passed;
}

int main() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║          CMS Unit Test Suite - Testing Framework             ║\n");
    printf("║                   Group9 Database System                      ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Setting up test database...\n");
    create_test_database();
    printf("\n");
    
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("Running Unit Tests...\n");
    printf("═══════════════════════════════════════════════════════════════\n\n");
    
    test_open_file();
    test_find_existing_record();
    test_find_nonexistent_record();
    test_validate_id_valid();
    test_validate_id_invalid();
    test_validate_marks_valid();
    test_validate_marks_invalid();
    test_validate_marks_range();
    test_validate_name_valid();
    test_validate_name_invalid();
    test_strcasecmp_equal();
    test_strcasecmp_different();
    test_find_id_value();
    test_find_id_value_lowercase();
    test_find_id_value_missing();
    test_save_file();
    test_empty_records();
    test_duplicate_id();
    test_boundary_id();
    test_large_id();
    
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("Test Summary:\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("Total Tests: %d\n", TOTAL_TESTS);
    printf("Passed:      %d\n", passed_tests);
    printf("Failed:      %d\n", failed_tests);
    printf("Pass Rate:   %.1f%%\n", (float)passed_tests / TOTAL_TESTS * 100);
    printf("═══════════════════════════════════════════════════════════════\n\n");
    
    if (failed_tests == 0) {
        printf("All tests passed. The CMS is working correctly.\n\n");
    } else {
        printf("%d test(s) failed. Please review the results above.\n\n", failed_tests);
    }
    
    printf("Cleaning up test database...\n");
    cleanup_test_database();
    
    return failed_tests > 0 ? 1 : 0;
}
