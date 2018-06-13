#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h> 
typedef struct alterchess{
	int posit[2];
	int spscore;
	struct alterchess *next;
}alter;

void Pos(int,int);
int mouse(int*);//传递鼠标坐标与状态 
void initmap(void);
void fontsize(int,int);
void luozi(char [][15],int*,char);
int evaluate(char [][15],int[],char);
int lianxu(char m[][15],int luoz[]);
int lianxufen(int liann);
int computer(char [][15],char,int,int);
alter* generate(char [][15]);
int manzu(char [][15],int,int);
alter* add(alter*,int,int);
void quzhi(alter*);
void freeall(alter*);

int mscore=0;

int main(void){
	char map[15][15];
	int cursor[2]={3,10};//提示你在点哪里的光标 
	char nowcolor='b';
	memset(map,48,sizeof(map));
	initmap();
	while(1)
	{
		luozi(map,cursor,'b');
		Pos(0,0);
		printf("  %d  ",mscore);
		luozi(map,cursor,'w');
		Pos(0,0);
		printf("  %d  ",mscore);
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
				if(ccolor=='b')
				{
					printf("○");
				}
				else if(ccolor=='w')
				{
					printf("●");
				}
				cs[0]-=3;
				cs[1]=(cs[1]-10)/2;
				mscore+=evaluate(m,cs,ccolor);
				m[cs[0]][cs[1]]=ccolor;
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
				continue;
			}
		}
	}
}
int evaluate(char m[][15],int luoz[],char ccolor){//'0'是空,'b'是黑,'w'是白,活1 10，100,1000,10000,100000，死降一级 
	int boscore,bnscore;
	boscore=lianxu(m,luoz);
	m[luoz[0]][luoz[1]]=ccolor;
	bnscore=lianxu(m,luoz);
	m[luoz[0]][luoz[1]]='0';
	return bnscore-boscore;
}
int lianxu(char m[][15],int luoz[]){
	int i,min,max;
	int liann[4]={0,0,0,0}; 
	int life[4]={1,1,1,1};
	int oscore=0; 
	char lianc[4]={'0','0','0','0'};
/*————————————————————————————————————————*/
	min=(luoz[1]-5>=0?-5:-luoz[1]);
	max=(15-luoz[1]>6?6:15-luoz[1]);
	for(i=min;i<max;i++)
	{
		if(liann[0]!=0&&m[luoz[0]][luoz[1]+i]==lianc[0])
		{
			liann[0]++;
			if(liann[0]>=5)
			{
				return 999999;
			}
		}
		else
		{
			if((luoz[1]+i)==0)
			{
				life[0]--;
			}
			if(liann[0]==0)
			{
				if(m[luoz[0]][luoz[1]+i]=='0')
				{
					life[0]=1;
				}
				else
				{
					liann[0]=1;
				}
			}
			else
			{
				if(m[luoz[0]][luoz[1]+i]=='0')
				{
					if(life[0]==0)//单死降一级 
					{
						liann[0]--;
					}
					oscore+=(lianc[0]=='b'?1:-1)*lianxufen(liann[0]);
					liann[0]=0;
					life[0]=1;
				}
				else
				{
					if(life[0]==0)//双死没有分 
					{
						liann[0]=0;
					}
					else//单死降一级
					{
						liann[0]--;
					}
					oscore+=(lianc[0]=='b'?1:-1)*lianxufen(liann[0]);
					liann[0]=1;
					life[0]=0;
				}
			}
		}
		lianc[0]=m[luoz[0]][luoz[1]+i];	
	}
	if((luoz[1]+i)==15&&life[0]==1)
	{
		liann[0]--;
		oscore+=(lianc[0]=='b'?1:-1)*lianxufen(liann[0]);
	}
/*——————————————————上面是水平————————————————————*/ 
	min=(luoz[0]-5>=0?-5:-luoz[0]);
	max=(15-luoz[0]>6?6:15-luoz[0]);
	for(i=min;i<max;i++)
	{
		if(liann[1]!=0&&m[luoz[0]+i][luoz[1]]==lianc[1])
		{
			liann[1]++;
			if(liann[1]>=5)
			{
				return 999999;
			}
		}
		else
		{
			if((luoz[0]+i)==0)
			{
				life[1]--;
			}
			if(liann[1]==0)
			{
				if(m[luoz[0]+i][luoz[1]]=='0')
				{
					life[1]=1;
				}
				else
				{
					liann[1]=1;
				}
			}
			else
			{
				if(m[luoz[0]+i][luoz[1]]=='0')
				{
					if(life[1]==0)//单死降一级 
					{
						liann[1]--;
					}
					oscore+=(lianc[1]=='b'?1:-1)*lianxufen(liann[1]);
					liann[1]=0;
					life[1]=1;
				}
				else
				{
					if(life[1]==0)//双死没有分 
					{
						liann[1]=0;
					}
					else//单死降一级
					{
						liann[1]--;
					}
					oscore+=(lianc[1]=='b'?1:-1)*lianxufen(liann[1]);
					liann[1]=1;
					life[1]=0;
				}
			}
		}
		lianc[1]=m[luoz[0]+i][luoz[1]];	
	}
	if((luoz[0]+i)==15&&life[1]==1)
	{
		liann[1]--;
		oscore+=(lianc[1]=='b'?1:-1)*lianxufen(liann[1]);
	}
/*——————————————————上面是竖直————————————————————*/
	min=((luoz[0]-5>=0&&luoz[1]-5>=0)?-5:-(luoz[0]<luoz[1]?luoz[0]:luoz[1]));
	max=((15-luoz[1]>6&&15-luoz[0]>6)?6:((15-luoz[0]<15-luoz[1])?15-luoz[0]:15-luoz[1]));
	for(i=min;i<max;i++)
	{
		if(liann[2]!=0&&m[luoz[0]+i][luoz[1]+i]==lianc[2])
		{
			liann[2]++;
			if(liann[2]>=5)
			{
				return 999999;
			}
		}
		else
		{
			if((luoz[0]+i)==0||(luoz[1]+i)==0)
			{
				life[2]--;
			}
			if(liann[2]==0)
			{
				if(m[luoz[0]+i][luoz[1]+i]=='0')
				{
					life[2]=1;
				}
				else
				{
					liann[2]=1;
				}
			}
			else
			{
				if(m[luoz[0]+i][luoz[1]+i]=='0')
				{
					if(life[2]==0)//单死降一级 
					{
						liann[2]--;
					}
					oscore+=(lianc[2]=='b'?1:-1)*lianxufen(liann[2]);
					liann[2]=0;
					life[2]=1;
				}
				else
				{
					if(life[2]==0)//双死没有分 
					{
						liann[2]=0;
					}
					else//单死降一级
					{
						liann[2]--;
					}
					oscore+=(lianc[2]=='b'?1:-1)*lianxufen(liann[2]);
					liann[2]=1;
					life[2]=0;
				}
			}
		}
		lianc[2]=m[luoz[0]+i][luoz[1]+i];	
	}
	if(((luoz[0]+i==15)||(luoz[1]+i==15))&&life[2]==1)
	{
		liann[2]--;
		oscore+=(lianc[2]=='b'?1:-1)*lianxufen(liann[2]);
	}
/*——————————————————上面是135斜的————————————————————*/
	min=(15-luoz[0]>5&&luoz[1]-5>=0)?-5:-(14-luoz[0]<luoz[1]?14-luoz[0]:luoz[1]);			
	max=(15-luoz[1]>6&&luoz[0]-5>=0)?6:((luoz[0]+1<15-luoz[1])?luoz[0]+1:15-luoz[1]);
	for(i=min;i<max;i++)
	{
		if(liann[3]!=0&&m[luoz[0]-i][luoz[1]+i]==lianc[3])
		{
			liann[3]++;
			if(liann[3]>=5)
			{
				return 999999;
			}
		}
		else
		{
			if((luoz[0]-i)==14||(luoz[1]+i)==0)
			{
				life[3]--;
			}
			if(liann[3]==0)
			{
				if(m[luoz[0]-i][luoz[1]+i]=='0')
				{
					life[3]=1;
				}
				else
				{
					liann[3]=1;
				}
			}
			else
			{
				if(m[luoz[0]-i][luoz[1]+i]=='0')
				{
					if(life[3]==0)//单死降一级 
					{
						liann[3]--;
					}
					oscore+=(lianc[3]=='b'?1:-1)*lianxufen(liann[3]);
					liann[3]=0;
					life[3]=1;
				}
				else
				{
					if(life[3]==0)//双死没有分 
					{
						liann[3]=0;
					}
					else//单死降一级
					{
						liann[3]--;
					}
					oscore+=(lianc[3]=='b'?1:-1)*lianxufen(liann[3]);
					liann[3]=1;
					life[3]=0;
				}
			}
		}
		lianc[3]=m[luoz[0]-i][luoz[1]+i];	
	}
	if(((luoz[0]-i==-1)||(luoz[1]+i==15))&&life[3]==1)
	{
		liann[3]--;
		oscore+=(lianc[3]=='b'?1:-1)*lianxufen(liann[3]);
	}
/*——————————————————上面是45斜的————————————————————*/ 
	return oscore;
}
int lianxufen(int liann){
	switch(liann)
	{
		case 1:
			return 1;
		case 2:
			return 10;
		case 3:
			return 100;
		case 4:
			return 1000;
		case 5://好像没有什么意义 
			return 10000;
		default:
			return 0;
	}
}
int computer(char m[][15],char ccolor,int score,int depth){
			
}
alter* generate(char m[][15]){
	int i,j;
	alter *head=(alter*)malloc(sizeof(alter));
	alter *p=head;
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			if(manzu(m,i,j))
			{
				p=add(p,i,j);
			}
		}
	}
	return head; 
}
int manzu(char m[][15],int l1,int l2){
	if(m[l1][l2]=='0')
	{
		int i,j;
		for(i=(l1>=2?-2:-l1);l1+i<15&&i<=2;i++)
		{
			for(j=(l2>=2?-2:-l2);l2+j<15&&j<=2;j++)
			{
				if(m[l1+i][l2+j]!='0')
				{
					return 1;
				}
			}
		}
	}
	return 0;
}
alter* add(alter* yend,int pos0,int pos1){
	alter* newend;
	newend=(alter*)malloc(sizeof(alter));
	yend->next=newend;
	newend->posit[0]=pos0;
	newend->posit[1]=pos1;
	newend->next=NULL;
	return newend;
}
void quzhi(alter* head){
	alter *p1=head->next;
	while(p1!=NULL)
	{
		Pos(p1->posit[0]+3,(p1->posit[1]+5)*2);
		printf("□");
		p1=p1->next;
	}
	freeall(head);
}
void freeall(alter* head){
	alter *p1=head,*p2=p1;
	while(p2!=NULL)
	{
		p2=p2->next;
		free(p1);
		p1=p2;
	}
}
