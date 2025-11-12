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
    int line_number;
};

int parse_log_line(const char* line, struct LogEntry* entry, int line_num) {
    if (!line || !entry) return 0;
    
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    int scanned = sscanf(line, "%19s %9s %[^\n]", 
                        entry->timestamp, entry->level, entry->message);
    
    if (scanned != 3) return 0;
    
    for (int i = 0; entry->level[i]; i++) {
        entry->level[i] = toupper((unsigned char)entry->level[i]);
    }
    
    entry->line_number = line_num;
    return 1;
}

void analyze_log_recursive(FILE* file, int* error_count, int* warning_count, 
                          int* info_count, int current_line, int max_lines) {
    if (!file || !error_count || !warning_count || !info_count) return;
    if (current_line >= max_lines) return;
    if (feof(file)) return;
    
    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) return;
    
    struct LogEntry entry;
    if (parse_log_line(buffer, &entry, current_line + 1)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
        }
    }
    
    analyze_log_recursive(file, error_count, warning_count, info_count, 
                         current_line + 1, max_lines);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    int line_count = 0;
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_count++;
        if (line_count > 10000) {
            fprintf(stderr, "Error: File too large\n");
            fclose(file);
            return 1;
        }
    }
    
    if (line_count == 0) {
        printf("No log entries found.\n");
        fclose(file);
        return 0;
    }
    
    rewind(file);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(file, &error_count, &warning_count, &info_count, 0, line_count);
    
    printf("Log Analysis Results:\n");
    printf("Total lines processed: %d\n", line_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Other entries: %d\n", line_count - (error_count + warning_count + info_count));
    
    fclose(file);
    return 0;
}