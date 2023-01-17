#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>


int nSets = 15;
int n;
int **c;
int **matrixOne;
int **matrixTwo;
typedef struct {
    int first;
    int last;
} parameters;

void write_in_file(const char *path, char *text) {

    HANDLE handle;
    BOOL ErrorFlag;
    DWORD NumberOfLetters = (DWORD) strlen(text);
    DWORD NumberOfLettersWritten = 0;
    handle = CreateFileA(path, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        // printf("Eroare la deschiderea fisierului");
        CloseHandle(handle);
    }

    ErrorFlag = WriteFile(handle, text, NumberOfLetters, &NumberOfLettersWritten, NULL);

    if (ErrorFlag == 0) {

        printf("Nu s e putut scrie in fisier");
        CloseHandle(handle);
    } else {
        if (NumberOfLetters != NumberOfLettersWritten) {
            printf("Nu s au putut scrie toate literele");
            CloseHandle(handle);
        } else {
            // printf("S-au scris toate literele");
            CloseHandle(handle);
        }
    }
}


int my_pow(int n, int power) {
    int result = n;
    for (int i = 1; i < power; i++) {
        result *= n;
    }
    return result;
}

void multiplyMatrix(int **matrixOne, int **matrixTwo, int n, int first, int last) {
    for (int i = first; i < last; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                c[i][j] += matrixOne[i][k] * matrixTwo[k][j];
            }
        }
        // printf("%d\n",i);
    }

//    for (int i = 0; i < n; i++)
//    {
//        free(c[i]);
//    }
}

// n dimension of matrix

void printFinal(int n, char *path) {
    char *matrix_as_message;
    for (int i = 0; i < n; i++) {
        matrix_as_message = (char *) malloc(100000 * sizeof(char));
        strcpy(matrix_as_message, "");
        for (int j = 0; j < n; j++) {
            char number[10];
            itoa(c[i][j], number, 10);
            strcat(matrix_as_message, number);
            strcat(matrix_as_message, " ");
        }
        strcat(matrix_as_message, "\n");
        write_in_file(path, matrix_as_message);
        free(matrix_as_message);
    }
}

void generateMatrix(int k, int matrix_number) {
    n = my_pow(10, k);
    srand(time(0));
    matrixOne = (int **) malloc(n * sizeof(int *));
    matrixTwo = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matrixOne[i] = (int *) malloc(n * sizeof(int));
        matrixTwo[i] = (int *) malloc(n * sizeof(int));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrixOne[i][j] = rand() % 100;
            matrixTwo[i][j] = rand() % 100;
        }
    }

    c = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        c[i] = (int *) malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = 0;
        }
    }
    char *matrix_as_message_one;
    char *matrix_as_message_two;
    char file_name_one[50];
    char file_name_two[50];
    sprintf(file_name_one, "C:\\Facultate\\CSSO\\Laboratoare\\Week6\\date\\%d_%d_A", k, matrix_number);
    sprintf(file_name_two, "C:\\Facultate\\CSSO\\Laboratoare\\Week6\\date\\%d_%d_B", k, matrix_number);
    for (int i = 0; i < n; i++) {
        matrix_as_message_one = (char *) malloc(10000 * sizeof(char));
        strcpy(matrix_as_message_one, "");
        for (int j = 0; j < n; j++) {
            char number[10];
            itoa(matrixOne[i][j], number, 10);
            strcat(matrix_as_message_one, number);
            strcat(matrix_as_message_one, " ");
        }
        strcat(matrix_as_message_one, "\n");
        write_in_file(file_name_one, matrix_as_message_one);
        //printf("%s",matrix_as_message_one);
        free(matrix_as_message_one);
    }

    for (int i = 0; i < n; i++) {
        matrix_as_message_two = (char *) malloc(10000 * sizeof(char));
        strcpy(matrix_as_message_two, "");
        for (int j = 0; j < n; j++) {
            char number[10];
            itoa(matrixTwo[i][j], number, 10);
            strcat(matrix_as_message_two, number);
            strcat(matrix_as_message_two, " ");
        }
        strcat(matrix_as_message_two, "\n");
        write_in_file(file_name_two, matrix_as_message_two);
        //printf("%s",matrix_as_message_two);
        free(matrix_as_message_two);
    }

    //    printf("done generating\n");
    //    printf("Multiplication of matrix:\n");
    // multiplyMatrix(matrixOne, matrixTwo, n, 0, n);
}

