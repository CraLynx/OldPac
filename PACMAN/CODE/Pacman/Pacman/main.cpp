#include <iostream> 
#include <SFML/Graphics.hpp>
#include <sstream>
#include "view.h"
#include "map.h" //подключили код с картой
#include "mission.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
//#include "Audio.hpp"

using namespace sf;

class Player { // класс Игрока
public:
	int dir, playerScore, health;	//новая переменная, хранящая жизни игрока
	bool life;//логическая переменная жизнь
	float x, y, w, h, dx, dy, speed = 0; //координаты игрока х и у, высота ширина, ускорение (по х и по у), сама скорость
	String File; //файл с расширением
	Image image;//сфмл изображение
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт

	Player (String F, float X, float Y, float W, float H) {  //Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту
		dir = 0; speed = 0; playerScore = 0; health = 100; // инициализировали переменную жизни в конструкторе
		life = true;//инициализировали логическую переменную жизни
		File = F;//имя файла+расширение
		w = W; h = H;//высота и ширина
		image.loadFromFile("images/" + File);//запихиваем в image наше изображение вместо File мы передадим то, что пропишем при создании объекта. В нашем случае "hero.png" и получится запись идентичная 	image.loadFromFile("images/hero/png");
		image.createMaskFromColor(Color(255, 255, 255));//убираем ненужный темно-синий цвет, эта тень мне показалась не красивой.
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);//заливаем спрайт текстурой
		x = X; y = Y;//координата появления спрайта
		sprite.setTextureRect(IntRect(0, 0, w, h));  //Задаем спрайту один прямоугольник для вывода одного льва, а не кучи львов сразу. IntRect - приведение типов
	}

	void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
			case 0: dx = speed; dy = 0;   break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
			case 1: dx = -speed; dy = 0;   break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
			case 2: dx = 0; dy = speed;   break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
			case 3: dx = 0; dy = -speed;   break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		}

		x += dx*time;//то движение. наше ускорение на время получаем смещение координат и как следствие движение
		y += dy*time;//аналогично по игреку

		speed = 0;//зануляем скорость, чтобы персонаж остановился.
		sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
		if (health <= 0) { life = false; speed = 0; }//если жизней меньше либо равно 0, то умираем и исключаем движение игрока после смерти
	}

	void interactionWithMap()//ф-ция взаимодействия с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j<(x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy>0)//если мы шли вниз,
					{
						y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy<0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx>0)
					{
						x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}

				if (TileMap[i][j] == 's') { //если символ равен 's' (камень)
					playerScore++;
					TileMap[i][j] = 'b';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
				}

				if (TileMap[i][j] == ' ') {
					playerScore++;
					//health -= 20;//если взяли ядовитый цветок,то переменная health=health-90;
					TileMap[i][j] = 'b';//убрали цветок
				}
			/*
				if (TileMap[i][j] == 'h') {
					health += 20;//если взяли бонус,то переменная health=health+20;
					TileMap[i][j] = ' ';//убрали сердечко
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

	void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{

		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j<(x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy>0)//если мы шли вниз,
					{
						y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy<0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx>0)
					{
						x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}

				this->dir = randDir();
				//Check collusion with player

			}


		switch (dir)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case 0: dx = speed; dy = 0;   break;//по иксу задаем положительную скорость, по игреку зануляем. получаем, что персонаж идет только вправо
		case 1: dx = -speed; dy = 0;   break;//по иксу задаем отрицательную скорость, по игреку зануляем. получается, что персонаж идет только влево
		case 2: dx = 0; dy = speed;   break;//по иксу задаем нулевое значение, по игреку положительное. получается, что персонаж идет только вниз
		case 3: dx = 0; dy = -speed;   break;//по иксу задаем нулевое значение, по игреку отрицательное. получается, что персонаж идет только вверх
		}

		x += dx*time;//то движение. наше ускорение на время получаем смещение координат и как следствие движение
		y += dy*time;//аналогично по игреку
		
		sprite.setPosition(x, y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
	
		if (health <= 0) { life = false; speed = 0; }//если жизней меньше либо равно 0, то умираем и исключаем движение игрока после смерти
	}
};

int main()
{
	RenderWindow window(sf::VideoMode(640, 480), "Pacman:Game"); //принимает несколько параметров 1- размер экрана 2-title 3- стиль окна например [sf::Style::None]
	view.reset(sf::FloatRect(0, 0, 640, 480));
	Font font;//шрифт 
	font.loadFromFile("DejaVuSans.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 25);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый


	Image map_image;//объект изображения для карты
	map_image.loadFromFile("images/map.png");//загружаем файл для карты

	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

	Image quest_image;
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //приведение типов, размеры картинки исходные
	s_quest.setScale(0.6f, 0.6f);//чуть уменьшили картинку, => размер стал меньше



	float CurrentFrame = 0;//хранит текущий кадр
	Clock clock;

	Player p("PacmanPhoto(new).png", 260, 320, 25, 25);//создаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.
	

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

	bool showMissionText = true;//логическая переменная, отвечающая за появление текста миссии на экране
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

			if (event.type == Event::KeyPressed)//событие нажатия клавиши
				if ((event.key.code == Keyboard::Tab)) {//если клавиша ТАБ


					switch (showMissionText) {//переключатель, реагирующий на логическую переменную showMissionText
					case true: {
						std::ostringstream playerHealthString;//строка здоровья игрока
						playerHealthString << p.playerScore; //заносим в строку здоровье 
						std::ostringstream task;//строка текста миссии
						task << getTextMission(getCurrentMission(p.getplayercoordinateX()));//вызывается функция getTextMission (она возвращает текст миссии), которая принимает в качестве аргумента функцию getCurrentMission(возвращающую номер миссии), а уже эта ф-ция принимает в качестве аргумента функцию p.getplayercoordinateX() (эта ф-ция возвращает Икс координату игрока)
						text.setString("Score" + playerHealthString.str() + "\n" + task.str());//задаем
						showMissionText = false;//эта строка позволяет убрать все что мы вывели на экране
						break;//выходим , чтобы не выполнить условие "false" (которое ниже)
					}
					case false: {
						text.setString("");//если не нажата клавиша таб, то весь этот текст пустой
						showMissionText = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
						break;
					}
					}
				}
			//Условие завершения игры не в пользу игрока
			if (p.health <= 0) {			// если у герой собрал все
				std::cout << "You Die,Maannn! It's not Funny,guy" << '\n';
				system("pause"); //паузу
				window.close();		//после нажатия клавиши загрываем игру
			}
			//Условие завершения игры в пользу игрока
			if (p.playerScore == 100) {			// если у герой собрал все
				std::cout << "You Win!" << '\n';
				system("pause"); //паузу
				window.close();		//после нажатия клавиши загрываем игру
			}
		}

		srand(std::time(0));
		for (int i = 0; i < mobs.size(); i++) {

			CurrentFrame += 0.005*time;
			if (CurrentFrame > 3) CurrentFrame -= 3;

			if ((std::sqrt((mobs[i]->x - p.x)*(mobs[i]->x - p.x) + (mobs[i]->y - p.y)*((mobs[i]->y - p.y)))) < 25) {
				p.health -= 100;
			}

			mobs.at(i)->sprite.setTextureRect(IntRect(34 * int(CurrentFrame), 1, 32, 32));  //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку
			mobs.at(i)->update(time);
		}

		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
		if (p.life) {
			if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
				p.dir = 1; p.speed = 0.07;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый 
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(34 * int(CurrentFrame), 33, 32, 32)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
			}

			if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
				p.dir = 0; p.speed = 0.07;//направление вправо, см 
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(34 * int(CurrentFrame), 1, 32, 32));  //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
				p.dir = 3; p.speed = 0.07;//направление вниз, см 
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(34 * int(CurrentFrame), 66, 32, 32));  //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)

			}

			if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //если нажата клавиша стрелка влево или англ буква А
				p.dir = 2; p.speed = 0.07;//направление вверх, см 
				CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (CurrentFrame > 3) CurrentFrame -= 3; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
				p.sprite.setTextureRect(IntRect(34 * int(CurrentFrame), 100, 32, 32)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96

			}
		}

		p.update(time);//оживляем объект p класса Player с помощью времени sfml, передавая время в качестве параметра функции update. благодаря этому персонаж может двигаться


					   //viewmap(time);//функция скроллинга карты, передаем ей время sfml
					   //changeview();//прикалываемся с камерой
					   //window.setView(view);//"оживляем" камеру в окне sfml  (не очень корректно работает(-;-))
		window.clear();


		/////////////////////////////Рисуем карту/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{

				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //точки
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//боль
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//0-стена
				if (TileMap[i][j] == 'b')  s_map.setTextureRect(IntRect(97, 0, 32, 32)); //пустота

				s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

				window.draw(s_map);//рисуем квадратики на экран
			}

		/*
		std::ostringstream playerScoreString;    // объявили переменную
		playerScoreString << p.playerScore;		//занесли в нее число очков, то есть формируем строку
		text.setString("Score:" + playerScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str()
		text.setPosition(265, 32);//задаем позицию текста, отступая от центра камеры
		*/
		if (!showMissionText) {
			text.setPosition(view.getCenter().x + -70, view.getCenter().y - 150);//позиция всего этого текстового блока
			s_quest.setPosition(view.getCenter().x + -105, view.getCenter().y - 200);//позиция фона для блока		
			window.draw(s_quest); window.draw(text); //рисуем спрайт свитка (фон для текста миссии). а затем и текст. все это завязано на логическую переменную, которая меняет свое состояние от нажатия клавиши ТАБ
		}
		window.draw(p.sprite);	 //рисуем спрайт объекта p класса player

		for (int i = 0; i < mobs.size(); i++) {
			window.draw(mobs[i]->sprite);
		}
		window.display();

		/*
		std::ostringstream playerHealthString;    // объявили переменную
		playerHealthString << p.health;		//занесли в нее число здоровья, то есть формируем строку
		text.setString("Здоровье:" + playerHealthString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str()
		text.setPosition(240,32);//задаем позицию текста, отступая от центра камеры
		window.draw(text);//рисую этот текст
		*/
	}

	return 0;
}




