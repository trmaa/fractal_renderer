#include "window.hpp"
#include "camera.hpp"

int main() {
	Window window;
	Camera camera;

	sf::Event ev;
	sf::Clock clock;
	while (window.isOpen()) {
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}
		float delta_time = clock.getElapsedTime().asSeconds();
		clock.restart();

		window.render();
		camera.move(delta_time);
		camera.handle_mouse_movement(window);
	}
}
