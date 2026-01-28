#include <fstream>
#include <string_view>

struct Headers{
  
};


class FileManager{
public:
  
  FileManager(std::string_view path)
  : path_{path}
  , file_{path_}
  {}

  

  
private:
  std::string path_;
  std::ofstream file_;
};
