//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

typedef struct {
    LogEntry *entries;
    int count;
    int capacity;
} LogCollection;

int parse_severity(const char *level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int validate_timestamp(const char *timestamp) {
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

int add_log_entry(LogCollection *collection, const char *timestamp, const char *level, const char *message) {
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity <= 0) new_capacity = 16;
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }

    LogEntry *entry = &collection->entries[collection->count];
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }

    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->severity = parse_severity(level);
    if (entry->severity == 0) return 0;
    
    collection->count++;
    return 1;
}

void analyze_logs(const LogCollection *collection) {
    int severity_count[6] = {0};
    char latest_timestamp[32] = "";
    
    for (int i = 0; i < collection->count; i++) {
        const LogEntry *entry = &collection->entries[i];
        if (entry->severity >= 1 && entry->severity <= 5) {
            severity_count[entry->severity]++;
        }
        if (strcmp(entry->timestamp, latest_timestamp) > 0) {
            strncpy(latest_timestamp, entry->timestamp, sizeof(latest_timestamp) - 1);
            latest_timestamp[sizeof(latest_timestamp) - 1] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    printf("DEBUG: %d\n", severity_count[1]);
    printf("INFO: %d\n", severity_count[2]);
    printf("WARNING: %d\n", severity_count[3]);
    printf("ERROR: %d\n", severity_count[4]);
    printf("CRITICAL: %d\n", severity_count[5]);
    printf("Latest timestamp: %s\n", latest_timestamp);
}

int main(void) {
    LogCollection collection = {0};
    collection.capacity = 16;
    collection.entries = malloc(collection.capacity * sizeof(LogEntry));
    if (!collection.entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entry limit reached\n");
            break;
        }
        
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }