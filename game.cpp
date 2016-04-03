class Game : public sf::Drawable, public sf::Transformable {
public:
    int round = -1, turn = -1, turns_in_round;
    string my_score = "x", scores = "0 1 2 3 4 x";

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        
        stringstream ss;
        ss << "Press space or click" << endl;
        ss << "My score: " << my_score << endl;
        ss << "All scores: " << scores << endl;

        sf::Text text(ss.str().c_str(), font, 14);
        text.setColor(sf::Color::White);
        text.setPosition(120, 200);
        target.draw(text, states);
    }

    void DoSomething() {

        // Send a message, expect "OK" from server
        stream.SendMessageWithCheck("DESCRIBE_WORLD");

        // Read a line from server
        int a, b, c, d;
        response = stream.GetMessage();
        sscanf(response.c_str(), "%d %d %d %d ", &a, &b, &c, &d);

        // Read an integer
        int n = stream.GetVectorOf<int>(1)[0];

        // Read a vector
        vector<int> vec = stream.GetVectorOf<int>(n);
    }

} game;