DWORD WINAPI thread_function(LPVOID lpParam) {
    parameters *params = (parameters *) lpParam;
    multiplyMatrix(matrixOne, matrixTwo, n, params->first, params->last);
    printf("Done my part\n");
    return 0;
}

void matrixMultUsingThreads() {
    HANDLE threads[20];

    int first = 0;
    int last = (n / 10) * 2;
    parameters param[10];
    param[0].first = first;
    param[0].last = last;
    first = last;
    last = first + (n / 10) * 2;
    for (int i = 1; i < 8; i++) {
        param[i].first = first;
        param[i].last = last;
        first = last;
        last = first + n / 10;
    }
//    for (int i = 0; i < 8; i++) {
//        printf("%d %d \n", param[i].first, param[i].last);
//    }
    for (int i = 0; i < 8; i++) {

        threads[i] = CreateThread(NULL, 0, thread_function, (LPVOID) &param[i], 0, NULL);
        if (threads[i] == NULL) {
            printf("Eroare la crearea threadului");
        }
    }
    WaitForMultipleObjects(8, threads, TRUE, INFINITE);
    for (int i = 0; i < 8; i++) {
        CloseHandle(threads[i]);
    }

}

void static_and_dinamic_Multiply() {
    clock_t start, end, start2, end2, matrix_generated_start, matrix_generated_end;
    double generated_matrix_times[20], generated_multiply_static_time[20], generated_multiply_dynamic_time[20];
    double elapsed, elapsed2;
    char *file_name;
    char *file_name2;
    for (int k = 1; k < 4; k++) {
        for (int i = 0; i < 15; i++) {
            matrix_generated_start = clock();
            generateMatrix(k, i);
            matrix_generated_end = clock();
            generated_matrix_times[i] =
                    (double) (matrix_generated_end - matrix_generated_start) / (CLOCKS_PER_SEC / 1000);
            start = clock();
            multiplyMatrix(matrixOne, matrixTwo, n, 0, n);
            end = clock();
            start2 = clock();
            matrixMultUsingThreads();
            end2 = clock();
            elapsed = (double) (end - start) / (CLOCKS_PER_SEC / 1000);
            elapsed2 = (double) (end2 - start2) / (CLOCKS_PER_SEC / 1000);
            file_name = (char *) malloc(100 * sizeof(char));
            file_name2 = (char *) malloc(100 * sizeof(char));
            strcpy(file_name, "");
            strcpy(file_name2, "");
            sprintf(file_name, "C:\\Facultate\\CSSO\\Laboratoare\\Week6\\rezultate\\secvential\\Seq_%d_%d_%.6f.txt", k,
                    i, elapsed);
            sprintf(file_name2, "C:\\Facultate\\CSSO\\Laboratoare\\Week6\\rezultate\\static\\16_%d_%d_%.6f.txt", k,
                    i, elapsed2);
            printFinal(n, file_name);
            printFinal(n, file_name2);
            generated_multiply_static_time[i] = elapsed;
            generated_multiply_dynamic_time[i] = elapsed2;
            free(file_name);
            free(file_name2);
            for (int index = 0; index < n; index++) {
                free(c[index]);
            }
        }
        //
        char generated_times[200];
        char csv_times_static[200];
        char csv_times_dynamic[200];
        sprintf(csv_times_static, "k_static=%d,", k);
        sprintf(csv_times_dynamic, "k_dynamic=%d,", k);
        sprintf(generated_times, "k=%d ", k);
        for (int i = 0; i < 15; i++) {
            char time[10];
            sprintf(time, "%.3f", generated_matrix_times[i]);
            strcat(generated_times, time);
            strcat(generated_times, " ");
            sprintf(time, "%.3f", generated_multiply_static_time[i]);
            strcat(csv_times_static, time);
            strcat(csv_times_static, ",");
            sprintf(time, "%.3f", generated_multiply_dynamic_time[i]);
            strcat(csv_times_dynamic, time);
            strcat(csv_times_dynamic, ",");
        }
        strcat(generated_times, "\n");
        strcat(csv_times_static, "\n");
        strcat(csv_times_dynamic, "\n");
        write_in_file("C:\\Facultate\\CSSO\\Laboratoare\\Week6\\rezultate\\pentruraport\\generate.txt",
                      generated_times);
        write_in_file("C:\\Facultate\\CSSO\\Laboratoare\\Week6\\grafice.csv", csv_times_static);
        write_in_file("C:\\Facultate\\CSSO\\Laboratoare\\Week6\\grafice.csv", csv_times_dynamic);
    }
}


int main() {
    static_and_dinamic_Multiply();
    return 0;
}
