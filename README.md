gcc delete.c insert.c misc.c open.c query.c save.c showall.c showsummary.c snapshot.c sort.c update.c main.c -o cms.exe
gcc delete.c insert.c misc.c open.c query.c save.c showall.c showsummary.c snapshot.c sort.c update.c test_cms.c -o test_cms.exe
./test_cms.exe
bash run_integration_tests.sh

./run_tests.ps1


# INF1002-C CMS (Group 9)

Console-based Student Course Management System (CMS) for INF1002.

## 1. Project Overview

This project implements a command-driven CMS that manages student records stored in a text database file (`Group9_CMS.txt`).

Each record contains:
- Student ID
- Student Name
- Programme
- Mark

The system supports loading, querying, insertion, update, deletion, sorting, summary display, snapshot management, and saving.

## 2. Features

- `OPEN [filename]` - Load records from database file
- `SHOW ALL` - Display all records in table format
- `SHOW SUMMARY` / `SHOWSUMMARY` - Display summary statistics
- `INSERT ...` - Add a new record
- `QUERY ID=<id>` - Search for a specific record
- `UPDATE ID=<id> <Field>=<Value>` - Modify record fields
- `DELETE ID=<id>` - Delete a record with confirmation
- `SORT BY <ID|MARK> <ASC|DESC>` - Sort records
- `SAVE` - Persist in-memory records to database file
- `SHOW SNAPSHOT`, `CREATE SNAPSHOT <name>`, `RESTORE SNAPSHOT <name>`, `DELETE SNAPSHOT <name>` - Snapshot operations
- `HELP [command]` - Command help menu
- `EXIT` - Terminate the program

## 3. Logic Flow (Required Section)

### 3.1 Program Startup Flow

1. Program starts and prints declaration, team details, and CMS welcome banner.
2. Runtime state is initialized:
   - global dynamic array pointer `records = NULL`
   - global record count `records_size = 0`
   - `file_opened = false`
   - default filename set to `Group9_CMS.txt`
3. Program enters an infinite command loop.

### 3.2 Command Processing Flow

1. Prompt user input.
2. Validate input length and remove trailing newline.
3. Parse first token as command (case-insensitive).
4. Route command to the corresponding module function.
5. Continue loop unless `EXIT` is entered.

### 3.3 Main Control Rules

- `OPEN` can be executed before database access commands.
- For database operations (`INSERT`, `QUERY`, `UPDATE`, `DELETE`, `SORT`, `SHOW`, `SAVE`), file must be opened first.
- On successful `OPEN`, records are loaded into memory.
- On `SAVE`, in-memory records are written back to file.
- On `EXIT`, allocated memory is released when applicable.

### 3.4 Operation Logic Flow (By Command)

#### OPEN
1. Resolve optional filename argument.
2. Clear previous in-memory records if already loaded.
3. Read file line by line.
4. Parse CSV fields into record objects.
5. Store records in dynamic memory and set `file_opened = true`.

#### INSERT
1. Parse `ID`, `NAME`, `PROGRAMME`, `MARK` key-value inputs.
2. Enforce `ID` as mandatory.
3. Validate optional fields policy (all optional fields provided together when inline mode is used).
4. Validate value types and ranges.
5. Reject duplicate IDs.
6. Append valid record into in-memory array.

#### QUERY
1. Parse `ID` from input.
2. Search in-memory array by ID.
3. Display found record or "not found" message.

#### UPDATE
1. Parse `ID` and target field/value.
2. Locate record by ID.
3. Validate new value based on field type.
4. Update requested field and print status.

#### DELETE
1. Parse `ID`.
2. Locate record by ID.
3. Prompt confirmation (`Y` / `N`).
4. If confirmed, remove record and shift array content.
5. Print success or cancellation message.

#### SORT
1. Validate syntax `SORT BY <field> <order>`.
2. Sort in-memory records by selected field and order.
3. Reflect sorted order in subsequent displays/saves.

#### SHOW / SUMMARY
1. Read current in-memory records.
2. Print formatted list (`SHOW ALL`) or aggregate summary (`SHOW SUMMARY`).

#### SNAPSHOT
1. Resolve snapshot directory path.
2. Execute sub-command (`SHOW`, `CREATE`, `RESTORE`, `DELETE`).
3. Validate snapshot name and file existence as required.
4. Perform file-level snapshot operation.

#### SAVE
1. Traverse in-memory record array.
2. Write each record in CSV format to active database file.
3. Print completion status.

## 4. Code Structure

The codebase follows a modular C architecture, where each command domain is split into dedicated files.

- `main.c`: application entry point, command loop, command routing, runtime state control
- `headers.h`: common includes, constants, and module header aggregation
- `open.c` / `open.h`: database file loading and CSV parsing
- `insert.c` / `insert.h`: insertion logic, field parsing, validation, duplicate prevention
- `query.c` / `query.h`: record lookup display
- `update.c` / `update.h`: in-place record updates
- `delete.c` / `delete.h`: record deletion with confirmation
- `sort.c` / `sort.h`: ordering records by selected key
- `showall.c` / `showall.h`: tabular display of all records
- `showsummary.c` / `showsummary.h`: summary/statistics output
- `save.c` / `save.h`: persistence to database file
- `snapshot.c` / `snapshot.h`: snapshot directory and snapshot file operations
- `misc.c` / `misc.h`: helpers (case-insensitive compare, validation, parser helpers, help menu)
- `test_cms.c`: unit tests for utility and core functions

This separation improves maintainability, readability, and testing by keeping command logic isolated.

## 5. Data Structure Used

### 5.1 Primary Data Structure

The system uses a **dynamic array of structures**:

```c
struct Record {
	int id;
	char name[100];
	char prog[256];
	float marks;
	bool has_id;
	bool has_name;
	bool has_prog;
	bool has_mark;
};
```

- Records are stored in contiguous memory (`struct Record *records`).
- Current number of valid records is tracked by `records_size`.
- Data is loaded from file into memory during `OPEN`.
- Most operations (`QUERY`, `UPDATE`, `DELETE`, `SHOW`, `SORT`) operate directly on this in-memory array.

### 5.2 Why This Structure Fits

- Simple and efficient for sequential traversal.
- Fast indexed access and straightforward integration with C file I/O.
- Suitable for moderate record counts used in this project context.
- Works naturally with existing command-oriented workflow.

## 6. Build and Run

### Compile Main Program

```bash
gcc delete.c insert.c misc.c open.c query.c save.c showall.c showsummary.c snapshot.c sort.c update.c main.c -o cms.exe
```

### Compile Unit Tests

```bash
gcc delete.c insert.c misc.c open.c query.c save.c showall.c showsummary.c snapshot.c sort.c update.c test_cms.c -o test_cms.exe
```

### Run Unit Tests

```bash
./test_cms.exe
```

### Run Integration Tests

```bash
bash run_integration_tests.sh
```

or on PowerShell:

```powershell
./run_tests.ps1
```

## 7. Testing Summary

- Unit tests validate parsing, validation, lookup, and save behavior.
- Integration tests validate end-to-end command workflows and expected outputs.
- Test details and expected messages are documented in `TEST_CASES.md`.

## 8. Notes

- Commands are designed to be case-insensitive.
- Input validation is included for key fields such as `ID`, `MARK`, and name format.
- Snapshot features provide optional file-level rollback support.
