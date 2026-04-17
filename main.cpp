/*
 * Created by Svyatoslav
 * CLI Utility for managing notes: folders as notes, content as txt.
 * Features: Logged paths, Smart Search, Create, Write, Read, List, Delete.
 GNU License
 */

#include <cctype>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <algorithm>
#include <string>
#include <vector>
#include "CLI11.hpp"

namespace fs = std::filesystem;

void update_logs(fs::path path) {
    fs::path log_path = fs::path(std::getenv("HOME")) / ".clinote_paths";
    std::vector<std::string> known_paths;
    std::string line;
    std::ifstream in(log_path);
    while (std::getline(in, line)) known_paths.push_back(line);
    in.close();

    if (std::find(known_paths.begin(), known_paths.end(), path.string()) == known_paths.end()) {
        std::ofstream out(log_path, std::ios::app);
        out << path.string() << std::endl;
    }
}

int main(int argc, char** argv) {
    CLI::App clinote("A simple util for making notes in cli");
    fs::path standart_path = std::getenv("HOME");

    std::string user_path = "";
    std::string note_name = "";

    auto create_note = clinote.add_subcommand("create_note", "create note folder");
    auto write_note = clinote.add_subcommand("write_note", "write in note content");
    auto read_note = clinote.add_subcommand("read_note", "read note content");
    auto list_notes = clinote.add_subcommand("list_notes", "list all notes");
    auto delete_note = clinote.add_subcommand("delete_note", "delete note folder");
    clinote.add_option("-u, --user_path", user_path, "Standard path for notes");
    clinote.add_option("-t, --title", note_name, "Title of the note");


    CLI11_PARSE(clinote, argc, argv);

    auto find_full_path = [&]() -> fs::path {
        if (!user_path.empty()) return fs::path(user_path) / note_name / "content.txt";
        
        std::ifstream log(fs::path(std::getenv("HOME")) / ".clinote_paths");
        std::string p;
        while (std::getline(log, p)) {
            fs::path target = fs::path(p) / note_name / "content.txt";
            if (fs::exists(target)) return target;
        }
        return "";
    };

    if (create_note->parsed()) {
        fs::path base = user_path.empty() ? (standart_path / "notes") : fs::path(user_path);
        fs::path note_path = base / note_name;
        fs::create_directories(note_path);
        std::ofstream(note_path / "content.txt").close();
        update_logs(base);
        std::cout << "Folder and file created at: " << note_path << std::endl;
    }

    if (write_note->parsed()) {
        fs::path p = find_full_path();
        if (p.empty()) { std::cerr << "Note not found!" << std::endl; return 1; }
        
        std::cout << "1 - Append, 2 - Replace: ";
        int mode; std::cin >> mode; std::cin.ignore();
        
        std::ofstream note;
        if (mode == 1) note.open(p, std::ios::app);
        else note.open(p, std::ios::out | std::ios::trunc);
        
        if (note.is_open()) {
            std::string content;
            std::cout << "Enter content: "; 
            std::getline(std::cin, content);
            note << content << std::endl;
            note.close();
            std::cout << "Success!" << std::endl;
        }
    }

    if (read_note->parsed()) {
        fs::path p = find_full_path();
        if (p.empty()) { std::cerr << "Note not found!" << std::endl; return 1; }
        std::ifstream f(p);
        std::cout << "\n--- CONTENT OF: " << note_name << " ---\n" << f.rdbuf() << "\n-------------------\n";
    }

    if (list_notes->parsed()) {
        std::ifstream log(fs::path(std::getenv("HOME")) / ".clinote_paths");
        std::string p;
        bool found = false;
        while (std::getline(log, p)) {
            if (!fs::exists(p)) continue;
            found = true;
            std::cout << "Path: " << p << "\n";
            for (const auto& entry : fs::directory_iterator(p)) {
                if (entry.is_directory()) std::cout << "  -> " << entry.path().filename().string() << "\n";
            }
        }
        if (!found) std::cout << "No notes found in logs." << std::endl;
    }

    if (delete_note->parsed()) {
        fs::path p = find_full_path().parent_path();
        if (!p.empty() && fs::exists(p)) {
            fs::remove_all(p);
            std::cout << "Deleted folder: " << p << std::endl;
        } else {
            std::cerr << "Could not find folder to delete." << std::endl;
        }
    }

    return 0;
}
