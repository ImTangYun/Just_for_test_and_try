/*
 *by:yuntang
 *filename:transmission.h
 *
 */
#include "transmission.h"
#include "Cthread.h"

namespace myspace
{
using cthread::CThread;
Transmission::Transmission(SocketContent* listen_content, EventPoller* event_poller)
{
	trans_thread_ = new CThread;
}
Transmission::~Transmission()
{
}
}
