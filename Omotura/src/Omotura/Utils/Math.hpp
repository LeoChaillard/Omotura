#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/matrix3x3.h>

namespace Omotura
{
    /*** CF Revolver4DMathEngine ****/

     ////// Vector3 ///////
    struct Vector3
    {
        float x = 0;
        float y = 0;
        float z = 0;

        Vector3() = default;

        Vector3(float _x)
        {
            this->x = _x;
            this->y = _x;
            this->z = _x;
        }

        Vector3(float _x, float _y, float _z)
        {
            this->x = _x;
            this->y = _y;
            this->z = _z;
        }

        Vector3(const Vector3& _vObject)
        {
            this->x = _vObject.x;
            this->y = _vObject.y;
            this->z = _vObject.z;
        }

        float dot(const Vector3& _vObject) const
        {
            return this->x * _vObject.x + this->y * _vObject.y + this->z * _vObject.z;
        }

        Vector3 cross(const Vector3& _vObject) const
        {
            return Vector3(this->y * _vObject.z - _vObject.y * this->z, -(this->x * _vObject.z - _vObject.x * this->z), this->x * _vObject.y - _vObject.x * this->y);
        }

        Vector3 operator+(const Vector3& _vOther) const
        {
            return Vector3(this->x + _vOther.x, this->y + _vOther.y, this->z + _vOther.z);
        }

        void operator+=(const Vector3& _vOther)
        {
            this->x += _vOther.x;
            this->y += _vOther.y;
            this->z += _vOther.z;
        }

        Vector3 operator*(float _fValue) const
        {
            return Vector3(_fValue * this->x, _fValue * this->y, _fValue * this->z);
        }

        void operator*=(float _fValue)
        {
            this->x *= _fValue;
            this->y *= _fValue;
            this->z *= _fValue;
        }

        Vector3 operator-(const Vector3& _vOther) const
        {
            return Vector3(this->x - _vOther.x, this->y - _vOther.y, this->z - _vOther.z);
        }

        void operator-=(const Vector3& _vOther)
        {
            this->x -= _vOther.x;
            this->y -= _vOther.y;
            this->z -= _vOther.z;
        }

        bool operator==(const Vector3& _vOther) const
        {
            return (this->x == _vOther.x && this->y == _vOther.y && this->z == _vOther.z);
        }

        bool operator!=(const Vector3& _vOther) const
        {
            return this->x != _vOther.x || this->y != _vOther.y || this->z != _vOther.z;
        }

        glm::vec3 ToGLM()
        {
            return glm::vec3(this->x, this->y, this->z);
        }
    };

    inline  std::ostream& operator<<(std::ostream& os, Vector3 _vObject)
    {
        os << '(' << _vObject.x << ", " << _vObject.y << ", " << _vObject.z << ')';
        return os;
    };

    ////// Vector2 ///////
    struct Vector2
    {
        float x = 0;
        float y = 0;

        Vector2() = default;

        Vector2(float _x)
        {
            this->x = _x;
            this->y = _x;
        }

        Vector2(float _x, float _y)
        {
            this->x = _x;
            this->y = _y;
        }

        Vector2(const Vector2& _vObject)
        {
            this->x = _vObject.x;
            this->y = _vObject.y;
        }

        Vector2 operator+(const Vector2& _vOther) const
        {
            return Vector2(this->x + _vOther.x, this->y + _vOther.y);
        }

        void operator+=(const Vector2& _vOther)
        {
            this->x += _vOther.x;
            this->y += _vOther.y;
        }

        Vector2 operator*(float _fValue) const
        {
            return Vector2(_fValue * this->x, _fValue * this->y);
        }

        void operator*=(float _fValue)
        {
            this->x *= _fValue;
            this->y *= _fValue;
        }

        Vector2 operator-(const Vector2& _vOther) const
        {
            return Vector2(this->x - _vOther.x, this->y - _vOther.y);
        }

        void operator-=(const Vector2& _vOther)
        {
            this->x -= _vOther.x;
            this->y -= _vOther.y;
        }

