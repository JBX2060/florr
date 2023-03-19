#include <Client.hh>

#include <iostream>

#include <BinaryCoder/BinaryCoder.hh>
#include <BinaryCoder/NativeTypes.hh>

#include <Server.hh>
#include <Simulation.hh>

namespace app
{
    Client::Client(websocketpp::connection_hdl hdl, Simulation &simulation)
        : m_Hdl(hdl),
          m_Simulation(simulation)
    {
        std::cout << "client create\n";
    }

    Client::~Client()
    {
        std::cout << "client destroy\n";
    }

    void Client::BroadcastUpdate()
    {
        static uint8_t data[1024 * 1024];
        bc::BinaryCoder coder{data};
        coder.Write<bc::Uint8>(0);
        m_Simulation.WriteUpdate(coder, m_Camera);

        SendPacket(coder);
    }

    void Client::Tick()
    {
        BroadcastUpdate();
    }

    void Client::SendPacket(bc::BinaryCoder const &data) const
    {
        std::string packet{data.Data(), data.Data() + data.At()};
        m_Simulation.m_Server.m_Server.send(m_Hdl, packet, websocketpp::frame::opcode::binary);
    }

    websocketpp::connection_hdl Client::GetHdl()
    {
        return m_Hdl;
    }
}