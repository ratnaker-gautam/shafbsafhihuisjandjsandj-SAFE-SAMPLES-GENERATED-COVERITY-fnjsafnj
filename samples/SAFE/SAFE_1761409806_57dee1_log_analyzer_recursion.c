//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
    int count;
};

int parse_log_recursive(FILE *file, struct LogEntry entries[], int *entry_count, int depth) {
    if (depth >= MAX_LEVELS || feof(file)) {
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }

    line[strcspn(line, "\n")] = 0;

    if (strlen(line) == 0) {
        return parse_log_recursive(file, entries, entry_count, depth + 1);
    }

    char timestamp[32] = {0};
    char level[16] = {0};
    char message[MAX_LINE_LENGTH] = {0};

    int scanned = sscanf(line, "%31s %15s %1023[^\n]", timestamp, level, message);
    if (scanned < 3) {
        return parse_log_recursive(file, entries, entry_count, depth + 1);
    }

    for (int i = 0; i < *entry_count; i++) {
        if (strcmp(entries[i].level, level) == 0 && strcmp(entries[i].message, message) == 0) {
            entries[i].count++;
            return parse_log_recursive(file, entries, entry_count, depth + 1);
        }
    }

    if (*entry_count < MAX_LEVELS) {
        strncpy(entries[*entry_count].timestamp, timestamp, sizeof(entries[*entry_count].timestamp) - 1);
        strncpy(entries[*entry_count].level, level, sizeof(entries[*entry_count].level) - 1);
        strncpy(entries[*entry_count].message, message, sizeof(entries[*entry_count].message) - 1);
        entries[*entry_count].count = 1;
        (*entry_count)++;
    }

    return parse_log_recursive(file, entries, entry_count, depth + 1);
}

void print_summary_recursive(struct LogEntry entries[], int index, int total) {
    if (index >= total) {
        return;
    }

    printf("%-8s: %s (count: %d)\n", entries[index].level, entries[index].message, entries[index].count);
    print_summary_recursive(entries, index + 1, total);
}

int main(void) {
    struct LogEntry entries[MAX_LEVELS];
    int entry_count = 0;

    printf("Enter log entries (timestamp level message). Empty line to finish:\n");

    int parse_result = parse_log_recursive(stdin, entries, &entry_count, 0);

    if (parse_result != 0) {
        fprintf(stderr, "Error parsing log entries\n");
        return 1;
    }

    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }

    printf("\nLog Summary:\n");
    print_summary_recursive(entries, 0, entry_count);

    return 0;
}