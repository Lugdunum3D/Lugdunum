inline void Camera::setName(const std::string& name) {
    _name = name;
}

inline void Camera::setType(Type type) {
    _type = type;
}

inline void Camera::setFovY(float fovy) {
    LUG_ASSERT(_type == Camera::Type::Perspective, "You need to build a persepctive camera to set the fov y");
    _fovy = fovy;
}

inline void Camera::setAspectRatio(float aspectRatio) {
    LUG_ASSERT(_type == Camera::Type::Perspective, "You need to build a persepctive camera to set the aspect ratio");
    _aspectRatio = aspectRatio;
}

inline void Camera::setXMag(float xmag) {
    LUG_ASSERT(_type == Camera::Type::Orthographic, "You need to build a orthographic camera to set the x magnification");
    _xmag = xmag;
}

inline void Camera::setYMag(float ymag) {
    LUG_ASSERT(_type == Camera::Type::Orthographic, "You need to build a orthographic camera to set the y magnification");
    _ymag = ymag;
}

inline void Camera::setZNear(float znear) {
    _znear = znear;
}

inline void Camera::setZFar(float zfar) {
    _zfar = zfar;
}

