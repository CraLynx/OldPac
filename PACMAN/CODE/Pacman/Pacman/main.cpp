#include <iostream> 
#include <SFML/Graphics.hpp>
#include <sstream>
#include "view.h"
#include "map.h" //���������� ��� � ������
#include "mission.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
//#include "Audio.hpp"

using namespace sf;

class Player { // ����� ������
public:
	int dir, playerScore, health;	//����� ����������, �������� ����� ������
	bool life;//���������� ���������� �����
	float x, y, w, h, dx, dy, speed = 0; //���������� ������ � � �, ������ ������, ��������� (�� � � �� �), ���� ��������
	String File; //���� � �����������
	Image image;//���� �����������
	Texture texture;//���� ��������
	Sprite sprite;//���� ������

	Player (String F, float X, float Y, float W, float H) {  //����������� � �����������(�����������) ��� ������ Player. ��� �������� ������� ������ �� ����� �������� ��� �����, ���������� � � �, ������ � ������
		dir = 0; speed = 0; playerScore = 0; health = 100; // ���������������� ���������� ����� � ������������
		life = true;//���������������� ���������� ���������� �����
		File = F;//��� �����+����������
		w = W; h = H;//������ � ������
		image.loadFromFile("images/" + File);//���������� � image ���� ����������� ������ File �� ��������� ��, ��� �������� ��� �������� �������. � ����� ������ "hero.png" � ��������� ������ ���������� 	image.loadFromFile("images/hero/png");
		image.createMaskFromColor(Color(255, 255, 255));//������� �������� �����-����� ����, ��� ���� ��� ���������� �� ��������.
		texture.loadFromImage(image);//���������� ���� ����������� � ��������
		sprite.setTexture(texture);//�������� ������ ���������
		x = X; y = Y;//���������� ��������� �������
		sprite.setTextureRect(IntRect(0, 0, w, h));  //������ ������� ���� ������������� ��� ������ ������ ����, � �� ���� ����� �����. IntRect - ���������� �����
	}

	void update(float time) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
	{
		switch (dir)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
		{
			case 0: dx = speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ��������, ��� �������� ���� ������ ������
			case 1: dx = -speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ����������, ��� �������� ���� ������ �����
			case 2: dx = 0; dy = speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ ����
			case 3: dx = 0; dy = -speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ �����
		}

		x += dx*time;//�� ��������. ���� ��������� �� ����� �������� �������� ��������� � ��� ��������� ��������
		y += dy*time;//���������� �� ������

		speed = 0;//�������� ��������, ����� �������� �����������.
		sprite.setPosition(x, y); //������� ������ � ������� x y , ����������. ���������� ������� � ���� �������, ����� �� ��� ������ ����� �� �����.
		interactionWithMap();//�������� �������, ���������� �� �������������� � ������
		if (health <= 0) { life = false; speed = 0; }//���� ������ ������ ���� ����� 0, �� ������� � ��������� �������� ������ ����� ������
	}

	void interactionWithMap()//�-��� �������������� � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = x / 32; j<(x + w) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
			{
				if (TileMap[i][j] == '0')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
				{
					if (dy>0)//���� �� ��� ����,
					{
						y = i * 32 - h;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
					}
					if (dy<0)
					{
						y = i * 32 + 32;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
					}
					if (dx>0)
					{
						x = j * 32 - w;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//���������� ���� �����
					}
				}

				if (TileMap[i][j] == 's') { //���� ������ ����� 's' (������)
					playerScore++;
					TileMap[i][j] = 'b';//������� ������, ���� ����� �����. ����� � �� �������, ������.
				}

				if (TileMap[i][j] == ' ') {
					playerScore++;
					//health -= 20;//���� ����� �������� ������,�� ���������� health=health-90;
					TileMap[i][j] = 'b';//������ ������
				}
			/*
				if (TileMap[i][j] == 'h') {
					health += 20;//���� ����� �����,�� ���������� health=health+20;
					TileMap[i][j] = ' ';//������ ��������
				}
			*/


			}
	}


	float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}
};

int randDir()
{
	return rand() % 4;
}

class Eminem : public Player
{

public:
	Eminem(String F, float X, float Y, float W, float H) : Player(F, X, Y, W, H)
	{
		this->dir = 0;
		this->speed = 0.07;
	}

