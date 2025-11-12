//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

typedef struct {
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int count;
} LogData;

typedef void (*LogProcessor)(const char* line);

void count_lines(const char* line) {
    static int line_count = 0;
    line_count++;
    printf("Processed line %d: %s\n", line_count, line);
}

void find_errors(const char* line) {
    if (strstr(line, "ERROR") != NULL || strstr(line, "error") != NULL) {
        printf("Found error: %s\n", line);
    }
}

void extract_timestamps(const char* line) {
    const char* ptr = line;
    while (*ptr) {
        if (isdigit(*ptr) && isdigit(*(ptr+1)) && *(ptr+2) == ':' && 
            isdigit(*(ptr+3)) && isdigit(*(ptr+4)) && *(ptr+5) == ':') {
            printf("Found timestamp: ");
            for (int i = 0; i < 8 && *(ptr+i); i++) {
                putchar(*(ptr+i));
            }
            printf(" in line: %s\n", line);
            break;
        }
        ptr++;
    }
}

int read_log_file(LogData* log_data, const char* filename) {
    if (!log_data || !filename) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    log_data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) && log_data->count < MAX_LINES) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        if (len < sizeof(log_data->lines[0])) {
            strncpy(log_data->lines[log_data->count], buffer, sizeof(log_data->lines[0]) - 1);
            log_data->lines[log_data->count][sizeof(log_data->lines[0]) - 1] = '\0';
            log_data->count++;
        }
    }
    
    fclose(file);
    return 1;
}

void process_log(const LogData* log_data, LogProcessor processor) {
    if (!log_data || !processor) return;
    
    for (int i = 0; i < log_data->count; i++) {
        processor(log_data->lines[i]);
    }
}

int main() {
    LogData log_data;
    const char* filename = "sample.log";
    
    printf("Log Analyzer\n");
    printf("============\n\n");
    
    if (!read_log_file(&log_data, filename)) {
        printf("Creating sample log file...\n");
        
        FILE* sample = fopen(filename, "w");
        if (sample) {
            fprintf(sample, "2023-10-01 10:30:15 INFO Application started\n");
            fprintf(sample, "2023-10-01 10:31:22 ERROR Database connection failed\n");
            fprintf(sample, "2023-10-01 10:32:45 WARNING High memory usage\n");
            fprintf(sample, "2023-10-01 10:33:10 INFO User login successful\n");
            fprintf(sample, "2023-10-01 10:34:05 ERROR File not found\n");
            fprintf(sample, "2023-10-01 10:35:20 INFO Processing completed\n");
            fclose(sample);
            
            if (!read_log_file(&log_data, filename)) {
                printf("Failed to read created log file\n");
                return 1;
            }
        } else {
            printf("Failed to create sample log file\n");
            return 1;
        }
    }
    
    printf("Processing log with line counter:\n");
    process_log(&log_data, count_lines);
    
    printf("\nSearching for errors:\n");
    process_log(&log_data, find_errors);
    
    printf("\nExtracting timestamps:\n");
    process_log(&log_data, extract_timestamps);
    
    printf("\nAnalysis complete. Processed %d log entries.\n", log_data.count);
    
    return 0;
}