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

    buffer[strcspn(buffer, "\n")] = 0;

    if (strlen(buffer) == 0) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    struct LogEntry *entry = &entries[current_depth];
    int parsed_fields = sscanf(buffer, "%d %15s %1023[^\n]", 
                              &entry->timestamp, entry->level, entry->message);

    if (parsed_fields != 3) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    for (int i = 0; entry->level[i]; i++) {
        entry->level[i] = toupper((unsigned char)entry->level[i]);
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

void display_results_recursive(struct LogEntry *entries, int count, int index, 
                              const char *filter_level) {
    if (index >= count) {
        return;
    }

    if (filter_level == NULL || strcmp(entries[index].level, filter_level) == 0) {
        printf("Time: %d | Level: %-7s | Message: %s\n", 
               entries[index].timestamp, entries[index].level, entries[index].message);
    }

    display_results_recursive(entries, count, index + 1, filter_level);
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = 0;

    if (strlen(filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }

    struct LogEntry entries[MAX_LEVELS];
    int entry_count = parse_log_recursive(file, entries, MAX_LEVELS, 0);
    fclose(file);

    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }

    int error_count = 0, warning_count = 0, info_count = 0;
    analyze_logs_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);

    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
    printf("Info: %d\n", info_count);

    char filter[16];
    printf("\nEnter level to filter (or 'all'): ");
    if (fgets(filter, sizeof(filter), stdin) == NULL) {
        return 1;
    }
    filter[strcspn(filter, "\n")] = 0;

    for (int i = 0; filter[i]; i++) {
        filter[i] = toupper((unsigned char)filter[i]);
    }

    const char *filter_level = NULL;
    if (strcmp(filter, "ERROR") == 0) {
        filter_level = "ERROR";
    } else if (strcmp(filter, "WARNING") == 0) {
        filter_level = "WARNING";
    } else if (strcmp(filter, "INFO") == 0) {
        filter_level = "INFO";
    }

    printf("\nFiltered Log Entries:\n");
    display_results_recursive(entries, entry_count, 0, filter_level);

    return 0;
}