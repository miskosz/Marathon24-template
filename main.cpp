#include <memory>
#include <iostream>
#include <map>

#include <SFML/Graphics.hpp>

#include <glog/logging.h>
#include <gflags/gflags.h>
#include "c24/c24.h"

using c24::communication::StreamBackendInterface;
using c24::communication::StreamTcpClient;
using c24::communication::Stream;
using namespace std;

DEFINE_string(host, "marathon",
              "Define hostname to which do you want to connect.");
DEFINE_int32(port, 5500, "Define port number to which do you want to connect.");

typedef pair<int, int> PII;
std::unique_ptr<sf::RenderWindow> window_;
sf::Font font;

// common
Stream stream;
string response;
char message[1000];

#include "game.cpp"
#include "visualizer.cpp"

void MainLoop() {
    while (true) {

        // get round number
        int cur_round;
        while (!stream.SendMessageWithCheck("GET_TIME"))
            usleep(750*1000);
        response = stream.GetMessage();
        sscanf(response.c_str(), "%d %d %d ", &cur_round, &game.turn, &game.turns_in_round);

        // check for new round
        if (cur_round != game.round) {
            game.round = cur_round;
            VLOG(1) << "NEW ROUND " << game.round;
        }

        // <<BEGIN>> HERE DO THE STUFF

        // get scores
        stream.SendMessageWithCheck("GET_MY_SCORE");
        game.my_score = stream.GetMessage();
        stream.SendMessageWithCheck("GET_ALL_SCORES");
        stream.GetMessage();
        game.scores = stream.GetMessage();

        game.DoSomething();

        // <<END>> HERE DO THE STUFF

        // wait for next turn
        stream.SendMessageWithCheck("WAIT");
        stream.GetMessage(); // WAITING 0.21000
        //CHECK_EQ(stream.GetMessage(), "WAITING") << "Did not receive 'WAITING'.";
        CHECK_EQ(stream.GetMessage(), "OK") << "Did not receive 'OK' on 'WAIT'.";
        VLOG(1) << "NEW TURN " << game.round;
    }
}

int Init(int argc, char** argv) {
    // check the arguments
    if (argc == 1) {
        cout << "Run as: ./solution --log_dir=logs --stderrthreshold=0 --port=300X" << endl;
        return 1;
    }

    // google init
    google::InitGoogleLogging(*argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    VLOG(1) << "Google initialized!";

    // connect to a stream
    stream = Stream(std::unique_ptr<StreamBackendInterface>(
        new StreamTcpClient(FLAGS_host, FLAGS_port)));
    if (!stream.Connected()) {
        LOG(ERROR) << "Opening Tcp stream failed.";
//        return 1;
    }

    // authenticate
    // CHECK_EQ(stream.GetMessage(), "LOGIN") << "Did not receive 'LOGIN'.";
    // stream.SendMessage("we_must_go_deeper");
    // CHECK_EQ(stream.GetMessage(), "PASSWORD") << "Did not receive 'PASSWORD'.";
    // stream.SendMessageWithCheck("vWZ2gh0ZUEPPCAH9");
    // VLOG(1) << "Authenticated!";
    
    srand(time(NULL));

    // Declare and load a font
    font.loadFromFile("FreeSans.ttf");

    return 0;
}

int main(int argc, char** argv) {

    // Initialize
    if (Init(argc, argv))
        return 1;

    // Create window
    window_ = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(
        sf::VideoMode(1240, 800), "My window"));
    window_->setVerticalSyncEnabled(true);

    // Run the game loop in a thread
    std::thread game(MainLoop);

    // Run the visualizer loop
    visualizer.Loop();

    // Wait for the game loop to finish
    game.join();

    return 0;
}
