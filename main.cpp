#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>   
#include <sys/wait.h>  
#include<cstdlib>
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
    // execvp traži niz običnih C stringova koji završava sa nullptr
    std::vector<char*> argv;
    for (const auto& r : rijeci)
        argv.push_back(const_cast<char*>(r.c_str()));
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0) {
        // DIJETE: pretvori se u traženi program
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
 if(rijeci[0]=="cd"){
const char *putanja;
if(rijeci.size()>1)
putanja=rijeci[1].c_str();
else
putanja=getenv("HOME");

if(chdir(putanja)==-1)
perror("cd");
continue;
}
        pokreni(rijeci);
    }
    return 0;
}
