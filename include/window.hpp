#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "camera.hpp"

class Window: public sf::RenderWindow {
private:
	int m_width;
	int m_height;
	std::string m_title;

	sf::Font m_font;
	sf::Text m_fps_text;

	sf::Shader m_shader;
	sf::RectangleShape m_screen;

public:
	const int& get_width() { return m_width; }
	const int& get_height() { return m_height; }

public:
	Window(std::string title):
		m_title(title) {
		float factor = 0.5f;

		this->create(sf::VideoMode(1280*factor, 720*factor), m_title, sf::Style::None);
		this->setFramerateLimit(60);

		m_font.loadFromFile("bin/fonts/terminus.ttf");
		m_fps_text.setFont(m_font);
		m_fps_text.setCharacterSize(50);
		m_fps_text.setFillColor(sf::Color(255, 0, 255));
		m_fps_text.setPosition(10, 10);

		std::system("python3 scripts/shader_compiler.py");
		m_shader.loadFromFile("shaders/compiled_shader.glsl", sf::Shader::Fragment);
		m_shader.setUniform("screen_size", sf::Vector2f(this->getSize()));

		sf::RectangleShape s(sf::Vector2f(this->getSize().x, this->getSize().y));
		m_screen = s;
		m_screen.setPosition(0, 0);
		m_screen.setScale(1,1);
	}

	void repaint(float delta_time, Camera camera) {
		this->clear();

		m_shader.setUniform("cam_pos", camera.get_position());
		m_shader.setUniform("cam_ang", camera.get_angle());
		m_shader.setUniform("iTime", delta_time);
		m_shader.setUniform("screen_size", sf::Vector2f(this->getSize()));
		this->draw(m_screen, &m_shader);

		m_fps_text.setString("fps: "+std::to_string(1 + (int)(1.f/delta_time)) + " (hz)");
		this->draw(m_fps_text);

		this->display();
	}
};

#endif
