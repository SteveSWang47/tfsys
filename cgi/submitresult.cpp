#include <stdio.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <string.h>
#include <sys/stat.h>

int main()
{
    std::string dat;
    int res, cres;
    std::cin >> dat >> res;
    if(!std::regex_match(dat, std::regex("[0-9]{12}")))
    {
        printf("Content-type: text/html\n\nIllegal.\n");
        return 0;
    }
    char path[32] = "/var/www/tfsys/";
    strcat(path, dat.data());
    int mx=0, sum=0;
    {
        struct stat tbuf;
        if(!stat(path, &tbuf))
        {
            std::ifstream datin(path);
            if(datin.fail())
            {
                std::cout << "Content-type: text/html\n\nERROR OPENING FILE: " << strerror(errno) << "\v<br />请向弘毅27王思齐报告此问题。";
                return 0;
            }
            while(datin >> cres) sum++, mx = ((mx > res) ? mx : cres);
            if(sum > 50)
            {
                printf("本题您的提交次数过多，此次结果将不被记录。");
                return 0;
            }
        }
    }
    {
        std::ofstream datout;
        try{datout.open(path, std::ios_base::app);}
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
    printf(R"(本题您累计总共挑战过 %d 次，最高记录为 %d 道。)", sum + 1, ((mx > res ? mx : res)));
    return 0;
}