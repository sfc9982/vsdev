#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/ThreadPool.h"
#include "Poco/Timestamp.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/ServerApplication.h"

#include <iostream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

class TimeRequestHandler : public HTTPRequestHandler {
public:
    TimeRequestHandler(std::string format);

    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) override {
        const Application &app = Application::instance();
        app.logger().information("Request from %s", request.clientAddress().toString());

        const Timestamp   now;
        const std::string dt(DateTimeFormatter::format(now, _format));

        response.setChunkedTransferEncoding(true);
        response.setContentType("text/html");

        std::ostream &ostr = response.send();
        ostr << "<html><head><title>HTTPTimeServer powered by POCO C++ Libraries</title>"
             << "<meta http-equiv=\"refresh\" content=\"1\"></head>"
             << "<body><p style=\"text-align: center; font-size: 48px;\">"
             << dt
             << "</p></body></html>";
    }

private:
    std::string _format;
};

TimeRequestHandler::TimeRequestHandler(std::string format)
    : _format(std::move(format)) {
}

class TimeRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    TimeRequestHandlerFactory(std::string format)
        : _format(std::move(format)) {
    }

    HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &request) override {
        if (request.getURI() == "/")
            return new TimeRequestHandler(_format);
        return nullptr;
    }

private:
    std::string _format;
};

class HTTPTimeServer : public Poco::Util::ServerApplication {
protected:
    void initialize(Application &self) override {
        loadConfiguration();
        ServerApplication::initialize(self);
    }

    void defineOptions(OptionSet &options) override {
        ServerApplication::defineOptions(options);

        options.addOption(
                Option("help", "h", "display argument help information")
                        .required(false)
                        .repeatable(false)
                        .callback(OptionCallback<HTTPTimeServer>(this, &HTTPTimeServer::handleHelp)));
    }

    void handleHelp(const std::string &name, const std::string &value) {
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("A web server that serves the current date and time.");
        helpFormatter.format(std::cout);
        stopOptionsProcessing();
        _helpRequested = true;
    }

    int main(const std::vector<std::string> &args) override {
        if (!_helpRequested) {
            const unsigned short port = static_cast<unsigned short>(config().getInt("HTTPTimeServer.port", 8080));
            const std::string    format(config().getString("HTTPTimeServer.format", DateTimeFormat::HTTP_FORMAT));

            const ServerSocket svs(port);
            auto               params = new HTTPServerParams;
            params->setKeepAlive(true);
            HTTPServer srv(new TimeRequestHandlerFactory(format), svs, params);
            srv.start();
            const Application &app = Application::instance();
            app.logger().information("Server listening on http://localhost:%hu", port);
            waitForTerminationRequest();
            srv.stop();
        }
        return EXIT_OK;
    }

private:
    bool _helpRequested = false;
};

int main(int argc, char **argv) {
    HTTPTimeServer app;
    return app.run(argc, argv);
}