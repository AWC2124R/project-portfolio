#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

FILE * fptr;

struct Position {
  int pos_prisoner_1[4];
  int pos_prisoner_2[4];
  int pos_map[6][5];
  int kaisu;
};

struct BestPV {
  char sasite[500];
};

struct Hash {
  int hash_prisoner_1[4];
  int hash_prisoner_2[4];
  int hash_map[6][5];

  int deep;
  int low;
  int upp;

  char hash_te[4];
};

void input(char * te, int turn_num);
void output(int map[6][5], int prisoner_1[], int prisoner_2[]);

int check(int map[6][5], char * te, int turn_num, int prisoner_1[], int prisoner_2[]);
int c_check(int map[6][5], char * te, int turn_num, int prisoner_1[], int prisoner_2[]);
int check_draw(struct Position map_state[], int map[6][5], int prisoner_1[], int prisoner_2[]);

int move(int map[6][5], char * te, int turn_num, int prisoner_1[], int prisoner_2[]);
void back(int map[6][5], char * te, int turn_num, int prisoner_1[], int prisoner_2[], int koma);

void call_AI(int map[6][5], struct Position map_state[], char * te, int turn_num, int prisoner_1[], int prisoner_2[], int l);
void non_AI(int map[6][5], char * te, int turn_num, int prisoner_1[], int prisoner_2[]);
void low_AI(int map[6][5], struct Position map_state[], char * te, int turn_num, int prisoner_1[], int prisoner_2[], int depth);
void high_AI(int map[6][5], struct Position map_state[], char * te, int turn_num, int prisoner_1[], int prisoner_2[]);

int NegaMax(int map[6][5], struct Position map_state[], struct BestPV pv[], struct Hash tikan[], char * str, int turn_num, int prisoner_1[], int prisoner_2[], int deep, int depth, int * node, int al, int be);
int hashti(int map[6][5], int prisoner_1[], int prisoner_2[], int turn_num);

void person_to_preson(void);
void person_to_computer(int computer);
int computer_to_computer(int computer_1, int computer_2);

int before_length = 0;

int main(void) {
  system("chcp 65001");
  int n, i, b, w, computer[2];
  double start, end;
  char str[100];

  srand((unsigned) time(NULL));
  do {
    before_length = 0;
    fptr = fopen("C:\\Users\\admin\\Documents\\12Chess\\pyctalk\\ctopy.txt", "w");
    fclose(fptr);
    n = 2;
    do {
      printf("Enter COM Level (2-9): ");
      scanf("%d", & computer[0]);
    } while (computer[0] <= 1 || 9 < computer[0]);
    person_to_computer(computer[0]);

    do {
      /*
			printf("Do you want to play again?(y/n): ");
			str[0]=getche();
			printf("\n");
			*/
      str[0] = 'y';
    } while (str[0] != 'y' && str[0] != 'n');
  } while (str[0] == 'y');

  return 0;
}

void input(char * m_te, int turn_num) {
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  if (turn_num % 2 == 1) {
    while (1) {
      fptr = fopen("C:\\Users\\admin\\Documents\\12Chess\\pyctalk\\pytoc.txt", "r");
      int idx = 0;
      char str[10];
      char line[20] = {
        0
      };
      while (fgets(line, 20, fptr)) {
        strcpy(str, line);
        idx++;
      }
      fclose(fptr);
      char str2[10];
      strncpy(str2, str, 4);
      str2[4] = '\0';
      int length = idx;
      if (length > before_length) {
        before_length++;
        printf("%s\n", str2);
        m_te = str2;
        break;
      }
    }
    if (m_te[0] == '0') {
      switch (m_te[1]) {
      case 'p':
        m_te[1] = 1;
        break;
      case 'n': // ?
        m_te[1] = 2;
        break;
      case 's':
        m_te[1] = 3;
        break;
      case 'j':
        m_te[1] = 4;
        break;
      case 'r': // ?
        m_te[1] = 5;
        break;
      }
    }

  } else {
    while (1) {
      fptr = fopen("C:\\Users\\admin\\Documents\\12Chess\\pyctalk\\pytoc.txt", "r");
      int idx = 0;
      char str[10];
      char line[20] = {
        0
      };
      while (fgets(line, 20, fptr)) {
        strcpy(str, line);
        idx++;
      }
      fclose(fptr);
      char str2[10];
      strncpy(str2, str, 4);
      str2[4] = '\0';
      int length = idx;
      if (length > before_length) {
        before_length++;
        printf("%s\n", str2);
        m_te = str2;
        break;
      }
    }
    if (m_te[0] == '0') {
      switch (m_te[1]) {
      case 'p':
        m_te[1] = 6;
        break;
      case 'n': // ?
        m_te[1] = 7;
        break;
      case 's':
        m_te[1] = 8;
        break;
      case 'j':
        m_te[1] = 9;
        break;
      case 'r': // ?
        m_te[1] = 10;
        break;
      }
    }
  }
  printf("m_te %s\n", m_te);
}

void output(int map[6][5], int prisoner_1[], int prisoner_2[]) {
  int i, j;

  printf("The piece Blue has: ");
  switch (prisoner_2[0]) {
  case 0:
    break;
  case 1:
    printf("p");
    break;
  case 2:
    printf("pp");
    break;
  }
  switch (prisoner_2[1]) {
  case 0:
    break;
  case 1:
    printf("s");
    break;
  case 2:
    printf("ss");
    break;
  }
  switch (prisoner_2[2]) {
  case 0:
    break;
  case 1:
    printf("j");
    break;
  case 2:
    printf("jj");
    break;
  }
  printf("\n\n");

  printf("a b c\n");
  for (i = 1; i < 5; i++) {
    for (j = 1; j < 4; j++) {
      switch (map[i][j]) {
      case 0:
        printf("- ");
        break;
      case 1:
        printf("P ");
        break;
      case 2:
        printf("H ");
        break;
      case 3:
        printf("S ");
        break;
      case 4:
        printf("J ");
        break;
      case 5:
        printf("K ");
        break;
      case 6:
        printf("p ");
        break;
      case 7:
        printf("h ");
        break;
      case 8:
        printf("s ");
        break;
      case 9:
        printf("j ");
        break;
      case 10:
        printf("k ");
        break;
      }
    }
    printf("%d\n", i);
  }
  printf("\n");

  printf("The piece Red has: ");
  switch (prisoner_1[0]) {
  case 0:
    break;
  case 1:
    printf("P");
    break;
  case 2:
    printf("PP");
    break;
  }
  switch (prisoner_1[1]) {
  case 0:
    break;
  case 1:
    printf("S");
    break;
  case 2:
    printf("SS");
    break;
  }
  switch (prisoner_1[2]) {
  case 0:
    break;
  case 1:
    printf("J");
    break;
  case 2:
    printf("JJ");
    break;
  }
  printf("\n\n");
}

