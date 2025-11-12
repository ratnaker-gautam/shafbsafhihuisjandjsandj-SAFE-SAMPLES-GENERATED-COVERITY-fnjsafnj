//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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
} LogHeap;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level_buf[16];
    char timestamp_buf[32];
    char message_buf[256];
    
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

void log_heap_init(LogHeap *heap) {
    if (!heap) return;
    heap->entries = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

int log_heap_grow(LogHeap *heap) {
    if (!heap) return 0;
    
    size_t new_capacity = heap->capacity == 0 ? 16 : heap->capacity * 2;
    if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
    
    LogEntry *new_entries = realloc(heap->entries, new_capacity * sizeof(LogEntry));
    if (!new_entries) return 0;
    
    heap->entries = new_entries;
    heap->capacity = new_capacity;
    return 1;
}

int log_heap_add(LogHeap *heap, const LogEntry *entry) {
    if (!heap || !entry || heap->size >= MAX_ENTRIES) return 0;
    
    if (heap->size >= heap->capacity) {
        if (!log_heap_grow(heap)) return 0;
    }
    
    if (heap->size < heap->capacity) {
        heap->entries[heap->size] = *entry;
        heap->size++;
        return 1;
    }
    
    return 0;
}

void log_heap_free(LogHeap *heap) {
    if (heap && heap->entries) {
        free(heap->entries);
        heap->entries = NULL;
        heap->size = 0;
        heap->capacity = 0;
    }
}

void analyze_logs(LogHeap *heap) {
    if (!heap || heap->size == 0) return;
    
    uint32_t error_count = 0;
    uint32_t warning_count = 0;
    uint32_t info_count = 0;
    
    for (size_t i = 0; i < heap->size; i++) {
        const char *level = heap->entries[i].level;
        if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "WARNING") == 0) warning_count++;
        else if (strcmp(level, "INFO") == 0) info_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", heap->size);
    printf("ERROR entries: %u\n", error_count);
    printf("WARNING entries: %u\n", warning_count);
    printf("INFO entries: %u\n", info_count);
    
    if (heap->size > 0) {
        printf("\nFirst entry: %s %s %s\n", 
               heap->entries[0].timestamp, 
               heap->entries[0].level, 
               heap->entries[0].message);
        printf("Last entry: %s %s %s\n", 
               heap->entries[heap->size-1].timestamp, 
               heap->entries[heap->size-1].level, 
               heap->entries[heap->size-1].message);
    }
}

int main(void) {
    LogHeap heap;
    log_heap_init(&heap);
    
    LogEntry stack_entries[10];
    size_t stack_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LEN];
    
    while (fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (stack_count < 10) {
                stack_entries[stack_count] = entry;
                stack_count++;
            }
            
            if (!log_heap_add(&heap, &entry)) {
                printf