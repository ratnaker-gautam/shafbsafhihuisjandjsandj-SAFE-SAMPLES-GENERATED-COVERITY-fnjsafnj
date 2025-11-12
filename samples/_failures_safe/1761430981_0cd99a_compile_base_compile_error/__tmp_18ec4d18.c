//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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

int validate_timestamp(const char* ts) {
    if (strlen(ts) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (ts[i] != '-') return 0;
        } else if (i == 10) {
            if (ts[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (ts[i] != ':') return 0;
        } else {
            if (!isdigit(ts[i])) return 0;
        }
    }
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) >= MAX_LINE_LEN - 1) {
            printf("Line too long, skipping\n");
            continue;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            printf("Invalid format, skipping\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format, skipping\n");
            continue;
        }
        
        int severity = parse_severity(level);
        if (severity == -1) {
            printf("Unknown log level, skipping\n");
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Empty message, skipping\n");
            continue;
        }
        
        struct LogEntry* entry = &entries[entry_count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->severity = severity;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries processed\n");
        return 0;
    }
    
    struct LogEntry** heap_entries = malloc(entry_count * sizeof(struct LogEntry*));
    if (heap_entries == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < entry_count; i++) {
        heap_entries[i] = &entries[i];
    }
    
    for (int i = 0; i < entry_count - 1; i++) {
        for (int j = 0; j < entry_count - i - 1; j++) {
            if (heap_entries[j]->severity < heap_entries[j + 1]->severity) {
                struct LogEntry* temp = heap_entries[j];
                heap_entries[j] = heap_entries[j + 1];
                heap_entries[j + 1] = temp;
            }
        }
    }
    
    printf("\nLog entries sorted by severity:\n");
    printf("===============================\n");
    
    for (int i = 0; i < entry_count; i++) {
        printf("%s %s %s\n", heap_entries[i]->timestamp, heap_entries[i]->level, heap_entries[i]->message);
    }
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        switch (entries[i].severity) {
            case 3: error_count++; break;
            case 2: warn_count++; break;
            case 1: info_count++; break;
            case 0: debug_count++; break;
        }
    }
    
    printf("\nSummary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info