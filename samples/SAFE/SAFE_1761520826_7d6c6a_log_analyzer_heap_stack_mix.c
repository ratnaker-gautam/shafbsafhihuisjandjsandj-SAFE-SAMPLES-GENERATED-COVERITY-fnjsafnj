//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

typedef struct {
    LogEntry* entries;
    int count;
    int capacity;
} LogCollection;

int parse_severity(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int is_valid_timestamp(const char* ts) {
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

int add_log_entry(LogCollection* collection, const char* line) {
    if (collection->count >= collection->capacity) {
        int new_cap = collection->capacity * 2;
        if (new_cap <= 0) new_cap = 10;
        LogEntry* new_entries = realloc(collection->entries, new_cap * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_cap;
    }

    LogEntry entry;
    memset(&entry, 0, sizeof(LogEntry));
    
    int scanned = sscanf(line, "%63s %15s %255[^\n]", 
                        entry.timestamp, entry.level, entry.message);
    if (scanned != 3) return 0;
    
    if (!is_valid_timestamp(entry.timestamp)) return 0;
    
    entry.severity = parse_severity(entry.level);
    if (entry.severity < 0) return 0;
    
    collection->entries[collection->count] = entry;
    collection->count++;
    return 1;
}

void analyze_logs(const LogCollection* collection) {
    int severity_count[4] = {0};
    int total_entries = collection->count;
    
    if (total_entries == 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    for (int i = 0; i < total_entries; i++) {
        int sev = collection->entries[i].severity;
        if (sev >= 0 && sev <= 3) {
            severity_count[sev]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", total_entries);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARN: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    
    int error_percent = (severity_count[3] * 100) / total_entries;
    printf("Error percentage: %d%%\n", error_percent);
}

int main(void) {
    LogCollection logs;
    logs.entries = NULL;
    logs.count = 0;
    logs.capacity = 0;
    
    char input_buffer[MAX_LINE_LEN];
    int entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARN, ERROR\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        
        if (strcmp(input_buffer, "END") == 0) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        if (add_log_entry(&logs, input_buffer)) {
            entry_count++;
            printf("Added entry %d\n", entry_count);
        } else {
            printf("Invalid log format. Please use: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
        }
    }
    
    analyze_logs(&logs);
    
    if (logs.entries) {
        free(logs.entries);
    }
    
    return 0;
}