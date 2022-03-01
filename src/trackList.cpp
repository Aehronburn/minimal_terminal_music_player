#include "trackList.h"

TrackList::TrackList(std::filesystem::path dir) {
    for(auto entry : std::filesystem::directory_iterator{dir}) {
        if(entry.path().extension() == ".ogg" || entry.path().extension() == ".flac" || entry.path().extension() == ".wav") {
            list.push_back(entry);
        }
    }
}

std::vector<std::string> TrackList::get_list_names() const {
    std::vector<std::string> output;
    for(auto entry : list) {
        std::string path = entry.path().string();
        int last_slash = path.find_last_of('\\');
        std::string name = path.substr(last_slash + 1, path.size());
        output.push_back(name);
    }
    return output;
}

std::filesystem::path TrackList::get_path_at(int index) const {
    return list[index].path();
}