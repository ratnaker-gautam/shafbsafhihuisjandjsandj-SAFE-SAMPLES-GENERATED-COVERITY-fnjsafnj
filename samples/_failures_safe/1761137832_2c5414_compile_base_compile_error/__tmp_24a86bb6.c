//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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
} LogBuffer;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char *token = strtok(temp, " ");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

int init_log_buffer(LogBuffer *buffer, size_t initial_capacity) {
    if (!buffer || initial_capacity == 0) return 0;
    
    buffer->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!buffer->entries) return 0;
    
    buffer->count = 0;
    buffer->capacity = initial_capacity;
    return 1;
}

int add_to_log_buffer(LogBuffer *buffer, const LogEntry *entry) {
    if (!buffer || !entry) return 0;
    
    if (buffer->count >= buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity <= buffer->capacity) return 0;
        
        LogEntry *new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }
    
    if (buffer->count < buffer->capacity) {
        buffer->entries[buffer->count] = *entry;
        buffer->count++;
        return 1;
    }
    
    return 0;
}

void count_log_levels(const LogBuffer *buffer, int *error_count, int *warning_count, int *info_count) {
    if (!buffer || !error_count || !warning_count || !info_count) return;
    
    *error_count = 0;
    *warning_count = 0;
    *info_count = 0;
    
    for (size_t i = 0; i < buffer->count; i++) {
        if (strcmp(buffer->entries[i].level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(buffer->entries[i].level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(buffer->entries[i].level, "INFO") == 0) {
            (*info_count)++;
        }
    }
}

void free_log_buffer(LogBuffer *buffer) {
    if (buffer && buffer->entries) {
        free(buffer->entries);
        buffer->entries = NULL;
        buffer->count = 0;
        buffer->capacity = 0;
    }
}

int main() {
    LogBuffer buffer;
    if (!init_log_buffer(&buffer, 10)) {
        fprintf(stderr, "Failed to initialize log buffer\n");
        return 1;
    }
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entry limit reached\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (!add_to_log_buffer(&buffer, &entry)) {
                printf("Failed to add log entry to buffer\n");
                break;
            }
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries processed: %zu\n", buffer.count);
    
    int error_count, warning_count, info_count;
    count_log_levels(&buffer, &error_count, &warning_count, &info_count);
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    free