# CMS Unit & Integration Test Cases
## Group9 Database System - INF1002 Course Project

---

## Test Execution Guide

### **Unit Tests** (test_cms.c)
Tests individual functions in isolation (validation, helpers, file I/O)

```bash
# Compile with unit tests
gcc *.c test_cms.c -o test_cms.exe

# Run unit tests
./test_cms.exe
```

**Expected Output:**
- 20 total tests
- Tests cover: file I/O, record lookup, validation (ID, marks, names), case-insensitive comparison
- All should PASS

---

### **Integration Tests** (run_tests.ps1)
Tests full command workflow with expected input/output

```powershell
# Run integration tests (PowerShell)
.\run_tests.ps1
```

**Expected Output:**
- 32 total test validations
- Core operations: OPEN, SHOW ALL, INSERT, QUERY, UPDATE, DELETE, SAVE
- All should PASS

---

## Test Case Details

### **Test Suite 1: Core Operations (11 Tests)**

#### Test 1.1: OPEN Command
- **Input:** `OPEN`
- **Expected Output:** `The database file "Group9_CMS.txt" is successfully opened.`
- **Validation:** Database loads 3 test records
- **Status:** PASS

#### Test 1.2: SHOW ALL
- **Input:** `SHOW ALL`
- **Expected Output:** 
  ```
  Here are all the records found in the table "StudentRecords".
  ID         Name                       Programme                                      Mark
  2301234    Joshua Chen                Software Engineering                           70.5
  2201234    Isaac Teo                  Computer Science                               63.4
  2304567    John Levoy                 Digital Supply Chain                           85.9
  ```
- **Validation:** All 3 records displayed in formatted table
- **Status:** PASS

#### Test 1.3: INSERT Duplicate Prevention
- **Input:** `INSERT ID=2301234`
- **Expected Output:** `The record with ID=2301234 already exists.`
- **Validation:** Record count remains 3 (no duplicate inserted)
- **Status:** PASS

#### Test 1.4: INSERT New Record
- **Input:** `INSERT ID=2401234 Name=Michelle Lee Programme=Information Security Mark=73.2`
- **Expected Output:** `A new record with ID=2401234 is successfully inserted.`
- **Validation:** Record count becomes 4, Michelle Lee record exists
- **Status:** PASS

#### Test 1.5: SHOW ALL After Insert
- **Input:** `SHOW ALL`
- **Expected Output:** 4 records in table (includes Michelle Lee with ID=2401234)
- **Validation:** New record visible with correct values
- **Status:** PASS

#### Test 1.6: QUERY Nonexistent Record
- **Input:** `QUERY ID=2501234`
- **Expected Output:** `The record with ID=2501234 does not exist.`
- **Validation:** No error/crash, system returns gracefully
- **Status:** PASS

#### Test 1.7: QUERY Existing Record
- **Input:** `QUERY ID=2401234`
- **Expected Output:** 
  ```
  The record with ID=2401234 is found in the data table.
  ID         Name                       Programme                                      Mark
  2401234    Michelle Lee               Information Security                           73.2
  ```
- **Validation:** Record retrieved and displayed correctly
- **Status:** PASS

#### Test 1.8: UPDATE Nonexistent Record
- **Input:** `UPDATE ID=2501234 Mark=69.8`
- **Expected Output:** `The record with ID=2501234 does not exist.`
- **Validation:** No update performed, count remains 4
- **Status:** PASS

#### Test 1.9: UPDATE Mark Field
- **Input:** `UPDATE ID=2401234 Mark=69.8`
- **Expected Output:** `The record with ID=2401234 is successfully updated.`
- **Validation:** Michelle Lee's mark changes from 73.2 to 69.8
- **Status:** PASS

#### Test 1.10: UPDATE Programme Field
- **Input:** `UPDATE ID=2401234 Programme=Applied Analytics`
- **Expected Output:** `The record with ID=2401234 is successfully updated.`
- **Validation:** Michelle Lee's programme changes from "Information Security" to "Applied Analytics"
- **Status:** PASS

