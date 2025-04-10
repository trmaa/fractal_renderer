#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "camera.hpp"

class Window: public sf::RenderWindow {
private:
    int m_width = 160;
    int m_height = 90;
    std::string m_title;

    sf::Font m_font;
    sf::Text m_fps_text;

    sf::Shader m_shader;
    sf::RenderTexture m_render_texture;
    sf::Sprite m_screen_sprite;         
    sf::RectangleShape m_screen_shape;  

public:
    const int& get_width() { return m_width; }
    const int& get_height() { return m_height; }

    Window(std::string title): m_title(title) {
	float factor = 3.f;
	m_width *= factor;
	m_height *= factor;

	this->create(sf::VideoMode::getDesktopMode(), m_title, sf::Style::Fullscreen);
	this->setFramerateLimit(60);

	m_font.loadFromFile("bin/fonts/terminus.ttf");
	m_fps_text.setFont(m_font);
	m_fps_text.setCharacterSize(50);
	m_fps_text.setFillColor(sf::Color(255, 0, 255));
	m_fps_text.setPosition(10, 10);

	std::system("python3 scripts/shader_compiler.py");
	m_shader.loadFromFile("shaders/compiled_shader.glsl", sf::Shader::Fragment);
	m_shader.setUniform("screen_size", sf::Vector2f(m_width, m_height));

	m_render_texture.create(m_width, m_height);
	m_screen_sprite.setTexture(m_render_texture.getTexture());

	sf::Vector2f window_size = static_cast<sf::Vector2f>(this->getSize());
	sf::Vector2f scale(window_size.x / m_width, window_size.y / m_height);
	m_screen_sprite.setScale(scale);
    }

    void repaint(float i_time, float delta_time, Camera camera) {
	m_render_texture.clear();

	m_shader.setUniform("cam_pos", camera.get_position());
	m_shader.setUniform("cam_ang", camera.get_angle());
	m_shader.setUniform("i_time", i_time);
	m_shader.setUniform("screen_size", sf::Vector2f(m_width, m_height));
	m_render_texture.draw(sf::RectangleShape(sf::Vector2f(m_width, m_height)), &m_shader);
	m_render_texture.display();

	this->clear();
	this->draw(m_screen_sprite);

	m_fps_text.setString("fps: " + std::to_string(1 + (int)(1.f / delta_time)) + " (hz)");
	this->draw(m_fps_text);

	sf::Vertex cross[] = {
	    sf::Vertex(sf::Vector2f(this->getSize().x / 2, this->getSize().y / 2 - 8), sf::Color(255, 0, 255)),
	    sf::Vertex(sf::Vector2f(this->getSize().x / 2, this->getSize().y / 2 + 8), sf::Color(255, 0, 255)),
	    sf::Vertex(sf::Vector2f(this->getSize().x / 2 - 8, this->getSize().y / 2), sf::Color(255, 0, 255)),
	    sf::Vertex(sf::Vector2f(this->getSize().x / 2 + 8, this->getSize().y / 2), sf::Color(255, 0, 255))
	};
	this->draw(cross, 4, sf::Lines);

	this->display();
    }
};

#endif
