//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[256];
} LogEntry;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    int count;
} LogBuffer;

int parse_timestamp(const char* ts, struct tm* tm) {
    return sscanf(ts, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 ||
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

int read_log_entry(FILE* file, LogEntry* entry) {
    char line[MAX_LINE_LENGTH];
    if (!fgets(line, sizeof(line), file)) return 0;
    
    char ts[20], lvl[10], msg[256];
    if (sscanf(line, "%19s %9s %255[^\n]", ts, lvl, msg) != 3) return 0;
    
    struct tm tm;
    if (!parse_timestamp(ts, &tm)) return 0;
    if (!is_valid_level(lvl)) return 0;
    
    strncpy(entry->timestamp, ts, sizeof(entry->timestamp)-1);
    strncpy(entry->level, lvl, sizeof(entry->level)-1);
    strncpy(entry->message, msg, sizeof(entry->message)-1);
    
    entry->timestamp[sizeof(entry->timestamp)-1] = '\0';
    entry->level[sizeof(entry->level)-1] = '\0';
    entry->message[sizeof(entry->message)-1] = '\0';
    
    return 1;
}

void analyze_logs(LogBuffer* buffer) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        const char* level = buffer->entries[i].level;
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    printf("Total entries: %d\n", buffer->count);
}

int main() {
    LogBuffer* buffer = malloc(sizeof(LogBuffer));
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    buffer->count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char input[MAX_LINE_LENGTH];
    while (buffer->count < MAX_ENTRIES) {
        printf("Log entry %d: ", buffer->count + 1);
        if (!fgets(input, sizeof(input), stdin)) break;
        
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "END") == 0) break;
        
        FILE* temp = tmpfile();
        if (!temp) continue;
        
        fputs(input, temp);
        rewind(temp);
        
        LogEntry entry;
        if (read_log_entry(temp, &entry)) {
            buffer->entries[buffer->count] = entry;
            buffer->count++;
        }
        
        fclose(temp);
    }
    
    if (buffer->count > 0) {
        analyze_logs(buffer);
        
        printf("\nRecent entries:\n");
        int start = buffer->count > 5 ? buffer->count - 5 : 0;
        for (int i = start; i < buffer->count; i++) {
            printf("%s %s %s\n", buffer->entries[i].timestamp,
                   buffer->entries[i].level, buffer->entries[i].message);
        }
    } else {
        printf("No valid log entries found\n");
    }
    
    free(buffer);
    return 0;
}