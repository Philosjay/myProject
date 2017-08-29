#include "..\Include\SceneNode.h"

#include <queue>
#include <Windows.h>

SceneNode::SceneNode(const char* fileName)
{
	loadImageFromFile(fileName);

	mDrawPoint.x = 0;
	mDrawPoint.y = 0;
}

char * SceneNode::getParticle()
{
	//��ȡͼ�ε���������
	return mImage;
}

void SceneNode::setDrawPoint(int x, int y)
{
	mDrawPoint.x = x;
	mDrawPoint.y = y;
}

void SceneNode::draw()
{
	//��ӡͼ��
	int rowCount = 1;
	setImagePosition(getPosition().x, getPosition().y);

	for (int i = 0; i < getSize().x * getSize().y; i++) {
		printf("%c", mImage[i]);
		if (mImage[i] == '\n') {
			setImagePosition(getPosition().x, getPosition().y+rowCount);
			rowCount++;
		}
	}
}

void SceneNode::loadImageFromFile(const char* fileName)
{
	//���ı��ж�ȡͼ��
	FILE* mfile;
	char tmp;
	int particleCount = 0;
	int width = 0;
	bool	isFirstLine = true;
	fopen_s(&mfile, fileName, "r");
	//��ɨ������ͼ����¼����������ͼ�γߴ�
	while (fscanf_s(mfile, "%c", &tmp) != EOF) {
		particleCount++;
		if (isFirstLine) {
			width++;
		}
		if (tmp == '\n') {
			isFirstLine = false;
		}
	}
	
	//�趨�ߴ�
	setSize(width,particleCount/width);
	//�ٴ�ɨ��ͼ�Σ������������ص�
	fseek(mfile, 0, 0);
	fopen_s(&mfile, fileName, "r");
	mImage = new char[particleCount];
	for (int i = 0; i < particleCount; i++) {
		fscanf_s(mfile, "%c", (mImage + i));
	}
	fclose(mfile);
}
	



void SceneNode::setImagePosition(int x, int y)
{
	HANDLE hout;
	COORD coord;
	coord.X = x + mDrawPoint.x;
	coord.Y = y + mDrawPoint.y;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord);
}
