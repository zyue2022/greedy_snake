#include"snake.h"

//定义变量
snake* head, * end, * p;        //蛇头、蛇尾、辅助指针
int  xfood[9], yfood[9];	    //9个食物坐标
char new_key;			        //方向（按键）
clock_t start_time, end_time;   //时间

int i = 0, j = 0, score = 0, index = 0;
int count = 0, new_grid_num = 3;
char Si;

Monster m, m_temp;

//分别存放是否通关、得分、时间的字符型数组
char s1[10] = "Failed!!!",
s2[20] = "最终的得分为:0",
s3[20] = "Snake-Food:9,Time:0";

//开始界面
void begin()
{
	setcolor(WHITE);
	setcaption("贪吃蛇_按任意键开始!!!");
	setfont(100, 0, "华文彩云");
	outtextxy(250, 150, "贪吃蛇");
	setfont(30, 0, "宋体");
	outtextxy(180, 350, "每吃到一个食物，得1分，满分9分");
}

//结束界面
void over()
{
	cleardevice();
	setcolor(WHITE);
	setfont(100, 0, "华文彩云");
	outtextxy(130, 180, "GAME OVER!");

	setcaption(s1);

	setfont(50, 0, "宋体");
	outtextxy(220, 350, s2);
}

//初始化蛇，最好用头插法，因为键盘控制的是头
void init_Snake()
{
	ege_enable_aa(true);				//抗锯齿

	end = (snake*)malloc(sizeof(snake));
	end->next = NULL;

	//创建三个（x,y,next)蛇身位置
	head = (snake*)malloc(sizeof(snake));
	head->x = WIDTH / 2;
	head->y = HEIGHT / 2;
	head->next = end;   //头插法，让新建立的头跟尾连起来
	end = head;	        //把头赋给尾，成为尾的一部分

	//画蛇头
	setfillcolor(EGERGB(0xFF, 0xA5, 0));
	bar(end->x, end->y, end->x + grid_len, end->y + grid_len);
}

void set_monster_coordinate()
{
	//m.xmonster = grid_len * (rand() % (WIDTH / grid_len));
	//m.ymonster = grid_len * (rand() % (HEIGHT / grid_len));
	m.xmonster = 368;
	m.ymonster = 304;
}

void set_food_coordinate()
{
	for (i = 0; i < 9; i++)
	{
		xfood[i] = grid_len * (rand() % (WIDTH / grid_len));
		yfood[i] = grid_len * (rand() % (HEIGHT / grid_len));
	}
}

//创建食物
void init_food_monster()
{
	srand(time(0));
	set_monster_coordinate();
	set_food_coordinate();

	p = head;

	//检查坐标是否重合
	while (1)
	{
		//若怪物坐标和蛇重叠了，回到生成坐标循环
		if ((p->x == m.xmonster) && (p->y == m.ymonster))
		{
			set_monster_coordinate();
			continue;
		}

		//若食物坐标与蛇、怪物坐标重叠了，回到生成坐标循环
		for (i = 0; i < 9; i++)
		{
			if (((p->x == xfood[i]) && (p->y == yfood[i])) || ((p->x == m.xmonster) && (p->y == m.ymonster)))
			{
				set_food_coordinate();
				p = head;
				break;
			}
		}

		if (p->next == NULL) //与逐段蛇身比较，比较完毕后，跳出循环 ！！！next是从头往尾的方向
		{
			break;
		}
		p = p->next;
	}

	//画怪物
	setfillcolor(RED);
	bar(m.xmonster, m.ymonster, m.xmonster + grid_len, m.ymonster + grid_len);

	//画食物
	setcolor(GREEN);
	setfont(grid_len, grid_len, "宋体");
	setfillcolor(EGERGB(0xFF, 0xFF, 0x66));
	for (i = 0; i < 9; i++)
	{
		bar(xfood[i], yfood[i], xfood[i] + grid_len, yfood[i] + grid_len);
		Si = (i + 1) + '0';
		outtextxy(xfood[i], yfood[i], Si);
	}
}

//判断是否撞墙、蛇头是否与其他蛇身重合、被怪物吃
int Hitwall_and_Hitself_HeadEatByMonster()
{
	if (head->x < 0 || head->x == WIDTH || head->y < 0 || head->y == HEIGHT)
	{
		return 1;
	}

	if ((head->x == m.xmonster) && (head->y == m.ymonster))
	{
		setfillcolor(RED);
		bar(m.xmonster, m.ymonster, m.xmonster + grid_len, m.ymonster + grid_len);
		return 1;
	}

	/*p = head;//蛇头是否与其他蛇身重合
	while (p->next != NULL)
	{
		p = p->next;
		if (head->x == p->x && head->y == p->y)
		{
			return 1;
		}
	}*/
	return 0;
}

