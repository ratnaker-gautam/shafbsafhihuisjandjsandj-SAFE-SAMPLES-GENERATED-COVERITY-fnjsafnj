//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

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
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL Message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        struct LogEntry* entry = &entries[entry_count];
        char level_buf[16];
        char message_buf[256];
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", 
                           entry->timestamp, level_buf, message_buf);
        
        if (parsed != 3) {
            printf("Invalid log format. Skipping.\n");
            continue;
        }
        
        if (!validate_timestamp(entry->timestamp)) {
            printf("Invalid timestamp format. Skipping.\n");
            continue;
        }
        
        int severity = parse_log_level(level_buf);
        if (severity == 0) {
            printf("Invalid log level. Skipping.\n");
            continue;
        }
        
        strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        entry->severity = severity;
        
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    struct LogEntry** heap_entries = malloc(entry_count * sizeof(struct LogEntry*));
    if (heap_entries == NULL) {
        printf("Memory allocation failed.\n");
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
    
    int level_counts[6] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            level_counts[entries[i].severity]++;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    printf("DEBUG: %d\n", level_counts[1]);
    printf("INFO: %d\n", level_counts[2]);
    printf("WARNING: %d\n", level_counts[3]);
    printf("ERROR: %d\n", level_counts[4]);
    printf("CRITICAL: %d\n", level_counts[5]);
    
    printf("\nEntries sorted by severity (highest first):\n");
    for (int i = 0; i < entry_count; i++) {
        printf("%s %s %s\n", heap_entries[i]->