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

void izvrsi(const std::vector<std::string>& rijeci) {
    std::string datoteka;
    std::vector<std::string> args;
    for (size_t i = 0; i < rijeci.size(); i++) {
        if (rijeci[i] == ">" && i + 1 < rijeci.size()) {
            datoteka = rijeci[i + 1];
            i++;   
        } else {
            args.push_back(rijeci[i]);
        }
    }

    if (args.empty())
        _exit(1);

    std::vector<char*> argv;
    for (const auto& r : args)
        argv.push_back(const_cast<char*>(r.c_str()));
    argv.push_back(nullptr);

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
    perror("moj-shell");  
    _exit(1);
}


void pokreni(const std::vector<std::string>& rijeci, bool pozadina) {
    pid_t pid = fork();
    if (pid == 0) {
        izvrsi(rijeci);
    } else if (pid > 0) {
if(pozadina)
std::cout<<"[pokrenuto u pozadini, PID "<<pid<<"]"<<std::endl;
else
        waitpid(pid, nullptr, 0);
    } else {
        perror("fork");
    }
}


void pokreniPipe(const std::vector<std::string>& lijeva,
                 const std::vector<std::string>& desna) {
    int cijev[2];
    if (pipe(cijev) == -1) {
        perror("pipe");
        return;
    }

    pid_t p1 = fork();
    if (p1 == 0) {
        dup2(cijev[1], 1);
        close(cijev[0]);
        close(cijev[1]);
        izvrsi(lijeva);
    }

    pid_t p2 = fork();
    if (p2 == 0) {
        dup2(cijev[0], 0);
        close(cijev[0]);
        close(cijev[1]);
        izvrsi(desna);
    }

   
    close(cijev[0]);
    close(cijev[1]);
    waitpid(p1, nullptr, 0);
    waitpid(p2, nullptr, 0);
}

int main() {
    std::string linija;
    while (true) {
        std::cout << "moj-shell> ";
        if (!std::getline(std::cin, linija)) {  
            std::cout << std::endl;
            break;
        }
        if (linija == "exit")
            break;

        std::vector<std::string> rijeci = razdvoji(linija);
        if (rijeci.empty())
            continue;
bool pozadina=false;
if(rijeci.back()=="&"){
pozadina=true;
rijeci.pop_back();
if(rijeci.empty())
continue;
}
       
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

       
        size_t poz = rijeci.size();
        for (size_t i = 0; i < rijeci.size(); i++) {
            if (rijeci[i] == "|") {
                poz = i;
                break;
            }
        }

        if (poz == rijeci.size()) {
            pokreni(rijeci, pozadina);                      
        } else if (poz == 0 || poz == rijeci.size() - 1) {
            std::cout << "moj-shell: pogresna upotreba |" << std::endl;
        } else {
            std::vector<std::string> lijeva(rijeci.begin(), rijeci.begin() + poz);
            std::vector<std::string> desna(rijeci.begin() + poz + 1, rijeci.end());
            pokreniPipe(lijeva, desna);
        }
    }
    return 0;
}
