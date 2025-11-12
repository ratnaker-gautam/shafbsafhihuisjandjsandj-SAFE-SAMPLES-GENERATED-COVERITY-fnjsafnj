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
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return 0;
    }

    log_data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && log_data->count < MAX_LINES) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Warning: Line too long, truncating\n");
            buffer[MAX_LINE_LENGTH - 1] = 0;
        }
        strncpy(log_data->lines[log_data->count], buffer, MAX_LINE_LENGTH - 1);
        log_data->lines[log_data->count][MAX_LINE_LENGTH - 1] = 0;
        log_data->count++;
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return 0;
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    LogData log_data;
    if (!read_log_file(&log_data, argv[1])) {
        return 1;
    }

    if (log_data.count == 0) {
        printf("No log entries found\n");
        return 0;
    }

    LogProcessor processors[] = {count_lines, find_errors, extract_timestamps};
    const char* processor_names[] = {"Line Counter", "Error Finder", "Timestamp Extractor"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);

    for (int i = 0; i < num_processors; i++) {
        printf("\n=== %s ===\n", processor_names[i]);
        process_log(&log_data, processors[i]);
    }

    printf("\nTotal log entries processed: %d\n", log_data.count);
    return 0;
}