#include<iostream>
#include<string>

int main(){
std::string linija;
while(true){
std::cout<<"moj-shell> ";
std::getline(std::cin, linija);
if(linija=="exit")
break;
std::cout<<"Unijeli ste: "<<linija<<std::endl;
}
return 0;
}

