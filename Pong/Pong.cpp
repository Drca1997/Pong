// Pong.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>

#define INITIAL_BALL_SPEED 0.12f

using namespace sf; 
using namespace std;

enum Goal
{
    RedGoal,
    BlueGoal,
    NotGoal
};

void moveGameObjects(vector<RectangleShape> & paddles, const vector<Vector2f> & speeds, CircleShape & ball)
{
    for (size_t i=0; i < paddles.size(); i++)
    {
        paddles[i].setPosition(paddles[i].getPosition() + speeds[i]);
    }
    ball.setPosition(ball.getPosition() + speeds[speeds.size() - 1]);
}

void render(RenderWindow & window, const vector<RectangleShape> & paddles, 
    const CircleShape & ball, const Text & scoreboard)
{
    for (auto & paddle : paddles)
    {
        window.draw(paddle);
    }
    window.draw(ball);
    window.draw(scoreboard);
}

void loadFont(Font& font, const string filename)
{
    if (!font.loadFromFile(filename))
    {
        cerr << "Could not load font\n";
        exit(-1);
    }
}

void createPaddle(vector<RectangleShape> & paddles, const Vector2f size, const Vector2f initialPosition, Color color)
{
    RectangleShape paddle(size);
    paddle.setPosition(initialPosition);
    paddle.setFillColor(color);
    paddles.push_back(paddle);
}

int goalCheck(const FloatRect & ballRect, const int windowWidth)
{
    if (ballRect.left <= 0.0f) // blue goal
    {
        return BlueGoal;
    }
    else if (ballRect.left >= windowWidth) //red goal
    {
        return RedGoal;
    }
    return NotGoal;
}

void ballRebound(FloatRect & ballRect, vector<RectangleShape> & paddles, 
    Vector2f & ballSpeed, const float windowHeight)
{
    if (ballRect.top <= 0.0f || ballRect.top >= windowHeight) //Rebound of Wall
    {
        ballSpeed.y *= -1.0f;
    }
    for (auto& paddle : paddles)
    {
        if (paddle.getGlobalBounds().intersects(ballRect))
        {
            ballSpeed.x *= -1.12f;
            
        }
        
    }
}

void ResetBall(CircleShape & ball, const float windowWidth, const float windowHeight, 
    Vector2f & ballSpeed)
{
    ball.setPosition({windowWidth / 2, windowHeight / 2});
    if (ballSpeed.x > 0)
    {
        ballSpeed.x = -INITIAL_BALL_SPEED;
    }
    else if (ballSpeed.x < 0)
    {
        ballSpeed.x = INITIAL_BALL_SPEED;
    }
   
    ballSpeed.y = INITIAL_BALL_SPEED;
}

void ballLogic(CircleShape & ball, vector<RectangleShape>& paddles, Vector2f & ballSpeed, 
    vector<int> & score, int windowWidth, int windowHeight)
{
    FloatRect ballRect = ball.getGlobalBounds();
    if (goalCheck(ballRect, windowWidth) == RedGoal)
    {
        cout << "Red Goal!\n";
        ResetBall(ball, windowWidth, windowHeight, ballSpeed);
        score[0] += 1;
    }
    else if (goalCheck(ballRect, windowWidth) == BlueGoal)
    {
        cout << "Blue Goal!\n";
        ResetBall(ball, windowWidth, windowHeight, ballSpeed);
        score[1] += 1;
    }
    ballRebound(ballRect, paddles, ballSpeed, windowHeight);
}

void updateScoreboard(const vector<int> & score, Text &  scoreboard)
{
    string newScore = to_string(score[0]) + " - " + to_string(score[1]);
    scoreboard.setString(newScore);
}

int main()
{
    const Color backgroundColor = { 58, 181, 74 };
    //window settings and creation
    const int windowWidth = 1000;
    const int windowHeight = 600;
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Pong Game");
    
    //paddle settings
    const float paddleWidth = 20;
    const float paddleHeight = 75;
    const Vector2f paddleSize = { paddleWidth, paddleHeight };
    const Vector2f initialRedPos = { 10 + paddleWidth / 2, windowHeight / 2 };
    const Vector2f initialBluePos = { windowWidth - 10 - paddleWidth * 1.5, windowHeight / 2 };
    
    //velocities init
    vector<Vector2f> speeds;
    speeds.push_back({ 0.0f, 0.0f });
    speeds.push_back({ 0.0f, 0.0f });
    speeds.push_back({-INITIAL_BALL_SPEED, INITIAL_BALL_SPEED});

    //paddles creation
    vector<RectangleShape> paddles;
    createPaddle(paddles, paddleSize, initialRedPos, Color::Red);
    createPaddle(paddles, paddleSize, initialBluePos, Color::Blue);
    
    //ball settings and creation
    const float ballRadius = 12;
    CircleShape ball(ballRadius);
    ball.setPosition(windowWidth / 2, windowHeight / 2);
    ball.setFillColor(Color::Yellow);

    //scoreboard settings and creation
    const int scoreboardSize = 42;
    Font scoreboardFont;
    loadFont(scoreboardFont, "comic_sans.ttf");
    vector<int> score;
    score.push_back(0);
    score.push_back(0);

    Text scoreboard("0 - 0", scoreboardFont, scoreboardSize);
    scoreboard.setFillColor(Color::White);
    scoreboard.setPosition({windowWidth / 2, 0});
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::KeyPressed)
            {     
                switch (event.key.code)
                {
                    case Keyboard::W:
                        speeds[0].y = -0.1f;
                        break;
                    case Keyboard::S:
                        speeds[0].y = 0.1f;
                        break;
                    case Keyboard::Down:
                        speeds[1].y = 0.1f;
                        break;
                    case Keyboard::Up:
                        speeds[1].y = -0.1f;
                        break;
                }
            }
            else if (event.type == Event::KeyReleased)
            {
                switch (event.key.code)
                {
                case Keyboard::W:
                    speeds[0].y = 0.0f;
                    break;
                case Keyboard::S:
                    speeds[0].y = 0.0f;
                    break;
                case Keyboard::Down:
                    speeds[1].y = 0.0f;
                    break;
                case Keyboard::Up:
                    speeds[1].y = 0.0f;
                    break;
                }
            }
        }

        moveGameObjects(paddles, speeds, ball);
        ballLogic(ball, paddles, speeds[speeds.size() - 1], score, windowWidth, windowHeight);
        updateScoreboard(score, scoreboard);
        window.clear(backgroundColor);
        render(window, paddles, ball, scoreboard);
        window.display();
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
