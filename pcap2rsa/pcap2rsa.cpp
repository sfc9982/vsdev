#include <iostream>
#include <string>

#include <boost/regex.hpp>

#include <pcapplusplus/Packet.h>
#include <pcapplusplus/PcapFileDevice.h>
#include <pcapplusplus/TcpLayer.h>

#pragma comment(lib, "ws2_32")

#include <args.hxx>

std::string extract_payload(const std::string &payload, const std::string &parameter) {
    const boost::regex pattern(parameter + "=([^&]+)");
    boost::smatch      match;
    if (boost::regex_search(payload, match, pattern)) {
        if (match.size() > 1) {
            return match[1].str();
        }
    }
    return std::string{};
}

int main(int argc, char *argv[]) {
    args::ArgumentParser          parser("pcap2para - extract parameter of HTTP from PCAP/PCAPNG traffic files",
                                R"(Example: pcap2rsa.exe -p rsa -o test.txt "D:\Code\PayloadExtract\9.14.pcapng")");
    args::HelpFlag                help(parser, "help", "Display this help menu", {'h', "help"});
    args::CompletionFlag          completion(parser, {"complete"});
    args::ValueFlag<std::string>  extpara(parser, "parameter", "The HTTP parameter to extract", {'p', "para"});
    args::Positional<std::string> input_file(parser, "input", "The input pcap(ng) file");
    args::ValueFlag<std::string>  output_file(parser, "output", "The name of output file", {'o', "output"}, "out.txt");
    args::Flag                    debug_mode(parser, "debug", "log a counter per 1k payload", {'d', "debug"}, nullptr);
    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Completion &e) {
        std::cout << e.what();
        return 0;
    } catch (const args::Help &) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    bool debug = false;
    if (debug_mode) {
        debug = args::get(debug_mode);
        if (extpara) {
            std::cout << "parameter: " << args::get(extpara) << std::endl;
        }
        if (input_file) {
            std::cout << "input_file: " << args::get(input_file) << std::endl;
        }
        if (output_file) {
            std::cout << "output_file: " << args::get(output_file) << std::endl;
        }
    }

    std::ofstream     fout(args::get(output_file));
    const std::string pcap_path = (args::get(input_file));
    std::string       para(args::get(extpara));

    pcpp::IFileReaderDevice *reader = pcpp::IFileReaderDevice::getReader(pcap_path);
    if (reader == nullptr) {
        std::cerr << "Cannot determine reader for file type" << std::endl;
        return 1;
    }

    if (!reader->open()) {
        std::cerr << "Cannot open " + pcap_path + " for reading" << std::endl;
        return 1;
    }

    pcpp::RawPacket raw_packet;
    if (!reader->getNextPacket(raw_packet)) {
        std::cerr << "Couldn't read the first packet in the file" << std::endl;
        return 1;
    }

    int idx = 0;
    while (reader->getNextPacket(raw_packet)) {
        pcpp::Packet parsed_packet(&raw_packet);
        if (const pcpp::TcpLayer *tcp_layer = parsed_packet.getLayerOfType<pcpp::TcpLayer>()) {
            const uint8_t *data_ptr = tcp_layer->getData();
            const size_t   size     = tcp_layer->getDataLen();

            std::string payload(reinterpret_cast<std::string::const_pointer>(data_ptr), size);
            std::string rsa = extract_payload(payload, para);
            if (rsa.length() > 16) {
                if (debug) {
                    idx++;
                    if (idx % 1000 == 0)
                        std::cout << idx << '\n';
                }
                fout << rsa << '\n';
            }
        }
    }

    std::cout << std::flush;
    reader->close();

    return 0;
}
