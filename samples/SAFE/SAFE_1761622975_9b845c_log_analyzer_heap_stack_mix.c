//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

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
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
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
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_file(const char* filename, LogBuffer* buffer) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    buffer->count = 0;
    
    while (fgets(line, sizeof(line), file) && buffer->count < MAX_ENTRIES) {
        if (strlen(line) >= MAX_LINE_LENGTH - 1) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!validate_timestamp(timestamp)) continue;
        
        int severity = parse_severity(level);
        if (severity == 0) continue;
        
        LogEntry* entry = &buffer->entries[buffer->count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->severity = severity;
        
        buffer->count++;
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(LogBuffer* buffer) {
    if (buffer->count == 0) {
        printf("No valid log entries found.\n");
        return;
    }
    
    int severity_count[6] = {0};
    char latest_timestamp[32] = {0};
    
    for (int i = 0; i < buffer->count; i++) {
        LogEntry* entry = &buffer->entries[i];
        if (entry->severity >= 1 && entry->severity <= 5) {
            severity_count[entry->severity]++;
        }
        if (i == 0 || strcmp(entry->timestamp, latest_timestamp) > 0) {
            strncpy(latest_timestamp, entry->timestamp, sizeof(latest_timestamp) - 1);
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("DEBUG: %d\n", severity_count[1]);
    printf("INFO: %d\n", severity_count[2]);
    printf("WARNING: %d\n", severity_count[3]);
    printf("ERROR: %d\n", severity_count[4]);
    printf("CRITICAL: %d\n", severity_count[5]);
    printf("Latest timestamp: %s\n", latest_timestamp);
}

int main() {
    char filename[256];
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
    
    LogBuffer* buffer = malloc(sizeof(LogBuffer));
    if (!buffer) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    if (!read_log_file(filename, buffer)) {
        printf("Error reading log file: %s\n", filename);
        free(buffer);
        return 1;
    }
    
    analyze_logs(buffer);
    
    free(buffer);
    return 0;
}