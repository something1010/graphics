#include<SFML/Graphics.hpp>
#include<random>
#include<chrono>
#include<thread>
#include<math.h>
#include<iostream>

using namespace sf;

const int winX = 500;
const int winY = 200;

const int waitTime = 20;
const int variance = 10;

const int jumpBase = 9;
const int jumpvariance = 6;

const int chance = 5;
const float accelerationFactor = 2;

int behaivour = 1;   //0 = fall; 1 = stable; 2 = go up
int value;  //indicates progress

float stonks[winX] = {};
float next = winY / 2;
Uint16 seed = 2;

float pow(float base, int exponent) {
    float answer = base;
    for (int i = 0; i < exponent; i++) answer *= base;
    return answer;
}

static Uint16 randvalue() { //pacman rng
    Uint16 out = seed;

    out = out * 5 + 1;

    seed = out + 10;

    return out * seed;
}

float calculateStonks(int type) {

    float answer = 0;

    int value = stonks[winX - 20] - next;

    if(type == 0) answer -= value; 
    else if (type == 1) {
        answer += randvalue() % 21 - 10;
        if (value < -20) answer = abs(answer) * 0.5;
        if (value > 20) answer = -abs(answer) * 0.5;
    }
    else if(type == 2) answer += value;

    if (randvalue() % 100 >= 100 - chance) answer += (randvalue() % jumpvariance + jumpBase) * randvalue() % 2 == 0 ? 1 : -1;

    return answer;
}



int main() {

    srand(time(nullptr));
    
    RectangleShape rect;

    for (int i = 1; i < winX; i++) stonks[winX - i] = float(winY) / 2;
    

    rect.setSize({ 1, 1 });
    rect.setFillColor(Color::White);

    RenderWindow window(VideoMode(winX, winY), "a");

    while (window.isOpen()){
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed){
                window.close();
                return 0;
            }
        }
        for (int i = 2; i < winX; i++) {
            stonks[i - 1] = stonks[i];  //shift the values over
        }

        window.clear();

        //set next value offset to a "random" value
        if (behaivour == 1) next -= calculateStonks(behaivour);
             
        if (next > winY) next = winY;   //capping values at 200 > x > 0
        if (next < 0) next = 0;

        stonks[winX - 1] = next;

        for (int i = 2; i < winX; i++){

            float x1, y1, x2, y2, k1, k2;
            x1 = i - 1;
            x2 = i;
            y1 = stonks[i - 1];
            y2 = stonks[i];
            k1 = (y2 - y1);
            k2 = (x2 - x1);

            //dot representation
            int numIterations = sqrt(k1 * k1 + k2 * k2) * 3;

            CircleShape c1;
            c1.setRadius(0.6);
            c1.setFillColor(y2 < y1 ? Color::Green : Color::Red);

            for (int i = 1; i < numIterations - 1; i++){
                c1.setPosition(x2 * i / numIterations + x1 * (numIterations - i) / numIterations, y2 * i / numIterations + y1 * (numIterations - i) / numIterations);   //interpolation
                window.draw(c1);
            }
        }
        
        window.display();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
        
    }
    return 0;
}
