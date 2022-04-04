/*
This project uses the SFML multi-media library
*/
#include "defines.h"
#include <stdio.h>
#include <string>

global_variable sf::RenderWindow Window;
global_variable sf::RectangleShape Player;
global_variable sf::Vector2f Direction;
global_variable int Speed = 10;
global_variable std::string Room = "Main";
global_variable sf::RectangleShape Doors[3];
global_variable bool HasKey;
global_variable bool JustPickedUpKey;
global_variable bool InsertedKey;
global_variable bool JustInsertedKey;
global_variable sf::Font ArialFont;
global_variable bool HasGold;

struct
{
    sf::RectangleShape ExitDoor;
    sf::RectangleShape Key;
} WestRoom;

struct
{
    sf::RectangleShape ExitDoor;
    sf::RectangleShape Lock;
} NorthRoom;

struct
{
    sf::RectangleShape ExitDoor;
    sf::RectangleShape Barrier;
    sf::RectangleShape GoldPot;
    sf::RectangleShape Gold;
} EastRoom;

bool
CheckCollision(sf::RectangleShape First,
               sf::RectangleShape Second);
void
CenterObject(sf::RectangleShape *Object);

int
main(void)
{
    if(!ArialFont.loadFromFile("../resources/arial.ttf"))
    {
        printf("Failed to load arial.ttf");
        return(1);
    }
    sf::Text MessageText;
    MessageText.setFont(ArialFont);
    MessageText.setCharacterSize(35);
    MessageText.setFillColor(sf::Color::Blue);
    
    Player.setSize(sf::Vector2f(50, 50));
    
    // NOTE: Create the window in fullscreen mode,
    // passing in a dummy video mode
    // Make the rendering sync with the graphics card 
    // and enforce a max of 60fps to not burn out the CPU
    Window.create(sf::VideoMode(500, 500), "Untitled", sf::Style::Fullscreen);
    Window.setVerticalSyncEnabled(true);
    Window.setFramerateLimit(60);
    
    WestRoom.ExitDoor.setSize(sf::Vector2f(40, 375));
    WestRoom.ExitDoor.setPosition(Window.getSize().x - (WestRoom.ExitDoor.getGlobalBounds().width),
                                  (Window.getSize().y / 2) - (WestRoom.ExitDoor.getGlobalBounds().height / 2));
    WestRoom.ExitDoor.setFillColor(sf::Color(0x9c, 0x53, 0x06, 255));
    WestRoom.Key.setFillColor(sf::Color(0xF5, 0xEF, 0x42, 255));
    WestRoom.Key.setSize(sf::Vector2f(20, 20));
    WestRoom.Key.setPosition(40,
                             (Window.getSize().y / 2) - (WestRoom.Key.getSize().y / 2));
    
    NorthRoom.ExitDoor.setSize(sf::Vector2f(40, 375));
    NorthRoom.ExitDoor.setPosition(Window.getSize().x - (NorthRoom.ExitDoor.getGlobalBounds().width),
                                   (Window.getSize().y / 2) - (NorthRoom.ExitDoor.getGlobalBounds().height / 2));
    NorthRoom.ExitDoor.setFillColor(sf::Color(0x9C, 0x53, 0x06, 255));
    NorthRoom.Lock.setFillColor(sf::Color(0x69, 0x69, 0x69, 255));
    NorthRoom.Lock.setSize(sf::Vector2f(60, 60));
    NorthRoom.Lock.setPosition((Window.getSize().x / 2) - (NorthRoom.Lock.getSize().x / 2),
                               100);
    
    EastRoom.ExitDoor.setSize(sf::Vector2f(40, 375));
    EastRoom.ExitDoor.setPosition(0,
                                  (Window.getSize().y / 2) - (EastRoom.ExitDoor.getSize().y / 2));
    EastRoom.ExitDoor.setFillColor(sf::Color(0x9C, 0x53, 0x06, 255));
    EastRoom.Barrier.setSize(sf::Vector2f(60, Window.getSize().y));
    EastRoom.Barrier.setPosition(1250, 0);
    EastRoom.Barrier.setFillColor(sf::Color(0x69, 0x69, 0x69, 255));
    EastRoom.GoldPot.setSize(sf::Vector2f(50, 50));
    EastRoom.GoldPot.setPosition(1550,
                                 (Window.getSize().y / 2) - (EastRoom.GoldPot.getSize().y / 2));
    EastRoom.GoldPot.setFillColor(sf::Color(0x80, 0x80, 0x80, 255));
    EastRoom.Gold.setSize(sf::Vector2f(35, 35));
    EastRoom.Gold.setPosition((EastRoom.GoldPot.getPosition().x + (EastRoom.GoldPot.getSize().x / 2)) - (EastRoom.Gold.getSize().x / 2),
                              (EastRoom.GoldPot.getPosition().y + (EastRoom.GoldPot.getSize().y / 2)) - (EastRoom.Gold.getSize().y / 2));
    EastRoom.Gold.setFillColor(sf::Color(0xF5, 0xEF, 0x42, 255));
    
    // NOTE: Setup the main room
    for(int DoorIndex = 0;
        DoorIndex < 3;
        ++DoorIndex)
    {
        sf::RectangleShape *Door = &Doors[DoorIndex];
        
        if(DoorIndex < 2)
        {
            Door->setSize(sf::Vector2f(40, 375));
        }
        else
        {
            Door->setSize(sf::Vector2f(500, 40));
        }
        
        // #9c5306
        Door->setFillColor(sf::Color(0x9C, 0x53, 0x06, 255));
    }
    Doors[0].setPosition(0,
                         (Window.getSize().y / 2) - (Doors[0].getGlobalBounds().height / 2));
    Doors[1].setPosition(Window.getSize().x - (Doors[1].getGlobalBounds().width),
                         (Window.getSize().y / 2) - (Doors[1].getGlobalBounds().height / 2));
    Doors[2].setPosition((Window.getSize().x / 2) - (Doors[2].getGlobalBounds().width / 2),
                         0);
    
    // NOTE: Keep the application running while
    // the window is open
    sf::Clock Clock;
    float LastTime = 0;
    
    CenterObject(&Player);
    while(Window.isOpen())
    {
        // NOTE: When the user clicks the X button,
        // close the window and stop the program
        sf::Event Event;
        while(Window.pollEvent(Event))
        {
            switch(Event.type)
            {
                case sf::Event::Closed:
                {
                    Window.close();
                } break;
                
                // NOTE: Move our player when the user
                // presses the movement keys
                case sf::Event::KeyPressed:
                {
                    switch(Event.key.code)
                    {
                        case sf::Keyboard::Escape:
                        {
                            Window.close();
                        } break;
                        
                        case sf::Keyboard::A:
                        case sf::Keyboard::Left:
                        {
                            Direction.x = -1;
                        } break;
                        
                        case sf::Keyboard::D:
                        case sf::Keyboard::Right:
                        {
                            Direction.x = 1;
                        } break;
                        
                        case sf::Keyboard::W:
                        case sf::Keyboard::Up:
                        {
                            Direction.y = -1;
                        } break;
                        
                        case sf::Keyboard::S:
                        case sf::Keyboard::Down:
                        {
                            Direction.y = 1;
                        } break;
                        
                        default:
                        {} break;
                    }
                } break;
                
                case sf::Event::KeyReleased:
                {
                    switch(Event.key.code)
                    {
                        case sf::Keyboard::A:
                        case sf::Keyboard::D:
                        case sf::Keyboard::Left:
                        case sf::Keyboard::Right:
                        {
                            Direction.x = 0;
                        } break;
                        
                        case sf::Keyboard::W:
                        case sf::Keyboard::S:
                        case sf::Keyboard::Up:
                        case sf::Keyboard::Down:
                        {
                            Direction.y = 0;
                        } break;
                        
                        default:
                        {} break;
                    }
                } break;
                
                default:
                {} break;
            }
        }
        
        Window.clear(sf::Color::Black);
        
        // ---Update Code---
        sf::Vector2f PlayerMovement = Vec2MultInt(Vec2Normalize(Direction), Speed);
        sf::Vector2f NewPlayerPosition(PlayerMovement + Player.getPosition());
        Player.setFillColor(sf::Color(100, 250, 50));
        bool ChangedRooms = false;
        
        if(JustPickedUpKey || JustInsertedKey)
        {
        }
        else
        {
            MessageText.setString(HasGold ? "You Now Have 18 Doubloons" : "");
        }
        
        if(Room == "Main")
        {
            JustPickedUpKey = false;
            JustInsertedKey = false;
            for(int DoorIndex = 0;
                DoorIndex < 3;
                ++DoorIndex)
            {
                sf::RectangleShape *Door = &Doors[DoorIndex];
                Door->setSize(sf::Vector2f(Door->getGlobalBounds().width,
                                           Door->getGlobalBounds().height));
                Window.draw(*Door);
                
                sf::RectangleShape NewPlayer(Player);
                if(CheckCollision(Player, *Door))
                {
                    ChangedRooms = true;
                    CenterObject(&Player);
                    if(DoorIndex == 0)
                    {
                        Room = "West";
                    }
                    else if(DoorIndex == 2)
                    {
                        Room = "North";
                    }
                    else if(DoorIndex == 1)
                    {
                        Room = "East";
                    }
                }
            }
        }
        else if(Room == "West")
        {
            sf::RectangleShape NewPlayer(Player);
            NewPlayer.setPosition(NewPlayerPosition);
            if(CheckCollision(NewPlayer, WestRoom.ExitDoor))
            {
                ChangedRooms = true;
                Room = "Main";
                CenterObject(&Player);
            }
            
            if(!HasKey)
            {
                if(CheckCollision(NewPlayer, WestRoom.Key))
                {
                    JustPickedUpKey = true;
                    HasKey = true;
                    MessageText.setString("You Have Picked Up a Key");
                }
                Window.draw(WestRoom.Key);
            }
            
            Window.draw(WestRoom.ExitDoor);
        }
        else if(Room == "North")
        {
            sf::RectangleShape NewPlayer(Player);
            NewPlayer.setPosition(NewPlayerPosition);
            if(CheckCollision(NewPlayer, NorthRoom.ExitDoor))
            {
                ChangedRooms = true;
                Room = "Main";
                CenterObject(&Player);
            }
            
            if(!InsertedKey)
            {
                if(CheckCollision(NewPlayer, NorthRoom.Lock) && HasKey)
                {
                    JustInsertedKey = true;
                    InsertedKey = true;
                    HasKey = false;
                    MessageText.setString("You Have Inserted the Key");
                }
            }
            
            Window.draw(NorthRoom.ExitDoor);
            Window.draw(NorthRoom.Lock);
        }
        else if(Room == "East")
        {
            sf::RectangleShape NewPlayer(Player);
            NewPlayer.setPosition(NewPlayerPosition);
            if(CheckCollision(NewPlayer, EastRoom.ExitDoor))
            {
                ChangedRooms = true;
                Room = "Main";
                CenterObject(&Player);
            }
            
            if(InsertedKey)
            {
                if(CheckCollision(NewPlayer, EastRoom.GoldPot))
                {
                    HasGold = true;
                }
            }
            else
            {
                if(CheckCollision(NewPlayer, EastRoom.Barrier))
                {
                    NewPlayerPosition.x = Player.getPosition().x;
                }
                
                Window.draw(EastRoom.Barrier);
            }
            
            Window.draw(EastRoom.ExitDoor);
            Window.draw(EastRoom.GoldPot);
            
            if(!HasGold)
            {
                Window.draw(EastRoom.Gold);
            }
        }
        
        if(NewPlayerPosition.x < 0)
        {
            NewPlayerPosition.x = 0;
            NewPlayerPosition.y = NewPlayerPosition.y;
        }
        else if((NewPlayerPosition.x + Player.getSize().x) > Window.getSize().x)
        {
            NewPlayerPosition.x = Window.getSize().x - Player.getSize().x;
            NewPlayerPosition.y = NewPlayerPosition.y;
        }
        
        if(NewPlayerPosition.y < 0)
        {
            NewPlayerPosition.x = NewPlayerPosition.x;
            NewPlayerPosition.y = 0;
        }
        else if((NewPlayerPosition.y + Player.getSize().y) > Window.getSize().y)
        {
            NewPlayerPosition.x = NewPlayerPosition.x;
            NewPlayerPosition.y = Window.getSize().y - Player.getSize().y;
        }
        
        if(!ChangedRooms)
        {
            Player.setPosition(NewPlayerPosition);
        }
        
        Window.draw(Player);
        Window.draw(MessageText);
        // -----------------
        
        Window.display();
        
        float CurrentTime = Clock.getElapsedTime().asSeconds();
        float FPS = 1 / (CurrentTime - LastTime);
        printf("FPS: %f\n", FPS);
        LastTime = CurrentTime;
    }
    
    return(0);
}

bool
CheckCollision(sf::RectangleShape First,
               sf::RectangleShape Second)
{
    return(First.getGlobalBounds().intersects(Second.getGlobalBounds()));
}

void
CenterObject(sf::RectangleShape *Object)
{
    Object->setPosition((Window.getSize().x / 2) - (Object->getSize().x / 2),
                        (Window.getSize().y / 2) - (Player.getSize().y / 2));
}
