#pragma once
#include "../connection/connection.hpp"
class ConfigManager
{
public:
    void addRecentConnection(const Connection &connection);
    
private:
    void initRecent();
};
