//
//
//
//
#ifndef END_POINT
#define END_POINT
class StreamSocketContext;
class EndPoint
{
    public:
        EndPoint(StreamSocketContext* socket_contxt):socket_contxt_(socket_contxt) {}
        StreamSocketContext* socket_context() {return socket_contxt_;}
        void set_socket_context(StreamSocketContext* socket_contxt) {socket_contxt_ = socket_contxt;}
    private:
        StreamSocketContext* socket_contxt_;
};
#endif
