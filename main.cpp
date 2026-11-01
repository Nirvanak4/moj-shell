#include<iostream>
#include<string>
#include<vector>
#include<sstream>

std::vector<std::string>razdvoji(const std::string &linija){
std::vector<std::string>rijeci;
std::istringstream ulaz(linija);
std::string rijec;
while(ulaz>>rijec){
rijeci.push_back(rijec);
}
return rijeci;
}

int main(){
std::string linija;
while(true){
std::cout<<"moj-shell> ";
std::getline(std::cin, linija);
if(linija=="exit")
break;
std::vector<std::string>rijeci=razdvoji(linija);
for(int i=0; i<rijeci.size(); i++)
std::cout<<"Rijec "<<i<<": "<<rijeci[i]<<std::endl;
}
return 0;
}

