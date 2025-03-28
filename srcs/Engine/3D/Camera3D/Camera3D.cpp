#include "Engine/3D/Camera3D/Camera3D.hpp"

Camera3D::Camera3D()
{
    position = {0.0f, 0.0f, 3.0f};
    frontDirection = ml::vec3(1.0f, 1.0f, 1.0f);
    rightDirection = ml::vec3(1.0f, 1.0f, 1.0f);
    upDirection = {0.0f, 1.0f, 0.0f};

    yaw = -90.0f;
    pitch = 0.0f;
    roll = 0.0f;
    fov = 45.0f;
    speed = 2.5f;
}

Camera3D::Camera3D(const ml::vec3 &position, const ml::vec3 &upDirection, float yaw, float pitch, float roll, float fov,
               float speed)
{
    this->position = position;
    frontDirection = ml::vec3(1.0f, 1.0f, 1.0f);
    rightDirection = ml::vec3(1.0f, 1.0f, 1.0f);
    this->upDirection = upDirection;
    this->yaw = yaw;
    this->pitch = pitch;
    this->roll = roll;
    this->fov = fov;
    this->speed = speed;
}

Camera3D::Camera3D(const Camera3D &copy)
{
    *this = copy;
}

Camera3D &Camera3D::operator=(const Camera3D &copy)
{
    if (&copy != this)
    {
        position = copy.getPosition();
        frontDirection = copy.getFrontDirection();
        rightDirection = copy.getRightDirection();
        upDirection = copy.getUpDirection();
        yaw = copy.getYaw();
        pitch = copy.getPitch();
        roll = copy.getRoll();
        fov = copy.getFov();
        speed = copy.getSpeed();
    }
    return (*this);
}

Camera3D::~Camera3D()
{
}

ml::vec3 Camera3D::getPosition() const
{
    return (position);
}

ml::vec3 Camera3D::getFrontDirection() const
{
    return (frontDirection);
}

ml::vec3 Camera3D::getRightDirection() const
{
    return (rightDirection);
}

ml::vec3 Camera3D::getUpDirection() const
{
    return (upDirection);
}

float Camera3D::getYaw() const
{
    return (yaw);
}

float Camera3D::getPitch() const
{
    return (pitch);
}
float Camera3D::getRoll() const
{
    return (roll);
}

float Camera3D::getFov() const
{
    return (fov);
}
float Camera3D::getSpeed() const
{
    return (speed);
}

void Camera3D::setPosition(const ml::vec3 &position)
{
    this->position = position;
}
void Camera3D::setFrontDirection(const ml::vec3 &frontDirection)
{
    this->frontDirection = frontDirection;
}

void Camera3D::setRightDirection(const ml::vec3 &rightDirection)
{
    this->rightDirection = rightDirection;
}

void Camera3D::setUpDirection(const ml::vec3 &upDirection)
{
    this->upDirection = upDirection;
}

void Camera3D::setYaw(float yaw)
{
    this->yaw = yaw;
}

void Camera3D::setPitch(float pitch)
{
    this->pitch = pitch;
}

void Camera3D::setRoll(float roll)
{
    this->roll = roll;
}

void Camera3D::setFov(float fov)
{
    this->fov = fov;
}

void Camera3D::setSpeed(float speed)
{
    this->speed = speed;
}

void Camera3D::addToPosition(const ml::vec3 &position)
{
    this->position = this->position + position;
}

void Camera3D::addToYaw(float yaw)
{
    this->yaw += yaw;
}

void Camera3D::addToPitch(float pitch)
{
    this->pitch += pitch;
}

void Camera3D::addToFov(float fov)
{
    this->fov += fov;
}