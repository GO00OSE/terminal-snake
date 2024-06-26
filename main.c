#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

int gridW = 10;
int gridH = 10;
int tail_len = 0;
int tail_x[100];
int tail_y[100];

int apple_pos[] = {7,5};

int tailCheck(int x, int y) {
  for(int i = tail_len; i > 0; i--){
    if(x == tail_x[i] && y == tail_y[i]){
      return 1;
    }
  }
  return 0;
}

void newapple() {
  int overlapped = 1;
  while(overlapped){
    overlapped = 0;
    apple_pos[0] = rand() % 10;
    apple_pos[1] = rand() % 10;
    for(int i = tail_len; i > 0; i--){
      if(apple_pos[0] == tail_x[i] && apple_pos[1] == tail_y[i]){
        overlapped = 1;
        break;
      }
    }
  }
}

void drawscr(int player_pos[], int apple_pos[]){
  char bg[] = "\x1b[1;37m[ ]";
  char snake[] = "\x1b[1;32m[x]";
  char apple[] = "\x1b[1;31m[o]";

  for(int y = 0; y <= gridH - 1; y++){
    for(int x = 0; x <= gridH - 1; x++){
      if(x == player_pos[0] && y == player_pos[1]){
        printf("%s", snake);
      } else if(tailCheck(x, y) == 1) {
        printf("%s", snake);
      } else if(x == apple_pos[0] && y == apple_pos[1]) {
        printf("%s", apple);
      } else {
        printf("%s", bg);
      }
    }
    printf("\n");  
  }
  printf("             \x1b[1;32m %d \x1b[1;37m \n", tail_len); 
}

int main() {
  printf("\x1b[?25l\n"); // Hide cursor
  int player_pos[] = {1,5};
  clock_t cur_time = clock();
  clock_t start_time = clock();
  double time_passed;
  int tick_num = 0;

  int newdir = 0;
  int currdir = 0;
  char ch;
        //󰝤 󰝤 󰝤 󰝤 󰝤 󰝤 󰝤 󰝤 󰝤 󰝤
        //            [1][2][3][4][5][6][7][8][9][10]
  printf("\x1b[1;32m           - Snake - \x1b[1;37m\n");

  while(1){
    if(_kbhit()){
      ch = _getch();
      if(ch == 'q'){
        break;
      } else if(ch == 'w' && currdir != 3){
        newdir = 1;
      } else if(ch == 's' && currdir != 1){
        newdir = 3;
      } else if(ch == 'a' && currdir != 0){
        newdir = 2;
      } else if(ch == 'd' && currdir != 2){
        newdir = 0;
      } 
    }
    
    cur_time = clock();
    time_passed = ((double)(cur_time - start_time)) / CLOCKS_PER_SEC * 1000;

    if(time_passed >= 150){ //default: 250
      currdir = newdir;
      if(currdir == 0)
        player_pos[0]++;
      else if(currdir == 1)
        player_pos[1]--;
      else if (currdir == 2)
        player_pos[0]--;
      else if (currdir == 3)
        player_pos[1]++;

      
      if(player_pos[0] > gridW - 1) {
        break;
      } else if(player_pos[0] < 0){
        break;
      } 
      if(player_pos[1] > gridH - 1){
        break;
      } else if(player_pos[1] < 0){
        break;
      }
      
      for(int i = tail_len; i > 0; i--){
        if(player_pos[0] == tail_x[i] && player_pos[1] == tail_y[i])
          goto gameover;
      }

      if(apple_pos[0] == player_pos[0] && apple_pos[1] == player_pos[1]){
        tail_len++;
        newapple(); 
      }

      for(int i = tail_len; i > 0; i--){
        tail_x[i] = tail_x[i-1];
        tail_y[i] = tail_y[i-1];
      }

      tail_x[0] = player_pos[0];
      tail_y[0] = player_pos[1];

      drawscr(player_pos, apple_pos);
      printf("\e[%dA", gridH + 1);
      start_time = clock();

      tick_num++;
    }
  }
gameover:
  printf("\x1b[%dE", gridH + 1);
  for(int y = 0; y <= gridH; y++){
    printf("\x1b[1F"); // Move to beginning of previous line
    printf("\x1b[2K"); // Clear entire line
  }
  printf("\x1b[?25h");
  printf("\x1b[5m\x1b[1;31m          [GAMEOVER] \x1b[25m \n  \x1b[1;32m         Score: %d \x1b[1;37m \n", tail_len);
}
