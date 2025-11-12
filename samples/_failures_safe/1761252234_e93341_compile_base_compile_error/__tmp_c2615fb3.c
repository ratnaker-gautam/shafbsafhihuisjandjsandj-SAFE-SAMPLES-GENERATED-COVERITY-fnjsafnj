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
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogCollection;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level[16];
    char timestamp[32];
    char message[256];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

LogCollection *create_log_collection(size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_ENTRIES) return NULL;
    
    LogCollection *collection = malloc(sizeof(LogCollection));
    if (!collection) return NULL;
    
    collection->entries = malloc(sizeof(LogEntry) * initial_capacity);
    if (!collection->entries) {
        free(collection);
        return NULL;
    }
    
    collection->count = 0;
    collection->capacity = initial_capacity;
    return collection;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry || collection->count >= MAX_ENTRIES) return 0;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry *new_entries = realloc(collection->entries, sizeof(LogEntry) * new_capacity);
        if (!new_entries) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->count < collection->capacity) {
        collection->entries[collection->count] = *entry;
        collection->count++;
        return 1;
    }
    
    return 0;
}

void free_log_collection(LogCollection *collection) {
    if (collection) {
        free(collection->entries);
        free(collection);
    }
}

void analyze_logs(const LogCollection *collection) {
    if (!collection || collection->count == 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
        const LogEntry *entry = &collection->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (collection->count > 0) {
        printf("\nFirst entry: %s %s %s\n", 
               collection->entries[0].timestamp,
               collection->entries[0].level,
               collection->entries[0].message);
        
        printf("Last entry: %s %s %s\n", 
               collection->entries[collection->count - 1].timestamp,
               collection->entries[collection->count - 1].level,
               collection->entries[collection->count - 1].message);
    }
}

int main() {
    LogEntry stack_entries[5];
    LogCollection *heap_collection = create_log_collection(10);
    
    if (!heap_collection) {
        fprintf(stderr, "Failed to create log collection\n");
        return 1;
    }
    
    const char *sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARNING High memory usage detected",
        "2024-01-15T10:32:30 ERROR Database connection failed",
        "2024-01-15T10:33:45 INFO User login successful",
        "2024-01-15T10:34:20 ERROR File not found"
    };
    
    for (int i = 0; i < 5; i++) {
        if (parse_log_line(sample_logs[i], &stack_entries[i])) {
            if (!add_log_entry(heap_collection, &stack_entries[i])) {
                fprintf(stderr, "Failed to add log entry\n");
                free_log_collection(heap_collection);
                return 1;
            }
        }
    }
    
    analyze_logs(heap_collection);
    free_log_collection(heap_collection