#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h> 

void Pos(int,int);//��x�����Ѿ�����2�İ汾 
int mouse(int*);//�������������״̬ 
void initmap(void);
void fontsize(int,int);
void luozi(char [][15],int*,char);
int evaluate(char [][15],int[],char);
int lianxu(char m[][15],int luoz[]);
int lianxufen(int liann);

int mscore=0;

int main(void){
	char map[15][15];
	int cursor[2]={3,10};//��ʾ���ڵ�����Ĺ�� 
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
    DWORD dwMode, dwOldMode;                /* ԭ����ģʽ����ģʽ */
    DWORD dwRead;                           /* �Ѿ���ȡ�������¼��� */
    UINT count;                             /* ����ѭ������ */
    INPUT_RECORD ir[128];                   /* ����̨�����¼ */
    CONSOLE_CURSOR_INFO cci, cciOld;        /* �����ص����� */
    COORD co;                               /* ����ƶ�ʱ�������� */
    HANDLE hStdIn = NULL, hStdOut = NULL;   /* ��׼����������  */

    hStdIn = GetStdHandle(STD_INPUT_HANDLE);    /* ȡ������������ */
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if(!GetConsoleMode(hStdIn, &dwOldMode))    /* ȡ�ÿ���̨ԭ����ģʽ */
    {
        fprintf(stderr, "GetConsoleMode ����:%d\n", GetLastError());
        exit(0);
    }
    dwMode = dwOldMode;
    dwMode &= ~(ENABLE_QUICK_EDIT_MODE);
    dwMode |= ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    if(!SetConsoleMode(hStdIn, dwMode))    /* ��������¼� */
    {
        fprintf(stderr, "SetConsoleMode ����:%d\n", GetLastError());
        exit(0);
    }

    GetConsoleCursorInfo(hStdOut, &cciOld);
    cci.bVisible =false;    /* bu�ɼ� */
    cci.dwSize = 100;        /* ��״ */
    if(!SetConsoleCursorInfo(hStdOut, &cci))
    {
        fprintf(stderr, "SetConsoleCursorInfo ����:%d\n", GetLastError());
        exit(0);
    }

    while(1)   /* ѭ���ȴ��û��¼� */
    {
        if(!ReadConsoleInput(hStdIn, &ir[0], 128, &dwRead))
        {
            fprintf(stderr, "ReadConsoleInput ����:%d\n", GetLastError());
            exit(0);
        }
        for(count=0; count<dwRead; count++)
        {
            switch(ir[count].EventType)
            {
            case MOUSE_EVENT:
                switch(ir[count].Event.MouseEvent.dwEventFlags)
                {
                case 0:    /* ȡ����굥���¼� */
                    if(ir[count].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                    {
                        SetConsoleMode(hStdIn, dwOldMode);    /* ��ԭԭ�������� */
                        SetConsoleCursorInfo(hStdOut, &cciOld);
                        mouse_pos[1] = ir[count].Event.MouseEvent.dwMousePosition.X%2==0? ir[count].Event.MouseEvent.dwMousePosition.X:ir[count].Event.MouseEvent.dwMousePosition.X-1;
                        mouse_pos[0] = ir[count].Event.MouseEvent.dwMousePosition.Y;//x������������Ҫ��1����һ������ռ���������� 
                        return 1;
                    }
                    break;
                case MOUSE_MOVED:    /* �������λ�� */
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
	printf("�� �� �� �� �� �� �� �� �� �� �� �� �� �� ��");
	for(i=4;i<17;i++)
	{
		Pos(i,10);
        printf("�� �� �� �� �� �� �� �� �� �� �� �� �� �� ��");
	}
    Pos(17,10);
    printf("�� �� �� �� �� �� �� �� �� �� �� �� �� �� ��");
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
					printf("��");
				}
				else if(ccolor=='w')
				{
					printf("��");
				}
				cs[0]-=3;
				cs[1]=(cs[1]-10)/2;
				mscore+=evaluate(m,cs,ccolor);
				return;
			}
		}
		else
		{
			Pos(gb[0],gb[1]);
			if(m[gb[0]-3][(gb[1]-10)/2]=='0')
			{
					if(gb[0]==3&&gb[1]==10)
						printf("��");
					else if(gb[0]==3&&gb[1]==38)
						printf("��");
					else if(gb[0]==17&&gb[1]==10)
						printf("��");
					else if(gb[0]==17&&gb[1]==38)
						printf("��");
					else if(gb[0]==3)
						printf("��");
					else if(gb[0]==17)
						printf("��");
					else if(gb[1]==10)
						printf("��");
					else if(gb[1]==38)
						printf("��");
					else
						printf("��");		
			}
			if(m[cs[0]-3][(cs[1]-10)/2]=='0'&&cs[0]<=17&&cs[0]>=3&&cs[1]<=38&&cs[1]>=10)
			{
				gb[0]=cs[0];
				gb[1]=cs[1];
				Pos(cs[0],cs[1]);
				printf("��");
				continue;
			}
		}
	}
}
int evaluate(char m[][15],int luoz[],char ccolor){//'0'�ǿ�,'b'�Ǻ�,'w'�ǰ�,��1 10��100,1000,10000,100000������һ�� 
	int boscore,bnscore;
	char yuan=m[luoz[0]][luoz[1]];
	boscore=lianxu(m,luoz);
	m[luoz[0]][luoz[1]]=ccolor;
	bnscore=lianxu(m,luoz);
	return bnscore-boscore;
}
int lianxu(char m[][15],int luoz[]){
	int i;
	int liann[4]={0,0,0,0};//�ֱ�Ϊˮƽ��y=-x����ֱ��y=x 
	int life[4]={1,1,1,1};
	int oscore=0; 
	char lianc[4]={'0','0','0','0'};
	for(i=(luoz[1]-5>=0?-5:-luoz[1]);i<(15-luoz[1]>6?6:15-luoz[1]);i++)
	{
		if(lianc[0]!='0'&&m[luoz[0]][luoz[1]+i]==lianc[0])
		{
			liann[0]++;
			if(liann[0]>=5)
			{
				return 999999;
			}
		}
		else
		{
			if(lianc[0]=='0')
			{
				life[0]=1;
				if(m[luoz[0]][luoz[1]+i]=='0')
				{
					liann[0]=0;
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
					if(life[0]==0)//������һ�� 
					{
						liann[0]--;
					}
					oscore+=(lianc[0]=='b'?1:-1)*lianxufen(liann[0]);
					liann[0]=0;
					life[0]=1;
				}
				else
				{
					if(life[0]==0)//˫��û�з� 
					{
						liann[0]=0;
					}
					else//������һ��
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
	}//δ��� 
	for(i=((luoz[0]-5>=0&&luoz[1]-5>=0)?-5:-(luoz[0]<luoz[1]?luoz[0]:luozi[1]));i<(15-luoz[1]>6?6:15-luoz[1]);i++)
	{
		if(lianc[0]!='0'&&m[luoz[0]][luoz[1]+i]==lianc[0])
		{
			liann[0]++;
			if(liann[0]>=5)
			{
				return 999999;
			}
		}
		else
		{
			if(lianc[0]=='0')
			{
				life[0]=1;
				if(m[luoz[0]][luoz[1]+i]=='0')
				{
					liann[0]=0;
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
					if(life[0]==0)//������һ�� 
					{
						liann[0]--;
					}
					oscore+=(lianc[0]=='b'?1:-1)*lianxufen(liann[0]);
					liann[0]=0;
					life[0]=1;
				}
				else
				{
					if(life[0]==0)//˫��û�з� 
					{
						liann[0]=0;
					}
					else//������һ��
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
		case 5://����û��ʲô���� 
			return 10000;
		default:
			return 0;
	}
}
