#include"snake.h"

int main(void)
{
	setrendermode(RENDER_MANUAL);//设置为手动更新模式
	initgraph(WIDTH, HEIGHT, INIT_RENDERMANUAL);
	setbkcolor(CYAN); //背景
	setbkmode(TRANSPARENT);

	begin();

	getch();
	cleardevice();
	init_Snake();
	init_food_monster();

	new_key = getch();//按方向键开始游戏
	start_time = clock();

	playGame();
	free_snake(head);
	over();

	getch();
	closegraph();

	return 0;
}