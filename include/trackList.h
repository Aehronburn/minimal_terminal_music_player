#ifndef tracklist_h
#define tracklist_h

#include <filesystem>
#include <string>
#include <vector>

class TrackList {
    public:
        TrackList(std::filesystem::path dir);
        std::vector<std::string> get_list_names() const;
        std::filesystem::path get_path_at(int index) const;

    private:
        std::vector<std::filesystem::directory_entry> list;
};

#endif