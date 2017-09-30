#include <SFML/Config.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>

using namespace std;

int main()
{
    sf::SoundBuffer buffer1, buffer2;
    buffer1.loadFromFile("Us.ogg");
    buffer2.loadFromFile("Us2.ogg");

    unsigned int channels = std::max(buffer1.getChannelCount(), buffer2.getChannelCount());
    unsigned int rate     = std::max(buffer1.getSampleRate(), buffer2.getSampleRate());
    std::size_t  size     = std::max(buffer1.getSampleCount(), buffer2.getSampleCount());

    std::vector<sf::Int16> samples(size, 0);

    for(std::size_t i = 0; i < size; ++i)
    {
        sf::Int16 b1 = 0, b2 = 0;
        if(i < buffer1.getSampleCount())
            b1 = buffer1.getSamples()[i];
        if(i < buffer2.getSampleCount())
            b2 = buffer2.getSamples()[i];

        cout << "b1: " << b1 << endl;
        // Mixing
        if(b1 < 0 && b2 < 0)
            samples[i] = (b1 + b2) - static_cast<sf::Int16>((b1 * b2) / INT16_MIN);
        else if(b1 > 0 && b2 > 0)
            samples[i] = (b1 + b2) - static_cast<sf::Int16>((b1 * b2) / INT16_MAX);
        else
            samples[i] = b1 + b2;
    }

    sf::SoundBuffer buffer;
    buffer.loadFromSamples(samples.data(), samples.size(), 2, rate);

    sf::Sound sound(buffer);
    sound.play();

    while(sound.getStatus() == sf::Sound::Playing)
    {
        // Wait
    }
}