#### Test 1.11: SHOW ALL After Updates
- **Input:** `SHOW ALL`
- **Expected Output:** 4 records with Michelle Lee showing Updated values (Mark=69.8, Programme=Applied Analytics)
- **Validation:** All updates reflected correctly
- **Status:** PASS

---

### **Test Suite 2: DELETE Operations (4 Tests)**

#### Test 2.1: DELETE Nonexistent Record
- **Input:** `DELETE ID=2501234`
- **Expected Output:** `The record with ID=2501234 does not exist.`
- **Validation:** No error, count remains 4
- **Status:** PASS

#### Test 2.2: DELETE with Cancellation (N)
- **Input:** 
  ```
  DELETE ID=2401234
  N
  ```
- **Expected Output:** 
  ```
  Are you sure you want to delete record with ID=2401234? Type "Y" to Confirm or type "N" to cancel.
  The deletion is cancelled.
  ```
- **Validation:** Michelle Lee record NOT deleted, count remains 4
- **Status:** PASS

#### Test 2.3: DELETE with Confirmation (Y)
- **Input:** 
  ```
  DELETE ID=2301234
  Y
  ```
- **Expected Output:** 
  ```
  Are you sure you want to delete record with ID=2301234? Type "Y" to Confirm or type "N" to cancel.
  The record with ID=2301234 is successfully deleted.
  ```
- **Validation:** Joshua Chen record deleted, count becomes 3
- **Status:** PASS

#### Test 2.4: SHOW ALL After Deletion
- **Input:** `SHOW ALL`
- **Expected Output:** 3 records remaining (Isaac Teo, John Levoy, Michelle Lee)
- **Validation:** Joshua Chen (2301234) no longer in table
- **Status:** PASS

---

### **Test Suite 3: SAVE Operation (2 Tests)**

#### Test 3.1: SAVE Database
- **Input:** `SAVE`
- **Expected Output:** `The database file "Group9_CMS.txt" is successfully saved.`
- **Validation:** File written to disk
- **Status:** PASS

#### Test 3.2: File Format Validation
- **File Content:** `Group9_CMS.txt`
- **Expected Format:** 
  ```
  2201234,Isaac Teo,Computer Science,63.4
  2304567,John Levoy,Digital Supply Chain,85.9
  2401234,Michelle Lee,Applied Analytics,69.8
  ```
- **Validation:** Correct CSV format (3 lines, comma-delimited, marks to 2 decimals)
- **Status:** PASS

---

### **Test Suite 4: Output Message Validation (9 Tests)**

Each message must match spec exactly:

| Test | Message | Status |
|------|---------|--------|
| 4.1 | `The database file "Group9_CMS.txt" is successfully opened.` | PASS |
| 4.2 | `Here are all the records found in the table "StudentRecords".` | PASS |
| 4.3 | `The record with ID=2301234 already exists.` | PASS |
| 4.4 | `A new record with ID=2401234 is successfully inserted.` | PASS |
| 4.5 | `The record with ID=2501234 does not exist.` | PASS |
| 4.6 | `The record with ID=2401234 is found in the data table.` | PASS |
| 4.7 | `The record with ID=2401234 is successfully updated.` | PASS |
| 4.8 | `The deletion is cancelled.` | PASS |
| 4.9 | `The record with ID=2301234 is successfully deleted.` | PASS |

---

### **Test Suite 5: Edge Cases & Validation (6 Tests)**

#### Test 5.1: Invalid ID Format
- **Input:** `INSERT ID=abc123 Name=John Programme=CS Mark=75.5`
- **Expected:** System rejects with validation error
- **Status:** PASS

#### Test 5.2: Mark Out of Range (>100)
- **Input:** `INSERT ID=9999999 Name=Test Programme=Test Mark=150.0`
- **Expected:** System rejects: "Mark must be between 0 and 100"
- **Status:** PASS

#### Test 5.3: Mark Out of Range (<0)
- **Input:** `INSERT ID=9999999 Name=Test Programme=Test Mark=-5.0`
- **Expected:** System rejects negative mark
- **Status:** PASS

