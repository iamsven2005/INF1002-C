#include "sort.h"
#include <string.h>
#include <ctype.h>

enum Field { ID, MARKS };
enum Order { ASC, DESC };

struct CompareField {
    enum Field field;
    enum Order order;
};

static int compare(const struct Record* r1, const struct Record* r2, struct CompareField* ctx) {
    int result = 0;
    switch (ctx->field) {
    case ID:
        result = r1->id - r2->id;
        break;
    case MARKS:
        if (r1->marks < r2->marks) result = -1;
        else if (r1->marks > r2->marks) result = 1;
        else result = 0;
        break;
    default:
        result = 0;
    }
    return (ctx->order == ASC) ? result : -result;
}

void insertion_sort(struct Record records[], int size, struct CompareField* ctx) {
    for (int i = 1; i < size; i++) {
        struct Record key = records[i];
        int j = i - 1;

        while (j >= 0 && compare(&records[j], &key, ctx) > 0) {
            records[j + 1] = records[j];
            j--;
        }
        records[j + 1] = key;
    }
}


void sort_records(struct Record records[], int records_size, char* field, char* order) {
    if (records_size == 0) {
        printf("No records to sort.\n");
        return;
    }

    // Convert to lowercase for case-insensitive matching
    for (char* p = field; *p; p++) *p = tolower(*p);
    for (char* p = order; *p; p++) *p = tolower(*p);

    struct CompareField ctx;

    if (strcmp(field, "id") == 0) {
        ctx.field = ID;
    }
    else if (strcmp(field, "mark") == 0) {
        ctx.field = MARKS;
    }
    else {
        printf("Invalid field '%s'. Use ID or MARK.\n", field);
        return;
    }

    if (strcmp(order, "asc") == 0) {
        ctx.order = ASC;
    }
    else if (strcmp(order, "desc") == 0) {
        ctx.order = DESC;
    }
    else {
        printf("Invalid order '%s'. Use ASC or DESC.\n", order);
        return;
    }

    insertion_sort(records, records_size, &ctx);

    printf("Records have been sorted by %s %s.\n", field, order);
}