        bool operator==(const Vector2& _vOther) const
        {
            return (this->x == _vOther.x && this->y == _vOther.y);
        }

        bool operator!=(const Vector2& _vOther) const
        {
            return this->x != _vOther.x || this->y != _vOther.y;
        }
    };

    ///////Quaternion///////
    struct Quaternion
    {
        float fScalar;
        Vector3 vImaginary;

        Quaternion() = default;
        Quaternion(float _fScalar, const Vector3& _vImaginary) : fScalar(_fScalar), vImaginary(_vImaginary) {};
        Quaternion(const Quaternion& _q) : fScalar(_q.fScalar), vImaginary(_q.vImaginary) {};
        ~Quaternion() = default;

        Quaternion operator+(const Quaternion& _q) const
        {
            float fScalar = this->fScalar + _q.fScalar;
            Vector3 imaginary = this->vImaginary + _q.vImaginary;
            return Quaternion(fScalar, imaginary);
        }

        void operator+=(const Quaternion& _q)
        {
            this->fScalar += _q.fScalar;
            this->vImaginary += _q.vImaginary;
        }

        Quaternion operator-(const Quaternion& _q) const
        {
            float fScalar = this->fScalar - _q.fScalar;
            Vector3 imaginary = this->vImaginary - _q.vImaginary;
            return Quaternion(fScalar, imaginary);
        }

        void operator-=(const Quaternion& _q)
        {
            this->fScalar -= _q.fScalar;
            this->vImaginary -= _q.vImaginary;
        }

        Quaternion operator*(const Quaternion& _q) const
        {
            float fScalar = this->fScalar * _q.fScalar - this->vImaginary.dot(_q.vImaginary);
            Vector3 vImaginary = _q.vImaginary * this->fScalar + this->vImaginary * _q.fScalar + this->vImaginary.cross(_q.vImaginary);
            return Quaternion(fScalar, vImaginary);
        }

        //Quaternion operator*(const Vector3& _value)
        //{
        //    float sPart = -(this->vImaginary.x * _value.x + this->vImaginary.y * _value.y + this->vImaginary.z * _value.z);
        //    float xPart = this->fScalar * _value.x + this->vImaginary.y * _value.z - this->vImaginary.z * _value.y;
        //    float yPart = this->fScalar * _value.y + this->vImaginary.z * _value.x - this->vImaginary.x * _value.z;
        //    float zPart = this->fScalar * _value.z + this->vImaginary.x * _value.y - this->vImaginary.y * _value.x;
        //
        //    Vector3 vectorPart(xPart, yPart, zPart);
        //    Quaternion result(sPart, vectorPart);
        //
        //    return result;
        //}

        Vector3 operator*(const Vector3& _value)
        {
            Quaternion qRot = Quaternion(this->fScalar, this->vImaginary);
            Quaternion qRotConjugate = qRot.Conjugate();
            Quaternion qPure(0, _value);
            Quaternion qPureInverse = qRot * qPure * qRotConjugate;
            return qPureInverse.vImaginary;
        }

        Quaternion Conjugate()
        {
            return Quaternion(this->fScalar, Vector3(-this->vImaginary.x, -this->vImaginary.y, -this->vImaginary.z));
        }

        glm::quat ToGLM()
        {
            return glm::quat(this->fScalar, this->vImaginary.ToGLM());
        }
    };

    ///////Matrix Conversions/////
    inline glm::mat4 AiMatrix4x4ToGlm(const aiMatrix4x4& from) {
        glm::mat4 to;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;
    }

    inline glm::mat4 AiMatrix3x3ToGlm(const aiMatrix3x3& from) {
        glm::mat4 to;
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = 0.0;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = 0.0;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = 0.0;
        to[0][3] = 0.0; to[1][3] = 0.0; to[2][3] = 0.0; to[3][3] = 1.0;
        return to;
    }

