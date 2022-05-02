/*******************************************************
*   Timber
*   Author: Eric Muuo
*   Email:  hearterick57@gmail.com
*
*
*   Emulates the clasic timber game
*******************************************************/

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstddef>
#include <cstdlib>

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

    while (window.isOpen()) // game main loop
    {   
        /************************************************
        *   Handle the player input
        ************************************************/
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close(); // close the window
        }


        /***********************************************
        *   Update the game scenes
        ***********************************************/
        sf::Time dt = clock.restart();

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
        else {
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


        /***********************************************
        *   Draw the game scenes
        ***********************************************/

        // clear everything from the last frame
        window.clear();

        // draw thw game scenes
        window.draw(spriteBackground);
        window.draw(spriteTree);
        window.draw(spriteBee);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // show everything we drew
        window.display();
    }
    return 0;    
}