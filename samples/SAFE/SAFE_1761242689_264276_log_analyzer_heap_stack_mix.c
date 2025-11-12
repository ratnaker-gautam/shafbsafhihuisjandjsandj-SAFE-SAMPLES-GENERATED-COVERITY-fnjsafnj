//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t line_number;
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogCollection;

int validate_log_level(const char *level) {
    static const char *valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG", "CRITICAL"};
    for (size_t i = 0; i < 5; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int parse_timestamp(const char *timestamp) {
    struct tm tm = {0};
    return strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity < 4) new_capacity = 4;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->count < MAX_ENTRIES) {
        collection->entries[collection->count] = *entry;
        collection->count++;
        return 1;
    }
    return 0;
}

void analyze_logs(const LogCollection *collection) {
    int level_counts[5] = {0};
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG", "CRITICAL"};
    
    for (size_t i = 0; i < collection->count; i++) {
        for (int j = 0; j < 5; j++) {
            if (strcmp(collection->entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->count);
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
}

int main(void) {
    LogCollection collection = {0};
    collection.capacity = 10;
    collection.entries = malloc(collection.capacity * sizeof(LogEntry));
    if (!collection.entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char buffer[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15 10:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(buffer, sizeof(buffer), stdin)) {
        line_num++;
        if (line_num > MAX_ENTRIES) {
            printf("Maximum entries reached\n");
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed < 3) {
            printf("Invalid format at line %u\n", line_num);
            continue;
        }
        
        if (!parse_timestamp(timestamp)) {
            printf("Invalid timestamp at line %u\n", line_num);
            continue;
        }
        
        if (!validate_log_level(level)) {
            printf("Invalid log level at line %u\n", line_num);
            continue;
        }
        
        LogEntry entry;
        strncpy(entry.timestamp, timestamp, sizeof(entry.timestamp) - 1);
        strncpy(entry.level, level, sizeof(entry.level) - 1);
        strncpy(entry.message, message, sizeof(entry.message) - 1);
        entry.line_number = line_num;
        
        if (!add_log_entry(&collection, &entry)) {
            printf("Failed to add entry at line %u\n", line_num);
            break;
        }
    }
    
    if (collection.count > 0) {
        analyze_logs(&collection);
    } else {
        printf("No valid log entries to analyze\n");
    }
    
    free(collection.entries);
    return 0;
}