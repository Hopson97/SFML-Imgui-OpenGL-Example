#include "Utility.h"
#include "Graphics/GLDebug.h"
#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>

static int isColourSame(Colour a, Colour b)
{
    return a.red == b.red && a.green == b.green && a.blue == b.blue;
}

static void setTerminalColour(Colour colour, enum ColourSetMode mode)
{
    static int isFirstTimeBG = 1;
    static int isFirstTimeFG = 1;
    static struct Colour currentTextColour;
    static struct Colour currentBackgroundColour;
    uint8_t r = colour.red;
    uint8_t g = colour.green;
    uint8_t b = colour.blue;
    switch (mode) {
        case COL_SET_BG:
            if (!isColourSame(currentBackgroundColour, colour) || isFirstTimeBG) {
                printf("\x1B[%d;2;%d;%d;%dm", mode, r, g, b);
                isFirstTimeBG = 1;
            }
            break;

        case COL_SET_FG:
            if (!isColourSame(currentTextColour, colour) || isFirstTimeFG) {
                printf("\x1B[%d;2;%d;%d;%dm", mode, r, g, b);
                isFirstTimeFG = 1;
            }
            break;
    }
}

char* getFileContent(const char* fileName)
{
    char* buffer = NULL;
    long length = 0;
    FILE* file = fopen(fileName, "r");

    if (file) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = (char*)calloc(length + 1, 1);
        if (buffer) {
            fread(buffer, 1, length, file);
        }
        fclose(file);
    }
    return buffer;
}

void setBackgroundColour(Colour colour)
{
    setTerminalColour(colour, COL_SET_BG);
}

void setTextColour(Colour colour)
{
    setTerminalColour(colour, COL_SET_FG);
}

void setTextColourRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    struct Colour colour = {
        red,
        green,
        blue,
    };
    setTerminalColour(colour, COL_SET_FG);
}

void setBackgroundColourRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    struct Colour colour = {
        red,
        green,
        blue,
    };
    setTerminalColour(colour, COL_SET_BG);
}

bool initWindow(sf::Window* window)
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.stencilBits = 8;
    contextSettings.antialiasingLevel = 4;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 5;
    contextSettings.attributeFlags = sf::ContextSettings::Core;
    window->create({1600, 900}, "yare", sf::Style::Close, contextSettings);
    window->setPosition({(int)sf::VideoMode::getDesktopMode().width / 2 - (int)window->getSize().x / 2, 0});

    if (!gladLoadGL()) {
        printf("Error: Could not load OpenGL.");
        return false;
    }
    initGLDebug();
    return true;
}
