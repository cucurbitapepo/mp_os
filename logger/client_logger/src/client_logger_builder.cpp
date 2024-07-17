#include <not_implemented.h>

#include "../include/client_logger_builder.h"
#include "../include/client_logger.h"

client_logger_builder::client_logger_builder(
    client_logger_builder const &other)
    : _streams(other._streams),
      _format(other._format)
{}

client_logger_builder &client_logger_builder::operator=(
    client_logger_builder const &other)
{
    if(this != &other)
    {
        this->_streams = other._streams;
        this->_format = other._format;
    }
    return *this;
}

client_logger_builder::client_logger_builder(
    client_logger_builder &&other) noexcept
{
    this->_streams = std::move(other._streams);
    this->_format = std::move(other._format);
}

client_logger_builder &client_logger_builder::operator=(
    client_logger_builder &&other) noexcept
{
    if(this != &other)
    {
        this->_streams = std::move(other._streams);
        this->_format = std::move(other._format);
    }
    return *this;
}

client_logger_builder::~client_logger_builder() noexcept
{
    clear();
}

logger_builder *client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{

    if(!stream_file_path.empty())
    {
        auto path = std::filesystem::absolute(stream_file_path);
        _streams[path.string()].insert(severity);
    }

    return this;
}

logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
    _streams[""].insert(severity);
    return this;
}

void client_logger_builder::add_stream(
        std::string const &stream_file_path,
        logger::severity severity) noexcept
{
    if(stream_file_path.empty())
    {
        add_console_stream(severity);
    }
    else
    {
        add_file_stream(stream_file_path, severity);
    }
}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    std::ifstream file(configuration_file_path);

    nlohmann::json data = nlohmann::json::parse(file);
    nlohmann::json object;
    try
    {
        object = data.at(configuration_path);
    }
    catch(nlohmann::json::out_of_range &exception)
    {
        throw std::runtime_error("No '" + configuration_file_path + "' path was found");
    }
    //operator[] instead of .at() bc conf_file is guaranteed to be of a correct content and [] doesn't check bounds
    std::string log_file = object["file"].get<std::string>();
    std::vector<logger::severity> severities = object["severities"].get<std::vector<logger::severity>>();

    size_t len = severities.size();
    for(size_t i = 0; i <= len; i++)
    {
        add_stream(log_file, severities[i]);
    }
    return this;
}

logger_builder *client_logger_builder::clear()
{
    _streams.clear();
    return this;
}

logger *client_logger_builder::build() const
{
    auto *logger = new client_logger;

    for (auto iter = this->_streams.begin(); iter != this->_streams.end(); ++iter)
    {
        logger->insert_stream(iter->first, iter->second);
    }
    logger->set_format(this->_format);
    return logger;
}

void client_logger_builder::set_format(
        const std::string &format) noexcept
{
    this->_format.clear();
    this->_format = format;
}