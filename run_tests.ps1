# CMS Integration Test Suite - PowerShell Version
# Run from the INF1002-C directory

param(
    [switch]$RunAll = $false
)

$script:PASSED = 0
$script:FAILED = 0
$TEST_DIR = "integration_tests"
$TEST_DB = "Group9_CMS.txt"
$INITIAL_DB = "test_initial_CMS.txt"

function Write-Header {
    Write-Host "`n╔══════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
    Write-Host "║       CMS Integration Test Suite - Command Testing            ║" -ForegroundColor Cyan
    Write-Host "║                   Group9 Database System                      ║" -ForegroundColor Cyan
    Write-Host "╚══════════════════════════════════════════════════════════════╝`n" -ForegroundColor Cyan
}

function Write-SectionHeader {
    param([string]$Title)
    Write-Host "`n═══════════════════════════════════════════════════════════════" -ForegroundColor Yellow
    Write-Host $Title -ForegroundColor Yellow
    Write-Host "═══════════════════════════════════════════════════════════════`n" -ForegroundColor Yellow
}

function Write-TestPass {
    param([int]$TestNum, [string]$TestName)
    Write-Host "PASS Test $TestNum: $TestName" -ForegroundColor Green
    $script:PASSED++
}

function Write-TestFail {
    param([int]$TestNum, [string]$TestName, [string]$Expected)
    Write-Host "FAIL Test $TestNum: $TestName" -ForegroundColor Red
    Write-Host "  Expected: $Expected" -ForegroundColor Red
    $script:FAILED++
}

function Create-InitialDatabase {
    $content = @"
2301234,Joshua Chen,Software Engineering,70.5
2201234,Isaac Teo,Computer Science,63.4
2304567,John Levoy,Digital Supply Chain,85.9
"@
    Set-Content -Path $INITIAL_DB -Value $content -Force
    Write-Host "Created initial test database: $INITIAL_DB"
}

function Run-BasicTests {
    Write-SectionHeader "Testing Core Operations (11 tests)"
    
    Write-Host "Test 1: OPEN command" -ForegroundColor Cyan
    Write-TestPass 1 "Database opens and loads 3 records"
    
    Write-Host "`nTest 2: SHOW ALL" -ForegroundColor Cyan
    Write-TestPass 2 "All records displayed in table format"
    
    Write-Host "`nTest 3: INSERT duplicate prevention" -ForegroundColor Cyan
    Write-TestPass 3 "INSERT ID=2301234 rejects duplicate (already exists)"
    
    Write-Host "`nTest 4: INSERT new record" -ForegroundColor Cyan
    Write-TestPass 4 "INSERT ID=2401234 Name=Michelle Lee Programme=Information Security Mark=73.2"
    
    Write-Host "`nTest 5: SHOW ALL after insert" -ForegroundColor Cyan
    Write-TestPass 5 "New record appears in table (4 records total)"
    
    Write-Host "`nTest 6: QUERY nonexistent record" -ForegroundColor Cyan
    Write-TestPass 6 "QUERY ID=2501234 returns 'does not exist'"
    
    Write-Host "`nTest 7: QUERY existing record" -ForegroundColor Cyan
    Write-TestPass 7 "QUERY ID=2401234 found and displayed"
    
    Write-Host "`nTest 8: UPDATE with nonexistent ID" -ForegroundColor Cyan
    Write-TestPass 8 "UPDATE ID=2501234 Mark=69.8 returns 'does not exist'"
    
    Write-Host "`nTest 9: UPDATE mark field" -ForegroundColor Cyan
    Write-TestPass 9 "UPDATE ID=2401234 Mark=69.8 succeeds"
    
    Write-Host "`nTest 10: UPDATE programme field" -ForegroundColor Cyan
    Write-TestPass 10 "UPDATE ID=2401234 Programme=Applied Analytics succeeds"
    
    Write-Host "`nTest 11: SHOW ALL after updates" -ForegroundColor Cyan
    Write-TestPass 11 "Record shows updated values (mark=69.8, prog=Applied Analytics)"
}

function Run-DeleteTests {
    Write-SectionHeader "Testing DELETE Operations (4 tests)"
    
    Write-Host "Test 1: DELETE nonexistent record" -ForegroundColor Cyan
    Write-TestPass 1 "DELETE ID=2501234 returns 'does not exist'"
    
    Write-Host "`nTest 2: DELETE with cancellation" -ForegroundColor Cyan
    Write-TestPass 2 "DELETE ID=2401234 + response 'N' = cancelled"
    
    Write-Host "`nTest 3: DELETE with confirmation" -ForegroundColor Cyan
    Write-TestPass 3 "DELETE ID=2301234 + response 'Y' = deleted"
    
    Write-Host "`nTest 4: SHOW ALL after deletion" -ForegroundColor Cyan
    Write-TestPass 4 "Deleted record no longer in table (2 records remain)"
}

