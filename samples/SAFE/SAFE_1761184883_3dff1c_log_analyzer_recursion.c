//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_recursive(FILE *file, struct LogEntry *entries, int max_entries, int current_depth) {
    if (current_depth >= max_entries || feof(file)) {
        return 0;
    }

    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    if (strlen(buffer) == 0) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    struct LogEntry *entry = &entries[current_depth];
    int scanned = sscanf(buffer, "%d %15s %1023[^\n]", 
                        &entry->timestamp, entry->level, entry->message);

    if (scanned != 3) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    for (char *p = entry->level; *p; p++) {
        *p = toupper((unsigned char)*p);
    }

    int count = parse_log_recursive(file, entries, max_entries, current_depth + 1);
    return count + 1;
}

void analyze_logs_recursive(struct LogEntry *entries, int count, int index, 
                           int *error_count, int *warning_count, int *info_count) {
    if (index >= count) {
        return;
    }

    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }

    analyze_logs_recursive(entries, count, index + 1, error_count, warning_count, info_count);
}

void print_summary_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return;
    }

    printf("Time: %d | Level: %-7s | Message: %s\n", 
           entries[index].timestamp, entries[index].level, entries[index].message);
    print_summary_recursive(entries, count, index + 1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    struct LogEntry entries[MAX_LEVELS];
    int entry_count = parse_log_recursive(file, entries, MAX_LEVELS, 0);
    fclose(file);

    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }

    int error_count = 0, warning_count = 0, info_count = 0;
    analyze_logs_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);

    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
    printf("Info: %d\n", info_count);
    printf("\nDetailed Log:\n");
    print_summary_recursive(entries, entry_count, 0);

    return 0;
}