#include <stdio.h>
#include <string>
#include <fstream>  
#include <iostream>  
#include <conio.h>
#include <Windows.h>

using namespace std;

string loadFromFile(char* fileName);
string loadFromFile(const char* fileName);
void CreateMapFile(string mString);
void CreateDataFile(string mString);
void PrintToMap(char* map, const char* unit, int width, int& unitCount);
void HandleInput();
void HandleStatus();
void setCursorPosition(int x, int y);
int getNum();
void printImage(string mString,int x, int y);
void printPreview(string mString);
void setCursorVisible(bool isVisible);
void referMaps();


typedef struct {
	int x;
	int y;
}Location;

enum Status
{
	MainMenu,
	GetNum_Add,
	GetNum_OverWrite,
	Preview,
	Report,
	Refer,
};

int mStatus = Status::MainMenu;
int mNum = -1;
int mToltalMap;

bool isAdding = false;	//��ӵ�ͼʱ�����ڴ�����;����ʱmToltalMap�ĵݼ�

//��ӡ����ĳ�ʼλ��
int drawpoint_x = 30;
int drawpoint_y = 2;

FILE* fp_gameData;
string status_MainMenu;
string status_Add;
string status_OverWrite;
string status_Preview;
string status_Report;
string status_Refer;
string image_Player;
string image_Box;

void main() {

	//��ȡ���е�ͼ��
	fopen_s(&fp_gameData, "../Game/Data/Game.txt", "r");
	fscanf_s(fp_gameData, "%d", &mToltalMap);
	fclose(fp_gameData);
	//��ȡ����
	status_MainMenu = loadFromFile("Images/MainMenu.txt");
	status_Add = loadFromFile("Images/Add.txt");
	status_Preview = loadFromFile("Images/Preview.txt");
	status_OverWrite = loadFromFile("Images/OverWrite.txt");
	status_Report = loadFromFile("Images/Report.txt");
	status_Refer = loadFromFile("Images/Refer.txt");
	image_Box = loadFromFile("../Game/Images/Box.txt");
	image_Player = loadFromFile("../Game/Images/Player.txt");

	setCursorVisible(false);

	while (1) {


		HandleStatus();
		HandleInput();

	}



	
}
string loadFromFile(const char* fileName) {




	FILE* fp_file;
	fopen_s(&fp_file, fileName, "r");

	char tmp[100];
	std::string	mPtc;

	ifstream in(fileName);
	string filename;
	string line;
	string mString;

	//��ȡ�����ַ�
	if (in) // �и��ļ�  
	{
		while (getline(in, line)) // line�в�����ÿ�еĻ��з�  
		{
			mString += line;
			mString += "\n";
		}
	}
	else // û�и��ļ�  
	{
		cout << "no such file" << endl;
	}
	fclose(fp_file);
	return mString;
}
string loadFromFile(char* fileName) {

	FILE* fp_file;
	fopen_s(&fp_file, fileName, "r");

	char tmp[100] ;
	std::string	mPtc;

	ifstream in(fileName);
	string filename;
	string line ;
	string mString;

	//��ȡ�����ַ�
	if (in) // �и��ļ�  
	{
		while (getline(in, line)) // line�в�����ÿ�еĻ��з�  
		{
			mString += line;
			mString += "\n";
		}
	}
	else // û�и��ļ�  
	{
		cout << "no such file" << endl;
	}
	fclose(fp_file);
	return mString;
}

void CreateMapFile(string mString) {

	const char* mapParticals = mString.c_str();
	//�����ͼ�ߴ�
	int width = mString.find_first_of("\n") * 7;
	int height = mString.size()/(mString.find_first_of("\n") + 1) * 4 ;
	int totalParticles = height * width + height;
	char* mMap = new char[totalParticles];


	//��ȡWall��Terminal������stringתΪchar*
	string string_Terminal = loadFromFile("../Game/Images/Terminal.txt");
	string string_Wall = loadFromFile("../Game/Images/Wall.txt");
	string string_Blank = loadFromFile("../Game/Images/Blank.txt");
	const char* char_Terminal = new char[string_Terminal.size()];
	const char* char_Wall = new char[string_Wall.size()];
	const char* char_Blank = new char[string_Blank.size()];
	char_Terminal = string_Terminal.c_str();
	char_Wall = string_Wall.c_str();
	char_Blank = string_Blank.c_str();

	int unitCoumt = 0;

	//��Wall��Terminal���ַ�����ת��Ϊ����
	for (int i = 0; i < mString.size(); i++) {

		
		switch (*(mapParticals + i))
		{
		case ' ':
		case 'b':
		case 'p':
			PrintToMap(mMap, char_Blank,width,unitCoumt);
			break;
		case 'w':
			PrintToMap(mMap, char_Wall, width,unitCoumt);
			break;
		case 't':
			PrintToMap(mMap, char_Terminal, width,unitCoumt);
			break;
		default:
			break;
		}

	}
	//��ӡ��ͼ
	FILE* fp_map;
	string mapFileName;
	if (isAdding) {
		mapFileName = "../Game/Maps/Map" + to_string(mToltalMap) + ".txt";
	}
	else {
		mapFileName = "../Game/Maps/Map" + to_string(mNum) + ".txt";
	}
	fopen_s(&fp_map, mapFileName.c_str(), "w");


	//��map���ش�ӡ
	for (int i = 0; i < totalParticles; i++) {
		fprintf(fp_map,"%c", *(mMap + i));
 	}
	fclose(fp_map);
}

