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

int read_log_recursive(FILE *file, LogEntry entries[], int index, int max_entries) {
    if (index >= max_entries) {
        return index;
    }
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return index;
    }
    
    if (strlen(line) == 0 || line[0] == '\n') {
        return read_log_recursive(file, entries, index, max_entries);
    }
    
    LogEntry *entry = &entries[index];
    char *token = strtok(line, " ");
    int field = 0;
    
    while (token != NULL && field < 3) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token[token_len - 1] == '\n') {
            token[token_len - 1] = '\0';
        }
        
        switch (field) {
            case 0:
                if (token_len < sizeof(entry->timestamp)) {
                    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
                    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
                }
                break;
            case 1:
                if (token_len < sizeof(entry->level)) {
                    strncpy(entry->level, token, sizeof(entry->level) - 1);
                    entry->level[sizeof(entry->level) - 1] = '\0';
                }
                break;
            case 2:
                if (token_len < sizeof(entry->message)) {
                    strncpy(entry->message, token, sizeof(entry->message) - 1);
                    entry->message[sizeof(entry->message) - 1] = '\0';
                    
                    char *next_token = strtok(NULL, "");
                    if (next_token != NULL) {
                        size_t remaining_len = strlen(next_token);
                        if (remaining_len > 0 && remaining_len < sizeof(entry->message) - token_len - 1) {
                            strncat(entry->message, " ", sizeof(entry->message) - strlen(entry->message) - 1);
                            strncat(entry->message, next_token, sizeof(entry->message) - strlen(entry->message) - 1);
                        }
                    }
                }
                break;
        }
        field++;
        token = strtok(NULL, " ");
    }
    
    return read_log_recursive(file, entries, index + 1, max_entries);
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

void print_logs_by_level_recursive(LogEntry entries[], int count, int index, const char *level) {
    if (index >= count) {
        return;
    }
    
    if (strcmp(entries[index].level, level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    print_logs_by_level_recursive(entries, count, index + 1, level);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("Enter log data (format: TIMESTAMP LEVEL MESSAGE). Empty line to finish:\n");
    
    entry_count = read_log_recursive(stdin, entries, 0, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No log entries found.\n");
        return 0;
    }
    
    analyze_logs_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nERROR entries:\n");
    print_logs_by_level_recursive(entries, entry_count, 0, "ERROR");
    
    printf("\nWARNING entries:\n");
    print_logs_by_level_recursive(entries, entry_count, 0, "WARNING");
    
    return 0;
}