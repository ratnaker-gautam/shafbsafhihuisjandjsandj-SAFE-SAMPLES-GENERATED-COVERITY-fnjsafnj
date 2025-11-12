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
    
    if (strlen(line) >= MAX_LINE_LENGTH - 1) {
        return parse_log_recursive(file, entries, index, max_entries);
    }
    
    LogEntry *entry = &entries[index];
    int scanned = sscanf(line, "%19s %9s %1023[^\n]", 
                        entry->timestamp, entry->level, entry->message);
    
    if (scanned == 3) {
        for (int i = 0; i < strlen(entry->level); i++) {
            entry->level[i] = toupper((unsigned char)entry->level[i]);
        }
        return parse_log_recursive(file, entries, index + 1, max_entries);
    }
    
    return parse_log_recursive(file, entries, index, max_entries);
}

void analyze_logs_recursive(LogEntry entries[], int count, int index, 
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

void display_logs_recursive(LogEntry entries[], int count, int index, const char *filter) {
    if (index >= count) {
        return;
    }
    
    if (filter == NULL || strcmp(entries[index].level, filter) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    display_logs_recursive(entries, count, index + 1, filter);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log data (empty line to finish):\n");
    
    char input[MAX_LINE_LENGTH];
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Error creating temporary file\n");
        return 1;
    }
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (strlen(input) == 1 && input[0] == '\n') {
            break;
        }
        
        if (fputs(input, temp_file) == EOF) {
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
    
    int error_count = 0, warning_count = 0, info_count = 0;
    analyze_logs_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
    printf("Info: %d\n", info_count);
    
    printf("\nEnter log level to filter (ERROR, WARNING, INFO, or ALL): ");
    char filter[10];
    if (fgets(filter, sizeof(filter), stdin) != NULL) {
        filter[strcspn(filter, "\n")] = '\0';
        
        for (int i = 0; i < strlen(filter); i++) {
            filter[i] = toupper((unsigned char)filter[i]);
        }
        
        printf("\nFiltered Log Entries:\n");
        if (strcmp(filter, "ALL") == 0) {
            display_logs_recursive(entries, entry_count, 0, NULL);
        } else {
            display_logs_recursive(entries, entry_count, 0, filter);
        }
    }
    
    return 0;
}