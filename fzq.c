#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h> 

void Pos(int,int);//是x坐标已经乘了2的版本 
int mouse(int*);//传递鼠标坐标与状态 
void initmap(void);
void fontsize(int,int);
void luozi(char [][15],int*,char);
int evaluate(char [][15],int[],char,int);
int lianxu(char m[][15],int luoz[],char,int,int);

int main(void){
	char map[15][15];
	int cursor[2]={3,10};//提示你在点哪里的光标 
	memset(map,48,sizeof(map));
	initmap();
	while(1)
	{
		luozi(map,cursor,'b');
		
		
	}
	return 0;
}
void Pos(int row,int rank){
	COORD position;
	HANDLE hOutput;
	position.X=rank;
	position.Y=row;
	hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput,position);
}
int mouse(int mouse_pos[2]){
    DWORD dwMode, dwOldMode;                /* 原来的模式与新模式 */
    DWORD dwRead;                           /* 已经读取的输入事件数 */
    UINT count;                             /* 用于循环控制 */
    INPUT_RECORD ir[128];                   /* 控制台输入记录 */
    CONSOLE_CURSOR_INFO cci, cciOld;        /* 鼠标相关的设置 */
    COORD co;                               /* 鼠标移动时更新坐标 */
    HANDLE hStdIn = NULL, hStdOut = NULL;   /* 标准输入输出句柄  */

    hStdIn = GetStdHandle(STD_INPUT_HANDLE);    /* 取得输入输出句柄 */
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if(!GetConsoleMode(hStdIn, &dwOldMode))    /* 取得控制台原来的模式 */
    {
        fprintf(stderr, "GetConsoleMode 错误:%d\n", GetLastError());
        exit(0);
    }
    dwMode = dwOldMode;
    dwMode &= ~(ENABLE_QUICK_EDIT_MODE);
    dwMode |= ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    if(!SetConsoleMode(hStdIn, dwMode))    /* 开启鼠标事件 */
    {
        fprintf(stderr, "SetConsoleMode 错误:%d\n", GetLastError());
        exit(0);
    }

    GetConsoleCursorInfo(hStdOut, &cciOld);
    cci.bVisible =false;    /* bu可见 */
    cci.dwSize = 100;        /* 块状 */
    if(!SetConsoleCursorInfo(hStdOut, &cci))
    {
        fprintf(stderr, "SetConsoleCursorInfo 错误:%d\n", GetLastError());
        exit(0);
    }

    while(1)   /* 循环等待用户事件 */
    {
        if(!ReadConsoleInput(hStdIn, &ir[0], 128, &dwRead))
        {
            fprintf(stderr, "ReadConsoleInput 错误:%d\n", GetLastError());
            exit(0);
        }
        for(count=0; count<dwRead; count++)
        {
            switch(ir[count].EventType)
            {
            case MOUSE_EVENT:
                switch(ir[count].Event.MouseEvent.dwEventFlags)
                {
                case 0:    /* 取得鼠标单击事件 */
                    if(ir[count].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                    {
                        SetConsoleMode(hStdIn, dwOldMode);    /* 还原原来的设置 */
                        SetConsoleCursorInfo(hStdOut, &cciOld);
                        mouse_pos[1] = ir[count].Event.MouseEvent.dwMousePosition.X%2==0? ir[count].Event.MouseEvent.dwMousePosition.X:ir[count].Event.MouseEvent.dwMousePosition.X-1;
                        mouse_pos[0] = ir[count].Event.MouseEvent.dwMousePosition.Y;//x坐标是奇数就要退1——一个符号占了两格坐标 
                        return 1;
                    }
                    break;
                case MOUSE_MOVED:    /* 更新鼠标位置 */
                    mouse_pos[1] =ir[count].Event.MouseEvent.dwMousePosition.X%2==0? ir[count].Event.MouseEvent.dwMousePosition.X:ir[count].Event.MouseEvent.dwMousePosition.X-1;
                    mouse_pos[0] = ir[count].Event.MouseEvent.dwMousePosition.Y;
                    return 0;
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
}
void initmap(void){
	int i;
	fontsize(20,40);
	system("mode con:cols=50 lines=20");
	Pos(3,10);
	printf("┏ ┯ ┯ ┯ ┯ ┯ ┯ ┯ ┯ ┯ ┯ ┯ ┯ ┯ ┓");
	for(i=4;i<17;i++)
	{
		Pos(i,10);
        printf("┠ ┼ ┼ ┼ ┼ ┼ ┼ ┼ ┼ ┼ ┼ ┼ ┼ ┼ ┨");
	}
    Pos(17,10);
    printf("┗ ┷ ┷ ┷ ┷ ┷ ┷ ┷ ┷ ┷ ┷ ┷ ┷ ┷ ┛");
}
void fontsize(int x,int y){
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi1;
    cfi1.cbSize = sizeof(cfi1);
    GetCurrentConsoleFontEx(out, false, &cfi1);
    cfi1.dwFontSize.Y=y;
    cfi1.dwFontSize.X=x;
   	SetCurrentConsoleFontEx(out, false, &cfi1);
    return;
}
void luozi(char m[][15],int* gb,char ccolor){
	int cs[2];
	while(1)
	{
		if(mouse(cs))
		{
			if(m[cs[0]-3][(cs[1]-10)/2]=='0'&&cs[0]<=17&&cs[0]>=3&&cs[1]<=38&&cs[1]>=10)
			{
				Pos(cs[0],cs[1]);
				printf("●");
				m[cs[0]-3][(cs[1]-10)/2]=ccolor;
				return;
			}
		}
		else
		{
			Pos(gb[0],gb[1]);
			if(m[gb[0]-3][(gb[1]-10)/2]=='0')
			{
					if(gb[0]==3&&gb[1]==10)
						printf("┏");
					else if(gb[0]==3&&gb[1]==38)
						printf("┓");
					else if(gb[0]==17&&gb[1]==10)
						printf("┗");
					else if(gb[0]==17&&gb[1]==38)
						printf("┛");
					else if(gb[0]==3)
						printf("┯");
					else if(gb[0]==17)
						printf("┷");
					else if(gb[1]==10)
						printf("┠");
					else if(gb[1]==38)
						printf("┨");
					else
						printf("┼");		
			}
			if(m[cs[0]-3][(cs[1]-10)/2]=='0'&&cs[0]<=17&&cs[0]>=3&&cs[1]<=38&&cs[1]>=10)
			{
				gb[0]=cs[0];
				gb[1]=cs[1];
				Pos(cs[0],cs[1]);
				printf("□");
				return;
			}
		}
	}
}
int evaluate(char m[][15],int luoz[],char ccolor,int oldScore){//'0'是空,'b'是黑,'w'是白,活1 10，100,1000,10000,100000，死降一级 
	int liann[8];
	liann[0]=lianxu(m,luoz,ccolor,0,1);
	liann[1]=lianxu(m,luoz,ccolor,-1,1);
	liann[2]=lianxu(m,luoz,ccolor,-1,0);
	liann[3]=lianxu(m,luoz,ccolor,-1,-1);
	liann[4]=lianxu(m,luoz,ccolor,0,-1);
	liann[5]=lianxu(m,luoz,ccolor,1,-1);
	liann[6]=lianxu(m,luoz,ccolor,1,0);
	liann[7]=lianxu(m,luoz,ccolor,1,1);
	
	
	return 0;
}
int lianxu(char m[][15],int luoz[],char ccolor,int y,int x){
	int i;
	int liann=1;
	char lianc;
	if((lianc=m[luoz[0]+y][luoz[1]+x])!='0')
	{
		for(i=2;i++;(i<5&&luoz[0]+y*i<=14&&luoz[0]+y*i>=0&&luoz[1]+x*i<=14&&luoz[1]+x*i>=0))
		{
			if(m[luoz[0]+i*y][luoz[0]+i*x]==lianc)
			{
				liann++;
			}
			else
			{
				break;
			}
		}
		return lianc==ccolor?liann:-liann;
	}
	return 0;
}
