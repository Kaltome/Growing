/*******
	Reversi Ai  !!!!!
	Date:       2018/1/7
	Author:     Kaltome
	Version:    #85
	depthmax:   3
********/

#define _CRT_SECURE_NO_WARNINGS
#define NOCHESS 3
#define INSIDE_SCORE 300							//�ڲ��ӷ���
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>

int test = 0;

int depthmax;										//������ȣ���߿�5��
int chessnum = 4;
char chessboard[16][16];
char testboard[6][16][16];

char iside;											//˫��������ɫ
char oside;

const int direction[8][2] = {						//�����ƶ��ķ���
	{ 1,0 },{ 1,-1 },{ 0,-1 },{ -1,-1 },{ -1,0 },{ -1,1 },{ 0,1 },{ 1,1 },
};


//�÷ֱ���¼�����µ�λ�ã�����¼��λ�õĵ÷�
struct mine {
	char position[2];								//��������������λ��,[0]Ϊ�У�[1]Ϊ��
	int value;										//�����λ�õĵ÷�
}canmove[6][256];

//	��ֵ��
//	��Ϊʲô������㷴����������ߣ���Ҳ��֪������������
int table_start[16][16] = {
	{ 1000000,-100000,150   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,150   ,-100000,1000000 },
	{ -100000,-200000,60    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,60    ,-200000,-100000 },
	{ 150    ,60     ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,60     ,150     },
	{ 100    ,50     ,100   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,750   ,750   ,750   ,750   ,750   ,750   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,900   ,900   ,900   ,900   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,975   ,975   ,975   ,975   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,975   ,1000  ,1000  ,975   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,975   ,1000  ,1000  ,975   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,975   ,975   ,975   ,975   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,900   ,900   ,900   ,900   ,900   ,900   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,750   ,750   ,750   ,750   ,750   ,750   ,750   ,750   ,500   ,100   ,50     ,100     },
	{ 100    ,50     ,100   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,500   ,100   ,50     ,100     },
	{ 150    ,60     ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,60     ,150     },
	{ -100000,-200000,60    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,50    ,60    ,-200000,-100000 },
	{ 1000000,-100000,150   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,100   ,150   ,-100000,1000000 },
};
int table_end[16][16] = {
	{ 1000000,-100000,250   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,250   ,-100000,1000000 },
	{ -100000,-200000,50    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,50    ,-200000,-100000 },
	{ 250    ,50     ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,50     ,250     },
	{ 20     ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 200    ,40     ,10    ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,5     ,10    ,40     ,200     },
	{ 250    ,50     ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,10    ,50     ,250     },
	{ -100000,-200000,50    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,40    ,50    ,-200000,-100000 },
	{ 1000000,-100000,250   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,200   ,250   ,-100000,1000000 },
};
int(*table)[16];
char choosstable = 0;																	//ת����ֵ��ı�־
int check_choosstable();																//ת����ֵ����ж�����ѧϵ�У�
char board_stabilizer[2][16][16] = { 0 };												//��¼�ȶ��ӵ�λ��
int depthvalue[6];																		//����Alpha - Beta ֵ�����5��

void reset_canmove(int depth);															//���һ��÷ֱ��λ��
void printcboard(char bbb[16][16]);														//��ӡ���̣�Debug�ã�
void flipchess(char board[][16], int mrow, int mcol, char side);						//����
void Brain();																			//���ǵĴ��ԣ�
int find(int depth_find, char board[16][16], char side);								//Ѱ�ҿ������ӵ�λ�ã�˫�������ӣ���ͬ��ȣ����õ����̣�
int search(int depth);																	//�������������

