#ifndef SNAKE_H
#define SNAKE_H

#include<graphics.h>
#include<stdio.h>
#include<time.h>

const int WIDTH = 800, HEIGHT = 640, grid_len = 16;

typedef struct Snake
{
	int x;
	int y;
	struct Snake* next;
}snake;

//extern声明变量，未定义。
extern int  xfood[9], yfood[9];	      //食物坐标 
extern char a[10];			          //存放分数的字符型数组
extern char new_key;                  //方向（按键）
extern snake* head, * end, * p;       //蛇头、蛇尾 、辅助指针 
extern clock_t start_time, end_time;  //时间

class Monster
{
public:
      int  xmonster, ymonster;       //怪物坐标 
};

void begin();
void over();
void init_Snake();
void init_food_monster();
void playGame();
void free_snake(Snake* head);

#endif
