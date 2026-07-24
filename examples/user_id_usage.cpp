#include <iostream>
#include <tejoy/user.hpp>

int main()
{
    try
    {
        const auto alice = tejoy::UserId::generate();
        const auto bob = tejoy::UserId::generate();

        const std::string message = "Hello, World!";

        const std::string cypher = alice.encrypt(message, bob);
        const std::string out = bob.decrypt(cypher, alice);

        std::cout << cypher << std::endl;
        std::cout << out << std::endl;
    }
    catch (const tejoy::errors::sodium_error& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
