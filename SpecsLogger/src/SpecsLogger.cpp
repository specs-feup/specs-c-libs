//
// Created by pedro on 14-06-2017.
//

#include "SpecsLogger.h"
#include <iostream>





void StreamLogger::setStream(std::ostream &stream) {
    this->stream.rdbuf(stream.rdbuf());
}

StreamLogger::StreamLogger() : stream(NULL) {}

ConsoleLogger::ConsoleLogger() {
    setStream(std::cout);
}

//std::ofstream open_ostream(const std::string &name);
void open_ostream(std::ofstream &out_stream, const std::string &name);

FileLogger::FileLogger(const std::string &name) : fileStream(NULL) {
    //std::ofstream ofstream = open_ostream(name);
    open_ostream(this->fileStream, name);
    //this->fileStream = open_ostream(name);
    //this->fileStream.rdbuf(open_ostream(name).rdbuf());
    setStream(fileStream);
}


//std::ofstream open_ostream(std::ofstream &out_stream, const std::string &name) {
/**
 * Passing the stream as reference for compatibility with GCC < 5
 * @param out_stream
 * @param name
 */
void open_ostream(std::ofstream &out_stream, const std::string &name) {

        //std::ofstream out_stream;

        out_stream.open( name.c_str(), std::ios_base::binary|std::ios_base::out|std::ios_base::app );
        if( !out_stream.is_open() )
        {
            throw(std::runtime_error("LOGGER: Unable to open an output stream"));
        }

        //return out_stream;
}


