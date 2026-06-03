#include<iostream>
#include<string>
#include<sstream>
#include<jsoncpp/json/json.h>
#include<memory>


int main()
{
    Json::Value root;
    root["name"] = "zs";
    root["high"] = 1.78f;
    root["sex"] = "男";

    // Json::FastWriter writer;
    Json::StyledWriter writer;
    std::string s = writer.write(root);
    std::cout << s;








    Json::Value droot;
    Json::Reader reader;
    reader.parse(s,droot);

    std::string name = droot["name"].asString();
    float high = droot["high"].asFloat();
    std::string sex = droot["sex"].asString();

    std::cout << name << std::endl;
    std::cout << high << std::endl;
    std::cout << sex << std::endl;

    // Json::StreamWriterBuilder wbuilder;
    // std::unique_ptr<Json::StreamWriter> writer(wbuilder.newStreamWriter());

    // std::stringstream ss;
    // writer->write(root,&ss);
    // std::cout << ss.str();

    // //序列化
    // std::string s = root.toStyledString();

    // std::cout << s;
    return 0;
}