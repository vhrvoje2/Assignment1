#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#define FRAMERATE 60
#define DEFAULT_SEGMENTS 32

class RGBColor
{
public:
    int mRed;
    int mGreen;
    int mBlue;

    RGBColor(){};
    RGBColor(int red, int green, int blue)
        : mRed(red),
          mGreen(green),
          mBlue(blue) {}
};

class CShape : RGBColor
{
public:
    std::string mName;
    float mInitialX;
    float mInitialY;
    float mSpeedX;
    float mSpeedY;
    bool mIsDrawn = true;
    bool mIsTextDrawn = true;
    sf::RectangleShape mSfmlRectangle;
    sf::CircleShape mSfmlCircle;

    CShape(std::string name, float initialX, float initialY, float speedX, float speedY, int red, int green, int blue)
        : RGBColor(red, green, blue),
          mName(name),
          mInitialX(initialX),
          mInitialY(initialY),
          mSpeedX(speedX),
          mSpeedY(speedY) {}
};

class Circle : public CShape
{
public:
    float mRadius;

    Circle(std::string name, float initialX, float initialY, float speedX, float speedY, int red, int green, int blue, float radius)
        : CShape(name, initialX, initialY, speedX, speedY, red, green, blue),
          mRadius(radius) {}
};

class Rectangle : public CShape
{
public:
    float mWidth;
    float mHeight;

    Rectangle(std::string name, float initialX, float initialY, float speedX, float speedY, int red, int green, int blue, float width, float height)
        : CShape(name, initialX, initialY, speedX, speedY, red, green, blue),
          mWidth(width),
          mHeight(height) {}
};

class GameWindow
{
public:
    int mWidth;
    int mHeight;

    GameWindow(){};
    GameWindow(int width, int height) : mWidth(width),
                                        mHeight(height) {}
};

class FontConfig : RGBColor
{
public:
    std::string mFontFile;
    int mSize;

    FontConfig(){};
    FontConfig(int red, int green, int blue, std::string fontFile, int size) : RGBColor(red, green, blue),
                                                                               mFontFile(fontFile),
                                                                               mSize(size) {}
};

void parseConfig(std::string filename, std::vector<CShape> &shapes, GameWindow &gameWindow, FontConfig &fontConfig)
{
    float initialX, initialY, speedX, speedY, radius;
    int width, height, r, g, b, size;
    std::string temp;
    std::ifstream fileIn(filename);

    while (fileIn >> temp)
    {
        if (temp == "Window")
        {
            fileIn >> width >> height;
            gameWindow = GameWindow(width, height);
        }
        else if (temp == "Font")
        {
            fileIn >> temp >> size >> r >> g >> b;
            fontConfig = FontConfig(r, g, b, temp, size);
        }
        else if (temp == "Circle")
        {
            fileIn >> temp >> initialX >> initialY >> speedX >> speedY >> r >> g >> b >> radius;
            Circle newCircle = Circle(temp, initialX, initialY, speedX, speedY, r, g, b, radius);
            newCircle.mSfmlCircle = sf::CircleShape(radius, DEFAULT_SEGMENTS);
            newCircle.mSfmlCircle.setFillColor(sf::Color(r, g, b));
            newCircle.mSfmlCircle.setPosition(initialX, initialY);
            shapes.push_back(newCircle);
        }
        else if (temp == "Rectangle")
        {
            fileIn >> temp >> initialX >> initialY >> speedX >> speedY >> r >> g >> b >> width >> height;
            Rectangle newRectangle = Rectangle(temp, initialX, initialY, speedX, speedY, r, g, b, width, height);
            newRectangle.mSfmlRectangle = sf::RectangleShape(sf::Vector2f(height, width));
            newRectangle.mSfmlRectangle.setFillColor(sf::Color(r, g, b));
            newRectangle.mSfmlRectangle.setPosition(initialX, initialY);
            shapes.push_back(newRectangle);
        }
    }
};

float checkXBounds(GameWindow &gameWindow, sf::FloatRect &globalBounds)
{
    if (globalBounds.left < 0 || globalBounds.left + globalBounds.width > gameWindow.mWidth)
    {
        return -1;
    }
    return 1;
}

float checkYBounds(GameWindow &gameWindow, sf::FloatRect &globalBounds)
{
    if (globalBounds.top < 0 || globalBounds.top + globalBounds.height > gameWindow.mHeight)
    {
        return -1;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    std::vector<CShape> shapes;
    std::vector<sf::Shape> sfShapes;
    GameWindow gameWindow;
    FontConfig fontConfig;
    parseConfig("config.txt", shapes, gameWindow, fontConfig);

    // Create new window
    const int wWidth = gameWindow.mWidth;
    const int wHeight = gameWindow.mHeight;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "Assignment 1");
    window.setFramerateLimit(FRAMERATE);

    // Init IMGUI and create a clock used for its internal timing
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    // Scale the IMGUI UI by a factor
    ImGui::GetStyle().ScaleAllSizes(1.0f);

    sf::Font myFont;

    if (!myFont.loadFromFile(fontConfig.mFontFile))
    {
        std::cerr << "Could not load font!" << std::endl;
        exit(-1);
    }

    sf::Text text("Sample Text", myFont, 24);
    text.setPosition(0, wHeight - (float)text.getCharacterSize());

    char displayString[255] = "Sample Text";

    while (window.isOpen())
    {
        // Event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Pass the event to IMGUI
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        // drawUI();

        // Draw UI
        /* ImGui::Begin("Window title");
        ImGui::Text("Window text!");
        ImGui::Checkbox("Draw Circle", &drawCircle);
        ImGui::SameLine();
        ImGui::Checkbox("Draw Text", &drawText);
        ImGui::SliderFloat("Radius", &cRadius, 0.0f, 300.0f);
        ImGui::SliderInt("Sides", &cSegments, 3, 64);
        ImGui::ColorEdit3("Color Circle", c);
        ImGui::InputText("Text", displayString, 255);
        if (ImGui::Button("Set Text"))
        {
            text.setString(displayString);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Circle"))
        {
            circle.setPosition(0, 0);
        }
        ImGui::End(); */

        // Basic rendering
        window.clear();

        for (auto &shape : shapes)
        {
            if (shape.mName.rfind("C", 0) == 0)
            {
                sf::Vector2<float> currentPosition = shape.mSfmlCircle.getPosition();
                sf::FloatRect globalBounds = shape.mSfmlCircle.getGlobalBounds();
                shape.mSpeedX *= checkXBounds(gameWindow, globalBounds);
                shape.mSpeedY *= checkYBounds(gameWindow, globalBounds);
                shape.mSfmlCircle.setPosition(currentPosition.x + shape.mSpeedX, currentPosition.y + shape.mSpeedY);
                window.draw(shape.mSfmlCircle);
            }
            else
            {
                sf::Vector2<float> currentPosition = shape.mSfmlRectangle.getPosition();
                sf::FloatRect globalBounds = shape.mSfmlRectangle.getGlobalBounds();
                shape.mSpeedX *= checkXBounds(gameWindow, globalBounds);
                shape.mSpeedY *= checkYBounds(gameWindow, globalBounds);
                shape.mSfmlRectangle.setPosition(currentPosition.x + shape.mSpeedX, currentPosition.y + shape.mSpeedY);
                window.draw(shape.mSfmlRectangle);
            }
        }

        ImGui::SFML::Render(window); // Draw the UI last so it's on top
        window.display();            // Display window
    }

    return 0;
}