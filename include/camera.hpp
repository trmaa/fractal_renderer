#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <cmath>
//#include <iostream>
//#include <string>

class Camera {
private:
    sf::Vector3f m_position;
    sf::Vector2f m_angle;

    float m_aceleration;
    float m_max_speed = 1.f;
    glm::vec3 m_vector_speed;
    float m_mouse_sensitivity;

    sf::Vector2f m_center_position;
    bool m_mouse_locked;

public:
    sf::Vector3f get_position() const { return m_position; }
    sf::Vector2f get_angle() const { return m_angle; }

public:
    Camera()
        : m_position(2.961881, -4.429515, -0.327023), m_angle(M_PI/4, 0), m_aceleration(0.01f), m_mouse_sensitivity(0.2f), m_mouse_locked(false) {}
    ~Camera() = default;

    void lock_mouse(sf::RenderWindow& window) {
        m_center_position = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
        window.setMouseCursorVisible(false);
        sf::Mouse::setPosition(window.mapCoordsToPixel({ m_center_position.x, m_center_position.y }), window);
        m_mouse_locked = true;
    }

    
    void move(const float& delta_time) {
        float fixed_aceleration = m_aceleration * delta_time;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
            fixed_aceleration *= 10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
            fixed_aceleration *= 100;
        }

        glm::vec3 forward {
            std::cos(m_angle.y) * std::sin(m_angle.x),
            std::sin(m_angle.y),
            std::cos(m_angle.y) * std::cos(m_angle.x)
        };

        glm::vec3 right {
            std::sin(m_angle.x - 3.14159f / 2.0f),
            0,
            std::cos(m_angle.x - 3.14159f / 2.0f)
        };

        glm::vec3 up = glm::cross(right, forward);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            m_vector_speed += forward * fixed_aceleration;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            m_vector_speed -= forward * fixed_aceleration;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_vector_speed += right * fixed_aceleration;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            m_vector_speed -= right * fixed_aceleration;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            m_vector_speed += up * fixed_aceleration;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            m_vector_speed -= up * fixed_aceleration;
        }

        m_position += sf::Vector3f(m_vector_speed.x, m_vector_speed.y, m_vector_speed.z) * delta_time;

        m_vector_speed *= 0.97f;
        if (glm::length(m_vector_speed) > m_max_speed) {
            m_vector_speed = glm::normalize(m_vector_speed) * m_max_speed; 
        }
    }

    void handle_mouse_movement(sf::RenderWindow& window) {
        if (!m_mouse_locked) {
            lock_mouse(window);
            return;
        }

        sf::Vector2f mouse_position = (sf::Vector2f)sf::Mouse::getPosition(window);
        sf::Vector2f mouse_delta = mouse_position - m_center_position;

        m_angle.x -= mouse_delta.x * m_mouse_sensitivity * 0.0174533f;
        m_angle.y -= mouse_delta.y * m_mouse_sensitivity * 0.0174533f;

        const float max_pitch = 89.0f * 0.0174533f;
        if (m_angle.y > max_pitch) m_angle.y = max_pitch;
        if (m_angle.y < -max_pitch) m_angle.y = -max_pitch;

        sf::Mouse::setPosition(window.mapCoordsToPixel({ m_center_position.x, m_center_position.y }), window);
    }
};

#endif
