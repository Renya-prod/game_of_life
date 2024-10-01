#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define START 5  // Начальная скорость (милисек)
#define NMAX 25  // Количество строк
#define MMAX 80  // Количество столбцов

int input(int matrix1[NMAX][MMAX]);  // Функция ввода матрицы с файла (Перенаправление ввода)
int check_dont_null(int matrix1[NMAX][MMAX]);  // Функция проверки на нулевую матрицы
void print_matrix(int matrix1[NMAX][MMAX]);  // Функция вывода матрицы
int terminal(void);                          // Функция для работы с клавиатурой
void logics(int matrix1[NMAX][MMAX], int matrix2[NMAX][MMAX]);  // Функция логики (нахождение соседей)
int check_similar(int matrix1[NMAX][MMAX],
                  int matrix2[NMAX][MMAX]);  // Функция сравнения двух матриц на идентичность

void sleep_time(int speed);
// Главная функция
int main() {
    int matrix1[NMAX][MMAX];
    int matrix2[NMAX][MMAX];
    input(matrix1);
    (void)freopen("/dev/tty", "r", stdin);
    int speed = START;
    int flag = 0;
    while (flag != 1) {
        system("clear");
        if ((check_dont_null(matrix1)) == 1) {
            flag = 1;
        }

        print_matrix(matrix1);
        if (terminal()) {
            char key = getchar();
            if (key == 'q' || key == 'Q') {
                break;
            }

            if (key == 'w' || key == 'W') {
                if (speed > 1) {
                    speed -= 1;
                }
            }
            if (key == 's' || key == 'S') {
                speed += 1;
            }
        }

        logics(matrix1, matrix2);

        if ((check_similar(matrix1, matrix2)) == 1) {
            flag = 1;
        }
        sleep_time(speed * 100);  // Задержка
    }

    return 0;
}

int input(int matrix1[NMAX][MMAX]) {
    for (int i = 0; i < NMAX; i++) {
        for (int j = 0; j < MMAX; j++) {
            if (scanf("%d", &matrix1[i][j]) != 1) {
                printf("Неверный файл\n");
                return 1;
            }
        }
    }
    return 0;
}

int check_dont_null(int matrix1[NMAX][MMAX]) {
    int kol_null = 0;
    int flag = 0;
    for (int i = 0; i < NMAX; i++) {
        for (int j = 0; j < MMAX; j++) {
            kol_null += matrix1[i][j];
        }
    }
    if (kol_null == 0) {
        flag = 1;
    }
    return flag;
}

void print_matrix(int matrix1[NMAX][MMAX]) {
    for (int i = 0; i < NMAX; i++) {
        for (int j = 0; j < MMAX; j++) {
            printf("%c", matrix1[i][j] ? '*' : ' ');
        }
        printf("\n");
    }
}

int terminal(void) {
    struct termios oldSettings, newSettings;
    int oldFlags;
    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
    oldFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldFlags | O_NONBLOCK);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    fcntl(STDIN_FILENO, F_SETFL, oldFlags);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void logics(int matrix1[NMAX][MMAX], int matrix2[NMAX][MMAX]) {
    for (int i = 0; i < NMAX; i++) {
        for (int j = 0; j < MMAX; j++) {
            int sum = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) continue;
                    int ni = (i + di + NMAX) % NMAX;
                    int nj = (j + dj + MMAX) % MMAX;
                    sum += matrix1[ni][nj];
                }
            }
            if ((sum == 3) && (matrix1[i][j] == 0)) {
                matrix2[i][j] = 1;
            } else if ((sum == 2 || sum == 3) && (matrix1[i][j] == 1)) {
                matrix2[i][j] = 1;
            } else {
                matrix2[i][j] = 0;
            }
        }
    }
}

int check_similar(int matrix1[NMAX][MMAX], int matrix2[NMAX][MMAX]) {
    int sim_matr = 0;
    int flag = 0;
    for (int i = 0; i < NMAX; i++) {
        for (int j = 0; j < MMAX; j++) {
            if (matrix1[i][j] == matrix2[i][j]) sim_matr++;
        }
    }

    if (sim_matr == (NMAX * MMAX)) {
        flag = 1;
        return flag;
    }

    for (int i = 0; i < NMAX; i++) {
        for (int j = 0; j < MMAX; j++) {
            matrix1[i][j] = matrix2[i][j];
        }
    }
    return flag;
}

void sleep_time(int speed) {
    clock_t start_time = clock();
    while (clock() < start_time + speed * CLOCKS_PER_SEC / 1000)
        ;
}