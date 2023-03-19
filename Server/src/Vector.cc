#include <Vector.hh>

#include <cmath>

namespace app
{
    Vector::Vector(float x, float y)
    {
        m_X = x;
        m_Y = y;
    }

    void Vector::Set(float x, float y)
    {
        m_X = x;
        m_Y = y;
    }

    Vector &Vector::operator=(Vector v)
    {
        m_X = v.m_X;
        m_Y = v.m_Y;
        return *this;
    }
    Vector &Vector::operator+=(Vector v)
    {
        m_X += v.m_X;
        m_Y += v.m_Y;
        return *this;
    }
    Vector &Vector::operator-=(Vector v)
    {
        m_X -= v.m_X;
        m_Y -= v.m_Y;
        return *this;
    }
    Vector &Vector::operator*=(float v)
    {
        m_X *= v;
        m_Y *= v;
        return *this;
    }
    Vector Vector::operator+(Vector v)
    {
        return Vector(m_X + v.m_X, m_Y + v.m_Y);
    }
    Vector Vector::operator-(Vector v)
    {
        return Vector(m_X - v.m_X, m_Y - v.m_Y);
    }
    Vector Vector::operator*(float v)
    {
        return Vector(m_X * v, m_Y * v);
    }
    float Vector::Magnitude()
    {
        return std::sqrt(m_X * m_X + m_Y * m_Y);
    }
    Vector &Vector::Normalize()
    {
        float scale = 1 / Magnitude();
        m_X *= scale;
        m_Y *= scale;
        return *this;
    }
    bool Vector::operator>(float v)
    {
        return Magnitude() > v;
    }
    bool Vector::operator<(float v)
    {
        return Magnitude() < v;
    }
}