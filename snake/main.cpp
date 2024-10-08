#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include<iostream>
using namespace sf;

const int ROW = 17;
const int COL = 15;

int dx = 1, dy = 0;
bool running = true;
bool getApple = 0;

int score = 0;

struct Point
{
    int x,y;
    Point(int a, int b)
    {
        x = a; y = b;
    }
};
std::vector<Point> body;
Point newHead(-1,-1);
Point apl(-1,-1);

// function to check for collision
// return 1 for ok, 0 for not ok
bool check(int x, int y)
{
    for(int i = 0; i < body.size(); i++)
    {
        if(x == body[i].x && y == body[i].y) return 0;
        else if(x >= COL || y >= ROW) return 0;
        else if(x < 0 || y < 0) return 0;
    }
    return 1;
}

void turn(int keyid, int keyid2)
{
    if(keyid == 0) return;
    switch(keyid)
    {
        case Keyboard::Up:
            if(dy != 0) turn(keyid2,0);
            else{ dx = 0; dy = -1;}
            break;
        case Keyboard::Down:
            if(dy != 0) turn(keyid2,0);
            else{ dx = 0; dy = 1;}
            break;
        case Keyboard::Left:
            if(dx != 0) turn(keyid2,0);
            else{ dx = -1; dy = 0;}
            break;
        case Keyboard::Right:
            if(dx != 0) turn(keyid2,0);
            else{ dx = 1; dy = 0;}
            break;
        default:
            break;
    }
}

void move()
{
    newHead.x = body[0].x + dx;
    newHead.y = body[0].y + dy;

    if(!check(newHead.x,newHead.y)) { running = false; return;}

    if(newHead.x == apl.x && newHead.y == apl.y)
    {
        body.push_back(Point(-1,-1));
        score++;
        getApple = true;
    }

    if(body.size() > 1)
    {
        for(int i = body.size()-1; i >= 1;i--)
        {
            body[i].x = body[i-1].x;
            body[i].y = body[i-1].y;
        }
    }
    body[0].x = newHead.x;
    body[0].y = newHead.y;

}

int main()
{
    srand(time(0));

    Texture t1,t2,t3;
    t1.loadFromFile("images/green_circle.png");
    t2.loadFromFile("images/apple.png");
    t3.loadFromFile("images/green_circle_head.png");

    float scale = .07;
    float pixel = 278;

    // score
    Font font;
    font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");
    Text text;
    text.setFont(font);
    text.setFillColor(Color::White);
    text.setCharacterSize(20);
    text.setPosition(Vector2f((COL-1)*pixel*scale,0));
    //text.setPosition(Vector2f(0,0));
    

    Sprite snake(t1),apple(t2),snake_head(t3);
    snake.setScale(scale,scale);
    apple.setScale(scale,scale);
    snake_head.setScale(scale,scale);

    float length = snake.getTextureRect().width*scale;

    RenderWindow window(VideoMode(COL*pixel*scale,ROW*pixel*scale),"Snake!");

    float timer = 0, delay = 0.25;

    Clock clock;

    body.push_back(Point(3,10));
    apl.x = 1; apl.y = 1;

    int keyid = 0, keyid2 = 0;
    while(window.isOpen() && running)
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while(window.pollEvent(e))
        {
            if(e.type == Event::Closed)
                window.close();

            // key press
            if(e.type == Event::KeyPressed)
            {
                keyid2 = keyid;
                keyid = e.key.code;
            }
        }

        // tick
        if(timer > delay)
        {
            turn(keyid,keyid2);
            keyid = 0; keyid2 = 0;
            move();
            timer = 0;
            if(getApple)
            {
                do{
                    apl.x = rand() % COL;
                    apl.y = rand() % ROW;
                } while(!check(apl.x, apl.y));
                getApple = false;
            }
        }
        
        window.clear(Color::Black);
        //// draw ////

        //grid
        int c = 70;
        VertexArray gridR(Lines,ROW*2+2);
        for(int i = 0;i < ROW*2+2;i+=2)
        {
            gridR[i].position = Vector2f(0,i*pixel*scale/2);
            gridR[i+1].position = Vector2f(window.getSize().x,i*pixel*scale/2);
            gridR[i].color = Color(c,c,c);
            gridR[i+1].color = Color(c,c,c);
        }
        VertexArray gridC(Lines,COL*2+2);
        for(int i = 0;i < COL*2+2;i+=2)
        {
            gridC[i].position = Vector2f(i*pixel*scale/2,0);
            gridC[i+1].position = Vector2f(i*pixel*scale/2,window.getSize().y);
            gridC[i].color = Color(c,c,c);
            gridC[i+1].color = Color(c,c,c);
        }
        
        window.draw(gridR);
        window.draw(gridC);

        //snake head
        snake_head.setPosition(body[0].x*pixel*scale, body[0].y*pixel*scale);
        float angle;
        if(dy == -1) angle = 0;
        if(dx == 1) angle = 90;
        if(dy == 1) angle = 180;
        if(dx == -1) angle = 270;
        Transform tr;
        float centerX = body[0].x*pixel*scale + length/2;
        float centerY = body[0].y*pixel*scale + length/2;
        tr.rotate(angle,centerX,centerY);
        window.draw(snake_head,tr);

        //snake body
        for(int i = 1;i < body.size();i++)
        {
            snake.setPosition(body[i].x*pixel*scale, body[i].y*pixel*scale);
            window.draw(snake);
        }

        //apple
        apple.setPosition(apl.x*pixel*scale,apl.y*pixel*scale);
        window.draw(apple);

        //score
        text.setString(std::to_string(score));
        window.draw(text);
        
        window.display();

    } // end window.isOpen
    return 0;
} // end main