/*   ��ֵ����ϵ��   */
int findvalue(int canmove_i, int depth);												//�ռ����еĹ�ֵ����
int caltable(char board[][16]);															//��ֵ���ֵ
int findstabilizer(int i, int j, int boardnum);											//Ѱ���ȶ���
int stabilizer();																		//�����ȶ��Ӻ���������ֵ
int fine_canact(char side, int acti, int actj);											//Ѱ���ж���
int calaction();																		//�ж�����ֵ
int potential_action();																	//Ѱ��Ǳ���ж���������ֵ
int diffnum();																			//��¼˫�����Ӽ���Ŀ���죬����ֵ
int caltable(char board[][16]);															//��ֵ���ֵ


int main()
{
	char order[6];
	int getorow, getocol;
	printf("********************\n\n    JP Reversi~    \n\n********************\n\n\n");
	printf("��ѡ�����������ɫ������\n\n1. �� ������ X\n2. �� ������ O\n\n");

	while (1){
		char getcolor;
		scanf("%c", &getcolor);
		if (getcolor == '1') {
			oside = 'X';
			iside = 'O';
			break;
		}
		else if (getcolor == '2') {
			oside = 'O';
			iside = 'X';
			break;
		}
		else printf("������ѡ��\n");
		while (getchar() != '\n');
	}

	printf("\n\n��ѡ���Ѷȣ�����\n\n1. �����򵥣�\n2. ������ɣ�\n3. ����Է���\n\n");
	while (1) {
		scanf("%d", &depthmax);
		if (0 < depthmax && depthmax < 4) {
			printf("\n\n");
			break;
		}
		printf("������ѡ�񣡣���\n");
		while (getchar() != '\n');
	}

	memset(chessboard, NOCHESS, sizeof(chessboard));									//��ʼ������
	chessboard[7][8] = chessboard[8][7] = 'X';
	chessboard[7][7] = chessboard[8][8] = 'O';

	printcboard(chessboard);

	//�ռ�ָ��
	while (1)
	{
		while (scanf("%s", order))
		{
			if (order[0] == 'P') {
				scanf("%d%d", &getorow, &getocol);
				flipchess(chessboard, getorow, getocol, oside);
				chessnum++;
				//printcboard(chessboard);
			}
			else if (order[0] == 'T') break;
			else if (order[0] == 'E') {
				getchar();
				exit(0);
			}
			else printf("DEBUG The order is wrong, please entry again.\n");
			fflush(stdout);
		}
		Brain();
		//printcboard(chessboard);
	}
	return 0;
}


void Brain()
{
	int find_canmove = 0;
	int find_value;
																		//�˴�����������ȣ�
	if ((256 - chessnum) < 6) depthmax = 256 - chessnum;
	
	for (int i = 0; i < 6; i++)															//��ʼ��Alpha-Betaֵ
	{
		if ((i % 2) == 1) depthvalue[i] = INT_MIN;
		else depthvalue[i] = INT_MAX;
	}
	for (int i = 0; i < 16; i++)
	{
		strncpy(testboard[0][i], chessboard[i], 16);									//�����̸��Ƶ���������
	}
	if (choosstable == 0) {																//�վ�ת����ֵ��table_startΪtable_end����ʵ�㷴�ˣ��ⷴ�����������ˡ�����
		if (check_choosstable()) {
			choosstable = 1;
			table = table_start;
		}
		else table = table_end;
	}
	if (chessboard[0][0] != NOCHESS) {													//���䱻ռ��ʱ��ֵ�����������ռ�ˡ���������λ�û���ô��Σ��λ���ء�����
		table[0][1] = 2500; table[1][0] = 2500; table[1][1] = 2000;
	}
	if (chessboard[15][0] != NOCHESS) {
		table[15][1] = 2500; table[14][0] = 2500; table[14][1] = 2000;
	}
	if (chessboard[0][15] != NOCHESS) {
		table[0][14] = 2500; table[1][15] = 2500; table[1][14] = 2000;
	}
	if (chessboard[15][15] != NOCHESS) {
		table[14][15] = 2500; table[15][14] = 2500; table[14][14] = 2000;
	}
	
	//���������
	find_value = search(0);

	for (int i = 0; i < 256 && (canmove[1][i].position[0] != NOCHESS); i++)				//Ѱ�ҵ÷���ߵ�λ��
	{
		if (canmove[1][i].value == find_value) {
			find_canmove = i;
		}
	}
	printf("%d %d\n", canmove[1][find_canmove].position[0], canmove[1][find_canmove].position[1]);
	fflush(stdout);
	flipchess(chessboard, canmove[1][find_canmove].position[0], canmove[1][find_canmove].position[1], iside);
	chessnum++;
}