void CreateDataFile(string mString)
{
	//��ȡ���༭��ͼ���ַ���Ϣ
	const char* mapParticals = mString.c_str();
	//�����ͼ�ߴ�
	int width = mString.find_first_of("\n") + 1;
	int height = mString.size() / (mString.find_first_of("\n") + 1);

	//��ȡ������
	int boxCount = 0;
	for (int i = 0; i < mString.size(); i++) {
		if (*(mapParticals + i) == 'b') {
			boxCount++;
		}
	}

	//��ȡ���ӣ���ң��յ�����
	Location* box = new Location[boxCount];
	Location* player = new Location;
	Location* terminal = new Location[boxCount];

	int AddedBox = 0;
	int AddedTerminal = 0;
	for (int i = 0; i < mString.size(); i++) {

		switch (*(mapParticals+i))
		{
		case 'b':
			(box + AddedBox)->x = i % width;
			(box + AddedBox)->y = i / width;
			AddedBox++;
			break;
		case 'p':
			player->x = i % width;
			player->y = i / width;
			break;
		case 't':
			(terminal + AddedTerminal)->x = i % width;
			(terminal + AddedTerminal)->y = i / width;
			AddedTerminal++;
			break;
		default:
			break;
		}
	}
	//��ӡ����
	string dataFileName;
	if (isAdding) {
		dataFileName = "../Game/Data/Data" + to_string(mToltalMap) + ".txt";
	}
	else {
		dataFileName = "../Game/Data/Data" + to_string(mNum) + ".txt";
	}
	FILE* fp_dataFile;
	fopen_s(&fp_dataFile, dataFileName.c_str(), "w");

	//�������
	fprintf(fp_dataFile, "%d %d \n", player->x, player->y);
	//������
	fprintf(fp_dataFile, "%d \n", boxCount);
	//��������
	for (int i = 0; i < boxCount; i++) {
		fprintf(fp_dataFile, "%d %d \n", (box + i)->x, (box + i)->y);
	}
	//�յ�����
	for (int i = 0; i < boxCount; i++) {
		fprintf(fp_dataFile, "%d %d \n", (terminal + i)->x, (terminal + i)->y);
	}

	fclose(fp_dataFile);
}

void PrintToMap(char* map, const char* unit, int width,int& unitCount) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			*(map + (j + (unitCount * 7) % width) + (i + ((unitCount * 7) / width) * 4) * (width + 1)) = *(unit + i * 8 + j);

		}
	}
	unitCount++;
}

void HandleInput()
{
	if (mStatus != Status::GetNum_OverWrite) {

		char input;
		input = _getch();
		switch (input)
		{
			//��ӵ�ͼ
		case 'a':
			if (mStatus == Status::MainMenu) {
				mStatus = Status::GetNum_Add;
				isAdding = true;
				mToltalMap++;
			}
			break;
			//��д��ͼ
		case 'o':
			if (mStatus == Status::MainMenu) {
				mStatus = Status::GetNum_OverWrite;
			}
			break;
		case ' ':
			if (mStatus == Status::GetNum_Add) {
				mStatus = Preview;
			}
			else if (mStatus == Status::Report) {
				mStatus = MainMenu;
				isAdding = false;
			}
			else if (mStatus == Status::Preview) {
				//��ȡ���༭��ͼ���ַ���Ϣ
				string mString = loadFromFile("Generator/blueprint.txt");
				CreateMapFile(mString);
				CreateDataFile(mString);

				//ˢ�µ�ͼ����
				fopen_s(&fp_gameData, "../Game/Data/Game.txt","w");
				fprintf(fp_gameData,"%d",mToltalMap);
				fclose(fp_gameData);
				mStatus = Status::Report;
			}
			break;
		case 'm':
			//��ӵ�ͼʱ��;�˳���mToltalMap�ݼ�
			if (isAdding) {
				isAdding = false;
				mToltalMap--;
			}
			system("cls");
			mStatus = MainMenu;
			break;
		case 'r':
			if (mStatus == MainMenu) {
				system("cls");
				printImage(status_Refer,0,0);
				referMaps();
				mStatus = Status::Refer;
			}
			break;
		default:
			break;
		}
	}
	else {
		mNum = getNum();
		if (mNum >0&&mNum<=mToltalMap) {
			mStatus = GetNum_Add;
		}

	}


}

