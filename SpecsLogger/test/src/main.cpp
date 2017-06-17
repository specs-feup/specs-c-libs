#include <iostream>

#include "SpecsLogger.h"

int main() {

    ConsoleLogger cLog;

    cLog.info("Hello ", 2, " World");
    cLog.msg("A Simple Print");
    FileLogger fLog("log.txt");
    fLog.info("File");
//    FileLogPolicy fLog("/home/pedro/log_test.txt");
/*
    Logger<ConsoleLogPolicy> cLog("console log");
    Logger<FileLogPolicy> fLog("/home/pedro/log_test.txt");

    double xpto = 42.0;


    cLog.info("coisas ", 13, " ", xpto);
    fLog.info();
*/
//    cLog.warn("coisas ", 13, " ", xpto);
//    cLog.err("coisas ", 13, " ", xpto);

    return 0;
}