//����������
int search(int depth)
{
	depth++;																			//��ǰ���
	char color;
	if (depth % 2 == 0) color = oside;													//�ɵ�ǰ��Ⱦ����������ɫ
	else color = iside;
	reset_canmove(depth);																//��յ�ǰ�㱣���λ�ú͹�ֵ
	if (find(depth, testboard[depth - 1], color) == 0)									//Ѱ����һ������̿����µ�λ�ã��Ҳ������µ�λ��ʱ���ؼ��߻򼫵͵Ĺ�ֵ
	{
		if (depth % 2 == 1) return -300000;
		else return 400000;
	}

	int canmove_i = 0;																	//��ÿһ������Ĳ����������������ֵ
	for (; canmove[depth][canmove_i].position[0] != NOCHESS; canmove_i++)
	{
		for (int i = 0; i < 16; i++)													//����һ������̸��Ƶ���һ����ٲ�������
		{
			strncpy(testboard[depth][i], testboard[depth - 1][i], 16);
		}
		flipchess(testboard[depth], canmove[depth][canmove_i].position[0], canmove[depth][canmove_i].position[1], color);

		if (depth != depthmax) {
			if (depth % 2 == 0) depthvalue[depth+1] = INT_MIN;							//������һ������ǰ���ȳ�ʼ����һ���Alpha - Betaֵ
			else depthvalue[depth+1] = INT_MAX;
			canmove[depth][canmove_i].value = search(depth);
		}
		if (depth == depthmax) {														//�ﵽ�����ȱ㿪ʼ��ֵ
			//printcboard(testboard[depth]);
			canmove[depth][canmove_i].value = findvalue(canmove_i, depth);			
			//printf("%d\n", canmove[depth][canmove_i].value);
		}
		
		//����������ɫ������һ��� Alpha / Betaֵ ���Ƚϣ����Ͼ� break.
		//Alpha - Beta ��֦
		//��������֦������һ����¼��λ�ñ�Ϊ��
		if (depth % 2 == 0) {
			if (canmove[depth][canmove_i].value <= depthvalue[depth - 1]) {
				canmove[depth][canmove_i + 1].position[0] = NOCHESS;					
				break;
			}
		}
		else if (depth % 2 == 1) {
			if (canmove[depth][canmove_i].value >= depthvalue[depth - 1]) {				
				canmove[depth][canmove_i + 1].position[0] = NOCHESS;
				break;
			}
		}
		int aaaa = 0;
	}

	//Ѱ��ÿһ�����/��Сֵ
	int return_value;
	if (depth % 2 == 1) {
		return_value = INT_MIN;
		for (int i = 0; i < 256 && (canmove[depth][i].position[0] != NOCHESS); i++)
		{
			if (canmove[depth][i].value >= return_value) {
				return_value = canmove[depth][i].value;
			}
		}
	}
	else if (depth % 2 == 0) {
		return_value = INT_MAX;
		for (int i = 0; i < 256 && (canmove[depth][i].position[0] != NOCHESS); i++)
		{
			if (canmove[depth][i].value <= return_value) {
				return_value = canmove[depth][i].value;
			}
		}
	}
	if (depth % 2 == 1) {																	//���ݸò�Ĺ�ֵ����ı���һ��� Alpha - Betaֵ
		if (depthvalue[depth - 1] > return_value) depthvalue[depth - 1] = return_value;
	}
	else {
		if (depthvalue[depth - 1] < return_value) depthvalue[depth - 1] = return_value;
	}
	return return_value;																	//���ظò��ֵ
}

