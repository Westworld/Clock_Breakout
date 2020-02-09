#ifndef BALL_HPP_
#define BALL_HPP_

#include "screen.h"


class Ball
{
private:
    float move_x, move_y;
    long pos_x, pos_y;
    long max_x, max_y;
    const long radius=4;
    long color = ILI9486_RED;

    Screen * tft;

    //void setAngle(float ang);
    //float getAngle();

public:
    Ball(Screen * mastertft); //, float speed, float angle);
    void draw(void);
    void undraw(void);
    void move(long x, long y);
    void move_draw(long x, long y);
    void move_draw(void);

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