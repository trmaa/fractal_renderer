#ifndef WIDNOW_HPP
#define WIDNOW_HPP

#include <SFML/Graphics.hpp>
#include <cstdlib>

class Window : public sf::RenderWindow {
private:
	sf::RectangleShape m_buffer;
	sf::Shader m_shader;

public:
	Window() {
		int width = 1280;
		int height = 720;

		this->create(sf::VideoMode(width, height), "Ray Marcher", sf::Style::Fullscreen);

		m_buffer.setSize(sf::Vector2f(width, height));
		m_buffer.setFillColor(sf::Color::Blue);

		std::system("python3 scripts/shader_compiler.py");
		m_shader.loadFromFile("shaders/compiled_shader.glsl", sf::Shader::Fragment);
		m_shader.setUniform("resolution", sf::Vector2f(width, height));
	}

	void render() {
		this->clear();

		this->draw(m_buffer, &m_shader);
		
		this->display();
	}
};

#endif
