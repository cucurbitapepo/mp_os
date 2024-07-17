#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"
#include <map>
#include <set>
#include <unordered_map>

class client_logger final:
    public logger
{
private:

    std::string _format;
    std::map<std::string, std::pair<std::ofstream*, std::set<logger::severity>>> _streams;
    static std::unordered_map<std::string, std::pair<std::ofstream*, size_t>> _all_streams;

public:

    client_logger() = default;

    client_logger(
        client_logger const &other);

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;

    client_logger &operator=(
        client_logger &&other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

    void set_format(
            std::string const &format) noexcept;

    static void increment_all_streams()
    {
        for(auto iter = _all_streams.cbegin(); iter != _all_streams.cend(); ++iter)
        {
            ++_all_streams.find(iter->first)->second.second;
        }
    }

    static void close_all_streams()
    {
        for(auto iter = _all_streams.cbegin(); iter != _all_streams.cend(); ++iter)
        {
            if(_all_streams.find(iter->first)->second.second == 0)
                _all_streams.find(iter->first)->second.first->close();
        }
    }

    void insert_stream(
            const std::string &path,
            const std::set<logger::severity> &severity);

private:
    static void print_message(
            std::ofstream *stream,
            const std::string &format,
            const client_logger::severity &severity,
            const std::string &message)
    {

        size_t length = format.length();
        std::string date_time = current_datetime_to_string();
        //("%d.%m.%Y %H:%M:%S")

        for(size_t i = 0; i < length; ++i) {
            if (format[i] == '%') {
                if (i + 1 == length) {
                    *stream << format[i];
                    continue;
                }
                i++;
                switch (format[i])
                {
                    case 'd':
                    {
                        *stream << date_time.substr(0, date_time.find(' '));
                        continue;
                    }
                    case 't':
                    {
                        *stream << date_time.substr(date_time.find(' ') + 1);
                        continue;
                    }
                    case 's':
                    {
                        *stream << severity_to_string(severity);
                        continue;
                    }
                    case 'm':
                    {
                        *stream << message;
                        continue;
                    }

                    default:
                    {
                        *stream << format[i - 1] << format[i];
                        continue;
                    }
                }
            }
            else
            {
                *stream << format[i];
            }
        }
        *stream << std::endl;
    }
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H