    /////Interpolation///////
    inline void InterpolateQuaternion(glm::quat& Out, const glm::quat& Start, const glm::quat& End, float pFactor) {
        // calc cosine theta
        float cosom = Start.x * End.x + Start.y * End.y + Start.z * End.z + Start.w * End.w;
        // adjust signs (if necessary)
        glm::quat end = End;
        if (cosom < static_cast<float>(0.0)) {
            cosom = -cosom;
            end.x = -end.x;   // Reverse all signs
            end.y = -end.y;
            end.z = -end.z;
            end.w = -end.w;
        }
        // Calculate coefficients
        float sclp, sclq;
        if ((static_cast<float>(1.0) - cosom) > static_cast<float>(0.0001)) // 0.0001 -> some epsillon
        {
            // Standard case (slerp)
            float omega, sinom;
            omega = std::acos(cosom); // extract theta from dot product's cos theta
            sinom = std::sin(omega);
            sclp = std::sin((static_cast<float>(1.0) - pFactor) * omega) / sinom;
            sclq = std::sin(pFactor * omega) / sinom;
        }
        else {
            // Very close, do linear interp (because it's faster)
            sclp = static_cast<float>(1.0) - pFactor;
            sclq = pFactor;
        }
        Out.x = sclp * Start.x + sclq * end.x;
        Out.y = sclp * Start.y + sclq * end.y;
        Out.z = sclp * Start.z + sclq * end.z;
        Out.w = sclp * Start.w + sclq * end.w;
    }

    ////// Perlin Noise ///////
    inline Vector2 RandomGradient(int _iX, int _iY)
    {
        // No precomputed gradients, mean this works for any number of grid coordinates
        const unsigned long w = 8 * sizeof(unsigned long);
        const unsigned long s = w / 2;
        unsigned long a = _iX, b = _iY;
        a *= 3284157443;

        b ^= (unsigned long long)a << s | (unsigned long long)a >> (w - s);
        b *= 1911520717;    

        a ^= (unsigned long long)b << s | (unsigned long long)b >> (w - s);
        a *= 2048419325;
        float random = (float)(a * (3.14159265 / ~(~0u >> 1))); // in [0, 2*Pi]

        // Create the vector from the angle
        Vector2 v;
        v.x = sin(random);
        v.y = cos(random);

        return v;
    }

    // Compute the dot product of the distance and gradient vectors
    inline float DotGridGradient(int _iX, int _iY, float _fX, float _fY)
    {
        // Get gradient from integer coordinates
        Vector2 vGradient = RandomGradient(_iX, _iY);

        // Compute the distance vector
        float fDX = _fX - (float)_iX;
        float fDY = _fY - (float)_iY;

        return (fDX * vGradient.x + fDY * vGradient.y);
    }

    inline float Interpolate(float _fA0, float _fA1, float _fW)
    {
        return (_fA1 - _fA0) * (3.0f - _fW * 2.0f) * _fW * _fW + _fA0;  
    }

    // Compute the perlin noise for a set of coordinates, the return value is between -1 and 1
    inline float PerlinNoise(float _fX, float _fY)
    {
        // Determine grid cell corner coordinates
        int iX0 = (int)_fX;
        int iY0 = (int)_fY; 
        int iX1 = iX0 + 1;
        int iY1 = iY0 + 1;

        // Compute interpolation weights
        float fSX = _fX - (float)iX0;
        float fSY = _fY - (float)iY0;

        // Compute and interpolate top two corners
        float fN0 = DotGridGradient(iX0, iY0, _fX, _fY);
        float fN1 = DotGridGradient(iX1, iY0, _fX, _fY);
        float fX0 = Interpolate(fN0, fN1, fSX);

        // Compute and interpolate top two corners
        fN0 = DotGridGradient(iX0, iY1, _fX, _fY);
        fN1 = DotGridGradient(iX1, iY1, _fX, _fY);
        float fX1 = Interpolate(fN0, fN1, fSX);

        // Final step: intepolate between the two previously interpolated values, now in y
        float fValue = Interpolate(fX0, fX1, fSY);

        return fValue;
    }
}