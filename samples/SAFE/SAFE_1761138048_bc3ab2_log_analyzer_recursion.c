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

int parse_log_recursive(FILE *file, LogEntry entries[], int index, int max_entries) {
    if (index >= max_entries) {
        return index;
    }
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return index;
    }
    
    if (strlen(line) == 0 || line[0] == '\n') {
        return parse_log_recursive(file, entries, index, max_entries);
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
    
    return parse_log_recursive(file, entries, index + 1, max_entries);
}

void count_log_levels_recursive(const LogEntry entries[], int index, int total, int *error_count, int *warning_count, int *info_count) {
    if (index >= total) {
        return;
    }
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    count_log_levels_recursive(entries, index + 1, total, error_count, warning_count, info_count);
}

void display_logs_recursive(const LogEntry entries[], int index, int total) {
    if (index >= total) {
        return;
    }
    
    printf("%s [%s] %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    display_logs_recursive(entries, index + 1, total);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    entry_count = parse_log_recursive(stdin, entries, 0, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No log entries found.\n");
        return 0;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    display_logs_recursive(entries, 0, entry_count);
    
    int error_count = 0, warning_count = 0, info_count = 0;
    count_log_levels_recursive(entries, 0, entry_count, &error_count, &warning_count, &info_count);
    
    printf("\nLog Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
    
    return 0;
}