#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "window.hpp"
#include "camera.hpp"

Window window("Ray tracing");
Camera camera;

float i_time;
float delta_time;

void loop() {
    camera.move(delta_time);
    camera.handle_mouse_movement(window);

    window.repaint(i_time, delta_time, camera);
}

void setup_loop() {
    sf::Clock clock;
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        i_time += 0.02f * clock.getElapsedTime().asSeconds();
        delta_time = clock.restart().asSeconds();

        loop();
    }
}

int main() {
    camera.lock_mouse(window);

    setup_loop(); 
}
