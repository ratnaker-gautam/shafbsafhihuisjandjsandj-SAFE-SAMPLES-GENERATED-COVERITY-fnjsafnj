//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Filename cannot be empty\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    LogEntry* entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    int error_count = 0;
    int warning_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        if (strlen(line) > 0 && line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31[^|]|%15[^|]|%255[^\n]", timestamp, level, message);
        
        if (parsed == 3 && validate_timestamp(timestamp)) {
            int severity = parse_log_level(level);
            if (severity > 0) {
                strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp)-1);
                strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level)-1);
                strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message)-1);
                entries[entry_count].severity = severity;
                entry_count++;
                
                if (severity >= 4) error_count++;
                if (severity == 3) warning_count++;
            }
        }
    }
    
    fclose(file);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Error entries: %d\n", error_count);
    printf("Warning entries: %d\n", warning_count);
    
    if (entry_count > 0) {
        printf("\nMost severe entries:\n");
        int max_severity = 0;
        for (int i = 0; i < entry_count; i++) {
            if (entries[i].severity > max_severity) {
                max_severity = entries[i].severity;
            }
        }
        
        int printed = 0;
        for (int i = 0; i < entry_count && printed < 3; i++) {
            if (entries[i].severity == max_severity) {
                printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                printed++;
            }
        }
    }
    
    free(entries);
    return 0;
}