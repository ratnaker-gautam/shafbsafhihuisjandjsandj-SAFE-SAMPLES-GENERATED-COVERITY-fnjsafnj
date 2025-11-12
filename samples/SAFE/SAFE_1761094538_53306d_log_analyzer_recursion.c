//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp, line);
    
    char* token = strtok(temp, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= 19) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= 9) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS || file == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return;
    
    LogEntry entry;
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

int main() {
    printf("Log Analyzer - Enter log entries (timestamp level message):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input\n\n");
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        printf("Error: Cannot create temporary file\n");
        return 1;
    }
    
    char input[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (strncmp(input, "END", 3) == 0 && (input[3] == '\n' || input[3] == '\0')) {
            break;
        }
        
        if (line_count < 1000) {
            if (fputs(input, temp_file) == EOF) {
                printf("Error: Failed to write to temporary file\n");
                fclose(temp_file);
                return 1;
            }
            line_count++;
        } else {
            printf("Warning: Maximum log entries reached\n");
            break;
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