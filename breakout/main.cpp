#include <SFML/Graphics.hpp>
using namespace sf;

// bars
const int w = 30, h = 10, M = 5, N = 7;
Rect<float> bars[M][N];

// player
Rect<float> player(320/2 - 40/2,480/2+100,40,10);
float dx = 0;

// ball
Rect<float> ball(250,player.top-150,15,15);
float vx = -1, vy = 1, x_prev, y_prev;

//timer
float timer = 0, delay = 0.01;
float rollTime = 0, init_delay = 0.01;

bool running = true;
int score = 0;


void reflect(Rect<float> r)
{
    if(x_prev + ball.width < r.left || x_prev > r.left + r.width) // from left or right
        vx = -vx;
    else
        vy = -vy;
}

void player_reflect(Rect<float> r)
{
    if(x_prev + ball.width < r.left || x_prev > r.left + r.width) // from left or right
        vx = -vx;
    else
        vy = -vy;
    
    if(vy > 0) vy = -vy;
}


int main()
{
    RenderWindow window(VideoMode(320,400),"Breakout!");

    Clock clock;

    // setup bars
    for(int i = 0;i < M;i++)
    {
        for(int j=0;j < N;j++)
        {
            bars[i][j] = Rect<float>(20+j*(w+10),20+i*(h+10),w,h);
        }
    }

    while(window.isOpen() && running)
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        rollTime += time;

        delay = init_delay - rollTime*.0001;

        Event e;
        while(window.pollEvent(e))
        {
            if(e.type == Event::Closed)
                window.close();
            
            if(e.type == Event::KeyPressed){
                if(e.key.code == Keyboard::Left)
                    dx = -.07;
                else if(e.key.code == Keyboard::Right)
                    dx = .07;
            }else{
                dx = 0;
            }
                
        }

        // move player
        player.left += dx;
        if(player.left < 0) player.left = 0;
        else if(player.left + player.width > window.getSize().x) player.left = window.getSize().x - player.width;

        // move ball
        if(timer > delay)
        {
            x_prev = ball.left;
            y_prev = ball.top;
            ball.left += vx;
            ball.top += vy;

            //collison
            if(ball.top < 0)
            {
                ball.top = 0;
                vy = -vy;
            }
            if(ball.left < 0)
            {
                ball.left = 0;
                vx = -vx;
            }
            else if(ball.left + ball.width > window.getSize().x)
            {
                ball.left = window.getSize().x - ball.width;
                vx = -vx;
            }
            // game over
            if(ball.top > window.getSize().y)
                running = false;

            if(ball.intersects(player))
                player_reflect(player);
            
            for(int i = 0; i < M; i++)
            {
                for(Rect<float> &r : bars[i])
                {
                    if(ball.intersects(r))
                    {
                        reflect(r);
                        r.top = -50;
                        r.left = -50;
                        score++;
                        break;
                    }
                }
            }

            timer = 0;
        }
        if(score >= M*N) running = false;

        // draw
        window.clear(Color::Black);

        for(int i = 0;i < M;i++)
        {
            for(int j=0;j<N;j++)
            {
                RectangleShape r(Vector2f(w,h));
                r.setPosition(bars[i][j].left,bars[i][j].top);
                r.setFillColor(Color::White);
                window.draw(r);
            }
        }
        RectangleShape p(Vector2f(player.width,player.height));
        p.setPosition(player.left,player.top);
        p.setFillColor(Color::White);
        window.draw(p);


        CircleShape b(ball.width/2);
        b.setPosition(ball.left,ball.top);
        b.setFillColor(Color::White);
        window.draw(b);

        window.display();
    }

    if(score < M*N) return 0;
    
    // show "CLEAR!!!"
    timer = 0;
    delay = 1;

    Font font;
    font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");
    Text text;
    text.setFont(font);
    text.setString("CLEAR!!!");
    text.setFillColor(Color::White);
    text.setCharacterSize(40);
    text.setStyle(Text::Bold);
    text.setPosition(Vector2f(window.getSize().x/2 - text.getGlobalBounds().width/2,window.getSize().y/2 - 40));

    while(timer <= delay)
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        window.draw(text);
        window.display();
    }
    return 0;
} // end main