#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объявили sfml объект "вид", который и является камерой
void viewmap(float time) { //функция для перемещения камеры по карте. принимает время sfml
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		view.move(0.1*time, 0);//скроллим карту вправо (см урок, когда мы двигали героя - всё тоже самое)
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		view.move(0, 0.1*time);//скроллим карту вниз (см урок, когда мы двигали героя - всё тоже самое)
	}
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		view.move(-0.1*time, 0);//скроллим карту влево (см урок, когда мы двигали героя - всё тоже самое)
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		view.move(0, -0.1*time);//скроллим карту вправо (см урок, когда мы двигали героя - всё тоже самое)
	}
}