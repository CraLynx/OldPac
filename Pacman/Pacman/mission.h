#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
///////////////////////////////////����� ������//////////////////////////////////
int getCurrentMission(int x)//�-��� ������ ������, ������� ������ ����� ������, � ����������� �� ���������� ������ � (���� ����� ���������� ��� ����������)
{
	int mission = 0;
	if ((x>0) && (x<600)) { mission = 0; } //�������� ������ � �����
	if (x>400) { mission = 1; } //����� �� ������ ������
	if (x>700) { mission = 2; }//2��
	if (x>2200) { mission = 3; }//� ��
	return mission;//�-��� ���������� ����� ������
}
/////////////////////////////////////����� ������/////////////////////////////////
std::string getTextMission(int currentMission) {
	std::string missionText = "";//����� ������ � ��� �������������
	switch (currentMission)//����������� ����� ������ � � ����������� �� ���� ���������� missionText ������������� ��������� �����
	{
	case 0: missionText = "\nDon't die \nand win"; break;
	case 1: missionText = "\nDon't die \nand win"; break;
	case 2: missionText = "\nDon't die \nand win "; break;
	case 3: missionText = "\nDon't die \nand win"; break;
	}
	return missionText;//�-��� ���������� �����
};