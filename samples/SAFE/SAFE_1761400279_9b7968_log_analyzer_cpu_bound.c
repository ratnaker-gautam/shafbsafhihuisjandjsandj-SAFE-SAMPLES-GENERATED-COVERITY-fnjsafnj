//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_count++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (len >= MAX_LINE_LEN - 1) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            if (strcmp(entry.level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                total_warnings++;
            }
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = i + 1; j < entry_count; j++) {
            if (strcmp(entries[i].level, entries[j].level) > 0) {
                LogEntry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }
    
    int error_entries = 0;
    int warning_entries = 0;
    int info_entries = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_entries++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_entries++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_entries++;
        }
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Error entries: %d\n", error_entries);
    printf("Warning entries: %d\n", warning_entries);
    printf("Info entries: %d\n", info_entries);
    printf("Total errors: %d\n", total_errors);
    printf("Total warnings: %d\n", total_warnings);
    printf("Processing time: %.6f seconds\n", cpu_time);
    
    if (entry_count > 0) {
        printf("\nFirst 5 entries (sorted by level):\n");
        for (int i = 0; i < 5 && i < entry_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}