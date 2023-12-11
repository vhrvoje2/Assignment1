#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

int main(int argc, char *argv[])
{
    // Create new window w*h, needs to be read from config file
    const int wWidth = 1280;
    const int wHeight = 720;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
    window.setFramerateLimit(60);

    // Init IMGUI and create a clock used for its internal timing
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    // Scale the IMGUI UI by a factor
    ImGui::GetStyle().ScaleAllSizes(1.0f);

    float c[3] = {0.0f, 1.0f, 1.0f};

    // Configure shape
    float cRadius = 50;
    int cSegments = 32;
    float cSpeedX = 1.0f;
    float cSpeedY = 0.5f;
    bool drawCircle = true;
    bool drawText = true;

    sf::CircleShape circle(cRadius, cSegments); // Create SFML circle shape
    circle.setPosition(10.0f, 10.0f);           // Set the top-left position

    /* sf::Font myFont;

    if (!myFont.loadFromFile("fonts/tech.ttf"))
    {
        std::cerr << "Could not load font!" << std::endl;
        exit(-1);
    }

    sf::Text text("Sample Text", myFont, 24);
    text.setPosition(0, wHeight - (float)text.getCharacterSize()); */

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

            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code = " << event.key.code << std::endl;

                if (event.key.code == sf::Keyboard::X)
                {
                    cSpeedX *= -1.0f;
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // Draw UI
        ImGui::Begin("Window title");
        ImGui::Text("Window text!");
        ImGui::Checkbox("Draw Circle", &drawCircle);
        ImGui::SameLine();
        ImGui::Checkbox("Draw Text", &drawText);
        ImGui::SliderFloat("Radius", &cRadius, 0.0f, 300.0f);
        ImGui::SliderInt("Sides", &cSegments, 3, 64);
        ImGui::ColorEdit3("Color Circle", c);
        ImGui::InputText("Text", displayString, 255);
        /* if (ImGui::Button("Set Text"))
        {
            text.setString(displayString);
        } */
        ImGui::SameLine();
        if (ImGui::Button("Reset Circle"))
        {
            circle.setPosition(0, 0);
        }
        ImGui::End();

        // Set circle properties after UI updated them
        circle.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255));
        circle.setPointCount(cSegments);
        circle.setRadius(cRadius);

        // Basic animation
        circle.setPosition(circle.getPosition().x + cSpeedX, circle.getPosition().y + cSpeedY);

        // Basic rendering
        window.clear();
        if (drawCircle)
        {
            window.draw(circle);
        }
        /* if (drawText)
        {
            window.draw(text);
        } */

        ImGui::SFML::Render(window); // Draw the UI last so it's on top
        window.display();            // Display window
    }

    return 0;
}