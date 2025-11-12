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
    char message[MAX_LINE_LENGTH];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 19) return 0;
    if (strlen(level) >= 9) return 0;
    if (strlen(message) >= MAX_LINE_LENGTH - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS || file == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) != NULL) {
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
        analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
    }
}

int main() {
    printf("Log Analyzer - Enter log data (empty line to finish):\n");
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        printf("Error: Cannot create temporary file\n");
        return 1;
    }
    
    char input_line[MAX_LINE_LENGTH];
    while (1) {
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) break;
        
        if (strlen(input_line) == 1 && input_line[0] == '\n') break;
        
        if (strlen(input_line) >= MAX_LINE_LENGTH - 1) {
            printf("Error: Line too long\n");
            fclose(temp_file);
            return 1;
        }
        
        if (fputs(input_line, temp_file) == EOF) {
            printf("Error: Cannot write to temporary file\n");
            fclose(temp_file);
            return 1;
        }
    }
    
    rewind(temp_file);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(temp_file, 0, &error_count, &warning_count, &info_count);
    
    printf("\nLog Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", error_count + warning_count + info_count);
    
    fclose(temp_file);
    return 0;
}