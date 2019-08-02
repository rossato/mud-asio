#include "HttpInterface.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>
#include "Server/Ansi.hpp"
#include "Server/ConnectionBase.hpp"
#include "Server/Server.hpp"

using namespace Mud::Http;

// This is awful never implement HTTP yourself.

void HttpInterface::HandleLine(const std::string &line)
{
    std::istringstream lineStream(line);
    if (line.empty() || line[0] == '\r')
    {
        ExecuteRequest();
    }
    else if (m_method.empty())
    {
        // Request line
        lineStream >> m_method >> m_url >> m_version;

        // Not parsing the url, so it doesn't matter
        
        // if (!m_url.compare(0, 7, "http://") ||
        //     !m_url.compare(0, 8, "https://"))
        // {
        //     auto hostStart = m_url.find(':') + 3;
        //     auto hostEnd = m_url.find('/', hostStart + 1);

        //     if (hostEnd == std::string::npos)
        //     {
        //         m_requestHeaders["Host"] = m_url.substr(hostStart);
        //         m_url = "/";
        //     }
        //     else
        //     {
        //         m_requestHeaders["Host"] = m_url.substr(hostStart, hostEnd-hostStart);
        //         m_url.erase(0, hostEnd);
        //     }
        // }
    }
    else
    {
        // "Header: option" line
        std::pair<std::string, std::string> header;

        lineStream >> header.first >> header.second;
        header.first.erase(header.first.find(':'));

        m_requestHeaders.insert(header);
    }
}

void HttpInterface::ExecuteRequest()
{
    int status = 200;
    std::string response = "OK";
    
    if (m_version.compare(0, 7, "HTTP/1."))
    {
        status = 505;
        response = "HTTP Version Not Supported";
    }

    if (m_method != "GET" && m_method != "HEAD")
    {
        status = 501;
        response = "Not Implemented";
    }

    auto time = std::time(nullptr);
    *this << "HTTP/1.1 " << status << " " << response << NEWLINE
          << "Date: "
        // "Thu, 18 Oct 2018 13:44:35 GMT" format per RFC 2616
          << std::put_time(std::gmtime(&time), "%a, %d %b %Y %T %Z")
          << NEWLINE
          << "Server: mud-asio/1.0" NEWLINE
          << "Connection: close" NEWLINE;

    if (status == 200)
    {
        auto sizes = m_server.GetNumberOfConnections();
        
        std::ostringstream contentStream;
        contentStream
            << "{" NEWLINE
            << "  \"status\": \"ok\"," NEWLINE
            << "  \"currentConnections\": " << sizes.first << "," NEWLINE
            << "  \"maxConnections\": " << sizes.second << NEWLINE
            << "}" NEWLINE;
        
        std::string content = contentStream.str();

        *this << "Content-Type: application/json" NEWLINE
              << "Content-Length: " << content.size() << NEWLINE
              << NEWLINE;

        if (m_method == "GET")
        {
            *this << content;
        }
    }
    else
    {
        *this << NEWLINE;
    }

    Close("HTTP response completed");
}
