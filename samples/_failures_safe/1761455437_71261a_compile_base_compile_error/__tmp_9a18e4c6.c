//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t count;
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t size;
    size_t capacity;
} LogCollection;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level_buf[16];
    char message_buf[256];
    char timestamp_buf[32];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                       timestamp_buf, level_buf, message_buf);
    
    if (parsed != 3) return 0;
    
    if (strlen(timestamp_buf) >= sizeof(entry->timestamp) ||
        strlen(level_buf) >= sizeof(entry->level) ||
        strlen(message_buf) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp_buf);
    strcpy(entry->level, level_buf);
    strcpy(entry->message, message_buf);
    entry->count = 1;
    
    return 1;
}

LogCollection* create_log_collection(size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_ENTRIES) {
        return NULL;
    }
    
    LogCollection *collection = malloc(sizeof(LogCollection));
    if (!collection) return NULL;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) {
        free(collection);
        return NULL;
    }
    
    collection->size = 0;
    collection->capacity = initial_capacity;
    return collection;
}

void destroy_log_collection(LogCollection *collection) {
    if (collection) {
        free(collection->entries);
        free(collection);
    }
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry || collection->size >= MAX_ENTRIES) {
        return 0;
    }
    
    if (collection->size >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry *new_entries = realloc(collection->entries, 
                                       new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->size < collection->capacity) {
        collection->entries[collection->size] = *entry;
        collection->size++;
        return 1;
    }
    
    return 0;
}

void analyze_logs(LogCollection *collection) {
    if (!collection || collection->size == 0) return;
    
    uint32_t error_count = 0;
    uint32_t warning_count = 0;
    uint32_t info_count = 0;
    
    for (size_t i = 0; i < collection->size; i++) {
        const LogEntry *entry = &collection->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count += entry->count;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count += entry->count;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count += entry->count;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->size);
    printf("ERROR count: %u\n", error_count);
    printf("WARNING count: %u\n", warning_count);
    printf("INFO count: %u\n", info_count);
    
    if (collection->size > 0) {
        printf("First timestamp: %s\n", collection->entries[0].timestamp);
        printf("Last timestamp: %s\n", collection->entries[collection->size-1].timestamp);
    }
}

int main(void) {
    LogCollection *collection = create_log_collection(10);
    if (!collection) {
        fprintf(stderr, "Failed to create log collection\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    LogEntry stack_entry;
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &stack_entry)) {
            if (!add_log_entry(collection, &stack_entry)) {
                fprintf(stderr, "Failed to add log entry\n");
                break;
            }
        } else {
            fprintf(stderr, "Failed to parse log line: %s\n", line);
        }
        
        if (collection->size >= MAX_ENTRIES) {