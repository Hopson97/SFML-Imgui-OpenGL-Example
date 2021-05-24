#include "Utility.h"
#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>

static void GLAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                       const GLchar* message, const void* userParam)
{
    // clang-format off
    const char* severityString = "?";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:            severityString = "high";    break;
        case GL_DEBUG_SEVERITY_MEDIUM:          severityString = "medium";  break;
        case GL_DEBUG_SEVERITY_LOW:             severityString = "low";     break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:                                return;
    }

    const char* sourceString = "?";
    switch (source) {
        case GL_DEBUG_SOURCE_API:               sourceString = "API";               break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     sourceString = "window system";     break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   sourceString = "shader compiler";   break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       sourceString = "third party";       break;
        case GL_DEBUG_SOURCE_APPLICATION:       sourceString = "app";               break;
        case GL_DEBUG_SOURCE_OTHER:             sourceString = "other";             break;
    }

    const char* typeString = "?";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:                   typeString = "error";               break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:     typeString = "deprecated behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:      typeString = "undefined behavior";  break;
        case GL_DEBUG_TYPE_PORTABILITY:             typeString = "portability";         break;
        case GL_DEBUG_TYPE_MARKER:                  typeString = "marker";              break;
        case GL_DEBUG_TYPE_PUSH_GROUP:              typeString = "push group";          break;
        case GL_DEBUG_TYPE_POP_GROUP:               typeString = "pop group";           break;
        case GL_DEBUG_TYPE_OTHER:                   typeString = "other";               break;
    }
    // clang-format on

    fprintf(stderr, "OpenGL Message.\n Type: %s\nSeverity: %s\nSource: %s\nMessage: %s\n\n", typeString, severityString,
            sourceString, message);
}

static void initGLDebug()
{
#ifndef __APPLE__
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // disable if in release
#endif
    glDebugMessageCallback(glDebugCallback, NULL);

    // this disables messages printed for successfully compiled shaders
    glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL,
                          GL_FALSE);
#endif
}

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
