//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[256];
    int timestamp;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[16];
    char message_buf[256];
    int timestamp;
    
    int parsed = sscanf(line, "%d %15s %255[^\n]", &timestamp, level_buf, message_buf);
    if (parsed != 3) return 0;
    
    if (timestamp < 0 || timestamp > 999999999) return 0;
    
    for (int i = 0; level_buf[i]; i++) {
        if (!isalpha(level_buf[i])) return 0;
    }
    
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->timestamp = timestamp;
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS || file == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return;
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
        }
        
        printf("Depth %d: %s - %s\n", depth, entry.level, entry.message);
    }
    
    analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
}

int main(void) {
    const char* sample_logs[] = {
        "1000 INFO System started successfully",
        "1005 WARNING Low disk space detected",
        "1010 ERROR Database connection failed",
        "1015 INFO User login successful",
        "1020 WARNING High memory usage",
        "1025 ERROR File not found",
        "1030 INFO Backup completed",
        NULL
    };
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Failed to create temporary file\n");
        return 1;
    }
    
    for (int i = 0; sample_logs[i] != NULL; i++) {
        if (fprintf(temp_file, "%s\n", sample_logs[i]) < 0) {
            fprintf(stderr, "Failed to write to temporary file\n");
            fclose(temp_file);
            return 1;
        }
    }
    
    if (fseek(temp_file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Failed to rewind temporary file\n");
        fclose(temp_file);
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("Log Analysis Results:\n");
    analyze_log_recursive(temp_file, 0, &error_count, &warning_count, &info_count);
    
    printf("\nSummary:\n");
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
    printf("Info: %d\n", info_count);
    
    fclose(temp_file);
    return 0;
}