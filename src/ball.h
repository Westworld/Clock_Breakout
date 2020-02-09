#ifndef BALL_HPP_
#define BALL_HPP_

#include "screen.h"


class Ball
{
private:
    float move_x, move_y;
    int16_t pos_x, pos_y;
    int16_t max_x, max_y;
    int16_t speed = 1;
    const int16_t radius=8;
    uint16_t color = ILI9486_YELLOW;

    Screen * tft;


public:
    Ball(Screen * mastertft); //, float speed, float angle);
    void draw(void);
    void undraw(void);
    void move(int16_t x, int16_t y);
    void move_draw(int16_t x, int16_t y);
    void move_draw(void);
    void setAngle(float angle);
    float getAngle();

    /*
    bool checkColission(const Block & block);
    bool checkColission(const Paddle & paddle);
    bool exist() { return bottom() < GlobalObjects::windowHeight; }

    void Update(float deltaTime);

    void Draw(sf::RenderWindow & window);

    float left()   const { return circle.getPosition().x - circle.getRadius(); }
    float rigth()  const { return circle.getPosition().x + circle.getRadius(); }
    float top()    const { return circle.getPosition().y - circle.getRadius(); }
    float bottom() const { return circle.getPosition().y + circle.getRadius(); }

    float getX() const { return circle.getPosition().x; }
    float getY() const { return circle.getPosition().y; }
    sf::Vector2f getPosition() const { return circle.getPosition(); }
    float getRadius() const { return circle.getRadius(); }
    sf::Vector2f getVelocity() const {return velocity; }
    */
};

#endif // BALL_HPP_