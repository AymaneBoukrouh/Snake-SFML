#include "Config.h"
#include "Block.h"
#include "Snake.h"
#include <SFML/Graphics.hpp>


using std::pair;
using std::vector;
using std::string;


class MainWindow: public sf::RenderWindow {
public:
	int
		WIDTH = (BS+GSS) * (GXS-1),
		HEIGHT = (BS+GSS) * (GYS-1);

	Snake snake;
	sf::Thread *snake_mouvement;

	MainWindow () {
		this->create(
			sf::VideoMode(this->WIDTH, this->HEIGHT),
			"Snake in C++"
		);
	}

	void run () {
		auto *snake = &(this->snake);
		auto snake_mouvement_wrapper = [snake]() {
			snake->start_mouvement();
		};

		sf::Thread snake_mouvement(snake_mouvement_wrapper);
		snake_mouvement.launch();
		this->snake_mouvement = &snake_mouvement;

		this->main_loop();
	}

	void main_loop () {
		while (this->isOpen())
		{
			sf::Event event;
			while (this->pollEvent(event))
			{
				if (event.type == sf::Event::KeyPressed) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) snake.change_direction("left");
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) snake.change_direction("up");
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) snake.change_direction("right");
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) snake.change_direction("down");

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						if (snake.is_dead) snake.reset();
						else snake.is_moving = not snake.is_moving;
					}
				}

				if (event.type == sf::Event::Closed) {
					this->snake_mouvement->terminate();
					this->close();
				}
			}

			this->clear(sf::Color::White);
			for (int i=0; i<snake.blocks.size(); i++) this->draw(snake.blocks[i]);
			this->draw(*(snake.bait));
			this->display();
		}
	}
};


int main()
{
	srand(time(NULL));
	auto mainWindow = new MainWindow();
	mainWindow->run();

	return 0;
}