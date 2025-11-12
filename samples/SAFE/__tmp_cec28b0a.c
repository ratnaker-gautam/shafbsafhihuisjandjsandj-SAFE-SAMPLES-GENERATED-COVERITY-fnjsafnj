//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
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

void count_levels_recursive(const LogEntry entries[], int index, int total, int *error_count, int *warning_count, int *info_count) {
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
    
    count_levels_recursive(entries, index + 1, total, error_count, warning_count, info_count);
}

void display_entries_recursive(const LogEntry entries[], int index, int total) {
    if (index >= total) {
        return;
    }
    
    printf("Entry %d: [%s] %s - %s\n", 
           index + 1, 
           entries[index].timestamp, 
           entries[index].level, 
           entries[index].message);
    
    display_entries_recursive(entries, index + 1, total);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer\n");
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char input_line[MAX_LINE_LENGTH];
    FILE *temp_file = tmpfile();
    
    if (temp_file == NULL) {
        fprintf(stderr, "Error creating temporary file\n");
        return 1;
    }
    
    while (fgets(input_line, sizeof(input_line), stdin) != NULL) {
        if (strncmp(input_line, "END", 3) == 0 && (input_line[3] == '\n' || input_line[3] == '\0')) {
            break;
        }
        
        if (fputs(input_line, temp_file) == EOF) {
            fprintf(stderr, "Error writing to temporary file\n");
            fclose(temp_file);
            return 1;
        }
    }
    
    rewind(temp_file);
    
    entry_count = parse_log_recursive(temp_file, entries, 0, MAX_ENTRIES);
    fclose(temp_file);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    display_