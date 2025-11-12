//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
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
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
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
            if (timestamp[i] < '0' || timestamp[i] > '9') return 0;
        }
    }
    return 1;
}

int add_log_entry(LogCollection* collection, const char* timestamp, const char* level, const char* message) {
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity <= 0) new_capacity = 10;
        LogEntry* new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }

    LogEntry* entry = &collection->entries[collection->count];
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    if (entry->severity == -1) return 0;
    
    collection->count++;
    return 1;
}

void analyze_logs(LogCollection* collection) {
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < collection->count; i++) {
        if (collection->entries[i].severity >= 0 && collection->entries[i].severity < 5) {
            severity_count[collection->entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", severity_names[i], severity_count[i]);
    }
}

int main(void) {
    LogCollection collection = {0};
    char line[MAX_LINE_LENGTH];
    int sample_count = 0;
    
    printf("Enter number of log entries to analyze (1-100): ");
    if (scanf("%d", &sample_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (sample_count < 1 || sample_count > 100) {
        printf("Sample count must be between 1 and 100\n");
        return 1;
    }
    
    getchar();
    
    collection.capacity = sample_count;
    collection.entries = malloc(collection.capacity * sizeof(LogEntry));
    if (!collection.entries) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter log entries in format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
    
    for (int i = 0; i < sample_count; i++) {
        printf("Entry %d: ", i + 1);
        
        if (!fgets(line, sizeof(line), stdin)) {
            printf("Input error\n");
            free(collection.entries);
            return 1;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        char timestamp[32], level[16], message[256];
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE\n");
            i--;
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
            i--;
            continue;
        }
        
        if