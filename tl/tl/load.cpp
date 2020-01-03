#include <tl/load.hpp>

#include <fstream>
#include <tl/fmt.hpp>

using namespace std;

namespace tl
{

Str loadString(CStr fileName)
{
    ifstream fs(fileName.c_str());
    if(!fs) {
        tl::printError("Error opening file: ", fileName);
        return "";
    }
    Str str;
    fs.seekg(0, ios::end);
    const unsigned n = fs.tellg();
    if(n > 0) {
        str.resize(n);
        fs.seekg(0, ios::beg);
        fs.read(&str[0], n);
    }
    fs.close();
    return str;
}

tl::Vector<char> loadData(const Str& fileName)
{
    ifstream fs(fileName.c_str());
    if(!fs)
        tl::printError("Error opening file: ", fileName);
    fs.seekg(0, ios::end);
    const unsigned n = fs.tellg();
    tl::Vector<char> data(n);
    fs.seekg(0, ios::beg);
    fs.read(&data[0], n);
    fs.close();
    return data;
}

}
