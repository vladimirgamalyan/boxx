#include "file2vec.h"

std::vector<uint8_t> file2vec(const std::string& fileName)
{
    std::ifstream f(fileName);
    f.seekg(0, std::ios::end);
    size_t size = f.tellg();

    std::vector<uint8_t> v(size);
    f.seekg(0);
    f.read(&v[0], size);

    return v;
}
