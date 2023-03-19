#include <websocketpp/config/asio_no_tls.hpp>

#include <vector>

#include <websocketpp/server.hpp>

#include <Client.hh>
#include <Simulation.hh>

namespace
{
    typedef websocketpp::server<websocketpp::config::asio> WebSocketServer;
}

namespace app
{
    class Server
    {
        uint32_t m_TickCount = 0;
        std::vector<Client *> m_Clients;

    public:
        Simulation m_Simulation;
        WebSocketServer m_Server;

        Server();

        void Run();
        void OnClientConnect(websocketpp::connection_hdl);
        void OnClientDisconnect(websocketpp::connection_hdl);
        void Tick();
    };
}