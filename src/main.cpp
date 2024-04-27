#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "graphviz_vizitor.hpp"

void graphviz(const std::string string, const std::string name)
{
    std::system(
        std::format(
            R"(echo '{}' | dot -Tsvg > {}.svg && open -a "Google Chrome" ./{}.svg)",
            string, 
            name, 
            name
        ).data()
    );
}

std::string parseToGraphvizTree(std::string string)
{
    Lexer lexer { std::move(string) };
    size_t id = 0;
    return std::format(
        "strict graph G {{ {} }}", 
        visit(parseProgram(lexer), id)
    );
}

int main()
{
    // const auto file_path = std::filesystem::current_path() / "code.xyz";
    const auto file_path = "/Users/artem.karpov/University/Sem2/Cc/Lab3/code.xyz";

    std::cout << file_path << std::endl;

    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    graphviz(
        parseToGraphvizTree(
            buffer.str()
        ),
        "ast"
    );
}
