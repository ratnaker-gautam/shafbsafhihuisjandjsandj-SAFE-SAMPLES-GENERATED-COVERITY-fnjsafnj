//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_severity(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int main(void) {
    struct LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (heap_entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char filename[256];
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        free(heap_entries);
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Filename cannot be empty\n");
        free(heap_entries);
        return 1;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        free(heap_entries);
        return 1;
    }

    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (strlen(line) == 0) continue;

        struct LogEntry stack_entry;
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           stack_entry.timestamp, 
                           stack_entry.level, 
                           stack_entry.message);
        
        if (parsed != 3) {
            continue;
        }
        
        if (!validate_timestamp(stack_entry.timestamp)) {
            continue;
        }
        
        stack_entry.severity = parse_severity(stack_entry.level);
        if (stack_entry.severity == -1) {
            continue;
        }
        
        if (entry_count < MAX_ENTRIES) {
            heap_entries[entry_count] = stack_entry;
            entry_count++;
        }
    }
    
    fclose(file);

    printf("\nLog Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    
    int severity_counts[4] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (heap_entries[i].severity >= 0 && heap_entries[i].severity <= 3) {
            severity_counts[heap_entries[i].severity]++;
        }
    }
    
    printf("\nSeverity Breakdown:\n");
    printf("DEBUG: %d\n", severity_counts[0]);
    printf("INFO:  %d\n", severity_counts[1]);
    printf("WARN:  %d\n", severity_counts[2]);
    printf("ERROR: %d\n", severity_counts[3]);
    
    if (entry_count > 0) {
        printf("\nRecent entries (last 5):\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s [%s] %s\n", 
                   heap_entries[i].timestamp, 
                   heap_entries[i].level, 
                   heap_entries[i].message);
        }
    }
    
    free(heap_entries);
    return 0;
}