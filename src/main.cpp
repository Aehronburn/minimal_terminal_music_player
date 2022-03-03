#include <curses.h>

#include <thread>
#include <chrono>

#include "trackList.h"
#include "player.h"

void display_list(std::vector<std::string>&, int, int, int);

void update_selected(int&, int, int);

void display_duration(Player&, int);

void display_current_offset(Player&, int&, bool&);

void display_media_icon(bool, int);

void display_loop(bool loop, int);

void manage_autoplay(Player&, int&, int&, int, bool&, std::vector<std::string>&, int, bool&, TrackList&);

int main(int argc, char** argv) {

    std::string path = "C:\\Users\\zhuyi\\Music";

    if(argc == 2) {
        path = argv[1];
    }

    //setup
    initscr(); //initialization
    cbreak(); //disable buffering
    noecho(); //disable printing input
    start_color(); //use colors
    leaveok(stdscr, true); //leaves cursor where it was left
    keypad(stdscr, true); //arrow key
    curs_set(0); //set cursor as invisible

    //mp3 files in directory
    TrackList list{std::filesystem::path{path}};
    std::vector<std::string> track_names = list.get_list_names();

    //length of the list to be displayed
    int stdscr_height = getmaxy(stdscr) - 5;

    int selected = 0;

    bool exited = false;

    bool is_looping = false;
    
    int currently_playing = selected;

    bool is_playing = false;

    Player player;
    player.set_track(list.get_path_at(selected));           
    player.play();
    player.toggle();

    box(stdscr, 0, 0);
    display_list(track_names, selected, stdscr_height, currently_playing);
    display_media_icon(is_playing, stdscr_height);
    display_duration(player, stdscr_height);
    display_loop(is_looping, stdscr_height);

    std::thread offset_displayer(display_current_offset, std::ref(player), std::ref(stdscr_height), std::ref(exited));
    std::thread autoplayer(manage_autoplay, std::ref(player), std::ref(selected), std::ref(currently_playing), track_names.size(), std::ref(exited), std::ref(track_names), stdscr_height, std::ref(is_looping), std::ref(list));

    int ch = 0;
    while((ch = getch()) != 'q') {
        switch (ch)
        {
        case KEY_DOWN:
            update_selected(selected, 1, track_names.size());
            display_list(track_names, selected, stdscr_height, currently_playing);
            display_duration(player, stdscr_height);
            display_loop(is_looping, stdscr_height);
            break;
        case KEY_UP:
            update_selected(selected, -1, track_names.size());
            display_list(track_names, selected, stdscr_height, currently_playing);
            display_duration(player, stdscr_height);
            display_loop(is_looping, stdscr_height);
            break;
        case 10:  //enter key
            player.set_track(list.get_path_at(selected));
            player.play();
            currently_playing = selected;
            display_list(track_names, selected, stdscr_height, currently_playing);
            is_playing = true;
            display_media_icon(is_playing, stdscr_height);
            display_duration(player, stdscr_height);
            display_loop(is_looping, stdscr_height);
            break;
        case 'p':
            player.toggle();
            is_playing = !is_playing;
            display_media_icon(is_playing, stdscr_height);
            display_duration(player, stdscr_height);
            display_loop(is_looping, stdscr_height);
            break;
        case KEY_LEFT:
            player.advance(-5000);
            break;
        case KEY_RIGHT:
            player.advance(5000);
            break;
        case 'r':
            is_looping = !is_looping;
            player.set_loop(is_looping);
            display_loop(is_looping, stdscr_height);
        case KEY_RESIZE:
            clear();
            resize_term(0, 0);
            stdscr_height = getmaxy(stdscr) - 5;
            display_list(track_names, selected, stdscr_height, currently_playing);
            display_media_icon(is_playing, stdscr_height);
            display_duration(player, stdscr_height);
            display_loop(is_looping, stdscr_height);
            box(stdscr, 0, 0);
        default:
            break;
        }
    }
    exited = true;
    offset_displayer.join();
    autoplayer.join();
    endwin();
    return 0;
}

void display_list(std::vector<std::string>& track_names, int selected, int stdscr_height, int currently_playing) {
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    int row = 2; //from which row to start printing
    int starting_row = selected / stdscr_height; //first line of the current page
    int page_index = starting_row * stdscr_height;
    int left_to_display = track_names.size() - starting_row * stdscr_height;
    if(left_to_display > stdscr_height) {
        left_to_display = stdscr_height;
    }

    for(int i = page_index; i < (page_index + left_to_display); i++) {
        if(i == selected) {
            attron(A_STANDOUT);
        }
        if(i == currently_playing) {
            attron(COLOR_PAIR(1));
        }
        clrtoeol();
        mvprintw(row, 2, "%d\t %s", i, track_names[i].c_str());
        attroff(A_STANDOUT);
        attroff(COLOR_PAIR(1));
        row++;
    }

    //clear eventual old lines at the bottom
    for(int i = row; i < stdscr_height + 2; i++) {
        move(i, 1);
        clrtoeol();
    }
    box(stdscr, 0, 0);
    refresh();
}

void update_selected(int& selected, int offset, int size) {
    selected = (selected == 0 && offset == -1) ? size - 1 : (selected + offset) % size;
}

void display_duration(Player& player, int terminal_height) {
    auto [minutes, seconds] = player.get_duration();
    std::string seconds_s = std::to_string(seconds);
    if(seconds < 10) {
        seconds_s = "0" + seconds_s;
    }

    int row = terminal_height + 3;
    int column = 10;
    mvprintw(row, column, "%d:%s", minutes, seconds_s.c_str());
    refresh();
}

void display_current_offset(Player& player, int& terminal_height, bool& exited) {
    while(!exited) {
        auto [minutes, seconds] = player.get_current_offset();
        std::string seconds_s = std::to_string(seconds);
        if(seconds < 10) {
            seconds_s = "0" + seconds_s;
        }
        int row = terminal_height + 3;
        int column = 5;
        mvprintw(row, column, "%d:%s/", minutes, seconds_s.c_str());
        refresh();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void display_media_icon(bool is_playing, int terminal_height) {
    char* symbol = is_playing ? "||" : "> ";
    mvprintw(terminal_height + 3, 2, symbol);
}

void display_loop(bool loop, int terminal_height) {
    char* is_loop = loop ? "o" : "x";
    mvprintw(terminal_height + 3, 16, "Loop:%s", is_loop);
}

void manage_autoplay(Player& player, int& selected, int& currently_playing, int size, bool& exited, std::vector<std::string>& track_names, int terminal_height, bool& is_looping, TrackList& list) {
    while(!exited) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if(player.is_stopped()) {
            selected = (selected + 1) % size;
            currently_playing = selected;
            player.set_track(list.get_path_at(selected));
            player.play();
            clear();
            display_list(track_names, selected, terminal_height, currently_playing);
            display_media_icon(true, terminal_height);
            display_duration(player, terminal_height);
            display_loop(is_looping, terminal_height);
        }
    }
}