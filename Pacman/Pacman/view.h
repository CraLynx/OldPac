#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объ€вили sfml объект "вид", который и €вл€етс€ камерой
void viewmap(float time) { //функци€ дл€ перемещени€ камеры по карте. принимает врем€ sfml
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		view.move(0.1*time, 0);//скроллим карту вправо (см урок, когда мы двигали геро€ - всЄ тоже самое)
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		view.move(0, 0.1*time);//скроллим карту вниз (см урок, когда мы двигали геро€ - всЄ тоже самое)
	}
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		view.move(-0.1*time, 0);//скроллим карту влево (см урок, когда мы двигали геро€ - всЄ тоже самое)
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		view.move(0, -0.1*time);//скроллим карту вправо (см урок, когда мы двигали геро€ - всЄ тоже самое)
	}
}