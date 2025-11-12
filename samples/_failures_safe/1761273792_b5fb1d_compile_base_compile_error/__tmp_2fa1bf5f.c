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
    LogEntry* entries;
    size_t count;
    size_t capacity;
} LogBuffer;

LogBuffer* create_log_buffer(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    
    LogBuffer* buffer = malloc(sizeof(LogBuffer));
    if (!buffer) return NULL;
    
    buffer->entries = malloc(sizeof(LogEntry) * initial_capacity);
    if (!buffer->entries) {
        free(buffer);
        return NULL;
    }
    
    buffer->count = 0;
    buffer->capacity = initial_capacity;
    return buffer;
}

void destroy_log_buffer(LogBuffer* buffer) {
    if (buffer) {
        free(buffer->entries);
        free(buffer);
    }
}

int add_log_entry(LogBuffer* buffer, const char* timestamp, const char* level, const char* message) {
    if (!buffer || !timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    if (buffer->count >= buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity <= buffer->capacity) return 0;
        
        LogEntry* new_entries = realloc(buffer->entries, sizeof(LogEntry) * new_capacity);
        if (!new_entries) return 0;
        
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    LogEntry* entry = &buffer->entries[buffer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    buffer->count++;
    return 1;
}

void analyze_log_levels(LogBuffer* buffer) {
    if (!buffer || buffer->count == 0) return;
    
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (size_t i = 0; i < buffer->count; i++) {
        const char* level = buffer->entries[i].level;
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARNING") == 0) warning_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO: %d entries\n", info_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("ERROR: %d entries\n", error_count);
    printf("Total: %zu entries\n", buffer->count);
}

void generate_sample_logs(LogBuffer* buffer) {
    if (!buffer) return;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "High memory usage detected",
        "Network latency above threshold",
        "Failed to write to disk",
        "Backup completed",
        "Security scan finished"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    
    for (int i = 0; i < 8; i++) {
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
        
        const char* level = levels[i % 3];
        const char* message = messages[i];
        
        if (!add_log_entry(buffer, timestamp, level, message)) {
            printf("Failed to add log entry %d\n", i);
            break;
        }
        
        tm_info.tm_min += 5;
    }
}

int main(void) {
    LogBuffer stack_buffer;
    stack_buffer.entries = malloc(sizeof(LogEntry) * 10);
    if (!stack_buffer.entries) {
        printf("Failed to allocate stack buffer\n");
        return 1;
    }
    stack_buffer.count = 0;
    stack_buffer.capacity = 10;
    
    LogBuffer* heap_buffer = create_log_buffer(20);
    if (!heap_buffer) {
        printf("Failed to create heap buffer\n");
        free(stack_buffer.entries);
        return 1;
    }
    
    printf("Generating sample logs...\n");
    generate_sample_logs(&stack_buffer);
    generate_sample_logs(heap_buffer);
    
    printf("\nStack Buffer Analysis:\n");
    analyze_log_levels(&stack_buffer);
    
    printf("\nHeap Buffer Analysis:\n");
    analyze_log_levels(heap_buffer);
    
    printf("\nCombined Analysis:\n");
    LogBuffer combined;
    combined.count = 0;
    combined.capacity = stack_buffer.count + heap_buffer->count;
    combined.entries = malloc(sizeof(LogEntry) * combined.capacity);
    if (combined.entries) {
        for (