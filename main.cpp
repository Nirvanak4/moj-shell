#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>     
#include <unistd.h>    
#include <sys/wait.h>  
#include <fcntl.h>     

std::vector<std::string> razdvoji(const std::string& linija) {
    std::vector<std::string> rijeci;
    std::istringstream ulaz(linija);
    std::string rijec;
    while (ulaz >> rijec) {
        rijeci.push_back(rijec);
    }
    return rijeci;
}

void pokreni(const std::vector<std::string>& rijeci) {
    // 1. nađi ">" i izdvoji ime datoteke
    std::string datoteka;
    std::vector<std::string> args;
    for (size_t i = 0; i < rijeci.size(); i++) {
        if (rijeci[i] == ">" && i + 1 < rijeci.size()) {
            datoteka = rijeci[i + 1];
            i++;   // preskoči i ime datoteke
        } else {
            args.push_back(rijeci[i]);
        }
    }

    // ako je ostalo samo "> datoteka" bez komande, nema šta pokrenuti
    if (args.empty())
        return;

    // 2. argv pravimo od args (bez ">" i imena datoteke)
    std::vector<char*> argv;
    for (const auto& r : args)
        argv.push_back(const_cast<char*>(r.c_str()));
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0) {
        // DIJETE: prvo preusmjeri izlaz, pa pokreni program
        if (!datoteka.empty()) {
            int fd = open(datoteka.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("moj-shell");
                _exit(1);
            }
            dup2(fd, 1);
            close(fd);
        }
        execvp(argv[0], argv.data());
        // ovdje stižemo samo ako execvp nije uspio
        perror("moj-shell");
        _exit(1);
    } else if (pid > 0) {
        // RODITELJ: čekaj da dijete završi
        waitpid(pid, nullptr, 0);
    } else {
        perror("fork");
    }
}

int main() {
    std::string linija;
    while (true) {
        std::cout << "moj-shell> ";
        if (!std::getline(std::cin, linija)) {  // Ctrl+D
            std::cout << std::endl;
            break;
        }
        if (linija == "exit")
            break;

        std::vector<std::string> rijeci = razdvoji(linija);
        if (rijeci.empty())
            continue;  // prazna linija, samo novi prompt

        // ugrađena komanda cd
        if (rijeci[0] == "cd") {
            const char* putanja;
            if (rijeci.size() > 1)
                putanja = rijeci[1].c_str();
            else
                putanja = getenv("HOME");

            if (chdir(putanja) == -1)
                perror("cd");
            continue;
        }

        pokreni(rijeci);
    }
    return 0;
}
