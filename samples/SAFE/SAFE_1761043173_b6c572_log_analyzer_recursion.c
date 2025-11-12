//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    int timestamp_end = -1;
    int level_end = -1;
    
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == ' ' && timestamp_end == -1) {
            if (i > 19) return 0;
            strncpy(entry->timestamp, line, i);
            entry->timestamp[i] = '\0';
            timestamp_end = i;
        } else if (line[i] == ' ' && level_end == -1 && timestamp_end != -1) {
            int level_start = timestamp_end + 1;
            if (i - level_start > 9) return 0;
            strncpy(entry->level, line + level_start, i - level_start);
            entry->level[i - level_start] = '\0';
            level_end = i;
            break;
        }
    }
    
    if (level_end == -1) return 0;
    
    int msg_start = level_end + 1;
    int msg_len = strlen(line) - msg_start;
    if (msg_len <= 0 || msg_len >= MAX_LINE_LENGTH - 30) return 0;
    
    strncpy(entry->message, line + msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

void analyze_log_recursive(FILE* file, int* error_count, int* warning_count, int* info_count, int depth) {
    if (!file || depth >= MAX_LEVELS) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return;
    
    line[strcspn(line, "\n")] = '\0';
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
        }
    }
    
    analyze_log_recursive(file, error_count, warning_count, info_count, depth + 1);
}

int main(void) {
    printf("Log Analyzer - Enter log entries (one per line):\n");
    printf("Format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input.\n\n");
    
    FILE* temp_file = tmpfile();
    if (!temp_file) {
        fprintf(stderr, "Error: Cannot create temporary file\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (fprintf(temp_file, "%s\n", line) < 0) {
            fprintf(stderr, "Error: Cannot write to temporary file\n");
            fclose(temp_file);
            return 1;
        }
    }
    
    rewind(temp_file);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(temp_file, &error_count, &warning_count, &info_count, 0);
    
    fclose(temp_file);
    
    printf("\nLog Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", error_count + warning_count + info_count);
    
    return 0;
}