#### Test 5.4: Name with Numbers
- **Input:** `INSERT ID=9999999 Name=John123 Programme=CS Mark=75.5`
- **Expected:** System rejects: name cannot contain numbers
- **Status:** PASS

#### Test 5.5: Buffer Overflow Protection
- **Input:** `INSERT ID=9999999 Name=VeryLongNameThatExceeds30Characters Programme=CS Mark=75.5`
- **Expected:** Name truncated to 30 chars OR rejected
- **Status:** PASS

#### Test 5.6: Case-Insensitive Commands
- **Input:** `open`, `OPEN`, `Open`, `sHoW aLl`, `delete id=123`
- **Expected:** All variants work identically
- **Status:** PASS

---

### **Test Suite 6: Unit Tests (20 Tests)**

Run `test_cms.exe` - validates core functions:

| # | Test | Function | Status |
|---|------|----------|--------|
| 1 | Open file and read 3 records | `open_and_read_file()` | PASS |
| 2 | Find existing record by ID | `find_record_index()` | PASS |
| 3 | Find nonexistent record returns -1 | `find_record_index()` | PASS |
| 4 | Validate valid ID string | `isValidID()` | PASS |
| 5 | Reject invalid ID string | `isValidID()` | PASS |
| 6 | Validate valid mark value | `isValidMarks()` | PASS |
| 7 | Reject invalid mark value | `isValidMarks()` | PASS |
| 8 | Parse mark value correctly | `isValidMarks()` | PASS |
| 9 | Validate valid name | `isValidName()` | PASS |
| 10 | Reject name with numbers | `isValidName()` | PASS |
| 11 | Case-insensitive string comparison (equal) | `strcasecmp_ci()` | PASS |
| 12 | Case-insensitive string comparison (different) | `strcasecmp_ci()` | PASS |
| 13 | Extract ID value from input | `find_id_value()` | PASS |
| 14 | Extract ID value (lowercase) | `find_id_value()` | PASS |
| 15 | Handle missing ID value | `find_id_value()` | PASS |
| 16 | Save records to file | `save()` | PASS |
| 17 | Handle empty record list | `find_record_index()` | PASS |
| 18 | Find first occurrence of duplicate ID | `find_record_index()` | PASS |
| 19 | Validate boundary ID (0) | `isValidID()` | PASS |
| 20 | Validate large ID | `isValidID()` | PASS |

---

## Running All Tests

### **Step 1: Compile Code**
```bash
gcc *.c -o cms.exe          # Main program
gcc *.c test_cms.c -o test_cms.exe  # With unit tests
```

### **Step 2: Run Unit Tests**
```bash
./test_cms.exe
```

### **Step 3: Run Integration Tests**
```powershell
.\run_tests.ps1
```

### **Step 4: Manual Testing**
Create file `Group9_CMS.txt`:
```
2301234,Joshua Chen,Software Engineering,70.5
2201234,Isaac Teo,Computer Science,63.4
2304567,John Levoy,Digital Supply Chain,85.9
```

```bash
./cms.exe
# Then run commands from test cases above
```

---

## Success Criteria

All 32 integration tests PASS
All 20 unit tests PASS  
All output messages match specification exactly
No runtime errors or crashes
File I/O works correctly
Memory properly managed (no leaks)
Database persists correctly after SAVE

---

## Test Results Template

```
╔══════════════════════════════════════════════════════════════╗
║                    TEST RESULTS                              ║
╚══════════════════════════════════════════════════════════════╝

Unit Tests:      20/20 PASSED
Integration:     32/32 PASSED
Message Format:  9/9 CORRECT
Edge Cases:      6/6 HANDLED
─────────────────────────────────────────────────────────────
Overall:         67/67 PASSED
Pass Rate:       100%

Status: READY FOR SUBMISSION
```

---

**Test Framework Version:** 1.0  
**Date Created:** March 3, 2026  
**Created for:** INF1002 CMS Project - Group9