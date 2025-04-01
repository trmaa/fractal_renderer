#include "window.hpp"

int main() {
	Window window;

	sf::Event ev;
	while (window.isOpen()) {
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.render();
	}
}