void draw_snake()
{
	//建立新头部
	p = (Snake*)malloc(sizeof(Snake));
	p->next = head;

	switch (new_key)//每个蛇身相差grid_len
	{
	case 'D'://向右
		p->x = head->x + grid_len;
		p->y = head->y;
		break;
	case 'W'://向上
		p->x = head->x;
		p->y = head->y - grid_len;
		break;
	case 'S'://向下
		p->x = head->x;
		p->y = head->y + grid_len;
		break;
	case 'A'://向左
		p->x = head->x - grid_len;
		p->y = head->y;
		break;
	}

	//画出新的头部 !!!
	setfillcolor(EGERGB(0xFF, 0xA5, 0));
	bar(p->x, p->y, p->x + grid_len, p->y + grid_len);

	bool sign = true;

	head = p;

	for (i = 0; i < 9; i++)
	{
		//如果移动的下一格刚好是食物的位置，不用释放蛇尾
		if ((head->x == xfood[i]) && (head->y == yfood[i]))
		{
			xfood[i] = yfood[i] = 1500;//把食物移出窗口
			score += 1;

			sign = false;

			count = i + 1;
			new_grid_num = new_grid_num + count;
		}
	}

	//移动的下一格不是食物的位置
	if (sign)
	{
		while (p->next->next != NULL) { p = p->next; }//指向蛇尾前一格，因为可能需要释放蛇尾，节约内存

		if (index <= new_grid_num)
		{
			setfillcolor(EGERGB(0xFF, 0xA5, 0));
			bar(p->x, p->y, p->x + grid_len, p->y + grid_len);//不释放蛇尾

			index++;
		}
		else
		{
			setfillcolor(CYAN);//填成背景色
			bar(p->x, p->y, p->x + grid_len, p->y + grid_len);//释放蛇尾
			free(p->next);
			p->next = NULL;
		}
	}
}

void draw_monster()
{
	m_temp = m;

	if (m_temp.xmonster > head->x)
	{
		m.xmonster = m.xmonster - grid_len;
	}
	if (m_temp.xmonster < head->x)
	{
		m.xmonster = m.xmonster + grid_len;
	}
	if (m_temp.ymonster > head->y)
	{
		m.ymonster = m.ymonster - grid_len;
	}
	if (m_temp.ymonster < head->y)
	{
		m.ymonster = m.ymonster + grid_len;
	}

	//画移动的怪物
	setfillcolor(CYAN);//背景色
	int flag = 1;

	for (i = 0; i < 9; i++)
	{
		if ((m_temp.xmonster == xfood[i]) && (m_temp.ymonster == yfood[i]))
		{
			setfillcolor(EGERGB(0xFF, 0xFF, 0x66));
			bar(m_temp.xmonster, m_temp.ymonster, m_temp.xmonster + grid_len, m_temp.ymonster + grid_len);
			Si = (i + 1) + '0';
			outtextxy(xfood[i], yfood[i], Si);
			flag = 0;
			break;
		}
	}
	p = head;
	while (1)
	{
		if ((m_temp.xmonster == p->x) && (m_temp.ymonster == p->y))
		{
			setfillcolor(EGERGB(0xFF, 0xA5, 0));
			bar(m_temp.xmonster, m_temp.ymonster, m_temp.xmonster + grid_len, m_temp.ymonster + grid_len);
			flag = 0;
			break;
		}
		if (p->next == NULL)
		{
			break;
		}
		p = p->next;
	}

	if (flag)
	{
		bar(m_temp.xmonster, m_temp.ymonster, m_temp.xmonster + grid_len, m_temp.ymonster + grid_len);
	}

	setfillcolor(RED);
	bar(m.xmonster, m.ymonster, m.xmonster + grid_len, m.ymonster + grid_len);
}

//移动蛇身、怪物
int move()
{
	delay_fps(10);				//刷新

	//是否撞墙或撞蛇身
	if (Hitwall_and_Hitself_HeadEatByMonster() == 1)
	{
		getch();
		return 1;
	}

	if ((index <= new_grid_num) && (index > 3))//蛇生长时减慢速度
	{
		if (rand() % 2)
		{
			draw_snake();
		}
	}
	else
	{
		draw_snake();
	}

	if (rand() % 10 <= 3)//减缓怪物速度
	{
		draw_monster();
	}
	return 0;
}

//游戏控制
void playGame()
{
	int my_ret = 0;

	//new_key = 'D';//刚开始，贪吃蛇向右移动

	char keys[4] = { 'A', 'W', 'D', 'S' };
	char keys_against[4] = { 'D', 'S', 'A', 'W' };
	char directkeys[4] = { key_left, key_up, key_right, key_down };

	while (1)
	{
		//如果蛇不是向下前进的时候，按上键，执行向上前进操作
		for (int i = 0; i < 4; i++)
		{
			//判断是否是某个按键按下
			if ((keystate(keys[i]) || keystate(directkeys[i])) && new_key != keys_against[i])
			{
				new_key = keys[i];
			}
		}

		my_ret = move();//移动蛇身

		end_time = clock();
		sprintf(s2, "最终的得分为:%d", score);
		sprintf(s3, "Snake-Food:%d,Time:%ds", 9 - score, ((end_time - start_time) / CLOCKS_PER_SEC));//计时
		setcaption(s3);

		if ((score == 9) && (index == 3 + 45 + 1))
		{
			sprintf(s1, "Seccess!!!");
			sprintf(s2, "食物全被吃完!!!");
			getch();
			break;
		}

		if (my_ret == 1)
		{
			break;
		}
	}
}

//释放资源
void free_snake(Snake* head)
{
	while ((p = head) != NULL)
	{
		head = head->next;
		free(p);
	}
}