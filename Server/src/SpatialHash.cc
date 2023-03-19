#include <SpatialHash.hh>

#include <cstdint>
#include <vector>
#include <array>

#include <Simulation.hh>

namespace app
{
    SpatialHash::SpatialHash(Simulation &simulation)
        : m_Simulation(simulation)
    {
    }

    void SpatialHash::Insert(Entity id)
    {
        component::Position position = m_Simulation.Get<component::Position>(id);
        int32_t entityX = (int32_t)position.X();
        int32_t entityY = (int32_t)position.Y();
        int32_t entitySize = (int32_t)m_Simulation.Get<component::Physics>(id).Radius();

        int32_t sx = (entityX - entitySize) >> GRID_SIZE;
        int32_t sy = (entityY - entitySize) >> GRID_SIZE;
        int32_t ex = (entityX + entitySize) >> GRID_SIZE;
        int32_t ey = (entityY + entitySize) >> GRID_SIZE;
        for (int32_t y = sy; y <= ey; y++)
        {
            for (int32_t x = sx; x <= ex; x++)
            {
                uint32_t hash = ((x << 16) | (y >> 16)) % HASH_TABLE_SIZE;
                m_Cells[hash].push_back(id);
            }
        }
    }

    std::vector<Entity> SpatialHash::GetCollisions(int32_t x, int32_t y, int32_t w, int32_t h)
    {
        std::vector<Entity> result = {};
        int32_t sx = (x - w) >> GRID_SIZE;
        int32_t sy = (y - h) >> GRID_SIZE;
        int32_t ex = (x + w) >> GRID_SIZE;
        int32_t ey = (y + h) >> GRID_SIZE;
        for (int32_t y = sy; y <= ey; y++)
            for (int32_t x = sx; x <= ex; x++)
            {
                uint32_t hash = ((x << 16) | (y >> 16)) % HASH_TABLE_SIZE;
                std::vector<Entity> &cell = m_Cells[hash];
                for (uint64_t i = 0; i < cell.size(); i++)
                {
                    component::Physics &physics = m_Simulation.Get<component::Physics>(cell[i]);
                    if (physics.m_QueryId != m_QueryId)
                    {
                        physics.m_QueryId = m_QueryId;
                        result.push_back(cell[i]);
                    }
                }
            }

        m_QueryId++;
        return result;
    }
    std::vector<Entity> SpatialHash::GetCollisions(Entity id)
    {
        int32_t entityX = (int32_t)m_Simulation.Get<component::Position>(id).X();
        int32_t entityY = (int32_t)m_Simulation.Get<component::Position>(id).Y();
        int32_t entitySize = (int32_t)m_Simulation.Get<component::Physics>(id).Radius();

        return GetCollisions(entityX, entityY, entitySize, entitySize);
    }
    void SpatialHash::Clear()
    {
        for (int32_t i = 0; i < HASH_TABLE_SIZE; i++)
            m_Cells[i].clear();
    }
}