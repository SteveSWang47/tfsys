#include "macros.h"

#include <fstream>
#include <string>
#include <stdio.h>
#include <regex>
#include <queue>

int main()
{
    printf("Content-type: text/html\n\n");
    std::string uri(getenv("REQUEST_URI")), curlin;
    if(!std::regex_match(uri.substr(uri.size()-12, 12), std::regex(R"([0-9]{12})")))
    {
        printf("Illegal.\n");
        return 0;
    }
    std::string id = uri.substr(uri.size()-7, 7),
                probset = uri.substr(uri.size()-12, 5);
    std::ifstream probin(std::string("/usr/share/tfsys/") + probset);
    int siz;
    probin >> siz;
    getline(probin, curlin);
    printf(R"(<!DOCTYPE html>

<head>
    <title>[%s]作答: %s</title>
    <meta charset="UTF-8">
    <script>
)", probset.data(), id.data());
    std::queue<std::string> pbq;
    for(int i = 0; i < siz; i++)
    {
        getline(probin, curlin);
        if(curlin[0] == 'T') printf(R"(        right%d = %d;
        function true%d() { document.getElementById("curans%d").innerHTML = "(正确) "; right%d = 1;}
        function false%d() { document.getElementById("curans%d").innerHTML = "(错误) "; right%d = 0;}
)", i,i,i,i,i,i,i,i);
        else printf(R"(        right%d = %d;
        function true%d() { document.getElementById("curans%d").innerHTML = "(正确) "; right%d = 0;}
        function false%d() { document.getElementById("curans%d").innerHTML = "(错误) "; right%d = 1;}
)", i,i,i,i,i,i,i,i);
        pbq.push(curlin);
    }
    printf(R"(        function reqListener() {
            document.getElementById("remoteresult").innerHTML = this.responseText;
        }
        function on_submit() {
            let res = 0)");
    for(int i = 0; i < siz; i++) printf(" + right%d", i);
    printf(R"(;
            for(var ansspan of document.getElementsByClassName("ans")) {
                ansspan.style.display = "block";
            }
            for(var btnspan of document.getElementsByClassName("solvbtn")) {
                btnspan.innerHTML = "";
            }
            document.getElementById("result").innerHTML = " 您总共答对了 " + res + " 道题。";
            const req = new XMLHttpRequest();
            req.addEventListener("load", reqListener);
            req.open("POST", "%s/cgi-bin/submitresult.cgi");
            req.send("%s%s "+res);
        }
    </script>
    <style>
        body{
            background-image: linear-gradient(96deg, rgb(252, 225, 185), rgb(255, 240, 217));
            text-align: left;
        }
    </style>
</head>

<body>
    <h1 id="heada">请判断对错并点击提交</h1>
)", URL_ROOT, probset.data(), id.data());
    for(int i = 0; i < siz; i++)
    {
        curlin = pbq.front();
        pbq.pop();
        if(curlin[0] == 'T') printf(R"(    <span id="curans%d">() </span>
    <span class="solvbtn">
        <button onclick="true%d() ">正确</button>
        <button onclick="false%d() ">错误</button>
    </span>
    <span>%s</span>
    <span class="ans" style="display: none;"><br />正确答案: 正确<br /></span><br />)",i,i,i,curlin.substr(1, curlin.size()-1).data());
        else printf(R"(    <span id="curans%d">() </span>
    <span class="solvbtn">
        <button onclick="true%d() ">正确</button>
        <button onclick="false%d() ">错误</button>
    </span>
    <span>%s</span>
    <span class="ans" style="display: none;"><br />正确答案: 错误<br /></span><br />)",i,i,i,curlin.substr(1, curlin.size()-1).data());
    }
    printf(R"(    <button onclick="on_submit() ">提交</button><span id="result"></span>
    <div id="remoteresult"></div>
</body>)");
    return 0;
}