//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>

#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using asio::ip::tcp;

constexpr int       max_length  = 1024;
constexpr ptrdiff_t FLAG_OFFSET = 996;
constexpr char      FLAG_STR[]  = "flag{buffer_0verread}";

void session(tcp::socket sock) {
    try {
        for (;;) {
            char size[max_length] = {};
            char data[max_length] = {};
            strcpy(data + FLAG_OFFSET, FLAG_STR);

            std::error_code error;
            sock.read_some(asio::buffer(size), error);
            size_t length = sock.read_some(asio::buffer(data), error);
            length        = static_cast<size_t>(std::atoi(size));

            if (length >= max_length)
                error = asio::error::basic_errors::message_size;
            data[length] = '\0';

            if (error == asio::stream_errc::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw std::system_error(error); // Some other error.

            std::cout << "Message Length: " << length << std::endl;
            std::cout << "Message Received: " << data << std::endl;

            asio::write(sock, asio::buffer(data, length));
        }
    } catch (std::exception &e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(asio::io_context &io_context, unsigned short port) {
    tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
    for (;;) {
        tcp::socket sock(io_context);
        a.accept(sock);
        std::thread(session, std::move(sock)).detach();
    }
}

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: blocking_tcp_echo_server <port>\n"
                      << "Fallback to port 8080" << std::endl;
            constexpr char   port[] = "8080";
            constexpr size_t portsz = sizeof(port);
            argv[1]                 = new char[portsz];
            strcpy(argv[1], port);
            // strcpy_s(argv[1], portsz, port);
            // return 1;
        }

        asio::io_context io_context;

        server(io_context, static_cast<unsigned short>(std::atoi(argv[1])));
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
