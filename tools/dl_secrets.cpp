#include <iostream>
#include <boost/lexical_cast.hpp>
#include <darkleaks.hpp>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: dl_secrets DOCUMENT CHUNKS" << std::endl;
        std::cerr << "Good default for CHUNKS is 100" << std::endl;
        return -1;
    }
    const std::string document_filename = argv[1];
    const std::string chunks_str = argv[2];
    size_t chunks = 0;
    try
    {
        chunks = boost::lexical_cast<size_t>(chunks_str);
    }
    catch (const boost::bad_lexical_cast&)
    {
        std::cerr << "dl_secrets: bad CHUNKS provided." << std::endl;
        return -1;
    }
    auto result = darkleaks::secrets(document_filename, chunks);
    for (auto secret_key: result)
        std::cout << secret_key << std::endl;
    return 0;
}

