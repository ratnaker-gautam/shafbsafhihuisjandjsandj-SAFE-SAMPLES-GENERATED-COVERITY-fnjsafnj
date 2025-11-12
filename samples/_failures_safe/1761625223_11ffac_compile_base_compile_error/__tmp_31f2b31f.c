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
    uint32_t count;
} LogEntry;

typedef struct {
    LogEntry* entries;
    size_t size;
    size_t capacity;
} LogHeap;

void init_log_heap(LogHeap* heap, size_t capacity) {
    if (capacity == 0 || capacity > MAX_ENTRIES) capacity = MAX_ENTRIES;
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    heap->size = 0;
    heap->capacity = capacity;
}

void free_log_heap(LogHeap* heap) {
    if (heap->entries != NULL) {
        free(heap->entries);
        heap->entries = NULL;
    }
    heap->size = 0;
    heap->capacity = 0;
}

int add_log_entry(LogHeap* heap, const char* timestamp, const char* level, const char* message) {
    if (heap->size >= heap->capacity) return 0;
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    size_t ts_len = strlen(timestamp);
    size_t lvl_len = strlen(level);
    size_t msg_len = strlen(message);
    
    if (ts_len >= 32 || lvl_len >= 16 || msg_len >= 256) return 0;
    
    LogEntry* entry = &heap->entries[heap->size];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->count = 1;
    
    heap->size++;
    return 1;
}

void analyze_logs(LogHeap* heap) {
    if (heap->size == 0) {
        printf("No logs to analyze\n");
        return;
    }
    
    uint32_t error_count = 0;
    uint32_t warning_count = 0;
    uint32_t info_count = 0;
    
    for (size_t i = 0; i < heap->size; i++) {
        const LogEntry* entry = &heap->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count += entry->count;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count += entry->count;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count += entry->count;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("  INFO: %u entries\n", info_count);
    printf("  WARNING: %u entries\n", warning_count);
    printf("  ERROR: %u entries\n", error_count);
    printf("  Total: %zu unique entries\n", heap->size);
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || 
            strcmp(level, "WARNING") == 0 || 
            strcmp(level, "ERROR") == 0);
}

int parse_log_line(const char* line, char* timestamp, char* level, char* message) {
    if (line == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* tokens[3];
    char* token = strtok(temp_line, "|");
    int token_count = 0;
    
    while (token != NULL && token_count < 3) {
        while (*token == ' ') token++;
        char* end = token + strlen(token) - 1;
        while (end > token && *end == ' ') end--;
        *(end + 1) = '\0';
        
        tokens[token_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (token_count != 3) return 0;
    if (strlen(tokens[0]) >= 32 || strlen(tokens[1]) >= 16 || strlen(tokens[2]) >= 256) return 0;
    if (!is_valid_level(tokens[1])) return 0;
    
    strncpy(timestamp, tokens[0], 31);
    timestamp[31] = '\0';
    strncpy(level, tokens[1], 15);
    level[15] = '\0';
    strncpy(message, tokens[2], 255);
    message[255] = '\0';
    
    return 1;
}

int main(void) {
    LogHeap heap;
    init_log_heap(&heap, 50);
    
    char line[MAX_LINE