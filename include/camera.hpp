#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

class Camera {
private:
    sf::Vector3f m_position;
    sf::Vector2f m_angle;

    float m_speed;
    float m_mouse_sensitivity;

    sf::Vector2f m_center_position;
    bool m_mouse_locked;

public:
    sf::Vector3f get_position() const { return m_position; }
    sf::Vector2f get_angle() const { return m_angle; }

public:
    Camera()
        : m_position(0, 0, 0), m_angle(M_PI, 0), m_speed(0.1f), m_mouse_sensitivity(0.2f), m_mouse_locked(false) {}
    ~Camera() = default;

    void lock_mouse(sf::RenderWindow& window) {
        m_center_position = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
        window.setMouseCursorVisible(false);
        sf::Mouse::setPosition(window.mapCoordsToPixel({ m_center_position.x, m_center_position.y }), window);
        m_mouse_locked = true;
    }

    void move(const float& dt) {
        float fixed_speed = m_speed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
            fixed_speed *= 10;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            m_position.x += std::sin(m_angle.x) * fixed_speed;
            m_position.z += std::cos(m_angle.x) * fixed_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            m_position.x -= std::sin(m_angle.x) * fixed_speed;
            m_position.z -= std::cos(m_angle.x) * fixed_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_position.x -= std::sin(m_angle.x + 3.14159f / 2) * fixed_speed;
            m_position.z -= std::cos(m_angle.x + 3.14159f / 2) * fixed_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            m_position.x += std::sin(m_angle.x + 3.14159f / 2) * fixed_speed;
            m_position.z += std::cos(m_angle.x + 3.14159f / 2) * fixed_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            m_position.y += fixed_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            m_position.y -= fixed_speed;
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
