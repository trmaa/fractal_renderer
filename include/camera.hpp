#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

class Camera {
private:
	sf::Vector3f m_position;
	sf::Vector3f m_direction;

	float m_speed = 1.f;

	float m_mouse_sensitivity = 0.2f;
	bool m_mouse_locked = false;
	sf::Vector2f m_center_position;

public:
	sf::Vector3f get_position() const { return this->m_position; }
	sf::Vector3f get_direction() const { return this->m_direction; }

	Camera(): m_position(0, 0, 0), m_direction(0, 0, -1) {}

	void move(float delta_time) {
		sf::Vector2f dir_2d(m_direction.x, m_direction.z);
		float angle = std::atan2(dir_2d.y, dir_2d.x);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			this->m_position.x +=  std::cos(angle) * this->m_speed * delta_time;	
			this->m_position.z +=  std::sin(angle) * this->m_speed * delta_time;	
		}		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			this->m_position.x -=  std::cos(angle) * this->m_speed * delta_time;	
			this->m_position.z -=  std::sin(angle) * this->m_speed * delta_time;	
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			this->m_position.x +=  std::cos(angle+M_PI/2) * this->m_speed * delta_time;	
			this->m_position.z +=  std::sin(angle+M_PI/2) * this->m_speed * delta_time;	
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			this->m_position.x -=  std::cos(angle+M_PI/2) * this->m_speed * delta_time;	
			this->m_position.z -=  std::sin(angle+M_PI/2) * this->m_speed * delta_time;	
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            this->m_position.y += this->m_speed * delta_time;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            this->m_position.y -= this->m_speed*delta_time;
        }
	}

	void lock_mouse(sf::RenderWindow& window) {
		this->m_center_position = sf::Vector2f(window.getSize().x/2.f, window.getSize().y/2.f);
		window.setMouseCursorVisible(false);
		sf::Mouse::setPosition(
				window.mapCoordsToPixel(
				{ this->m_center_position.x, this->m_center_position.y }), window);
        this->m_mouse_locked = true;
	}

	void handle_mouse_movement(sf::RenderWindow& window) {
        if (!this->m_mouse_locked) {
            lock_mouse(window);
            return;
        }

		sf::Vector2f angle;
		angle.y = std::atan2(this->m_direction.z, this->m_direction.x);
		angle.x = std::atan2(this->m_direction.y, this->m_direction.x);

        sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
        sf::Vector2f mouseDelta = mousePosition - this->m_center_position;

        angle.y += mouseDelta.x * this->m_mouse_sensitivity * 0.0174533f;
        angle.x -= mouseDelta.y * this->m_mouse_sensitivity * 0.0174533f;

        const float maxPitch = 89.0f * 0.0174533f;
        if (angle.x > maxPitch) angle.x = maxPitch;
        if (angle.x < -maxPitch) angle.x = -maxPitch;

        sf::Mouse::setPosition(window.mapCoordsToPixel({ this->m_center_position.x, this->m_center_position.y }), window);
    }
};

#endif
