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
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

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


        /***********************************************
        *   Draw the game scenes
        ***********************************************/

        // clear everything from the last frame
        window.clear();

        // draw thw game scenes
        window.draw(spriteBackground);
        window.draw(spriteTree);
        window.draw(spriteBee);

        // show everything we drew
        window.display();
    }
    return 0;    
}