/*
 *by:yuntang
 *filename:yuntang
 *
 */
#include "http_socket_handler.h"
namespace http
{
HttpSocketHandler::HttpSocketHandler(int port):ListenHandler(port)
{
}
HttpSocketHandler::~HttpSocketHandler()
{
}
} // namespace http