int check(int map[6][5], char * te, int turn_num, int prisoner_1[], int prisoner_2[]) {
  int i, j, k = -1, l = -1, m = 0, n = -1, koma;
  char x[1000] = {
    ""
  }, y[1000] = {
    ""
  };

  strcpy(te, x);

  turn_num = turn_num % 2;

  if (turn_num == 0) {

    for (i = '1'; i < '5'; i++) {
      for (j = 1; j < 4; j++) {
        if (map[i - '0'][j] / 6 == 1) {
          if (map[i - '0'][j] == 6) {
            if (map[i - '0' + 1][j] != -1 && map[i - '0' + 1][j] / 6 != 1) {
              x[m] = 'a' + j - 1;
              x[m + 1] = i;
              x[m + 2] = 'a' + j - 1;
              x[m + 3] = i + 1;
              m += 4;
            }
          } else {
            if (map[i - '0'][j] == 7) {
              if (map[i - '0' + 1][j] != -1 && map[i - '0' + 1][j] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j - 1;
                x[m + 3] = i + 1;
                m += 4;
              }
              if (map[i - '0' + 1][j - 1] != -1 && map[i - '0' + 1][j - 1] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j - 2;
                x[m + 3] = i + 1;
                m += 4;
              }
              if (map[i - '0'][j - 1] != -1 && map[i - '0'][j - 1] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j - 2;
                x[m + 3] = i;
                m += 4;
              }
              if (map[i - '0' - 1][j] != -1 && map[i - '0' - 1][j] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j - 1;
                x[m + 3] = i - 1;
                m += 4;
              }
              if (map[i - '0'][j + 1] != -1 && map[i - '0'][j + 1] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j;
                x[m + 3] = i;
                m += 4;
              }
              if (map[i - '0' + 1][j + 1] != -1 && map[i - '0' + 1][j + 1] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j;
                x[m + 3] = i + 1;
                m += 4;
              }
            } else {
              if (map[i - '0'][j] == 8) {
                if (map[i - '0' + 1][j - 1] != -1 && map[i - '0' + 1][j - 1] / 6 != 1) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 2;
                  x[m + 3] = i + 1;
                  m += 4;
                }
                if (map[i - '0' - 1][j - 1] != -1 && map[i - '0' - 1][j - 1] / 6 != 1) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 2;
                  x[m + 3] = i - 1;
                  m += 4;
                }
                if (map[i - '0' - 1][j + 1] != -1 && map[i - '0' - 1][j + 1] / 6 != 1) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j;
                  x[m + 3] = i - 1;
                  m += 4;
                }
                if (map[i - '0' + 1][j + 1] != -1 && map[i - '0' + 1][j + 1] / 6 != 1) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j;
                  x[m + 3] = i + 1;
                  m += 4;
                }
              } else {
                if (map[i - '0'][j] == 9) {
                  if (map[i - '0' + 1][j] != -1 && map[i - '0' + 1][j] / 6 != 1) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j - 1;
                    x[m + 3] = i + 1;
                    m += 4;
                  }
                  if (map[i - '0'][j - 1] != -1 && map[i - '0'][j - 1] / 6 != 1) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j - 2;
                    x[m + 3] = i;
                    m += 4;
                  }
                  if (map[i - '0' - 1][j] != -1 && map[i - '0' - 1][j] / 6 != 1) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j - 1;
                    x[m + 3] = i - 1;
                    m += 4;
                  }
                  if (map[i - '0'][j + 1] != -1 && map[i - '0'][j + 1] / 6 != 1) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j;
                    x[m + 3] = i;
                    m += 4;
                  }
                } else {
                  if (map[i - '0'][j] == 10) {
                    if (map[i - '0' + 1][j] != -1 && map[i - '0' + 1][j] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 1;
                      x[m + 3] = i + 1;
                      m += 4;
                    }
                    if (map[i - '0' + 1][j - 1] != -1 && map[i - '0' + 1][j - 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 2;
                      x[m + 3] = i + 1;
                      m += 4;
                    }
                    if (map[i - '0'][j - 1] != -1 && map[i - '0'][j - 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 2;
                      x[m + 3] = i;
                      m += 4;
                    }
                    if (map[i - '0' - 1][j - 1] != -1 && map[i - '0' - 1][j - 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 2;
                      x[m + 3] = i - 1;
                      m += 4;
                    }
                    if (map[i - '0' - 1][j] != -1 && map[i - '0' - 1][j] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 1;
                      x[m + 3] = i - 1;
                      m += 4;
                    }
                    if (map[i - '0' - 1][j + 1] != -1 && map[i - '0' - 1][j + 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j;
                      x[m + 3] = i - 1;
                      m += 4;
                    }
                    if (map[i - '0'][j + 1] != -1 && map[i - '0'][j + 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j;
                      x[m + 3] = i;
                      m += 4;
                    }
                    if (map[i - '0' + 1][j + 1] != -1 && map[i - '0' + 1][j + 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j;
                      x[m + 3] = i + 1;
                      m += 4;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    for (l = 0; l < 3; l++) {
      if (prisoner_2[l] > 0) {
        switch (l) {
        case 0:
          koma = 6;
          break;
        case 1:
          koma = 8;
          break;
        case 2:
          koma = 9;
          break;
        }
        for (i = '1'; i < '5'; i++) {
          for (j = 1; j < 4; j++) {
            if (map[i - '0'][j] == 0) {
              x[m] = '0';
              x[m + 1] = koma;
              x[m + 2] = 'a' + j - 1;
              x[m + 3] = i;
              m += 4;
            }
          }
        }
      }
    }
  }

  if (turn_num == 1) {
    for (i = '1'; i < '5'; i++) {
      for (j = 1; j < 4; j++) {
        if (map[i - '0'][j] != 0) {
          if (map[i - '0'][j] / 6 == 0) {
            if (map[i - '0'][j] == 1) {
              if (map[i - '0' - 1][j] == 0 || map[i - '0' - 1][j] > 5) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j - 1;
                x[m + 3] = i - 1;
                m += 4;
              }
            } else {
              if (map[i - '0'][j] == 2) {
                if (map[i - '0' - 1][j] == 0 || map[i - '0' - 1][j] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 1;
                  x[m + 3] = i - 1;
                  m += 4;
                }
                if (map[i - '0' - 1][j + 1] == 0 || map[i - '0' - 1][j + 1] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j;
                  x[m + 3] = i - 1;
                  m += 4;
                }
                if (map[i - '0'][j + 1] == 0 || map[i - '0'][j + 1] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j;
                  x[m + 3] = i;
                  m += 4;
                }
                if (map[i - '0' + 1][j] == 0 || map[i - '0' + 1][j] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 1;
                  x[m + 3] = i + 1;
                  m += 4;
                }
                if (map[i - '0'][j - 1] == 0 || map[i - '0'][j - 1] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 2;
                  x[m + 3] = i;
                  m += 4;
                }
                if (map[i - '0' - 1][j - 1] == 0 || map[i - '0' - 1][j - 1] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 2;
                  x[m + 3] = i - 1;
                  m += 4;
                }
              } else {
                if (map[i - '0'][j] == 3) {
                  if (map[i - '0' + 1][j - 1] == 0 || map[i - '0' + 1][j - 1] > 5) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j - 2;
                    x[m + 3] = i + 1;
                    m += 4;
                  }
                  if (map[i - '0' - 1][j - 1] == 0 || map[i - '0' - 1][j - 1] > 5) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j - 2;
                    x[m + 3] = i - 1;
                    m += 4;
                  }
                  if (map[i - '0' - 1][j + 1] == 0 || map[i - '0' - 1][j + 1] > 5) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j;
                    x[m + 3] = i - 1;
                    m += 4;
                  }
                  if (map[i - '0' + 1][j + 1] == 0 || map[i - '0' + 1][j + 1] > 5) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j;
                    x[m + 3] = i + 1;
                    m += 4;
                  }
                } else {
                  if (map[i - '0'][j] == 4) {
                    if (map[i - '0' + 1][j] == 0 || map[i - '0' + 1][j] > 5) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 1;
                      x[m + 3] = i + 1;
                      m += 4;
                    }
                    if (map[i - '0'][j - 1] == 0 || map[i - '0'][j - 1] > 5) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 2;
                      x[m + 3] = i;
                      m += 4;
                    }
                    if (map[i - '0' - 1][j] == 0 || map[i - '0' - 1][j] > 5) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 1;
                      x[m + 3] = i - 1;
                      m += 4;
                    }
                    if (map[i - '0'][j + 1] == 0 || map[i - '0'][j + 1] > 5) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j;
                      x[m + 3] = i;
                      m += 4;
                    }
                  } else {
                    if (map[i - '0'][j] == 5) {
                      if (map[i - '0' + 1][j] == 0 || map[i - '0' + 1][j] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j - 1;
                        x[m + 3] = i + 1;
                        m += 4;
                      }
                      if (map[i - '0' + 1][j - 1] == 0 || map[i - '0' + 1][j - 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j - 2;
                        x[m + 3] = i + 1;
                        m += 4;
                      }
                      if (map[i - '0'][j - 1] == 0 || map[i - '0'][j - 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j - 2;
                        x[m + 3] = i;
                        m += 4;
                      }
                      if (map[i - '0' - 1][j - 1] == 0 || map[i - '0' - 1][j - 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j - 2;
                        x[m + 3] = i - 1;
                        m += 4;
                      }
                      if (map[i - '0' - 1][j] == 0 || map[i - '0' - 1][j] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j - 1;
                        x[m + 3] = i - 1;
                        m += 4;
                      }
                      if (map[i - '0' - 1][j + 1] == 0 || map[i - '0' - 1][j + 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j;
                        x[m + 3] = i - 1;
                        m += 4;
                      }
                      if (map[i - '0'][j + 1] == 0 || map[i - '0'][j + 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j;
                        x[m + 3] = i;
                        m += 4;
                      }
                      if (map[i - '0' + 1][j + 1] == 0 || map[i - '0' + 1][j + 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j;
                        x[m + 3] = i + 1;
                        m += 4;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    for (l = 0; l < 3; l++) {
      if (prisoner_1[l] > 0) {
        switch (l) {
        case 0:
          koma = 1;
          break;
        case 1:
          koma = 3;
          break;
        case 2:
          koma = 4;
          break;
        }
        for (i = '1'; i < '5'; i++) {
          for (j = 1; j < 4; j++) {
            if (map[i - '0'][j] == 0) {
              x[m] = '0';
              x[m + 1] = koma;
              x[m + 2] = 'a' + j - 1;
              x[m + 3] = i;
              m += 4;
            }
          }
        }
      }
    }

  }
  l = -1;
  for (i = 1; i < 5; i++) {
    for (j = 'a'; j < 'd'; j++) {
      if (map[i][j - 'a' + 1] == 5) {
        if (i == 1) {
          m = 2;
          do {
            if (x[m] == j && (x[m + 1] - '0') == i) {
              n = 0;
            }
            m += 4;
          } while (strlen(x) > m);
          if (n == -1) {
            return 1;
          }
        }
        k = 0;
      }
      if (map[i][j - 'a' + 1] == 10) {
        if (i == 4) {
          m = 2;
          do {
            if (x[m] == j && (x[m + 1] - '0') == i) {
              n = 0;
            }
            m += 4;
          } while (strlen(x) > m);
          if (n == -1) {
            return 2;
          }
        }
        l = 0;
      }
    }
  }
  if (k == -1) {
    return 2;
  }
  if (l == -1) {
    return 1;
  }

  m = 1;
  k = 0;
  n = 0;
  if (turn_num == 0) {
    for (i = 0; x[m] != 0; i++) {
      if (map[x[m + 2] - '0'][x[m + 1] - 'a' + 1] / 5 == 0 && map[x[m + 2] - '0'][x[m + 1] - 'a' + 1] != 0) {
        y[k] = x[m - 1];
        y[k + 1] = x[m];
        y[k + 2] = x[m + 1];
        y[k + 3] = x[m + 2];
        if (map[x[m + 2] - '0'][x[m + 1] - 'a' + 1] == 5) {
          if (n == 0) {
            strcpy(te, y);
            n++;
          } else {
            strcat(te, y);
          }
        }
        j = m;
        k += 4;
        for (l = j + 2; x[j - 1] != 0; l++) {
          x[j - 1] = x[j + 3];
          j++;
        }
      }
      m += 4;
    }
  } else {
    for (i = 0; x[m] != 0; i++) {
      if (map[x[m + 2] - '0'][x[m + 1] - 'a' + 1] / 6 == 1) {
        y[k] = x[m - 1];
        y[k + 1] = x[m];
        y[k + 2] = x[m + 1];
        y[k + 3] = x[m + 2];
        if (map[x[m + 2] - '0'][x[m + 1] - 'a' + 1] == 10) {
          if (n == 0) {
            strcpy(te, y);
            n++;
          } else {
            strcat(te, y);
          }
        }
        j = m;
        k += 4;
        for (l = j + 2; x[j - 1] != 0; l++) {
          x[j - 1] = x[j + 3];
          j++;
        }
      }
      m += 4;
    }
  }

  strcat(y, x);
  strcpy(te, y);

  return 0;
}

int c_check(int map[6][5], char * te, int turn_num, int prisoner_1[], int prisoner_2[]) {
  int i, j, k = -1, l = -1, m = 0, n = -1, i_prisoner_1[4], i_prisoner_2[4], koma;
  int t_map[6][5];
  char x[1000] = {
    ""
  }, y[1000] = {
    ""
  }, t_te[1000] = {
    ""
  };

  strcpy(te, x);

  turn_num = turn_num % 2;

  if (turn_num == 0) {

    for (i = '1'; i < '5'; i++) {
      for (j = 1; j < 4; j++) {
        if (map[i - '0'][j] / 6 == 1) {
          if (map[i - '0'][j] == 6) {
            if (map[i - '0' + 1][j] != -1 && map[i - '0' + 1][j] / 6 != 1) {
              x[m] = 'a' + j - 1;
              x[m + 1] = i;
              x[m + 2] = 'a' + j - 1;
              x[m + 3] = i + 1;
              m += 4;
            }
          } else {
            if (map[i - '0'][j] == 7) {
              if (map[i - '0' + 1][j] != -1 && map[i - '0' + 1][j] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j - 1;
                x[m + 3] = i + 1;
                m += 4;
              }
              if (map[i - '0' + 1][j - 1] != -1 && map[i - '0' + 1][j - 1] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j - 2;
                x[m + 3] = i + 1;
                m += 4;
              }
              if (map[i - '0'][j - 1] != -1 && map[i - '0'][j - 1] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j - 2;
                x[m + 3] = i;
                m += 4;
              }
              if (map[i - '0' - 1][j] != -1 && map[i - '0' - 1][j] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j - 1;
                x[m + 3] = i - 1;
                m += 4;
              }
              if (map[i - '0'][j + 1] != -1 && map[i - '0'][j + 1] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j;
                x[m + 3] = i;
                m += 4;
              }
              if (map[i - '0' + 1][j + 1] != -1 && map[i - '0' + 1][j + 1] / 6 != 1) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j;
                x[m + 3] = i + 1;
                m += 4;
              }
            } else {
              if (map[i - '0'][j] == 8) {
                if (map[i - '0' + 1][j - 1] != -1 && map[i - '0' + 1][j - 1] / 6 != 1) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 2;
                  x[m + 3] = i + 1;
                  m += 4;
                }
                if (map[i - '0' - 1][j - 1] != -1 && map[i - '0' - 1][j - 1] / 6 != 1) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 2;
                  x[m + 3] = i - 1;
                  m += 4;
                }
                if (map[i - '0' - 1][j + 1] != -1 && map[i - '0' - 1][j + 1] / 6 != 1) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j;
                  x[m + 3] = i - 1;
                  m += 4;
                }
                if (map[i - '0' + 1][j + 1] != -1 && map[i - '0' + 1][j + 1] / 6 != 1) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j;
                  x[m + 3] = i + 1;
                  m += 4;
                }
              } else {
                if (map[i - '0'][j] == 9) {
                  if (map[i - '0' + 1][j] != -1 && map[i - '0' + 1][j] / 6 != 1) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j - 1;
                    x[m + 3] = i + 1;
                    m += 4;
                  }
                  if (map[i - '0'][j - 1] != -1 && map[i - '0'][j - 1] / 6 != 1) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j - 2;
                    x[m + 3] = i;
                    m += 4;
                  }
                  if (map[i - '0' - 1][j] != -1 && map[i - '0' - 1][j] / 6 != 1) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j - 1;
                    x[m + 3] = i - 1;
                    m += 4;
                  }
                  if (map[i - '0'][j + 1] != -1 && map[i - '0'][j + 1] / 6 != 1) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j;
                    x[m + 3] = i;
                    m += 4;
                  }
                } else {
                  if (map[i - '0'][j] == 10) {
                    if (map[i - '0' + 1][j] != -1 && map[i - '0' + 1][j] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 1;
                      x[m + 3] = i + 1;
                      m += 4;
                    }
                    if (map[i - '0' + 1][j - 1] != -1 && map[i - '0' + 1][j - 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 2;
                      x[m + 3] = i + 1;
                      m += 4;
                    }
                    if (map[i - '0'][j - 1] != -1 && map[i - '0'][j - 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 2;
                      x[m + 3] = i;
                      m += 4;
                    }
                    if (map[i - '0' - 1][j - 1] != -1 && map[i - '0' - 1][j - 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 2;
                      x[m + 3] = i - 1;
                      m += 4;
                    }
                    if (map[i - '0' - 1][j] != -1 && map[i - '0' - 1][j] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 1;
                      x[m + 3] = i - 1;
                      m += 4;
                    }
                    if (map[i - '0' - 1][j + 1] != -1 && map[i - '0' - 1][j + 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j;
                      x[m + 3] = i - 1;
                      m += 4;
                    }
                    if (map[i - '0'][j + 1] != -1 && map[i - '0'][j + 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j;
                      x[m + 3] = i;
                      m += 4;
                    }
                    if (map[i - '0' + 1][j + 1] != -1 && map[i - '0' + 1][j + 1] / 6 != 1) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j;
                      x[m + 3] = i + 1;
                      m += 4;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    for (l = 0; l < 3; l++) {
      if (prisoner_2[l] > 0) {
        switch (l) {
        case 0:
          koma = 6;
          break;
        case 1:
          koma = 8;
          break;
        case 2:
          koma = 9;
          break;
        }
        for (i = '1'; i < '5'; i++) {
          for (j = 1; j < 4; j++) {
            if (map[i - '0'][j] == 0) {
              x[m] = '0';
              x[m + 1] = koma;
              x[m + 2] = 'a' + j - 1;
              x[m + 3] = i;
              m += 4;
            }
          }
        }
      }
    }

  }

  if (turn_num == 1) {
    for (i = '1'; i < '5'; i++) {
      for (j = 1; j < 4; j++) {
        if (map[i - '0'][j] != 0) {
          if (map[i - '0'][j] / 6 == 0) {
            if (map[i - '0'][j] == 1) {
              if (map[i - '0' - 1][j] == 0 || map[i - '0' - 1][j] > 5) {
                x[m] = 'a' + j - 1;
                x[m + 1] = i;
                x[m + 2] = 'a' + j - 1;
                x[m + 3] = i - 1;
                m += 4;
              }
            } else {
              if (map[i - '0'][j] == 2) {
                if (map[i - '0' - 1][j] == 0 || map[i - '0' - 1][j] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 1;
                  x[m + 3] = i - 1;
                  m += 4;
                }
                if (map[i - '0' - 1][j + 1] == 0 || map[i - '0' - 1][j + 1] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j;
                  x[m + 3] = i - 1;
                  m += 4;
                }
                if (map[i - '0'][j + 1] == 0 || map[i - '0'][j + 1] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j;
                  x[m + 3] = i;
                  m += 4;
                }
                if (map[i - '0' + 1][j] == 0 || map[i - '0' + 1][j] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 1;
                  x[m + 3] = i + 1;
                  m += 4;
                }
                if (map[i - '0'][j - 1] == 0 || map[i - '0'][j - 1] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 2;
                  x[m + 3] = i;
                  m += 4;
                }
                if (map[i - '0' - 1][j - 1] == 0 || map[i - '0' - 1][j - 1] > 5) {
                  x[m] = 'a' + j - 1;
                  x[m + 1] = i;
                  x[m + 2] = 'a' + j - 2;
                  x[m + 3] = i - 1;
                  m += 4;
                }
              } else {
                if (map[i - '0'][j] == 3) {
                  if (map[i - '0' + 1][j - 1] == 0 || map[i - '0' + 1][j - 1] > 5) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j - 2;
                    x[m + 3] = i + 1;
                    m += 4;
                  }
                  if (map[i - '0' - 1][j - 1] == 0 || map[i - '0' - 1][j - 1] > 5) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j - 2;
                    x[m + 3] = i - 1;
                    m += 4;
                  }
                  if (map[i - '0' - 1][j + 1] == 0 || map[i - '0' - 1][j + 1] > 5) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j;
                    x[m + 3] = i - 1;
                    m += 4;
                  }
                  if (map[i - '0' + 1][j + 1] == 0 || map[i - '0' + 1][j + 1] > 5) {
                    x[m] = 'a' + j - 1;
                    x[m + 1] = i;
                    x[m + 2] = 'a' + j;
                    x[m + 3] = i + 1;
                    m += 4;
                  }
                } else {
                  if (map[i - '0'][j] == 4) {
                    if (map[i - '0' + 1][j] == 0 || map[i - '0' + 1][j] > 5) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 1;
                      x[m + 3] = i + 1;
                      m += 4;
                    }
                    if (map[i - '0'][j - 1] == 0 || map[i - '0'][j - 1] > 5) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 2;
                      x[m + 3] = i;
                      m += 4;
                    }
                    if (map[i - '0' - 1][j] == 0 || map[i - '0' - 1][j] > 5) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j - 1;
                      x[m + 3] = i - 1;
                      m += 4;
                    }
                    if (map[i - '0'][j + 1] == 0 || map[i - '0'][j + 1] > 5) {
                      x[m] = 'a' + j - 1;
                      x[m + 1] = i;
                      x[m + 2] = 'a' + j;
                      x[m + 3] = i;
                      m += 4;
                    }
                  } else {
                    if (map[i - '0'][j] == 5) {
                      if (map[i - '0' + 1][j] == 0 || map[i - '0' + 1][j] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j - 1;
                        x[m + 3] = i + 1;
                        m += 4;
                      }
                      if (map[i - '0' + 1][j - 1] == 0 || map[i - '0' + 1][j - 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j - 2;
                        x[m + 3] = i + 1;
                        m += 4;
                      }
                      if (map[i - '0'][j - 1] == 0 || map[i - '0'][j - 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j - 2;
                        x[m + 3] = i;
                        m += 4;
                      }
                      if (map[i - '0' - 1][j - 1] == 0 || map[i - '0' - 1][j - 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j - 2;
                        x[m + 3] = i - 1;
                        m += 4;
                      }
                      if (map[i - '0' - 1][j] == 0 || map[i - '0' - 1][j] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j - 1;
                        x[m + 3] = i - 1;
                        m += 4;
                      }
                      if (map[i - '0' - 1][j + 1] == 0 || map[i - '0' - 1][j + 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j;
                        x[m + 3] = i - 1;
                        m += 4;
                      }
                      if (map[i - '0'][j + 1] == 0 || map[i - '0'][j + 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j;
                        x[m + 3] = i;
                        m += 4;
                      }
                      if (map[i - '0' + 1][j + 1] == 0 || map[i - '0' + 1][j + 1] > 5) {
                        x[m] = 'a' + j - 1;
                        x[m + 1] = i;
                        x[m + 2] = 'a' + j;
                        x[m + 3] = i + 1;
                        m += 4;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    for (l = 0; l < 3; l++) {
      if (prisoner_1[l] > 0) {
        switch (l) {
        case 0:
          koma = 1;
          break;
        case 1:
          koma = 3;
          break;
        case 2:
          koma = 4;
          break;
        }
        for (i = '1'; i < '5'; i++) {
          for (j = 1; j < 4; j++) {
            if (map[i - '0'][j] == 0) {
              x[m] = '0';
              x[m + 1] = koma;
              x[m + 2] = 'a' + j - 1;
              x[m + 3] = i;
              m += 4;
            }
          }
        }
      }
    }
  }
  l = -1;
  for (i = 1; i < 5; i++) {
    for (j = 'a'; j < 'd'; j++) {
      if (map[i][j - 'a' + 1] == 5) {
        if (i == 1) {
          m = 2;
          do {
            if (x[m] == j && (x[m + 1] - '0') == i) {
              n = 0;
            }
            m += 4;
          } while (strlen(x) > m);
          if (n == -1) {
            return 1;
          }
        }
        k = 0;
      }
      if (map[i][j - 'a' + 1] == 10) {
        if (i == 4) {
          m = 2;
          do {
            if (x[m] == j && (x[m + 1] - '0') == i) {
              n = 0;
            }
            m += 4;
          } while (strlen(x) > m);
          if (n == -1) {
            return 2;
          }
        }
        l = 0;
      }
    }
  }
  if (k == -1) {
    return 2;
  }
  if (l == -1) {
    return 1;
  }

  m = 1;
  k = 0;
  n = 0;
  if (turn_num == 0) {
    for (i = 0; x[m] != 0; i++) {
      if (map[x[m + 2] - '0'][x[m + 1] - 'a' + 1] == 5) {
        y[0] = x[m - 1];
        y[1] = x[m];
        y[2] = x[m + 1];
        y[3] = x[m + 2];
        y[4] = '\0';
        strcpy(te, y);
        return 0;
      }
      if (x[m + 2] == '4' && map[x[m] - '0'][x[m - 1] - 'a' + 1] == 10) {
        for (n = 0; n < 5; n++) {
          for (l = 0; l < 6; l++) {
            t_map[l][n] = map[l][n];
          }
        }
        for (n = 0; n < 3; n++) {
          i_prisoner_1[n] = prisoner_1[n];
          i_prisoner_2[n] = prisoner_2[n];
        }
        t_te[0] = x[m - 1];
        t_te[1] = x[m];
        t_te[2] = x[m + 1];
        t_te[3] = x[m + 2];
        move(t_map, t_te, turn_num, i_prisoner_1, i_prisoner_2);
        if (check(t_map, t_te, turn_num + 1, i_prisoner_1, i_prisoner_2) != 0) {
          y[0] = x[m - 1];
          y[1] = x[m];
          y[2] = x[m + 1];
          y[3] = x[m + 2];
          y[4] = '\0';
          strcpy(te, y);
          return 0;
        }
      }
      if (map[x[m + 2] - '0'][x[m + 1] - 'a' + 1] / 6 == 0 && map[x[m + 2] - '0'][x[m + 1] - 'a' + 1] != 0) {
        y[k] = x[m - 1];
        y[k + 1] = x[m];
        y[k + 2] = x[m + 1];
        y[k + 3] = x[m + 2];
        j = m;
        k += 4;
        for (l = j + 2; x[j - 1] != 0; l++) {
          x[j - 1] = x[j + 3];
          j++;
        }
      }
      m += 4;
    }
  } else {
    for (i = 0; x[m] != 0; i++) {
      if (map[x[m + 2] - '0'][x[m + 1] - 'a' + 1] == 10) {

        y[0] = x[m - 1];
        y[1] = x[m];
        y[2] = x[m + 1];
        y[3] = x[m + 2];
        y[4] = '\0';
        strcpy(te, y);
        return 0;
      }
      if (x[m + 2] == '1' && map[x[m] - '0'][x[m - 1] - 'a' + 1] == 5) {
        for (n = 0; n < 5; n++) {
          for (l = 0; l < 6; l++) {
            t_map[l][n] = map[l][n];
          }
        }
        for (n = 0; n < 3; n++) {
          i_prisoner_1[n] = prisoner_1[n];
          i_prisoner_2[n] = prisoner_2[n];
        }
        t_te[0] = x[m - 1];
        t_te[1] = x[m];
        t_te[2] = x[m + 1];
        t_te[3] = x[m + 2];
        move(t_map, t_te, turn_num, i_prisoner_1, i_prisoner_2);
        if (check(t_map, t_te, turn_num + 1, i_prisoner_1, i_prisoner_2) != 0) {
          y[0] = x[m - 1];
          y[1] = x[m];
          y[2] = x[m + 1];
          y[3] = x[m + 2];
          y[4] = '\0';
          strcpy(te, y);
          return 0;
        }
      }
      if (map[x[m + 2] - '0'][x[m + 1] - 'a' + 1] / 6 == 1) {
        y[k] = x[m - 1];
        y[k + 1] = x[m];
        y[k + 2] = x[m + 1];
        y[k + 3] = x[m + 2];
        j = m;
        k += 4;
        for (l = j + 2; x[j - 1] != 0; l++) {
          x[j - 1] = x[j + 3];
          j++;
        }
      }
      m += 4;
    }
  }

  strcat(y, x);
  strcpy(te, y);

  return 0;
}

int check_draw(struct Position map_state[], int map[6][5], int prisoner_1[], int prisoner_2[]) {
  int i, j, k, l, n = -1;

  for (k = 0; map_state[k].kaisu > 0; k++) {
    n = 0;
    for (i = 1; i < 4; i++) {
      for (j = 1; j < 5; j++) {
        if (map_state[k].pos_map[j][i] != map[j][i]) {
          n = -1;
          break;
        }
      }
      if (n == -1) {
        break;
      }
    }
    if (n == 0) {
      for (l = 0; l < 3; l++) {
        if (map_state[k].pos_prisoner_1[l] != prisoner_1[l] || map_state[k].pos_prisoner_2[l] != prisoner_2[l]) {
          n = -1;
          break;
        }
      }
    }
    if (n == 0) {
      map_state[k].kaisu++;
      break;
    }
  }

  if (n == -1) {
    for (i = 0; i < 5; i++) {
      for (j = 0; j < 6; j++) {
        map_state[k].pos_map[j][i] = map[j][i];
      }
    }
    for (l = 0; l < 3; l++) {
      map_state[k].pos_prisoner_1[l] = prisoner_1[l];
      map_state[k].pos_prisoner_2[l] = prisoner_2[l];
    }
    map_state[k].kaisu = 1;
  }

  if (map_state[k].kaisu == 3) {
    return -1;
  } else {
    return 0;
  }
}

int move(int map[6][5], char * te, int turn_num, int prisoner_1[], int prisoner_2[]) {
  int temap, i, x = -1, temp, koma = 0;

  temap = turn_num % 2;

  if (temap == 1) {
    if (te[0] == '0') {
      map[te[3] - '0'][te[2] - 'a' + 1] = te[1];
      switch (te[1]) {
      case 1:
        prisoner_1[0]--;
        break;
      case 3:
        prisoner_1[1]--;
        break;
      case 4:
        prisoner_1[2]--;
        break;
      }
    } else {
      if (map[te[3] - '0'][te[2] - 'a' + 1] == 0) {
        map[te[3] - '0'][te[2] - 'a' + 1] = map[te[1] - '0'][te[0] - 'a' + 1];
        map[te[1] - '0'][te[0] - 'a' + 1] = 0;
      } else {
        koma = map[te[3] - '0'][te[2] - 'a' + 1];
        switch (map[te[3] - '0'][te[2] - 'a' + 1]) {
        case 6:
          prisoner_1[0]++;
          break;
        case 7:
          prisoner_1[0]++;
          break;
        case 8:
          prisoner_1[1]++;
          break;
        case 9:
          prisoner_1[2]++;
          break;
        }
        map[te[3] - '0'][te[2] - 'a' + 1] = map[te[1] - '0'][te[0] - 'a' + 1];
        map[te[1] - '0'][te[0] - 'a' + 1] = 0;
      }
      if (te[3] == '1' && map[te[3] - '0'][te[2] - 'a' + 1] == 1) {
        map[te[3] - '0'][te[2] - 'a' + 1] = 2;
        koma += 20;
      }
    }
  }

  if (temap == 0) {
    if (te[0] == '0') {
      map[te[3] - '0'][te[2] - 'a' + 1] = te[1];
      switch (te[1]) {
      case 6:
        prisoner_2[0]--;
        break;
      case 8:
        prisoner_2[1]--;
        break;
      case 9:
        prisoner_2[2]--;
        break;
      }
    } else {
      if (map[te[3] - '0'][te[2] - 'a' + 1] == 0) {
        map[te[3] - '0'][te[2] - 'a' + 1] = map[te[1] - '0'][te[0] - 'a' + 1];
        map[te[1] - '0'][te[0] - 'a' + 1] = 0;
      } else {
        koma = map[te[3] - '0'][te[2] - 'a' + 1];
        switch (map[te[3] - '0'][te[2] - 'a' + 1]) {
        case 1:
          prisoner_2[0]++;
          break;
        case 2:
          prisoner_2[0]++;
          break;
        case 3:
          prisoner_2[1]++;
          break;
        case 4:
          prisoner_2[2]++;
          break;
        }
        map[te[3] - '0'][te[2] - 'a' + 1] = map[te[1] - '0'][te[0] - 'a' + 1];
        map[te[1] - '0'][te[0] - 'a' + 1] = 0;
      }
      if (te[3] == '4' && map[te[3] - '0'][te[2] - 'a' + 1] == 6) {
        map[te[3] - '0'][te[2] - 'a' + 1] = 7;
        koma += 20;
      }
    }
  }

  return koma;

}

void back(int map[6][5], char * te, int turn_num, int prisoner_1[], int prisoner_2[], int koma) {
  int temap, i, x = -1, temp;

  temap = turn_num % 2;

  if (koma > 19) {
    koma -= 20;
    map[te[3] - '0'][te[2] - 'a' + 1] -= 1;
  }

  if (temap == 1) {
    if (te[0] == '0') {
      switch (te[1]) {
      case 1:
        prisoner_1[0]++;
        break;
      case 3:
        prisoner_1[1]++;
        break;
      case 4:
        prisoner_1[2]++;
        break;
      }
      map[te[3] - '0'][te[2] - 'a' + 1] = 0;
      i = 0;
    } else {
      if (koma == 0) {
        map[te[1] - '0'][te[0] - 'a' + 1] = map[te[3] - '0'][te[2] - 'a' + 1];
        map[te[3] - '0'][te[2] - 'a' + 1] = 0;
      } else {
        map[te[1] - '0'][te[0] - 'a' + 1] = map[te[3] - '0'][te[2] - 'a' + 1];
        map[te[3] - '0'][te[2] - 'a' + 1] = koma;
        switch (koma) {
        case 6:
          prisoner_1[0]--;
          break;
        case 7:
          prisoner_1[0]--;
          break;
        case 8:
          prisoner_1[1]--;
          break;
        case 9:
          prisoner_1[2]--;
          break;
        }
      }
    }
  } else {
    if (te[0] == '0') {
      switch (te[1]) {
      case 6:
        prisoner_2[0]++;
        break;
      case 8:
        prisoner_2[1]++;
        break;
      case 9:
        prisoner_2[2]++;
        break;
      }
      map[te[3] - '0'][te[2] - 'a' + 1] = 0;
    } else {
      if (koma == 0) {
        map[te[1] - '0'][te[0] - 'a' + 1] = map[te[3] - '0'][te[2] - 'a' + 1];
        map[te[3] - '0'][te[2] - 'a' + 1] = 0;
      } else {
        map[te[1] - '0'][te[0] - 'a' + 1] = map[te[3] - '0'][te[2] - 'a' + 1];
        map[te[3] - '0'][te[2] - 'a' + 1] = koma;
        switch (koma) {
        case 1:
          prisoner_2[0]--;
          break;
        case 2:
          prisoner_2[0]--;
          break;
        case 3:
          prisoner_2[1]--;
          break;
        case 4:
          prisoner_2[2]--;
          break;
        }
      }
    }
  }
}

void call_AI(int map[6][5], struct Position map_state[], char * te, int turn_num, int prisoner_1[], int prisoner_2[], int l) {
  if (l != 9) {
    l = l - 1;
    low_AI(map, & map_state[0], te, turn_num, prisoner_1, prisoner_2, l);
  } else {
    high_AI(map, & map_state[0], te, turn_num, prisoner_1, prisoner_2);
  }
}

void low_AI(int map[6][5], struct Position map_state[], char * te, int turn_num, int prisoner_1[], int prisoner_2[], int depth) {
  int best, node[4] = {
    0,
    -1,
    0,
    0
  }, i, k, j, hanpuku = 0;
  char str[1000] = {
    ""
  }, c;
  struct BestPV pv[100];
  struct Hash * tikan;

  for (i = 0; i < 100; i++) {
    pv[i].sasite[0] = '\0';
  }

  tikan = (struct Hash * ) malloc(sizeof(struct Hash) * 1000000);
  if (tikan == NULL) {
    printf("Not enough memory!\n");
  }
  for (i = 0; i < 1000000; i++) {
    for (k = 0; k < 5; k++) {
      for (j = 0; j < 6; j++) {
        tikan[i].hash_map[j][k] = 0;
      }
    }
    for (k = 0; k < 4; k++) {
      tikan[i].hash_prisoner_1[k] = 0;
      tikan[i].hash_prisoner_2[k] = 0;
    }
    tikan[i].deep = 0;
    tikan[i].low = 0;
    tikan[i].upp = 0;
  }

  do {
    /*for(i=0;i<100;i++){
    	pv[i].sasite[0]='\0';
    }*/
    hanpuku++;
    node[3] = hanpuku;
    node[2] = 0;
    node[0] = 0;
    best = NegaMax(map, & map_state[0], & pv[0], & tikan[0], str, turn_num, prisoner_1, prisoner_2, 0, hanpuku, node, -9999, 9999);
    if (node[1] == -1) {
      node[1] = 0;
    }
    strcpy(str, pv[0].sasite);
    i = 0;
    do {
      switch (pv[0].sasite[i]) {
      case 1:
        pv[0].sasite[i] = 'p';
        break;
      case 3:
        pv[0].sasite[i] = 's';
        break;
      case 4:
        pv[0].sasite[i] = 'j';
        break;
      case 6:
        pv[0].sasite[i] = 'p';
        break;
      case 8:
        pv[0].sasite[i] = 's';
        break;
      case 9:
        pv[0].sasite[i] = 'j';
        break;
      }
      i++;
    } while (strlen(pv[0].sasite) > i);
    printf("%s\n", pv[0].sasite);
    if (best > 9000 || -9000 > best) {
      break;
    }

  } while (hanpuku < depth);

  for (i = 0; i < 4; i++) {
    te[i] = str[i];
  }
  if (te[0] == '0') {
    switch (te[1]) {
    case 1:
      c = 'p';
      break;
    case 3:
      c = 's';
      break;
    case 4:
      c = 'j';
      break;
    case 6:
      c = 'p';
      break;
    case 8:
      c = 's';
      break;
    case 9:
      c = 'j';
      break;
    }
    printf("Best: %c%c%c%c, Rating: %d, Search Depth: %d, Total Search Phase: %d\n", te[0], c, te[2], te[3], best, hanpuku, node[0]);
    fptr = fopen("C:\\Users\\admin\\Documents\\12Chess\\pyctalk\\ctopy.txt", "a");
    fprintf(fptr, "%c%c%c%c\n", te[0], c, te[2], te[3]);
    fclose(fptr);
  } else {
    printf("Best Practice: %s, Rating: %d, Search Depth: %d, Total Search Phase: %d\n", te, best, hanpuku, node[0]);
    fptr = fopen("C:\\Users\\admin\\Documents\\12Chess\\pyctalk\\ctopy.txt", "a");
    fprintf(fptr, "%s\n", te);
    fclose(fptr);
  }
  k = 0;
  for (i = 0; i < 1000000; i++) {
    if (tikan[i].deep > 0) {
      k++;
    }
  }
  printf("%f\n", (double) k / 10000.0);
  free(tikan);
}

void high_AI(int map[6][5], struct Position map_state[], char * te, int turn_num, int prisoner_1[], int prisoner_2[]) {
  int best, node[4] = {
    0,
    -1,
    0,
    0
  }, depth = 0, i, j, k;
  double start, end;
  char str[1000] = {
    ""
  }, c;
  struct BestPV pv[100];
  struct Hash * tikan;

  for (i = 0; i < 100; i++) {
    pv[i].sasite[0] = '\0';
  }

  tikan = (struct Hash * ) malloc(sizeof(struct Hash) * 1000000);
  if (tikan == NULL) {
    printf("Not enough memory!\n");
  }
  for (i = 0; i < 1000000; i++) {
    for (k = 0; k < 5; k++) {
      for (j = 0; j < 6; j++) {
        tikan[i].hash_map[j][k] = 0;
      }
    }
    for (k = 0; k < 4; k++) {
      tikan[i].hash_prisoner_1[k] = 0;
      tikan[i].hash_prisoner_2[k] = 0;
    }
    tikan[i].deep = 0;
    tikan[i].low = 0;
    tikan[i].upp = 0;
  }

  do {
    /*for(i=0;i<100;i++){
    	pv[i].sasite[0]='\0';
    }*/
    depth++;
    node[3] = depth;
    node[2] = 0;
    node[0] = 0;
    start = clock();
    best = NegaMax(map, & map_state[0], & pv[0], & tikan[0], str, turn_num, prisoner_1, prisoner_2, 0, depth, node, -9999, 9999);
    end = clock();
    if (node[1] == -1) {
      node[1] = 0;
    }
    strcpy(str, pv[0].sasite);
    i = 0;
    do {
      switch (pv[0].sasite[i]) {
      case 1:
        pv[0].sasite[i] = 'p';
        break;
      case 3:
        pv[0].sasite[i] = 's';
        break;
      case 4:
        pv[0].sasite[i] = 'j';
        break;
      case 6:
        pv[0].sasite[i] = 'p';
        break;
      case 8:
        pv[0].sasite[i] = 's';
        break;
      case 9:
        pv[0].sasite[i] = 'j';
        break;
      }
      i++;
    } while (strlen(pv[0].sasite) > i);
    printf("%s, %f\n", pv[0].sasite, (end - start) / CLOCKS_PER_SEC);
    if (best > 9000 || -9000 > best) {
      break;
    }
  } while (((end - start) / CLOCKS_PER_SEC) < 1.0);

  for (i = 0; i < 4; i++) {
    te[i] = str[i];
  }
  if (te[0] == '0') {
    switch (te[1]) {
    case 1:
      c = 'p';
      break;
    case 3:
      c = 's';
      break;
    case 4:
      c = 'j';
      break;
    case 6:
      c = 'p';
      break;
    case 8:
      c = 's';
      break;
    case 9:
      c = 'j';
      break;
    }
    printf("call_AI3");
    printf("Best: %c%c%c%c, Rating: %d, Search Depth: %d, Total Search Phase: %d\n", te[0], c, te[2], te[3], best, depth, node[0]);
    fptr = fopen("C:\\Users\\admin\\Documents\\12Chess\\pyctalk\\ctopy.txt", "a");
    fprintf(fptr, "%c%c%c%c\n", te[0], c, te[2], te[3]);
    fclose(fptr);
  } else {
    printf("call_AI3");
    printf("Best Practice: %s, Rating: %d, Search Depth: %d, Total Search Phase: %d\n", te, best, depth, node[0]);
    fptr = fopen("C:\\Users\\admin\\Documents\\12Chess\\pyctalk\\ctopy.txt", "a");
    fprintf(fptr, "%s\n", te);
    fclose(fptr);
  }
  k = 0;
  for (i = 0; i < 1000000; i++) {
    if (tikan[i].deep > 0) {
      k++;
    }
  }
  printf("%f\n", (double) k / 10000.0);
  free(tikan);

}

int NegaMax(int map[6][5], struct Position map_state[], struct BestPV pv[], struct Hash tikan[], char * str, int turn_num, int prisoner_1[], int prisoner_2[], int deep, int depth, int * node, int al, int be) {
  int i, j = 0, k, l, n, x,
    var = 0, best, p, sen, i_prisoner_1[4], i_prisoner_2[4], koma;
  unsigned long long int Hash = 0;
  int t_map[6][5];
  char t_te[1000] = {
    ""
  }, i_te[1000] = {
    ""
  }, same[1000] = {
    ""
  };
  struct Position * t_map_state;

  Hash = hashti(map, prisoner_1, prisoner_2, turn_num);
  //Hash=Hash%1000000;

  n = 0;
  do {
    if (tikan[Hash].deep >= depth - deep && tikan[Hash].deep != 0) {
      for (i = 1; i < 4; i++) {
        for (j = 1; j < 5; j++) {
          if (tikan[Hash].hash_map[j][i] != map[j][i]) {
            n = -1;
            //output(map, prisoner_1, prisoner_2);
            //output(tikan[Hash].hash_map, tikan[Hash].hash_prisoner_1, tikan[Hash].hash_prisoner_2);
            break;
          }
        }
        if (n == -1) {
          break;
        }
      }
      if (n == 0) {
        for (l = 0; l < 3; l++) {
          if (tikan[Hash].hash_prisoner_1[l] != prisoner_1[l] || tikan[Hash].hash_prisoner_2[l] != prisoner_2[l]) {
            n = -1;
            break;
          }
        }
      }
      if (n == 0) {
        if (tikan[Hash].low >= be) {
          for (i = 0; i < 4; i++) {
            pv[99].sasite[deep * 4 + i] = tikan[Hash].hash_te[i];
          }
          pv[99].sasite[deep * 4 + i] = '\0';
          return tikan[Hash].low;
        }
        if (tikan[Hash].upp >= al) {
          for (i = 0; i < 4; i++) {
            pv[99].sasite[deep * 4 + i] = tikan[Hash].hash_te[i];
          }
          pv[99].sasite[deep * 4 + i] = '\0';
          return tikan[Hash].upp;
        }
        if (tikan[Hash].low > al) {
          al = tikan[Hash].low;
          break;
        }
        if (tikan[Hash].upp < be) {
          be = tikan[Hash].upp;
          break;
        }
      } else {
        Hash += 2;
      }
    } else {
      break;
    }
  } while (n = 0);

  x = c_check(map, t_te, turn_num, prisoner_1, prisoner_2);

  if (x == 1) {
    if (turn_num % 2 == 0) {
      return -9999 + deep - 1;
    } else {
      return 9999 - deep + 1;
    }
  }
  if (x == 2) {
    if (turn_num % 2 == 0) {
      return 9999 - deep + 1;
    } else {
      return -9999 + deep - 1;
    }
  }

  if (str[deep * 4] != '\0' && strlen(t_te) != 4) {
    for (i = 0; i < strlen(t_te); i += 4) {
      if (t_te[i] == str[deep * 4] && t_te[i + 1] == str[deep * 4 + 1] && t_te[i + 2] == str[deep * 4 + 2] && t_te[i + 3] == str[deep * 4 + 3]) {
        i_te[0] = t_te[0];
        i_te[1] = t_te[1];
        i_te[2] = t_te[2];
        i_te[3] = t_te[3];

        t_te[0] = str[deep * 4];
        t_te[1] = str[deep * 4 + 1];
        t_te[2] = str[deep * 4 + 2];
        t_te[3] = str[deep * 4 + 3];

        t_te[i] = i_te[0];
        t_te[i + 1] = i_te[1];
        t_te[i + 2] = i_te[2];
        t_te[i + 3] = i_te[3];
        break;
      }
    }
  }

  if (depth == deep) {
    var += prisoner_1[0] * 15;
    var += prisoner_1[1] * 30;
    var += prisoner_1[2] * 30;
    var -= prisoner_2[0] * 15;
    var -= prisoner_2[1] * 30;
    var -= prisoner_2[2] * 30;

    for (i = 1; i < 5; i++) {
      for (j = 1; j < 4; j++) {
        switch (map[i][j]) {
        case 0:
          break;
        case 1:
          var += 10;
          break;
        case 2:
          var += 25;
          break;
        case 3:
          var += 20;
          break;
        case 4:
          var += 20;
          break;
        case 6:
          var -= 10;
          break;
        case 7:
          var -= 25;
          break;
        case 8:
          var -= 20;
          break;
        case 9:
          var -= 20;
          break;
        }
      }
    }

    if (turn_num % 2 == 0) {
      return -
        var;
    } else {
      return var;
    }
  }

  t_map_state = (struct Position * ) malloc(sizeof(struct Position) * 1000);

  for (i = 0; i < 1000; i++) {
    t_map_state[i] = map_state[i];
  }
  if (deep != 0) {
    sen = check_draw( & t_map_state[0], map, prisoner_1, prisoner_2);
    if (sen == -1) {
      free(t_map_state);
      return 0;
    }
  }

  j = 0;
  for (i = 0; i < 4; i++) {
    i_te[i] = t_te[j];
    pv[99].sasite[deep * 4 + i] = i_te[i];
    j++;
  }
  pv[99].sasite[deep * 4 + i] = '\0';

  koma = move(map, i_te, turn_num, prisoner_1, prisoner_2);
  node[0]++;
  var = -NegaMax(map, & t_map_state[0], & pv[0], & tikan[0], str, turn_num + 1, prisoner_1, prisoner_2, deep + 1, depth, node, -be, -al);
  back(map, i_te, turn_num, prisoner_1, prisoner_2, koma);

  if (var > al) {
    al =
      var;
    best = al;

    Hash = hashti(map, prisoner_1, prisoner_2, turn_num);
    //Hash=Hash%1000000;
    n = 0;
    do {
      if (tikan[Hash].deep == 0) {
        for (i = 1; i < 5; i++) {
          for (p = 1; p < 4; p++) {
            tikan[Hash].hash_map[i][p] = map[i][p];
          }
        }
        for (k = 0; k < 3; k++) {
          tikan[Hash].hash_prisoner_1[k] = prisoner_1[k];
          tikan[Hash].hash_prisoner_2[k] = prisoner_2[k];
        }
        tikan[Hash].deep = depth - deep - 1;
        if (best <= al) {
          tikan[Hash].upp = best;
        } else {
          if (best >= be) {
            tikan[Hash].low = best;
          } else {
            tikan[Hash].upp = best;
            tikan[Hash].low = best;
          }
        }
        strcpy(tikan[Hash].hash_te, i_te);
      } else {
        for (i = 1; i < 4; i++) {
          for (p = 1; p < 5; p++) {
            if (tikan[Hash].hash_map[p][i] != map[p][i]) {
              n = -1;
              //output(map, prisoner_1, prisoner_2);
              //output(tikan[Hash].hash_map, tikan[Hash].hash_prisoner_1, tikan[Hash].hash_prisoner_2);
              break;
            }
          }
          if (n == -1) {
            break;
          }
        }
        if (n == 0) {
          for (l = 0; l < 3; l++) {
            if (tikan[Hash].hash_prisoner_1[l] != prisoner_1[l] || tikan[Hash].hash_prisoner_2[l] != prisoner_2[l]) {
              n = -1;
              break;
            }
          }
        }
        if (n == 0) {
          if (tikan[Hash].deep < depth - deep) {
            tikan[Hash].deep = depth - deep - 1;
            if (best <= al) {
              tikan[Hash].upp = best;
            } else {
              if (best >= be) {
                tikan[Hash].low = best;
              } else {
                tikan[Hash].upp = best;
                tikan[Hash].low = best;
              }
            }
            strcpy(tikan[Hash].hash_te, i_te);
          }
          break;
        }
        if (n == -1) {
          Hash = (Hash + 2) % 1000000;
        }
      }
    } while (n = 0);

    if (deep + 1 == depth) {
      strcpy(pv[deep].sasite, pv[99].sasite);
    } else {
      strcpy(pv[deep].sasite, pv[deep + 1].sasite);
    }

  }

  while (t_te[j] != 0 && al < be) {
    for (i = 0; i < 4; i++) {
      i_te[i] = t_te[j];
      pv[99].sasite[deep * 4 + i] = i_te[i];
      j++;
    }
    pv[99].sasite[deep * 4 + i] = '\0';

    koma = move(map, i_te, turn_num, prisoner_1, prisoner_2);
    var = -NegaMax(map, & t_map_state[0], & pv[0], & tikan[0], str, turn_num + 1, prisoner_1, prisoner_2, deep + 1, depth, node, -al - 1, -al);
    if (var > al) {
      al =
        var;
      var = -NegaMax(map, & t_map_state[0], & pv[0], & tikan[0], str, turn_num + 1, prisoner_1, prisoner_2, deep + 1, depth, node, -be, -al);
    }
    back(map, i_te, turn_num, prisoner_1, prisoner_2, koma);

    if (var > al) {
      al =
        var;

      Hash = hashti(map, prisoner_1, prisoner_2, turn_num);
      //Hash=Hash%1000000;
      n = 0;
      do {
        if (tikan[Hash].deep == 0) {
          for (i = 1; i < 5; i++) {
            for (p = 1; p < 4; p++) {
              tikan[Hash].hash_map[i][p] = map[i][p];
            }
          }
          for (k = 0; k < 3; k++) {
            tikan[Hash].hash_prisoner_1[k] = prisoner_1[k];
            tikan[Hash].hash_prisoner_2[k] = prisoner_2[k];
          }
          tikan[Hash].deep = depth - deep - 1;
          tikan[Hash].low = al;
          tikan[Hash].upp = be;
          strcpy(tikan[Hash].hash_te, i_te);
        } else {
          for (i = 1; i < 4; i++) {
            for (p = 1; p < 5; p++) {
              if (tikan[Hash].hash_map[p][i] != map[p][i]) {
                n = -1;
                //output(map, prisoner_1, prisoner_2);
                //output(tikan[Hash].hash_map, tikan[Hash].hash_prisoner_1, tikan[Hash].hash_prisoner_2);
                break;
              }
            }
            if (n == -1) {
              break;
            }
          }
          if (n == 0) {
            for (l = 0; l < 3; l++) {
              if (tikan[Hash].hash_prisoner_1[l] != prisoner_1[l] || tikan[Hash].hash_prisoner_2[l] != prisoner_2[l]) {
                n = -1;
                break;
              }
            }
          }
          if (n == 0) {
            if (tikan[Hash].deep < depth - deep) {
              tikan[Hash].deep = depth - deep - 1;
              tikan[Hash].low = al;
              tikan[Hash].upp = be;
              strcpy(tikan[Hash].hash_te, i_te);
            }
            break;
          }
          if (n == -1) {
            Hash = (Hash + 2) % 1000000;
          }
        }
      } while (n = 0);
      if (deep + 1 == depth) {
        strcpy(pv[deep].sasite, pv[99].sasite);
      } else {
        strcpy(pv[deep].sasite, pv[deep + 1].sasite);
      }
    }
  }
  free(t_map_state);
  return al;
}

int hashti(int map[6][5], int prisoner_1[], int prisoner_2[], int turn_num) {
  unsigned long long int index[4], Hash;
  unsigned long long int a = 2305843009213693951, b = 12345, c = 16807;

  index[0] = map[1][1] * 11 * 11 * 11 + map[2][1] * 11 * 11 + map[3][1] * 11 + map[4][1];
  index[1] = map[1][2] * 11 * 11 * 11 + map[2][2] * 11 * 11 + map[3][2] * 11 + map[4][2];
  index[2] = map[1][3] * 11 * 11 * 11 + map[2][3] * 11 * 11 + map[3][3] * 11 + map[4][3];
  index[3] = prisoner_1[0] * 3 * 3 * 3 * 3 * 3 + prisoner_1[1] * 3 * 3 * 3 * 3 + prisoner_1[2] * 3 * 3 * 3 + prisoner_2[0] * 3 * 3 + prisoner_2[1] * 3 + prisoner_2[2];

  Hash = index[3] * 17 * 17 * 17 + index[2] * 17 * 17 + index[1] * 17 + index[0];

  Hash = (c * Hash) % 1000001;
  if (turn_num % 2 != Hash % 2) {
    Hash++;
  }
  //init_genrand(Hash);
  //Hash=genrand_int32();
  //printf("%lld\n", Hash);
  return Hash;
}

void person_to_computer(int c) {
  int map[6][5] = {
   -1, -1, -1, -1, -1,
    -1, 9, 10, 8, -1,
    -1, 0, 6, 0, -1,
    -1, 0, 1, 0, -1,
    -1, 3, 5, 4, -1,
    -1, -1, -1, -1, -1
  };
  int temap, turn_num = 1, i, j, k, x = 0, m, sen, prisoner_1[7] = {
    0
  }, prisoner_2[7] = {
    0
  };
  char te[200] = {
    ""
  }, m_te[10] = {
    ""
  };
  struct Position map_state[1000];

  for (i = 0; i < 1000; i++) {
    for (k = 0; k < 5; k++) {
      for (j = 0; j < 6; j++) {
        map_state[i].pos_map[j][k] = 0;
      }
    }
    for (k = 0; k < 4; k++) {
      map_state -> pos_prisoner_1[k] = 0;
      map_state -> pos_prisoner_2[k] = 0;
    }
    map_state[i].kaisu = -1;
  }

  temap = rand() % 2;
  fptr = fopen("C:\\Users\\admin\\Documents\\12Chess\\pyctalk\\ctopy.txt", "a");
  if (temap == 1) {
    printf("It's your first move (Red).\n");
    fprintf(fptr, "RED\n");
  } else {
    printf("It's your second move (Blue).\n");
    fprintf(fptr, "BLUE\n");
  }
  fclose(fptr);

  output(map, prisoner_1, prisoner_2);

  do {
    do {
      if (x == 0) {
        sen = check_draw( & map_state[0], map, prisoner_1, prisoner_2);
      }
      x = check(map, te, turn_num, prisoner_1, prisoner_2);
      if (x == 1) {
        printf("First player wins!\n");
        return;
      }
      if (x == 2) {
        printf("Second player wins!\n");
        return;
      }
      if (sen == -1) {
        printf("Draw!\n");
        return;
      }
      if (turn_num % 2 == temap) {
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        if (turn_num % 2 == 1) {
          while (1) {
            fptr = fopen("C:\\Users\\admin\\Documents\\12Chess\\pyctalk\\pytoc.txt", "r");
            int idx = 0;
            char str[10];
            char line[20] = {
              0
            };
            while (fgets(line, 20, fptr)) {
              strcpy(str, line);
              idx++;
            }
            fclose(fptr);
            char str2[10];
            strncpy(str2, str, 4);
            str2[4] = '\0';
            int length = idx;
            if (length > before_length) {
              before_length++;
              // printf("%s\n", str2);
              strcpy(m_te, str2);
              break;
            }
          }
          if (m_te[0] == '0') {
            switch (m_te[1]) {
            case 'p':
              m_te[1] = 1;
              break;
            case 'n': // ?
              m_te[1] = 2;
              break;
            case 's':
              m_te[1] = 3;
              break;
            case 'j':
              m_te[1] = 4;
              break;
            case 'r': // ?
              m_te[1] = 5;
              break;
            }
          }

        } else {
          while (1) {
            fptr = fopen("C:\\Users\\admin\\Documents\\12Chess\\pyctalk\\pytoc.txt", "r");
            int idx = 0;
            char str[10];
            char line[20] = {
              0
            };
            while (fgets(line, 20, fptr)) {
              strcpy(str, line);
              idx++;
            }
            fclose(fptr);
            char str2[10];
            strncpy(str2, str, 4);
            str2[4] = '\0';
            int length = idx;
            if (length > before_length) {
              before_length++;
              // printf("%s\n", str2);
              strcpy(m_te, str2);
              break;
            }
          }
          if (m_te[0] == '0') {
            switch (m_te[1]) {
            case 'p':
              m_te[1] = 6;
              break;
            case 'n': // ?
              m_te[1] = 7;
              break;
            case 's':
              m_te[1] = 8;
              break;
            case 'j':
              m_te[1] = 9;
              break;
            case 'r': // ?
              m_te[1] = 10;
              break;
            }
          }
        }
      } else {
        call_AI(map, & map_state[0], m_te, turn_num, prisoner_1, prisoner_2, c);
      }
      m = 0;
      x = -1;
      printf("%s\n", m_te);
      if (strlen(m_te) == 4) {
        j = strlen(te) / 4;
        for (i = 0; i < j; i++) {
          if (m_te[0] == te[m] && m_te[1] == te[m + 1] && m_te[2] == te[m + 2] && m_te[3] == te[m + 3]) {
            x = 0;
            break;
          } else {
            m += 4;
          }
        }

      }
    } while (x == -1);
    // printf("%d", strlen(m_te));
    move(map, m_te, turn_num, prisoner_1, prisoner_2);
    turn_num++;
    output(map, prisoner_1, prisoner_2);
    x = 0;
  } while (x == 0);
}
