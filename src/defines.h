/* date = March 21st 2022 9:07 am */

/*
These are just basic defines and typedefs
for convenience
*/

#ifndef DEFINES_H
#define DEFINES_H

#include <SFML/Graphics.hpp>
#include <stdint.h>

#define internal static
#define global_variable static
#define local_persist static

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

sf::Vector2f
Vec2Normalize(sf::Vector2f Vector)
{
    float Length = sqrt((Vector.x * Vector.x) + (Vector.y * Vector.y));
    
    if(Length)
    {
        return(sf::Vector2f(Vector.x / Length, Vector.y / Length));
    }
    
    return(Vector);
}

sf::Vector2f
Vec2MultInt(sf::Vector2f Vector, int Integer)
{
    return(sf::Vector2f(Vector.x * Integer,
                        Vector.y * Integer));
}

#endif //DEFINES_H
