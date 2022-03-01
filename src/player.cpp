#include "player.h"

void Player::set_track(std::filesystem::path path) {
    this->path = path.string();
}

void Player::play() {
    if(!track.openFromFile(path)) {
        return;
    }
    this->duration = track.getDuration().asMilliseconds();
    track.play();
}

void Player::toggle() {
    if(track.getStatus() == sf::SoundSource::Status::Paused) {
        track.play();
    } else {
        track.pause();
    }
}

void Player::advance(int offset) {
    int current_offset = track.getPlayingOffset().asMilliseconds();
    int updated_offset = offset + current_offset;
    if(updated_offset >= duration) {
        return;
    }
    if(updated_offset <= 0) {
        updated_offset = 0;
    }
    track.setPlayingOffset(sf::milliseconds(updated_offset));
}

std::pair<int, int> Player::get_m_and_s(int time) const {
    int total_seconds = time / 1000;
    int minutes = total_seconds / 60;
    int seconds = total_seconds - minutes * 60;
    return {minutes, seconds};
}


std::pair<int, int> Player::get_duration() const {
    return get_m_and_s(duration);
}

std::pair<int, int> Player::get_current_offset() const {
    return get_m_and_s(track.getPlayingOffset().asMilliseconds());
}