//�ռ�������Ĺ�ֵ
int findvalue(int canmove_i, int depth)

 
{
	int actionscore = 0, tablescore = 0, pactionscore = 0, diffscore = 0, stablescore = 0;

	tablescore   = caltable(testboard[depth]);																		
	actionscore  = calaction();
	pactionscore = potential_action();
	diffscore    = diffnum();
	if (chessboard[15][15] == iside || chessboard[0][15] == iside || chessboard[15][0] == iside || chessboard[0][0] == iside || chessnum > 236) stablescore = stabilizer();
	
	//��ѧʱ�俪ʼ��
	if (chessnum <= 16)        canmove[depth][canmove_i].value = tablescore + 50   * actionscore + 40  * pactionscore - 100 * diffscore +               8 * table[canmove[depth][canmove_i].position[0]][canmove[depth][canmove_i].position[1]];
	else if (choosstable == 0) canmove[depth][canmove_i].value = tablescore + 160  * actionscore + 110 * pactionscore - 160 * diffscore + stablescore + 6 * table[canmove[depth][canmove_i].position[0]][canmove[depth][canmove_i].position[1]];
	else if (chessnum <= 236)  canmove[depth][canmove_i].value = tablescore + 50   * actionscore + 50  * pactionscore - 20  * diffscore + stablescore;
	else                       canmove[depth][canmove_i].value = tablescore +                                           800 * diffscore + stablescore;

	return canmove[depth][canmove_i].value;
}

//�ù�ֵ������̹�ֵ
int caltable(char board[][16])
{
	int calresult = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			int inside = 0;
			if (testboard[depthmax][i][j] == iside) {
				calresult += table[i][j];
				for (int dir = 0; dir < 8; dir++)				//�ж��ڲ��ӣ�����û�пո�
				{
					int ii = i + direction[dir][0];
					int jj = j + direction[dir][1];
					if (ii >= 0 && jj >= 0 && ii < 16 && jj < 16) {
						if (testboard[depthmax][ii][jj] == NOCHESS) inside++;
					}
				}
				if (inside == 0) calresult += INSIDE_SCORE;
			}
			if (testboard[depthmax][i][j] == oside) {
				calresult -= table[i][j];
				for (int dir = 0; dir < 8; dir++)				//�ж��ڲ��ӣ�����û�пո�
				{
					int ii = i + direction[dir][0];
					int jj = j + direction[dir][1];
					if (ii >= 0 && jj >= 0 && ii < 16 && jj < 16) {
						if (testboard[depthmax][ii][jj] == NOCHESS) inside++;
					}
				}
				if (inside == 0) calresult -= INSIDE_SCORE;
			}
		}
	}
	return calresult;
}

//�ж�����ֵ������Ѱ���ж�������
int calaction()
{
	int calresult = 0;
	for (int find_action_i = 0; find_action_i < 16; find_action_i++)
	{
		for (int find_action_j = 0; find_action_j < 16; find_action_j++)
		{
			if (testboard[depthmax][find_action_i][find_action_j] != NOCHESS) continue;
			if (fine_canact(iside, find_action_i, find_action_j)) calresult++;
			if (fine_canact(oside, find_action_i, find_action_j)) calresult--;
		}
	}
	return calresult;
}

//����Ǳ���ж���
int potential_action()
{
	int paresult = 0;
	for (int pi = 0; pi < 16; pi++)
	{
		for (int pj = 0; pj < 16; pj++)
		{
			if (testboard[depthmax][pi][pj] == NOCHESS) continue;
			int num = 0;
			for (int dir = 0; dir < 8; dir++)
			{
				if (testboard[depthmax][pi + direction[dir][0]][pj + direction[dir][1]] == NOCHESS) num++;
			}
			if (testboard[depthmax][pi][pj] == iside) paresult -= num;
			else paresult += num;
		}
	}
	return paresult;
}

