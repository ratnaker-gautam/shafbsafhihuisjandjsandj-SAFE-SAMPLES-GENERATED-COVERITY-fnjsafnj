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
    char *timestamp = strtok(line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) {
        return parse_log_recursive(file, entries, index, max_entries);
    }
    
    if (strlen(timestamp) < sizeof(entry->timestamp) - 1 &&
        strlen(level) < sizeof(entry->level) - 1 &&
        strlen(message) < sizeof(entry->message) - 1) {
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        entry->level[sizeof(entry->level) - 1] = '\0';
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        return parse_log_recursive(file, entries, index + 1, max_entries);
    }
    
    return parse_log_recursive(file, entries, index, max_entries);
}

void count_log_levels_recursive(const LogEntry entries[], int index, int count, 
                               int *error_count, int *warning_count, int *info_count) {
    if (index >= count) {
        return;
    }
    
    const LogEntry *entry = &entries[index];
    if (strcmp(entry->level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        (*info_count)++;
    }
    
    count_log_levels_recursive(entries, index + 1, count, error_count, warning_count, info_count);
}

void display_logs_recursive(const LogEntry entries[], int index, int count) {
    if (index >= count) {
        return;
    }
    
    const LogEntry *entry = &entries[index];
    printf("%s %s %s\n", entry->timestamp, entry->level, entry->message);
    display_logs_recursive(entries, index + 1, count);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char input_line[MAX_LINE_LENGTH];
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Error: Cannot create temporary file\n");
        return 1;
    }
    
    while (fgets(input_line, sizeof(input_line), stdin) != NULL) {
        if (strncmp(input_line, "END", 3) == 0 && (input_line[3] == '\n' || input_line[3] == '\0')) {
            break;
        }
        
        if (fputs(input_line, temp_file) == EOF) {
            fprintf(stderr, "Error: Cannot write to temporary file\n");
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
    display_logs_recursive(entries, 0, entry_count);
    
    int error_count = 0, warning_count = 0, info_count = 0;
    count_log_levels_recursive(entries, 0, entry_count, &error_count, &warning_count, &info_count);
    
    printf("\nLog Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
    
    return 0;
}