	void update(float time) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
	{

		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = x / 32; j<(x + w) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
			{
				if (TileMap[i][j] == '0')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
				{
					if (dy>0)//���� �� ��� ����,
					{
						y = i * 32 - h;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
					}
					if (dy<0)
					{
						y = i * 32 + 32;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
					}
					if (dx>0)
					{
						x = j * 32 - w;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//���������� ���� �����
					}
				}

				this->dir = randDir();
				//Check collusion with player

			}


		switch (dir)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
		{
		case 0: dx = speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ��������, ��� �������� ���� ������ ������
		case 1: dx = -speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ����������, ��� �������� ���� ������ �����
		case 2: dx = 0; dy = speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ ����
		case 3: dx = 0; dy = -speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ �����
		}

		x += dx*time;//�� ��������. ���� ��������� �� ����� �������� �������� ��������� � ��� ��������� ��������
		y += dy*time;//���������� �� ������
		
		sprite.setPosition(x, y); //������� ������ � ������� x y , ����������. ���������� ������� � ���� �������, ����� �� ��� ������ ����� �� �����.
	
		if (health <= 0) { life = false; speed = 0; }//���� ������ ������ ���� ����� 0, �� ������� � ��������� �������� ������ ����� ������
	}
};

int main()
{
	RenderWindow window(sf::VideoMode(640, 480), "Pacman:Game"); //��������� ��������� ���������� 1- ������ ������ 2-title 3- ����� ���� �������� [sf::Style::None]
	view.reset(sf::FloatRect(0, 0, 640, 480));
	Font font;//����� 
	font.loadFromFile("DejaVuSans.ttf");//�������� ������ ������ ���� ������
	Text text("", font, 25);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);//������ � ������������ �����. �� ��������� �� "�����":)) � �� ������������


	Image map_image;//������ ����������� ��� �����
	map_image.loadFromFile("images/map.png");//��������� ���� ��� �����

	Texture map;//�������� �����
	map.loadFromImage(map_image);//�������� �������� ���������
	Sprite s_map;//������ ������ ��� �����
	s_map.setTexture(map);//�������� �������� ��������

	Image quest_image;
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //���������� �����, ������� �������� ��������
	s_quest.setScale(0.6f, 0.6f);//���� ��������� ��������, => ������ ���� ������



	float CurrentFrame = 0;//������ ������� ����
	Clock clock;

	Player p("PacmanPhoto(new).png", 260, 320, 25, 25);//������� ������ p ������ player,������ "hero.png" ��� ��� �����+����������, ����� ���������� �,�, ������, ������.
	

	std::vector<Eminem*> mobs = { 
		new Eminem("Ghost.png", 35, 35, 30, 30),
		new Eminem("Ghost.png", 490, 35, 30, 30),
		new Eminem("Ghost.png", 490, 350, 30, 30),
		new Eminem("Ghost.png", 35, 350, 30, 30),
	};

	for (int i = 0; i < mobs.size(); i++) {
		mobs[i]->dir = 0;
		mobs[i]->speed = 0.07;
	}

	bool showMissionText = true;//���������� ����������, ���������� �� ��������� ������ ������ �� ������
	while (window.isOpen())
	{
		
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)//������� ������� �������
				if ((event.key.code == Keyboard::Tab)) {//���� ������� ���


					switch (showMissionText) {//�������������, ����������� �� ���������� ���������� showMissionText
					case true: {
						std::ostringstream playerHealthString;//������ �������� ������
						playerHealthString << p.playerScore; //������� � ������ �������� 
						std::ostringstream task;//������ ������ ������
						task << getTextMission(getCurrentMission(p.getplayercoordinateX()));//���������� ������� getTextMission (��� ���������� ����� ������), ������� ��������� � �������� ��������� ������� getCurrentMission(������������ ����� ������), � ��� ��� �-��� ��������� � �������� ��������� ������� p.getplayercoordinateX() (��� �-��� ���������� ��� ���������� ������)
						text.setString("Score" + playerHealthString.str() + "\n" + task.str());//������
						showMissionText = false;//��� ������ ��������� ������ ��� ��� �� ������ �� ������
						break;//������� , ����� �� ��������� ������� "false" (������� ����)
					}
					case false: {
						text.setString("");//���� �� ������ ������� ���, �� ���� ���� ����� ������
						showMissionText = true;// � ��� ������ ��������� ����� ������ ������� ��� � �������� ����� �� �����
						break;
					}
					}
				}
			//������� ���������� ���� �� � ������ ������
			if (p.health <= 0) {			// ���� � ����� ������ ���
				std::cout << "You Die,Maannn! It's not Funny,guy" << '\n';
				system("pause"); //�����
				window.close();		//����� ������� ������� ��������� ����
			}
			//������� ���������� ���� � ������ ������
			if (p.playerScore == 100) {			// ���� � ����� ������ ���
				std::cout << "You Win!" << '\n';
				system("pause"); //�����
				window.close();		//����� ������� ������� ��������� ����
			}
		}

		srand(std::time(0));
		for (int i = 0; i < mobs.size(); i++) {

			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;

			if ((std::sqrt((mobs[i]->x - p.x)*(mobs[i]->x - p.x) + (mobs[i]->y - p.y)*((mobs[i]->y - p.y)))) < 25) {
				p.health -= 100;
			}

			mobs.at(i)->sprite.setTextureRect(IntRect(34 * int(CurrentFrame), 1, 32, 32));  //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����
			mobs.at(i)->update(time);
		}

		///////////////////////////////////////////���������� ���������� � ���������////////////////////////////////////////////////////////////////////////
		if (p.life) {
			if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
				p.dir = 1; p.speed = 0.07;//dir =1 - ����������� �����, speed =0.1 - �������� ��������. �������� - ����� �� ��� ����� �� �� ��� �� �������� � ����� �� ���������� ������ 
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(34 * int(CurrentFrame), 33, 32, 32)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
			}

			if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
				p.dir = 0; p.speed = 0.07;//����������� ������, �� 
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(34 * int(CurrentFrame), 1, 32, 32));  //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
				p.dir = 3; p.speed = 0.07;//����������� ����, �� 
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(34 * int(CurrentFrame), 66, 32, 32));  //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)

			}

			if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //���� ������ ������� ������� ����� ��� ���� ����� �
				p.dir = 2; p.speed = 0.07;//����������� �����, �� 
				CurrentFrame += 0.005*time; //������ ��� ����������� �� "������". ���������� ������� �� ���� �������� ������������ ������� � ��������. ������� 0.005 ����� �������� �������� ��������
				if (CurrentFrame > 3) CurrentFrame -= 3; //���������� �� ������ � ������� �� ������ ������������. ���� ������ � �������� ����� - ������������ �����.
				p.sprite.setTextureRect(IntRect(34 * int(CurrentFrame), 100, 32, 32)); //���������� �� ����������� �. ���������� 96,96*2,96*3 � ����� 96

			}
		}

		p.update(time);//�������� ������ p ������ Player � ������� ������� sfml, ��������� ����� � �������� ��������� ������� update. ��������� ����� �������� ����� ���������


					   //viewmap(time);//������� ���������� �����, �������� �� ����� sfml
					   //changeview();//������������� � �������
					   //window.setView(view);//"��������" ������ � ���� sfml  (�� ����� ��������� ��������(-;-))
		window.clear();


		/////////////////////////////������ �����/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{

				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //�����
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//����
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//0-�����
				if (TileMap[i][j] == 'b')  s_map.setTextureRect(IntRect(97, 0, 32, 32)); //�������

				s_map.setPosition(j * 32, i * 32);//�� ���� ����������� ����������, ��������� � �����. �� ���� ������ ������� �� ��� �������. ���� ������, �� ��� ����� ���������� � ����� �������� 32*32 � �� ������ ���� �������

				window.draw(s_map);//������ ���������� �� �����
			}

		/*
		std::ostringstream playerScoreString;    // �������� ����������
		playerScoreString << p.playerScore;		//������� � ��� ����� �����, �� ���� ��������� ������
		text.setString("Score:" + playerScoreString.str());//������ ������ ������ � �������� �������������� ���� ������ ������� .str()
		text.setPosition(265, 32);//������ ������� ������, �������� �� ������ ������
		*/
		if (!showMissionText) {
			text.setPosition(view.getCenter().x + -70, view.getCenter().y - 150);//������� ����� ����� ���������� �����
			s_quest.setPosition(view.getCenter().x + -105, view.getCenter().y - 200);//������� ���� ��� �����		
			window.draw(s_quest); window.draw(text); //������ ������ ������ (��� ��� ������ ������). � ����� � �����. ��� ��� �������� �� ���������� ����������, ������� ������ ���� ��������� �� ������� ������� ���
		}
		window.draw(p.sprite);	 //������ ������ ������� p ������ player

		for (int i = 0; i < mobs.size(); i++) {
			window.draw(mobs[i]->sprite);
		}
		window.display();

		/*
		std::ostringstream playerHealthString;    // �������� ����������
		playerHealthString << p.health;		//������� � ��� ����� ��������, �� ���� ��������� ������
		text.setString("��������:" + playerHealthString.str());//������ ������ ������ � �������� �������������� ���� ������ ������� .str()
		text.setPosition(240,32);//������ ������� ������, �������� �� ������ ������
		window.draw(text);//����� ���� �����
		*/
	}

	return 0;
}




