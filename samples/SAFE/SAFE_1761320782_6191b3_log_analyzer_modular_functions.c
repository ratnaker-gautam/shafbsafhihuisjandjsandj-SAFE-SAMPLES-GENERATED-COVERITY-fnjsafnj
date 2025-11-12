//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        if (strlen(line) > 0) {
            line[strcspn(line, "\n")] = 0;
            
            char* timestamp = strtok(line, " ");
            char* level = strtok(NULL, " ");
            char* message = strtok(NULL, "");
            
            if (timestamp != NULL && level != NULL && message != NULL) {
                strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                
                entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
                entries[count].level[sizeof(entries[count].level) - 1] = '\0';
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void count_log_levels(LogEntry* entries, int count, int* error_count, int* warning_count, int* info_count) {
    *error_count = 0;
    *warning_count = 0;
    *info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            (*info_count)++;
        }
    }
}

void display_log_summary(LogEntry* entries, int count, int error_count, int warning_count, int info_count) {
    printf("Log Analysis Summary\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("\n");
    
    if (count > 0) {
        printf("Recent entries:\n");
        int display_count = count > 5 ? 5 : count;
        for (int i = 0; i < display_count; i++) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int validate_filename(const char* filename) {
    if (filename == NULL || strlen(filename) == 0 || strlen(filename) > 255) {
        return 0;
    }
    
    for (size_t i = 0; i < strlen(filename); i++) {
        if (!isalnum(filename[i]) && filename[i] != '.' && filename[i] != '-' && filename[i] != '_') {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    char filename[256];
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = 0;
    
    if (!validate_filename(filename)) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error: Could not open or read file '%s'\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No log entries found in file '%s'\n", filename);
        return 0;
    }
    
    int error_count, warning_count, info_count;
    count_log_levels(entries, entry_count, &error_count, &warning_count, &info_count);
    
    display_log_summary(entries, entry_count, error_count, warning_count, info_count);
    
    return 0;
}