//���˫�����Ӽ����Ŀ����
int diffnum()
{
	int diffresult = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if      (testboard[depthmax][i][j] == iside) diffresult++;
			else if (testboard[depthmax][i][j] == oside) diffresult--;
		}
	}
	return diffresult;
}

//�ȶ��ӹ�ֵ
int stabilizer()
{
	int stableresult = 0;
	for (int i = 0; i < 16; i++)											//����ȷ�������̵����Բ�
	{
		strncpy(board_stabilizer[1][i], board_stabilizer[0][i], 16);
	}
	if (chessboard[0][0] == iside || chessnum > 236) {						//��ĳ�������Լ�������ʱ��������Ӧ������������������վ�ʱ��ȫ��������
		for (int i = 0; i < 16; i++)										
		{
			for (int j = 0; j < 16; j++)
			{
				findstabilizer(i, j, 1);									//���ϡ���������
			}
		}
	}
	if (chessboard[15][15] == iside || chessnum > 236) {
		for (int i = 15; i >= 0; i--)
		{
			for (int j = 15; j >= 0; j--)
			{
				findstabilizer(i, j, 1);									//���¡���������
			}
		}
	}
	if (chessboard[0][15] == iside || chessnum > 236) {
		for (int i = 0; i < 16; i++)
		{
			for (int j = 15; j >= 0; j--)
			{
				findstabilizer(i, j, 1);									//���ϡ���������
			}
		}
	}
	if (chessboard[15][0] == iside || chessnum > 236) {
		for (int i = 15; i >= 0; i--)
		{
			for (int j = 0; j < 15; j++)
			{
				findstabilizer(i, j, 1);									//���¡���������
			}
		}
	}
	for (int i = 0; i < 16; i++)											//ͳ�Ʒ���
	{
		for (int j = 0; j < 16; j++)
		{
			if (board_stabilizer[1][i][j] != 0) {
				if      (board_stabilizer[1][i][j] == iside) stableresult += 2500;
				else if (board_stabilizer[1][i][j] == oside) stableresult -= 2500;
			}
		}
	}
	return stableresult;
}

//Ѱ���ȶ���
int findstabilizer(int i, int j, int boardnum)
{
	if (testboard[depthmax][i][j] == NOCHESS || board_stabilizer[boardnum][i][j] != 0) return 0;
	char me;											
	if (testboard[depthmax][i][j] == iside) me = iside;													//�ж���Ѱ���ҷ����ȶ��ӻ��ǶԷ����ȶ���
	else me = oside;
	
	//���ĸ������ж�����  ���� һ�� ���Լ����ȶ��ӻ����Ǳ�Ե  ������  ���� ���ǶԷ����ȶ��ӣ������ȶ���
	int found = 0;																						//����һ��������������ʱ found++
	for (int dir = 0; dir < 4; dir++)
	{
		int save_dir = dir;
		int found_sign = 0;																				//һ���������ҵ������Ҫ��ı�־
		int ostab = 0;
		for (int a = 0; a < 2; a++) {

			//��һ����������һ���Ǳ߽������ͬ�����ȶ���ʱ���������������ˣ�ֱ��found_sign = 2
			if ((i + direction[dir][0] < 0) || (i + direction[dir][0] > 15) || (j + direction[dir][1] > 15) || (j + direction[dir][1] < 0) || board_stabilizer[boardnum][i + direction[dir][0]][j + direction[dir][1]] == me) {
				found_sign = 2;
				break;
			}
			if (board_stabilizer[boardnum][i + direction[dir][0]][j + direction[dir][1]] != 0) {		//��һ������һ���ǶԷ����ȶ��ӣ���Ҫ����һ��������������
				found_sign++;
			}
			dir += 4;
		}
		if (found_sign >= 2) found++;
		dir = save_dir;
	}
	if (found == 4) board_stabilizer[boardnum][i][j] = me;
	return 0;
}

