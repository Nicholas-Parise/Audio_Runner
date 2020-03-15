#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;



void LineDraw(long int &interval, long int &sampleRate, std::vector<sf::RectangleShape> &Dot, std::vector<sf::RectangleShape> &lineArr, sf::SoundBuffer &buffer, int Wind) {

    int xpos = 0;
    int place = 0;
    int temp = 0;
    int lineplace = 0;


    for(int i = (interval-(sampleRate/60)); i<interval; i++) {

        if(place>800) {
            place = 0;
        }
        if(xpos>720) {
            xpos = 0;
        }
        if(lineplace >359) {

            lineplace = 0;
        }

        temp = ((buffer.getSamples()[i] / 50)+(Wind/2));

        Dot[place].setPosition(xpos,temp);

        if(temp <0) {
            temp = 0;
        }

        if(temp>300) {
            temp = 300;
        }

        lineArr[lineplace].setSize(sf::Vector2f(2, temp-100));

        lineplace ++;
        place++;
        xpos++;
    }

}



int main() {

    int xpos = 0;
    int place = 0;
    int temp = 0;
    long int interval = 0;
    int seconds = 0;
    int lineplace = 0;

    int mark = 0;

    bool resetSong = true;

    srand (time(NULL));

    sf::RenderWindow window(sf::VideoMode(720, 480), "Audio Runner");
    window.setFramerateLimit(60);



    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(100,100);
    shape.setPosition(window.getSize().x/2,window.getSize().y/2);

    sf::SoundBuffer buffer ;
    sf::Sound sound ;
    sound.setVolume(50);

    std::vector<sf::RectangleShape> Dot(805);

    for (int i = 0; i < Dot.size(); i++) {

        Dot[i].setFillColor(sf::Color::White);
        Dot[i].setSize(sf::Vector2f(3, 3));
        Dot[i].setOrigin(1.5,1.5);
    }


    std::vector<sf::RectangleShape> lineArr (360);

    for (int i = 0; i < lineArr.size(); i++) {

        lineArr[i].setFillColor(sf::Color::White);
        lineArr[i].setSize(sf::Vector2f(2, 240));
        lineArr[i].setOrigin(1,0);
        lineArr[i].setRotation(i);
        lineArr[i].setPosition(shape.getPosition().x,shape.getPosition().y);



    }


    string SongStr;


    ifstream readFile;
    readFile.open("Assets/Song.txt");

    if(readFile.is_open()) {

        while(!readFile.eof()) {

            readFile >> SongStr;
        }
    }
    readFile.close();


    buffer.loadFromFile("Assets/"+SongStr);


    sound.setBuffer(buffer) ;
    sound.setLoop(true);


    long int sampleCount = buffer.getSampleCount() ;
    long int sampleRate = buffer.getSampleRate()*buffer.getChannelCount() ;


    interval += sampleRate/60;


    cout<<sampleRate<<"\t\t"<<sampleCount<<endl;


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {

                sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());

                if(size.x > 800) {
                    size.x = 720*2;
                }

                if(size.y > 800) {
                    size.y = 480*2;
                }

                window.setSize(static_cast<sf::Vector2u>(size));
            }

        }



        if(resetSong == true) {
            sound.stop();
            sound.play();
            resetSong = false;
        }


        for(int i = (interval-(sampleRate/60)); i<sound.getPlayingOffset().asSeconds()*sampleRate; i++) {

            if(place>800) {
                place = 0;
            }
            if(xpos>720) {
                xpos = 0;
            }
            if(lineplace >359) {

                lineplace = 0;
            }

            temp = ((buffer.getSamples()[i] / 50)+(window.getSize().y/2));

            Dot[place].setPosition(xpos,temp);

            temp*=0.7;

            if(temp <0) {
                temp = 0;
            }

            if(temp>300) {
                temp = 300;
            }

            lineArr[lineplace].setSize(sf::Vector2f(2, temp-60));


            lineplace ++;
            place++;
            xpos++;
        }



//LineDraw(interval, sampleRate, Dot, lineArr, buffer,window.getSize().y);




        if((interval+sampleRate) < (sampleCount-(sampleRate/60))) {
            interval+=(sampleRate/60);
        } else {

            cout<<"reset"<<endl;
            resetSong = true;
            interval = sampleRate/60;
        }





        window.clear();



        for(int i = 0; i < 360; i++) {

            window.draw(lineArr[i]);
        }


        window.draw(shape);

        for(int i = 0; i < 800; i++) {

            window.draw(Dot[i]);
        }






        window.display();
    }

    return 0;
}

