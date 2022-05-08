/*******************************************************
*   Timber
*   Author: Eric Muuo
*   Email:  hearterick57@gmail.com
*
*
*   Emulates the clasic lumberjack game
*******************************************************/

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <sstream>

// function prototypes
void updateBranches(int);

const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];

// where is the player and branch left or right?
enum class side {LEFT,RIGHT,NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
    // create a video mode object
    sf::VideoMode vm(1920,1080);

    // create and open a window for the game
    sf::RenderWindow window(vm,"Timber",sf::Style::Fullscreen);

    // Texture to hold graphic onto the GPU
    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");

    // sprite to draw into the screen
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);

    // set the background to cover the screen
    spriteBackground.setPosition(0,0);

    // prepare the tree
    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    sf::Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810,0);

    //prepare the bee
    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0,800);

    bool beeActive = false; // is the bee moving?
    float beeSpeed = 0.0f; 

    // prepare the clouds
    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    // cloud sprites from a single texture
    sf::Sprite spriteCloud1, spriteCloud2, spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    // position the clouds at different heights
    spriteCloud1.setPosition(0,0);
    spriteCloud2.setPosition(0,250);
    spriteCloud3.setPosition(0,500);

    // are the clouds currently on the screen
    bool cloud1Active, cloud2Active, cloud3Acitve;

    // how fast is each cloud
    float cloud1speed = 0.0f;
    float cloud2speed = 0.0f;
    float cloud3speed = 0.0f;

    // clock to get the frame rate
    sf::Clock clock;

    // track pause
    bool paused = true;

    // draw some text
    int score = 0;
    
    sf::Text scoreText, messageText;

    // font
    sf::Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    
    messageText.setFont(font);
    scoreText.setFont(font);

    // Assign actual message
    messageText.setString("Hit <Return> to begin");
    scoreText.setString("Score = 0");

    // make the text big enough
    messageText.setCharacterSize(45);
    messageText.setCharacterSize(50);

    // set color
    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    // position the text
    sf::FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(
        textRect.width/2.0f,
        textRect.top +
        textRect.height / 2.0f
    );

    messageText.setPosition(1920/2.0f,1080/2.0f);
    scoreText.setPosition(20,20);

    // timebar
    sf::RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 20;
    timeBar.setSize(sf::Vector2f(timeBarStartWidth,timeBarHeight));
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition((1920.0/2) - timeBarStartWidth/2, 1060);

    sf::Time gameTimeTotal;
    float timeRemaining = 10.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    // prepare the branches
    sf::Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    // set the texture for each branch
    for(std::size_t i = 0; i<NUM_BRANCHES; ++i)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000,-2000);
        branches[i].setOrigin(220,20);
    }

    // prepare the player
    sf::Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    sf::Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580,720);

    // player starts on the left
    side playerSide = side::LEFT;

    //prepare the gravestone
    sf::Texture textureRip;
    textureRip.loadFromFile("graphics/rip.png");
    sf::Sprite spriteRip;
    spriteRip.setTexture(textureRip);
    spriteRip.setPosition(600,860);

    // prepare the axe
    sf::Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    sf::Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700,830);

    // line th axe with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;


    // prepare the flying log
    sf::Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    sf::Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810,720);

    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // handling player input
    bool acceptInput = false;


    while (window.isOpen()) // game main loop
    {   
        /************************************************
        *   Handle the player input
        ************************************************/

        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::KeyReleased && !paused)
            {
                acceptInput=true;

                // hide the axe
                spriteAxe.setPosition(2000,
                spriteAxe.getPosition().y);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close(); // close the window
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            paused = false;
            score = 0;
            timeRemaining = 10;

            // make all the branches disappear
            // start in the second position
            for(std::size_t i =0;i<NUM_BRANCHES;i++)
                branchPositions[i] = side::NONE;
            
            // hide the grave stone
            spriteRip.setPosition(2000,2000);

            // move the player into position
            spritePlayer.setPosition(580,720);

            acceptInput = true;
        }

        if(acceptInput)
        {

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;

                timeRemaining += (2.0f /score) + 0.15f;

                spriteAxe.setPosition(AXE_POSITION_RIGHT,
                spriteAxe.getPosition().y);

                spritePlayer.setPosition(1200,720);

                // update the branches
                updateBranches(score);

                // set the log to fly
                spriteLog.setPosition(810,720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

            }

            // handle the left arrow key

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;

                timeRemaining += (2.0f /score) + 0.15f;

                spriteAxe.setPosition(AXE_POSITION_LEFT,
                spriteAxe.getPosition().y);

                spritePlayer.setPosition(580,720);

                // update the branches
                updateBranches(score);

                // set the log to fly
                spriteLog.setPosition(810,720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

            }

        }

        /***********************************************
        *   Update the game scenes
        ***********************************************/
        if (!paused)
        {
            sf::Time dt = clock.restart();

            timeRemaining -= dt.asSeconds();

            timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * 
            timeRemaining,timeBarHeight));

            if(timeRemaining <= 0.0f)
            {
                // pause the game
                paused = true;

                messageText.setString("Game over!");
                
            }

            // setup the bee
            if(!beeActive)
            {
                // how fast is the bee
                srand((int)time(NULL));
                beeSpeed = (rand() % 200) + 200;
                // how high is the bee
                srand((int)time(NULL) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000,height);
                beeActive = true;
            }
            else 
            {
                // move the bee
                spriteBee.setPosition(
                    spriteBee.getPosition().x - 
                    (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y
                );

                // has the bee reached the end of the screen
                if(spriteBee.getPosition().x < -100)
                {
                    beeActive = false;
                }
            }

            // managing the clouds
            // cloud 1
            if(!cloud1Active)
            {
                // how fast is the cloud moving
                srand((int)time(NULL) * 10);
                cloud1speed = rand() % 200;

                // how high is the cloud
                srand((int)time(NULL) * 10);
                float height = (rand()%150);
                spriteCloud1.setPosition(-200,height);
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x +
                    (cloud1speed * dt.asSeconds()),
                    spriteCloud1.getPosition().y
                );

                if(spriteCloud1.getPosition().x > 1920)
                {
                    //  set it up for the next frame
                    cloud1Active = false;

                }
            }

            // cloud2
            if(!cloud2Active)
            {
                srand((int)time(NULL) * 20);
                cloud2speed = (rand() % 200);

                // how high is the cloud
                srand((int)time(NULL) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200,height);
                cloud2Active = true;
            }
            else  
            {
                // move  the cloud
                spriteCloud2.setPosition(
                    spriteCloud2.getPosition().x +
                    (cloud2speed * dt.asSeconds()),
                    spriteCloud2.getPosition().y
                );

                if(spriteCloud2.getPosition().x > 1920)
                    cloud2Active = false;
            }

            // cloud 3
            if(!cloud3Acitve)
            {
                srand((int)time(NULL)*30);
                cloud3speed = rand() % 200;

                srand((int)time(NULL) * 30);
                float height = (rand()% 450 )-150;
                spriteCloud3.setPosition(-200,height);
                cloud3Acitve = true;
            }
            else 
            {
                spriteCloud3.setPosition(
                    spriteCloud3.getPosition().x +
                    (cloud3speed * dt.asSeconds()),
                    spriteCloud3.getPosition().y
                );

                if(spriteCloud3.getPosition().x > 1920)
                {
                    cloud3Acitve = false;
                }
            }
            // update the score 
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            // update the branch sprites
            for(std::size_t i =0;i<NUM_BRANCHES;++i)
            {
                float height = i * 150;

                if(branchPositions[i] == side::LEFT)
                {
                    // move the sprite to the left side
                    branches[i].setPosition(610,height);

                    // flip the sprite round the other way
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT) 
                {
                    // move the sprite to the right side
                    branches[i].setPosition(1330,height);

                    // dont rotate the sprite
                    branches[i].setRotation(0);
                }
                else  
                {
                    // hide the branch
                    branches[i].setPosition(3000,height);
                }
                
            }

            // handle a flying log
            if(logActive)
            {
                spriteLog.setPosition(
                    spriteLog.getPosition().x +
                    (logSpeedX * dt.asSeconds()),
                    
                    spriteLog.getPosition().y + 
                    (logSpeedY * dt.asSeconds())
                );

                // hide the log when it reaches the end
                if(spriteLog.getPosition().x < -100 ||
                    spriteLog.getPosition().x > 2000)
                    {
                        logActive = false;
                        spriteLog.setPosition(810,720);
                    }
            }
            
        }


        /***********************************************
        *   Draw the game scenes
        ***********************************************/

        // clear everything from the last frame
        window.clear();

        // draw thw game scenes
        window.draw(spriteBackground);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteTree);
        window.draw(spriteBee);
        window.draw(scoreText);
        window.draw(timeBar);

        // draw the branches
        for(int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRip);

        // if paused draw the pause helper
        if(paused)
            window.draw(messageText);

        // show everything we drew
        window.display();
    }
    return 0;    
}

// function definition for moving branches
void updateBranches(int seed)
{
    // move the branches down by one place
    for(std::size_t j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    // spawn a new branch at position 0
    // left or right or None
    srand((int)(time(NULL))+seed);
    int r = (rand() % 5);

    switch(r)
    {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
        default:
            branchPositions[0] = side::NONE;
            break;
    }
}
