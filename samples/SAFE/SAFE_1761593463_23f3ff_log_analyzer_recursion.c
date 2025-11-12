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
    
    if (timestamp < 0) return 0;
    
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
    if (depth >= MAX_LEVELS) return;
    if (file == NULL) return;
    
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
    printf("Log Analyzer - Enter log entries (format: timestamp LEVEL message)\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Error creating temporary file\n");
        return 1;
    }
    
    char input[MAX_LINE_LENGTH];
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (strncmp(input, "END", 3) == 0 && (input[3] == '\n' || input[3] == '\0')) {
            break;
        }
        
        if (strlen(input) > 0) {
            if (fputs(input, temp_file) == EOF) {
                fprintf(stderr, "Error writing to temporary file\n");
                fclose(temp_file);
                return 1;
            }
        }
    }
    
    rewind(temp_file);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("\nAnalyzing log entries:\n");
    analyze_log_recursive(temp_file, 0, &error_count, &warning_count, &info_count);
    
    printf("\nSummary:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    fclose(temp_file);
    return 0;
}