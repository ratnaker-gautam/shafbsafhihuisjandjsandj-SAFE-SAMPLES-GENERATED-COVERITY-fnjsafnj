//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 19) return 0;
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    
    if (strlen(level) >= 9) return 0;
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    
    if (strlen(message) >= MAX_LINE_LENGTH - 30) return 0;
    strncpy(entry->message, message, MAX_LINE_LENGTH - 31);
    entry->message[MAX_LINE_LENGTH - 31] = '\0';
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (!file || depth >= MAX_LEVELS) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return;
    
    line[MAX_LINE_LENGTH - 1] = '\0';
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            if (*error_count < INT_MAX) (*error_count)++;
        } else if (strcmp(entry.level, "WARNING") == 0) {
            if (*warning_count < INT_MAX) (*warning_count)++;
        } else if (strcmp(entry.level, "INFO") == 0) {
            if (*info_count < INT_MAX) (*info_count)++;
        }
    }
    
    analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
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
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) break;
        
        input_line[MAX_LINE_LENGTH - 1] = '\0';
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
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
    
    analyze_log_recursive(temp_file, 0, &error_count, &warning_count, &info_count);
    
    fclose(temp_file);
    
    printf("\nLog Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", error_count + warning_count + info_count);
    
    return 0;
}