#ifndef DB_CONFIG_READER_HPP
#define DB_CONFIG_READER_HPP

#include <nlohmann/json.hpp>

#include <string>

namespace db_config_reader
{
namespace keys
{
    const std::string DATABASE = "database";
    const std::string USER = "user";
    const std::string PASSWORD = "password";
    const std::string HOST_ADDRESS = "host_address";
    const std::string PORT = "port";
}

    /// READ JSON FILE FROM DISK 
    std::string get_config(const std::string& file_path);
    std::string get_config(nlohmann::json& config, const bool env=false);

    bool verify_config(const nlohmann::json& config);

    void cast_invalid_config(const nlohmann::json& invalid_config); 
    
    std::string con_str(const nlohmann::json& config, bool verified=false); 
}


#endif /* DB_CONFIG_READER_HPP */ 
