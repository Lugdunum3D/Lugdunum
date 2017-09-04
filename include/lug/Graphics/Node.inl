inline void Node::setParent(Node *parent) {
    _parent = parent;
    needUpdate();
}

inline Node* Node::getParent() const {
    return _parent;
}

inline const std::string& Node::getName() const {
    return _name;
}

inline const Math::Vec3f& Node::getAbsolutePosition() {
    if (_needUpdate) {
        update();
    }

    return _absolutePosition;
}

inline const Math::Quatf& Node::getAbsoluteRotation() {
    if (_needUpdate) {
        update();
    }

    return _absoluteRotation;
}

inline const Math::Vec3f& Node::getAbsoluteScale() {
    if (_needUpdate) {
        update();
    }

    return _absoluteScale;
}

inline const Math::Mat4x4f& Node::getTransform() {
    if (_needUpdate) {
        update();
    }
    return _transform;
}

inline const std::vector<Node*>& Node::getChildren() const {
    return _children;
}
