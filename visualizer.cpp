class DrawingExample : public sf::Drawable, public sf::Transformable {
public:
    int mousex = 100, mousey = 100;
    int space_toggle = true;

    void MouseClick(int x, int y) {
        mousex = x;
        mousey = y;
    }

    void SpaceToggle() {
        space_toggle = !space_toggle;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // Apply the transform
        states.transform *= getTransform();

        // Draw circle
        sf::CircleShape circle(10);
        circle.setPosition(mousex-10, mousey-10);
        circle.setFillColor(sf::Color(255, 255, 0));
        target.draw(circle, states);

        // Draw rectangle
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(50, 20));
        rectangle.setOutlineColor(sf::Color::Red);
        rectangle.setOutlineThickness(5);
        rectangle.setPosition(20, 20);
        if (space_toggle) {
            target.draw(rectangle);
        }

        // Draw text
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        text.setColor(sf::Color::White);
        text.setString("Hello,\nworld!");
        text.setPosition(20, 200);
        target.draw(text, states);

    }

} drawing_example;

class Visualizer {
public:

    void ProcessEvents() {
        sf::Event event;
        while (window_->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_->close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                drawing_example.MouseClick(event.mouseButton.x, event.mouseButton.y);      
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    drawing_example.SpaceToggle();
                }
                
                // Other to use:
                // sf::Keyboard::Down
                // sf::Keyboard::Up
                // sf::Keyboard::Num9
                // sf::Keyboard::Num0
                // sf::Keyboard::LBracket
                // sf::Keyboard::RBracket
                // sf::Keyboard::A
                // sf::Keyboard::B
          }
        }
    }

    void Render() {
      window_->clear(sf::Color::Black);
      window_->draw(game);
      window_->draw(drawing_example);
      window_->display();
    }

    void Loop() {
        while (window_->isOpen()) {
            ProcessEvents();
            Render();
        }
    }
} visualizer;