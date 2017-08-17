inline void Camera::setName(const std::string& name) {
    _name = name;
}

inline void Camera::setType(Type type) {
    _type = type;
}

inline void Camera::setFovY(float fovY) {
    LUG_ASSERT(_type == Camera::Type::Perspective, "You need to build a persepctive camera to set the fov y");
    _fovY = fovY;
}

inline void Camera::setAspectRatio(float aspectRatio) {
    LUG_ASSERT(_type == Camera::Type::Perspective, "You need to build a persepctive camera to set the aspect ratio");
    _aspectRatio = aspectRatio;
}

inline void Camera::setXMag(float xMag) {
    LUG_ASSERT(_type == Camera::Type::Orthographic, "You need to build a orthographic camera to set the x magnification");
    _xMag = xMag;
}

inline void Camera::setYMag(float yMag) {
    LUG_ASSERT(_type == Camera::Type::Orthographic, "You need to build a orthographic camera to set the y magnification");
    _yMag = yMag;
}

inline void Camera::setZNear(float zNear) {
    _zNear = zNear;
}

inline void Camera::setZFar(float zFar) {
    _zFar = zFar;
}

