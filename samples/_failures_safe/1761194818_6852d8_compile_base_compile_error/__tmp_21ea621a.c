//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int parse_log_file_recursive(FILE *file, LogEntry entries[], int index, int max_entries) {
    if (index >= max_entries || feof(file)) {
        return index;
    }

    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return index;
    }

    if (strlen(line) == 0 || line[0] == '\n') {
        return parse_log_file_recursive(file, entries, index, max_entries);
    }

    char timestamp[20] = {0};
    char level[10] = {0};
    char message[MAX_LINE_LENGTH] = {0};

    int scanned = sscanf(line, "%19s %9s %1023[^\n]", timestamp, level, message);
    if (scanned >= 2) {
        if (scanned == 2) {
            strcpy(message, "");
        }
        
        strncpy(entries[index].timestamp, timestamp, sizeof(entries[index].timestamp) - 1);
        strncpy(entries[index].level, level, sizeof(entries[index].level) - 1);
        strncpy(entries[index].message, message, sizeof(entries[index].message) - 1);
        
        entries[index].timestamp[sizeof(entries[index].timestamp) - 1] = '\0';
        entries[index].level[sizeof(entries[index].level) - 1] = '\0';
        entries[index].message[sizeof(entries[index].message) - 1] = '\0';
        
        return parse_log_file_recursive(file, entries, index + 1, max_entries);
    }

    return parse_log_file_recursive(file, entries, index, max_entries);
}

void analyze_logs_recursive(LogEntry entries[], int count, int index, int *error_count, int *warning_count, int *info_count) {
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

void display_log_summary_recursive(LogEntry entries[], int count, int index) {
    if (index >= count) {
        return;
    }

    printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    display_log_summary_recursive(entries, count, index + 1);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;

    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'END' on a separate line to finish input\n");

    char input[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "END") == 0) {
            break;
        }

        if (strlen(input) == 0) {
            continue;
        }

        char timestamp[20] = {0};
        char level[10] = {0};
        char message[MAX_LINE_LENGTH] = {0};

        int scanned = sscanf(input, "%19s %9s %1023[^\n]", timestamp, level, message);
        if (scanned >= 2) {
            if (scanned == 2) {
                strcpy(message, "");
            }
            
            strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
            strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
            strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
            
            entries[entry_count].timestamp[sizeof(entries[entry_count].timestamp) - 1] = '\0';
            entries[entry_count].level[sizeof(entries[entry_count].level) - 1] = '\0';
            entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
            
            entry_count++;
        }
    }

    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 0;
    }

    analyze_logs_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);

    printf("\n=== Log Analysis Summary ===\n");
    printf("Total entries: %d\n",