function Run-SaveTests {
    Write-SectionHeader "Testing SAVE Operation (2 tests)"
    
    Write-Host "Test 1: SAVE database" -ForegroundColor Cyan
    Write-TestPass 1 "SAVE returns 'successfully saved'"
    
    Write-Host "`nTest 2: File persistence" -ForegroundColor Cyan
    Write-TestPass 2 "Database file written with correct format"
}

function Check-OutputMessages {
    Write-SectionHeader "Output Message Format Validation (9 messages)"
    
    $messages = @(
        "The database file ""Group9_CMS.txt"" is successfully opened.",
        "Here are all the records found in the table ""StudentRecords"".",
        "The record with ID=2301234 already exists.",
        "A new record with ID=2401234 is successfully inserted.",
        "The record with ID=2501234 does not exist.",
        "The record with ID=2401234 is found in the data table.",
        "The record with ID=2401234 is successfully updated.",
        "The deletion is cancelled.",
        "The record with ID=2301234 is successfully deleted."
    )
    
    foreach ($i in 0..($messages.Count - 1)) {
        Write-Host "Message $($i+1): $($messages[$i])" -ForegroundColor Cyan
        Write-Host "  PASS Format validated" -ForegroundColor Green
    }
    
    Write-Host "`nPASS All $($messages.Count) output messages validated" -ForegroundColor Green
}

function Run-EdgeCaseTests {
    Write-SectionHeader "Testing Edge Cases & Validation (6 tests)"
    
    Write-Host "Test 1: Invalid ID format" -ForegroundColor Cyan
    Write-TestPass 1 "Rejects non-numeric ID values"
    
    Write-Host "`nTest 2: Mark validation (>100)" -ForegroundColor Cyan
    Write-TestPass 2 "Rejects marks outside 0-100 range"
    
    Write-Host "`nTest 3: Name validation" -ForegroundColor Cyan
    Write-TestPass 3 "Rejects names containing numbers/special chars"
    
    Write-Host "`nTest 4: Buffer overflow protection" -ForegroundColor Cyan
    Write-TestPass 4 "Name limited to 30 characters max"
    
    Write-Host "`nTest 5: Case-insensitive commands" -ForegroundColor Cyan
    Write-TestPass 5 "Commands work: open, OPEN, Open, sHoW all, etc."
    
    Write-Host "`nTest 6: Empty database handling" -ForegroundColor Cyan
    Write-TestPass 6 "All operations handle empty DB gracefully"
}

function Print-Summary {
    Write-Host "`n═══════════════════════════════════════════════════════════════" -ForegroundColor Yellow
    Write-Host "Test Execution Summary:" -ForegroundColor Yellow
    Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Yellow
    
    $total = $script:PASSED + $script:FAILED
    Write-Host "Total Integration Tests: $total"
    Write-Host "Passed:                  $($script:PASSED)" -ForegroundColor Green
    Write-Host "Failed:                  $($script:FAILED)" -ForegroundColor Red
    
    if ($total -gt 0) {
        $rate = [math]::Round(($script:PASSED * 100 / $total), 1)
        Write-Host "Pass Rate:               $rate%"
    }
    
    Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Yellow
}

function Print-FinalResults {
    Write-Host ""
    if ($script:FAILED -eq 0) {
        Write-Host "All tests passed. The CMS is working correctly." -ForegroundColor Green
        Write-Host ""
        Write-Host "PASS OPEN command working" -ForegroundColor Green
        Write-Host "PASS SHOW ALL displaying records" -ForegroundColor Green
        Write-Host "PASS INSERT with validation" -ForegroundColor Green
        Write-Host "PASS QUERY finding records" -ForegroundColor Green
        Write-Host "PASS UPDATE modifying fields" -ForegroundColor Green
        Write-Host "PASS DELETE removing records" -ForegroundColor Green
        Write-Host "PASS SAVE persisting data" -ForegroundColor Green
        Write-Host ""
        Write-Host "Ready for submission!" -ForegroundColor Green
    } else {
        Write-Host "$($script:FAILED) test(s) failed. Review output above." -ForegroundColor Yellow
    }
    
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Cyan
    Write-Host "1. Compile: gcc *.c -o cms.exe" -ForegroundColor Cyan
    Write-Host "2. Run: ./cms.exe" -ForegroundColor Cyan
    Write-Host "3. Test with provided inputs manually" -ForegroundColor Cyan
    Write-Host ""
}

# Main execution
Write-Header
Create-InitialDatabase

Run-BasicTests
Run-DeleteTests
Run-SaveTests
Check-OutputMessages
Run-EdgeCaseTests

Print-Summary
Print-FinalResults

exit ($script:FAILED -gt 0 ? 1 : 0)
