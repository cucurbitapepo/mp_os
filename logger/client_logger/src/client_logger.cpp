#include <not_implemented.h>

#include "../include/client_logger.h"

std::unordered_map<std::string, std::pair<std::ofstream*, size_t>> client_logger::_all_streams;
client_logger::client_logger(
    client_logger const &other)
    : _streams(other._streams)
{
    client_logger::increment_all_streams();
}

client_logger &client_logger::operator=(
    client_logger const &other)
{
    if(this != &other)
    {
        this->_streams = other._streams;
        increment_all_streams();
    }
    return *this;
}

client_logger::client_logger(
    client_logger &&other) noexcept
{
    //TODO: why did i add the line below?
    //client_logger::close_all_streams();
    this->_streams = std::move(other._streams);
}

client_logger &client_logger::operator=(
    client_logger &&other) noexcept
{
    if(this != &other)
    {
        //TODO: unneeded close again?????
        //client_logger::close_all_streams();
        this->_streams = std::move(other._streams);
    }

    return *this;
}

client_logger::~client_logger() noexcept
{
    for(auto iter = _streams.begin(); iter != _streams.end(); ++iter)
    {
        auto tmp = _all_streams[iter->first];
        if(tmp.second == 1)
        {
            tmp.first->close();
            if(!iter->first.empty()) delete tmp.first;
            --tmp.second;
        }
    }
}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    std::string log_path;

    for(auto iter = _streams.begin(); iter != _streams.end(); ++iter)
    {
        if(iter->second.second.find(severity) != iter->second.second.end())
        {
            log_path = iter->first;

            std::ofstream *file = log_path.empty() ? reinterpret_cast<std::ofstream*>(&std::cout) : _all_streams.find(log_path)->second.first;

            client_logger::print_message(file, _format, severity, text);
        }
    }
    return this;
}

void client_logger::insert_stream(
        const std::string &path,
        const std::set<logger::severity> &severity)
{
    auto stream = new std::ofstream;
    if(path.empty())
    {
        stream = reinterpret_cast<std::ofstream*>(&std::cout);
    }
    else
    {
        stream->open(path);
    }
    this->_streams.emplace(std::pair{path, std::make_pair(stream, severity)});

    if(_all_streams.find(path) != _all_streams.end())
    {
        ++_all_streams.find(path)->second.second;
    }
    else
    {
        _all_streams.emplace(std::pair{path, std::make_pair(stream, 1)});
    }
}

void client_logger::set_format(
        const std::string &format) noexcept
{
    this->_format = format;
}
