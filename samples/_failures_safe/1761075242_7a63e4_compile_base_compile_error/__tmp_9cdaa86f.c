//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define MAX_FILENAME 256

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    int count;
} LogBuffer;

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

int read_log_file(const char* filename, LogBuffer* buffer) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    buffer->count = 0;
    
    while (fgets(line, sizeof(line), file) && buffer->count < MAX_ENTRIES) {
        if (strlen(line) >= sizeof(line) - 1) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!is_valid_timestamp(timestamp)) continue;
        
        int severity = parse_severity(level);
        if (severity == -1) continue;
        
        LogEntry* entry = &buffer->entries[buffer->count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->severity = severity;
        buffer->count++;
    }
    
    fclose(file);
    return buffer->count;
}

void analyze_logs(LogBuffer* buffer) {
    if (buffer->count == 0) {
        printf("No valid log entries found.\n");
        return;
    }
    
    int severity_count[4] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        LogEntry* entry = &buffer->entries[i];
        if (entry->severity >= 0 && entry->severity <= 3) {
            severity_count[entry->severity]++;
            total_severity += entry->severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARN: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    
    if (buffer->count > 0) {
        double avg_severity = (double)total_severity / buffer->count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

int main() {
    char filename[MAX_FILENAME];
    LogBuffer stack_buffer;
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename.\n");
        return 1;
    }
    
    int result = read_log_file(filename, &stack_buffer);
    if (result < 0) {
        printf("Error opening or reading file: %s\n", filename);
        return 1;
    }
    
    if (result == 0) {
        printf("No valid log entries found in file.\n");
        return 0;
    }
    
    analyze_logs(&stack_buffer);
    
    LogBuffer* heap_buffer = malloc(sizeof(LogBuffer));
    if (!heap_buffer) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    memcpy(heap_buffer, &stack_buffer, sizeof(LogBuffer));
    
    printf("\nHeap