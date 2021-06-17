#include "db-config-reader.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <vector>

#include <fstream>
#include <filesystem>

#include <cstdlib>

#include <exception>


namespace db_config_reader
{

    std::string get_config(const std::string& file_path)
    {
        if (!std::filesystem::exists(file_path))
        {
            throw std::runtime_error(fmt::format("File: {} not found", file_path));
        }

        std::ifstream input(file_path);

        if (input.fail())
        {
            throw std::runtime_error(fmt::format("Unable to open file: {}", file_path)); 
        }
        
        nlohmann::json config;
        input >> config;
        return con_str(config); 
    }
        
    std::string get_config(nlohmann::json& config, const bool env)
    {
        
        if (env && verify_config(config))
        {
            nlohmann::json _config;
            std::string err_pattern = "Enviroment variable: {} missing for {}";
            
            if (const char* database = std::getenv(config[keys::DATABASE].get<std::string>().c_str()))
            {
                _config[keys::DATABASE] = std::string(database);
            }
            else
            {
                throw std::runtime_error(fmt::format(err_pattern, std::string(database), keys::DATABASE)); 
            }

            if (const char* user = std::getenv(config[keys::USER].get<std::string>().c_str()))
            {
                _config[keys::USER] = std::string(user);
            }
            else
            {
                throw std::runtime_error(fmt::format(err_pattern, std::string(user), keys::USER)); 
            }

            if (const char* password = std::getenv(config[keys::PASSWORD].get<std::string>().c_str()))
            {
                _config[keys::PASSWORD] = std::string(password);
            }
            else
            {
                throw std::runtime_error(fmt::format(err_pattern, std::string(password), keys::PASSWORD)); 
            }

            if (const char* host_address = std::getenv(config[keys::HOST_ADDRESS].get<std::string>().c_str()))
            {
                _config[keys::HOST_ADDRESS] = std::string(host_address);
            }
            else
            {
                throw std::runtime_error(fmt::format(err_pattern, std::string(host_address), keys::HOST_ADDRESS)); 
            }
            
            if (const char* port = std::getenv(config[keys::PORT].get<std::string>().c_str()))
            {
                _config[keys::PORT] = std::string(port);
            }
            else
            {
                throw std::runtime_error(fmt::format(err_pattern, std::string(port), keys::PORT)); 
            }
            return con_str(config); 
        }
        else if (env && !verify_config(config))
        {
            cast_invalid_config(config); 
        }
        else
        {
            return con_str(config); 
        }
    }
    

    bool verify_config(const nlohmann::json& config)
    {
        return config.find("database") != config.end() && config.find("user") != config.end()
            && config.find("password") != config.end() && config.find("host_address") != config.end()
            && config.find("port") != config.end();
    }

    void cast_invalid_config(const nlohmann::json& invalid_config)
    {
        std::vector<std::string> elms; 
        if (invalid_config.find("database") == invalid_config.end())
        {
            elms.emplace_back("database"); 
        }

        if (invalid_config.find("user") == invalid_config.end())
        {
            elms.emplace_back("user");
        }

        if (invalid_config.find("password") == invalid_config.end())
        {
            elms.emplace_back("password");
        }

        if (invalid_config.find("host_address") == invalid_config.end())
        {
            elms.emplace_back("host_address");
        }

        if (invalid_config.find("port") == invalid_config.end())
        {
            elms.emplace_back("port");
        }

        std::string msg = fmt::format("Database configuration is missing the following elments: {}", elms);
        throw std::runtime_error(msg); 
        
    }
    
    std::string con_str(const nlohmann::json& config, bool verified)
    {
        if (!verified && !verify_config(config))
        {
            cast_invalid_config(config); 
        }

        return fmt::format("dbname = {} user = {} hostaddr = {} port = {}",
                    config["database"].get<std::string>(),
                    config["user"].get<std::string>(), config["password"].get<std::string>(),
                    config["host_address"].get<std::string>(), config["port"].get<std::string>());
    }

    

}
