//
// Created by pedro on 14-06-2017.
//

#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include <fstream>
#include <string>
#include <memory>
#include <stdarg.h>
#include <iostream>
#include <sstream>

/*
class FileLogPolicy {

    std::ofstream out_stream;

public:
    FileLogPolicy() {}

    void open_ostream(const std::string &name) {

        out_stream.open(name.c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::app);
        if (!out_stream.is_open()) {
            throw (std::runtime_error("LOGGER: Unable to open an output stream"));
        }
    }

    template<typename T>
    void write(const T &msg) {

        out_stream << msg;
    }

    template<typename T>
    void writeErr(const T &msg) {

        write(msg);
    }
};


class ConsoleLogPolicy {

public:
    ConsoleLogPolicy() {}

    void open_ostream(const std::string &name) {};

    template<typename T>
    void write(const T &msg) {

        std::cout << msg;
    }

    template<typename T>
    void writeErr(const T &msg) {

        std::cerr << msg;
    }
};

template<typename P>
class Logger {

    P policy;

    template<typename T>
    void write(T t) {

        policy.write(t);
        policy.write("\n");
    }

    template<typename T, typename... Rest>
    void write(T t, Rest... rest) {
        policy.write(t);
        write(rest...);
    };

//    template<typename T>
//    void writeErr(std::stringstream ss, T t) {
//
//        ss << t;
//
//        policy.writeErr(ss.str());
//        policy.writeErr("\n");
//    }
//
//    template<typename T, typename... Rest>
//    void writeErr(std::stringstream ss, T t, Rest... rest) {
//        ss << t;
//        write(ss, rest...);
//    }

public:

    explicit Logger(const std::string &name) {
        policy.open_ostream(name);
    }

    template<typename... Args>
    void info(Args... args) {

        policy.write("[INFO] ");
        write(args...);
    }

//    template<typename... Args>
//    void warn(Args... args) {
//
//        std::stringstream ss;
//        policy.write("[WARN] ");
//        write(ss, args...);
//    }
//
//    template<typename... Args>
//    void err(Args... args) {
//
//        std::stringstream ss;
//        policy.write("[ERR] ");
//        writeErr(ss, args...);
//    }
};

*/
class StreamLogger {

protected:

   explicit StreamLogger();

    std::ostream stream;

    template<typename T>
    void write(T t) {

        stream << t << "\n";
    }

    template<typename T, typename... Rest>
    void write(T t, Rest... rest) {
        stream << t;
        write(rest...);
    };


    void setStream(std::ostream& stream);

public:

//    StreamLogger(std::ostream &stream) : stream(stream) {}

    template<typename... Args>
    void msg(Args... args) {
        write(args...);
    }

    template<typename... Args>
    void info(Args... args) {

        stream << "[INFO] ";
        write(args...);
    }

    template<typename... Args>
    void warn(Args... args) {

        stream << "[WARN] ";
        write(args...);
    }

    template<typename... Args>
    void err(Args... args) {

        stream << "[ERR] ";
        write(args...);
    }
};


class ConsoleLogger : public StreamLogger {

public:
    explicit ConsoleLogger();
};

class FileLogger : public StreamLogger {

    std::ofstream fileStream;

public:

    explicit FileLogger(const std::string &name);

};

#endif //LOGGER_LOGGER_H
