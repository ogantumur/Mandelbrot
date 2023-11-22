#include "ComplexPlane.h"

int main()
{
    int width = VideoMode::getDesktopMode().width / 2;
    int height = VideoMode::getDesktopMode().height / 2;
    VideoMode vm(width, height);
    RenderWindow window(vm, "Mandelbrot Set", Style::Default);

    ComplexPlane bob(width, height);

    Font font;
    if (!font.loadFromFile("Vermin Vibes 1989.ttf"))
    {
        cerr << "Error loading font\n";
        return -1;
    }

    Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(Color::White);
    text.setPosition(10, 0);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Right)
                {
                    bob.zoomIn();
                    Vector2i pos(event.mouseButton.x, event.mouseButton.y);
                    bob.setCenter(pos);
                }
                else if (event.mouseButton.button == Mouse::Left)
                {
                    bob.zoomOut();
                    Vector2i pos(event.mouseButton.x, event.mouseButton.y);
                    bob.setCenter(pos);
                }
            }
            if (event.type == Event::MouseMoved)
            {
                Vector2i pos(event.mouseMove.x, event.mouseMove.y);
                bob.setMouseLocation(pos);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        bob.updateRender();
        bob.loadText(text);

        window.clear();

        window.draw(bob, RenderStates::Default);
        window.draw(text);
        window.display();
    }
}