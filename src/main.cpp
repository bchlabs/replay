#include <iostream>
#include <vector>
#include "common.h" 

std::string abc = "172.31.19.224:8332";
std::string sv = "127.0.0.1:8332";
std::string auth_abc = "mining:war";
std::string auth_sv = "mining:war";

std::vector<std::string> hexs;

bool loop(const std::vector<std::string> &vins, std::vector<std::string> &next_vins)
{
    for (size_t i = 0; i < vins.size(); ++i)
    {   
        std::string txid = vins.at(i);
        std::vector<std::string> tmp; 
        std::string hexx; 
        bool fsv = GetRawTransaction(txid, sv, auth_sv, tmp, hexx);
        if (fsv)
              continue;

        bool fabc = GetRawTransaction(txid, abc, auth_abc, tmp, hexx);
        if (!fabc)
              continue;
    
       hexs.push_back(hexx); 
       for (int k = 0; k < tmp.size(); ++k) 
            next_vins.push_back(tmp.at(k));
       tmp.clear(); 
    } 

    return next_vins.size() > 0;
}


int main(int argc, char*argv[])
{
    if (argc != 6)
    {
        std::cout << "Usage: broadcast url_abc url_sv auth_abc auth_sv txid" << std::endl;
        return 1;
    }

    abc = argv[1];
    sv = argv[2]; 
    auth_abc = argv[3];
    auth_sv = argv[4];
    std::string txid = argv[5];

    std::cout << "url_abc: " << abc << std::endl;
    std::cout << "url_sv: " << sv << std::endl;
    std::cout << "auth_abc: " << auth_abc << std::endl;
    std::cout << "auth_sv: " << auth_sv << std::endl;

    std::vector<std::string> vins;
    std::string hex;
    bool fsv = GetRawTransaction(txid, sv, auth_sv, vins, hex);    
    if (fsv)  
    {
        std::cout << txid << " has in sv. " <<  std::endl;
        return 0;
    }

    bool fabc = GetRawTransaction(txid, abc, auth_abc, vins, hex);
    if (!fabc) 
    {
        std::cout << txid << " doesn't in sv or abc. " <<  std::endl;
        return 0;
    }

    hexs.push_back(hex);
    std::vector<std::string> next_vins;
    while (loop(vins, next_vins)) 
    {
        vins.clear();
        vins = next_vins;
        next_vins.clear(); 
    }

    for (int i = 0; i < hexs.size(); ++i)
        std::cout << hexs.at(i) << std::endl;
    
    return 0;
}