void HandleStatus()
{
	string mPreview;
	switch (mStatus)
	{
	case Status::MainMenu:
		printImage(status_MainMenu,0,0);
		setCursorPosition(15 + drawpoint_x,5 + drawpoint_y);
		printf("%d",mToltalMap);
		break;
	case Status::GetNum_Add:
		printImage(status_Add,0,0);
		break;
	case Status::GetNum_OverWrite:
		printImage(status_OverWrite,0,0);
		setCursorPosition(24 + drawpoint_x, 5 + drawpoint_y);
		printf("%d~%d",1,mToltalMap);
		break;
	case Status::Preview:
		printImage(status_Preview,0,0);
		mPreview = loadFromFile("Generator/blueprint.txt");
		printPreview(mPreview);
		break;
	case Status::Report:
		printImage(status_Report,0,0);
		break;
	default:
		break;
	}
}

void setCursorPosition(int x, int y)
{
	HANDLE hout;
	COORD coord;
	coord.X = x;
	coord.Y = y;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord);
}

void setCursorVisible(bool isVisible)
{
	//���ù���Ƿ�ɼ�
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = isVisible;
	SetConsoleCursorInfo(hOut, &cci);
}

void referMaps()
{
	POINT pt;


	printf("\n");
	for (int i = 0; i < mToltalMap; i++) {

		string mapFileName = "../Game/Maps/Map";
		string dataFileName = "../Game/Data/Data";
		mapFileName += (to_string(i + 1) + ".txt");
		dataFileName += (to_string(i + 1) + ".txt");
		string Map = loadFromFile(mapFileName.c_str());
		printf("Map%d:\n\n", i + 1);

		//��ȡ���λ��
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO bInfo;
		GetConsoleScreenBufferInfo(hOut, &bInfo);
		
		int x_begin = bInfo.dwCursorPosition.X;
		int y_begin = bInfo.dwCursorPosition.Y;

		printf("%s", Map.c_str());
		printf("                                                                         �� M ������.\n");
		printf("-------------------------------------------------------------------------------------\n");

		GetConsoleScreenBufferInfo(hOut, &bInfo);

		int x_end = bInfo.dwCursorPosition.X;
		int y_end = bInfo.dwCursorPosition.Y;

		//��ȡ����
		FILE* fp;
		int postion_x;
		int postion_y;
		int boxCount;
		fopen_s(&fp, dataFileName.c_str(), "r");
		//���
		fscanf_s(fp, "%d", &postion_x);
		fscanf_s(fp, "%d", &postion_y);
		printImage(image_Player, postion_x * 7 + x_begin - drawpoint_x, postion_y * 4 + y_begin - drawpoint_y);
		//����
		fscanf_s(fp, "%d", &boxCount);
		for (int i = 0; i < boxCount; i++) {
			fscanf_s(fp, "%d", &postion_x);
			fscanf_s(fp, "%d", &postion_y);
			printImage(image_Box, postion_x * 7 + x_begin - drawpoint_x, postion_y * 4 + y_begin - drawpoint_y);
		}
		fclose(fp);
		setCursorPosition(x_end,y_end);

	}
}

int getNum() {
	setCursorVisible(true);
	int num;
	setCursorPosition(32 + drawpoint_x, 5 + drawpoint_y);
	scanf_s("%d", &num);
	setCursorVisible(false);
	return num;
}

void printImage(string mString, int x, int y)
{
	const char* mImage = new char[mString.size()];
	mImage = mString.c_str();
	setCursorPosition(x + drawpoint_x,y + drawpoint_y);
	int lineCount = 0;
	for (int i = 0; i < mString.size(); i++) {
		printf("%c", *(mImage + i));
		if (*(mImage + i) == '\n') {
			lineCount++;
			setCursorPosition(x + drawpoint_x, y + drawpoint_y + lineCount );
			
		}
	}
}

void printPreview(string mString) {
	const char* mImage = new char[mString.size()];
	mImage = mString.c_str();
	setCursorPosition(15 + drawpoint_x, 4 + drawpoint_y);
	int lineCount = 0;
	for (int i = 0; i < mString.size(); i++) {
		if (*(mImage + i) == '\n') {
			printf("%c", '<');
			lineCount++;
			setCursorPosition(15 + drawpoint_x, 4 + drawpoint_y + lineCount);
		}
		else {
			printf("%c", *(mImage + i));
		}
	

	}
}
