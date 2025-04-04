#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

class Camera {
private:
    sf::Vector3f m_position;
    sf::Vector3f m_angle;

    float m_speed;
    float m_mouseSensitivity;

    sf::Vector2f m_centerPosition;
    bool m_mouseLocked;

public:
    sf::Vector3f get_position() const { return m_position; }
    sf::Vector3f get_angle() const { return m_angle; }

public:
    Camera()
        : m_position(0, 0, -15), m_angle(0, 0, 0), m_speed(8.f), m_mouseSensitivity(0.2f), m_mouseLocked(false) {}
    ~Camera() = default;

    void lock_mouse(sf::RenderWindow& window) {
        m_centerPosition = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
        window.setMouseCursorVisible(false);
        sf::Mouse::setPosition(window.mapCoordsToPixel({ m_centerPosition.x, m_centerPosition.y }), window);
        m_mouseLocked = true;
    }

    void move(const float& dt) {
        float fixed_speed = m_speed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
            fixed_speed *= 10;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            m_position.x += std::sin(m_angle.y) * fixed_speed;
            m_position.z += std::cos(m_angle.y) * fixed_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            m_position.x -= std::sin(m_angle.y) * fixed_speed;
            m_position.z -= std::cos(m_angle.y) * fixed_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            m_position.x -= std::sin(m_angle.y + 3.14159f / 2) * fixed_speed;
            m_position.z -= std::cos(m_angle.y + 3.14159f / 2) * fixed_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_position.x += std::sin(m_angle.y + 3.14159f / 2) * fixed_speed;
            m_position.z += std::cos(m_angle.y + 3.14159f / 2) * fixed_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            m_position.y += fixed_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            m_position.y -= fixed_speed;
        }
    }

    void handle_mouse_movement(sf::RenderWindow& window) {
        if (!m_mouseLocked) {
            lock_mouse(window);
            return;
        }

        sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
        sf::Vector2f mouseDelta = mousePosition - m_centerPosition;

        m_angle.y += mouseDelta.x * m_mouseSensitivity * 0.0174533f;
        m_angle.x -= mouseDelta.y * m_mouseSensitivity * 0.0174533f;

        const float maxPitch = 89.0f * 0.0174533f;
        if (m_angle.x > maxPitch) m_angle.x = maxPitch;
        if (m_angle.x < -maxPitch) m_angle.x = -maxPitch;

        sf::Mouse::setPosition(window.mapCoordsToPixel({ m_centerPosition.x, m_centerPosition.y }), window);
    }
};

#endif
