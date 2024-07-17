//#include <gtest/gtest.h>
#include <fstream>
#include "client_logger.h"
#include "client_logger_builder.h"



int main(
    int argc,
    char *argv[])
{
    //testing::InitGoogleTest(&argc, argv);
    setlocale(LC_ALL, "Russian");

    client_logger_builder b;
    b.add_stream("C:\\Users\\cucurbita\\Desktop\\log.txt", logger::severity::trace);
    b.add_stream("C:\\Users\\cucurbita\\Desktop\\log.txt", logger::severity::debug);
    b.set_format("%d | %t | %s | %m");

    client_logger_builder d;
    d.add_stream("C:\\Users\\cucurbita\\Desktop\\log.txt", logger::severity::critical);
    d.set_format("%s | %d | %t | %m");
    client_logger* cld = dynamic_cast<client_logger*>(d.build());
    d.clear();
    client_logger* cl = dynamic_cast<client_logger*>(b.build());
    b.clear();

    std::ifstream in("C:\\Users\\cucurbita\\Desktop\\osi22.txt", std::ifstream::in);
    std::ofstream out("C:\\Users\\cucurbita\\Desktop\\osi23.txt", std::ofstream::out);

    cld->log("division by zer0", logger::severity::critical);
    cld->~client_logger();

    char c = 32;
    while((c = in.get()) != EOF)
    {
        out << c;
        cl->log("hello", logger::severity::trace);
        cl->log("omg hi", logger::severity::debug);
    }
    cl->~client_logger();
    //return RUN_ALL_TESTS();
}