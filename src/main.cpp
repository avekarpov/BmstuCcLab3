#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "graphviz_vizitor.hpp"
#include "polish_attribution.hpp"

NProgramPtr parse(std::string string)
{
    Lexer lexer { std::move(string) };
    return parseProgram(lexer);
}

template <class Ptr>
void graphviz(Ptr ptr, const std::string_view name)
{   
    size_t id = 0;
    const auto graph = std::format(
        "strict graph G {{ {} }}", 
        visit(ptr, id)
    );

    std::system(
        std::format(
            R"(echo '{}' | dot -Tsvg > {}.svg && open -a "Google Chrome" ./{}.svg)",
            graph, 
            name, 
            name
        ).data()
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

    auto ptr = parse(buffer.str());
    // graphviz(ptr, "ast");
    std::cout << attr(ptr) << std::endl;
}
