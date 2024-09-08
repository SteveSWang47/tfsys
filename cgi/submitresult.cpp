#include <stdio.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <string.h>

int main()
{
    std::string dat;
    int res;
    std::cin >> dat >> res;
    if(!std::regex_match(dat, std::regex("[0-9]{12}")))
    {
        printf("Content-type: text/html\n\nIllegal.\n");
        return 0;
    }
    int mx=0, sum=0;
    {
        std::ofstream datout;
        try{datout.open(std::string("/var/www/tfsys/") + dat, std::ios_base::app);}
        catch (std::ios_base::failure& e) {
            std::cout << "Content-type: text/html\n\n" << e.what() << '\n';
            return 0;
        }
        if(datout.fail())
        {
            std::cout << "Content-type: text/html\n\nERROR OPENING FILE: " << strerror(errno) << "\v<br />请向弘毅27王思齐报告此问题。";
            return 0;
        }
        datout << res << '\n';
        datout.close();
    }
    printf("Content-type: text/html\n\n");
    {
        std::ifstream datin(std::string("/var/www/tfsys/") + dat);
        if(datin.fail())
        {
            std::cout << "Content-type: text/html\n\nERROR OPENING FILE: " << strerror(errno) << "\v<br />请向弘毅27王思齐报告此问题。";
            return 0;
        }
        while(datin >> res) sum++, mx = ((mx > res) ? mx : res);
        printf(R"(本题您累计总共挑战过 %d 次，最高记录为 %d 道。)", sum, mx);
        datin.close();
    }
    return 0;
}