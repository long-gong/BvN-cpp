#include <inja/inja.hpp>
#include <nlohmann/json.hpp>
#include <fstream> // std::ifstream
// Just for convenience
using namespace inja;
using json = nlohmann::json;

int main()
{
    Environment env;

    // Or directly read a template file
    Template temp = env.parse_template("./template/bipartite.dot");

    json data;
    data["left"] = std::vector<int>({1, 2, 3});
    data["right"] = std::vector<int>({4, 5, 6});
    data["edges"] = {
        {{"source", 1}, {"target", 5}},
        {{"source", 2}, {"target", 4}},
        {{"source", 3}, {"target", 6}}};

    // Or write a rendered file
    env.write(temp, data, "./result.txt");

    return 0;
}