//Ѱ���Լ����������λ�ã��������ڵ÷ֱ��е�position[2]��
int find(int depth_find, char board[16][16], char side)
{
	int canrow, cancol;
	int can_move_step = 0;
	int found = 0;

	for (canrow = 0; canrow < 16; canrow++)
	{
		for (cancol = 0; cancol < 16; cancol++)
		{
			if (board[canrow][cancol] != NOCHESS) continue;
			for (int dir = 0; dir < 8; dir++)			//����������
			{
				int i = canrow; int j = cancol;			//i,j:search
				int flip = 0;
				i += direction[dir][0];
				j += direction[dir][1];
				if (board[i][j] == side) continue;
				while (i < 16 && j < 16 && i >= 0 && j >= 0)
				{
					if (board[i][j] == NOCHESS) break;
					if (board[i][j] == side) {
						canmove[depth_find][can_move_step].position[0] = canrow;
						canmove[depth_find][can_move_step].position[1] = cancol;
						can_move_step++;
						found++;
						break;
					}
					i += direction[dir][0];
					j += direction[dir][1];
				}
			}
		}
	}
	return found;
}

//Ѱ�ҿ����µĵط����ж�����
int fine_canact(char side, int acti, int actj)
{
	int found = 0;
	for (int dir = 0; dir < 8; dir++)			//����������
	{
		int i = acti; int j = actj;				//i,j:search
		int flip = 0;
		i += direction[dir][0];
		j += direction[dir][1];
		if (testboard[depthmax][i][j] == side) continue;
		while (i < 16 && j < 16 && i >= 0 && j >= 0)
		{
			if (testboard[depthmax][i][j] == NOCHESS) break;
			if (testboard[depthmax][i][j] == side) return 1;
			i += direction[dir][0];
			j += direction[dir][1];
		}
	}
	return 0;
}

//����󣬸ı�����
void flipchess(char board[][16], int mrow, int mcol, char side)
{
	board[mrow][mcol] = side;
	for (int dir = 0; dir < 8; dir++)		//����������
	{
		int search_i = mrow; int search_j = mcol;			//i,j:search
		int flip = 0; int flipnum = 0;						//�ѷ����������ɷ�������
		int fi = search_i; int fj = search_j;				//fi,fj:flip
		search_i += direction[dir][0];
		search_j += direction[dir][1];
		if (board[search_i][search_j] == side) continue;
		while (search_i < 16 && search_j < 16 && search_i >= 0 && search_j >= 0)
		{
			flipnum++;
			if (board[search_i][search_j] == NOCHESS) break;
			if (board[search_i][search_j] == side)
			{
				while (flip < flipnum) {
					fi += direction[dir][0];
					fj += direction[dir][1];
					board[fi][fj] = side;
					flip++;
				}
				break;
			}
			search_i += direction[dir][0];
			search_j += direction[dir][1];
		}
	}
}

//Ѱ�����������ӵ�����
//�㷴�������������ߣ��ʴ�Ϊ��ѧϵ��
int check_choosstable()
{
	int num = 0;
	for (int i = 2; i <= 14; i++)
	{
		for (int j = 2; j <= 14; j++)
		{
			if (chessboard[i][j] == NOCHESS) num++;
		}
	}
	if (num < 10 || chessnum > 190) return 1;						//���������ӽ��ٻ�߽������ӹ���ʱ��ת����ֵ��
	else return 0;
}

//��սṹcanmove
void reset_canmove(int depth)
{
	for (int i = 0; i < 256; i++)
	{
		memset(canmove[depth][i].position, NOCHESS, sizeof(canmove[depth][i].position));
		canmove[depth][i].value = 0;
	}
}

void printcboard(char bbb[16][16])
{
	int i, j;
	printf("  A  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5\n");
	for (i = 0; i < 16; i++) {
		printf("  %-3d", i);
		for (j = 0; j < 16; j++) {
			printf("%c ", bbb[i][j]);
		}
		printf(" \n");
	}
	printf("chessnum = %d\n", chessnum);
}
