/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
 *
 * This file is part of SoDA.
 *
 *  SoDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SoDA is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with SoDA.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
  * @file Main program of the instrumen server.
  *       The instrument server communicating with the instrumented code through
  *       sockets and saves the trace informations in SoDA binary format.
  */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <exception>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>

#include "boost/program_options.hpp"
#include "boost/thread.hpp"

#include "CTraceData.h"
#include "CTraceLogger.h"

using namespace soda;

namespace po = boost::program_options;

po::options_description desc("Allowed options");

std::string socketName;
std::vector<int> sockets;
std::vector<boost::thread *> threads;
CTraceData *data;
int serverSocket;
String coverageFilePath;
String baseDir;
String codeElementPath;
bool dumpCodeElements = false;

/**
 * @brief Starts the instrument server and waiting for connections.
 */
void startServer()
{
    struct sockaddr_un name;

    /* Create the socket. */
    serverSocket = socket(PF_LOCAL, SOCK_STREAM, 0);

    /* Indicate that this is a server. */
    name.sun_family = AF_LOCAL;
    strcpy(name.sun_path, socketName.c_str());
    bind(serverSocket, (struct sockaddr*)&name, SUN_LEN (&name));

    /* Listen for connections. */
    listen(serverSocket, SOMAXCONN);

}

/**
 * @brief Cathces the interrupt signal(CTRL+c) and stops the execution of the
 *        server.
 * @param signal
 */
void signalHandler(int signal)
{
    (std::cout << std::endl << "CTRL+c was pressed. Stopping server... ").flush();
    for (std::vector<boost::thread *>::iterator it = threads.begin(); it != threads.end(); it++) {
        boost::thread *thread = *it;
        thread->join();

        delete thread;
    }
    std::cerr << std::endl;
    threads.clear();

    for (std::vector<int>::iterator it = sockets.begin(); it != sockets.end(); it++) {
        close(*it);
    }

    data->save();

    // Dump the code elements location into the output file.
    if (dumpCodeElements) {
        std::ofstream out;
        out.open(codeElementPath.c_str());
        std::set<String> codeElements = data->getCodeElementLocations();
        for (std::set<String>::iterator it = codeElements.begin(); it != codeElements.end(); it++) {
            out << *it << std::endl;
        }
        out.close();
    }

    delete data;

    close(serverSocket);
    unlink(socketName.c_str());

    std::cout << "done." << std::endl;
    exit(0);
}

/**
 * @brief Process arguments.
 * @param ac The number of arguments.
 * @param av The arguments.
 * @return
 */
int processArgs(int ac, char *av[])
{
    po::positional_options_description p;
    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    // Process arguments.
    if (!vm.count("coverage-data") && !vm.count("base-dir")) {
        std::cout << "Missing options!" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }

    coverageFilePath = vm["coverage-data"].as<String>();
    baseDir = vm["base-dir"].as<String>();
    if (vm.count("dump-code-elements")) {
        dumpCodeElements = true;
        codeElementPath = vm["dump-code-elements"].as<String>();
    }
    socketName = vm["socket-file"].as<String>();

    // Create signal handler.
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    // Start the server.
    startServer();
    std::cout << "Server started. Press CTRL+c to save results and finish execution." << std::endl;
    data = new CTraceData(coverageFilePath, baseDir);
    do {
        struct sockaddr_un clientName;
        socklen_t clientNameLength;

        /* Accept a connection. */
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientName, &clientNameLength);
        // no new connection
        if (clientSocket < 0)
            continue;

        /* Handle the connection. */
        CTraceLogger logger = CTraceLogger(clientSocket, data);
        // Start a thread.
        boost::thread *thread = new boost::thread(logger);
        // Save it for later use.
        threads.push_back(thread);
        sockets.push_back(clientSocket);
    }
    while (true);

    return 0;
}


int main(int argc, char *argv[])
{
    // Usage
    desc.add_options()
            ("help,h", "help message")
            ("base-dir,d", po::value<String>(), "base directory of where the make check was started")
            ("coverage-data,c", po::value<String>(), "output file containing the coverage matrix")
            ("socket-file,s", po::value<String>()->default_value("/tmp/instrument-server"), "the temporary file that can be used for communication")
            ("dump-code-elements", po::value<String>(), "output file containing the code elements and their location")
    ;

    if (argc < 2) {
        std::cerr << "ERROR: There are no arguments!" << std::endl << desc << std::endl;
        return 1;
    }

    return processArgs(argc, argv);
}
