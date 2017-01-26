inline const std::string& SceneNode::getName() const {
    return _name;
}

inline const Math::Vector<3, float>& SceneNode::getAbsolutePosition() const {
    return _absolutePosition;
}

inline const Math::Quaternion<float>& SceneNode::getAbsoluteRotation() const {
    return _absoluteRotation;
}

inline const Math::Vector<3, float>& SceneNode::getAbsoluteScale() const {
    return _absoluteScale;
}
