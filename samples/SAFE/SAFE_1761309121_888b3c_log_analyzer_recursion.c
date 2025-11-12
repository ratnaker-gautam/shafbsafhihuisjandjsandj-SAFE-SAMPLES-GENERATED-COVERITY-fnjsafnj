//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
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
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_end = strchr(temp, ' ');
    if (!timestamp_end) return 0;
    *timestamp_end = '\0';
    
    if (strlen(temp) >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, temp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    char* level_start = timestamp_end + 1;
    char* level_end = strchr(level_start, ' ');
    if (!level_end) return 0;
    *level_end = '\0';
    
    if (strlen(level_start) >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    char* message_start = level_end + 1;
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_log_recursive(FILE* file, int* error_count, int* warning_count, int* info_count, int depth) {
    if (!file || depth >= MAX_LEVELS) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file)) {
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (strcmp(entry.level, "ERROR") == 0) {
                if (*error_count < 1000) (*error_count)++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                if (*warning_count < 1000) (*warning_count)++;
            } else if (strcmp(entry.level, "INFO") == 0) {
                if (*info_count < 1000) (*info_count)++;
            }
        }
        analyze_log_recursive(file, error_count, warning_count, info_count, depth + 1);
    }
}

int main() {
    printf("Log Analyzer - Enter log data (empty line to finish):\n");
    
    FILE* temp_file = tmpfile();
    if (!temp_file) {
        printf("Error: Cannot create temporary file\n");
        return 1;
    }
    
    char input_line[MAX_LINE_LENGTH];
    while (1) {
        if (!fgets(input_line, sizeof(input_line), stdin)) break;
        
        if (input_line[0] == '\n' || input_line[0] == '\0') break;
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (strlen(input_line) == 0) break;
        
        if (fprintf(temp_file, "%s\n", input_line) < 0) {
            printf("Error: Cannot write to temporary file\n");
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