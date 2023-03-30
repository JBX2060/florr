#include <Server/Component/Life.hh>

#include <cstdint>

#include <BinaryCoder/BinaryCoder.hh>
#include <BinaryCoder/NativeTypes.hh>

#include <Server/Simulation.hh>

namespace app::component
{
    Life::Life(Entity parent, Simulation *simulation)
        : m_Parent(parent),
          m_Simulation(simulation)
    {
    }

    Life::~Life()
    {
    }

    void Life::Reset()
    {
        m_State = 0;
    }

    void Life::Write(bc::BinaryCoder &coder, Life life, bool isCreation)
    {
        uint32_t state = isCreation ? 0b11 : life.m_State;
        coder.Write<bc::VarUint>(state);

        if (state & 1)
            coder.Write<bc::Float32>(life.m_Health);
        if (state & 2)
            coder.Write<bc::Float32>(life.m_MaxHealth);
    }

    float Life::Health() const { return m_Health; }
    float Life::MaxHealth() const { return m_MaxHealth; }

    void Life::Health(float v)
    {
        if (v < 0)
            v = 0;
        if (v == m_Health)
            return;
        m_Health = v;
        m_State |= 1 << 0;
    }

    void Life::MaxHealth(float v)
    {
        if (v == m_MaxHealth)
            return;
        m_MaxHealth = v;
        m_State |= 1 << 1;
    }
}
