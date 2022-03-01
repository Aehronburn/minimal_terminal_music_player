#ifndef player_h
#define player_h

#include <SFML/Audio.hpp>

#include <string>
#include <filesystem>
#include <utility>

class Player {

    private:
        std::string path;
        sf::Music track;
        int duration;

        std::pair<int, int> get_m_and_s(int time) const;

    public:
        void set_track(std::filesystem::path path);
        void play();
        void toggle();
        void advance(int offset);

        std::pair<int, int> get_duration() const;
        std::pair<int, int> get_current_offset() const;

        int get_duration_seconds() const {
            return track.getDuration().asMilliseconds() / 1000;
        }

        void set_loop(bool loop) {
            track.setLoop(loop);
        }

        bool get_loop() const {
            return track.getLoop();
        }

        bool is_stopped() const {
            return track.getStatus() == sf::SoundSource::Stopped;
        }
};

#endif