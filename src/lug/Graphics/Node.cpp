#include <lug/Graphics/Node.hpp>
#include <lug/Math/Geometry/Transform.hpp>

namespace lug {
namespace Graphics {

Node::Node(const std::string& name) : _name(name) {}

Node* Node::getNode(const std::string& name) {
    if (name == _name) {
        return this;
    }

    for (auto& child : _children) {
        Node* tmp = child->getNode(name);

        if (tmp) {
            return tmp;
        }
    }

    return nullptr;
}

const Node* Node::getNode(const std::string& name) const {
    if (name == _name) {
        return this;
    }

    for (const auto& child : _children) {
        const Node* tmp = child->getNode(name);

        if (tmp) {
            return tmp;
        }
    }

    return nullptr;
}

void Node::attachChild(Node& child) {
    child._parent = this;
    _children.push_back(&child);
}

void Node::translate(const Math::Vec3f& direction, TransformSpace space) {
    if (space == TransformSpace::Local) {
        _position += _rotation.transform() * direction;
    } else if (space == TransformSpace::Parent) {
        _position += direction;
    } else if (space == TransformSpace::World) {
        if (_parent) {
            _position += (Math::inverse(_parent->getAbsoluteRotation()).transform() * direction) / _parent->getAbsoluteScale();
        } else {
            _position += direction;
        }
    }

    needUpdate();
}

void Node::rotate(float angle, const Math::Vec3f& axis, TransformSpace space) {
    rotate(Math::Quatf(angle, axis), space);
}

void Node::rotate(const Math::Quatf& quat, TransformSpace space) {
    if (space == TransformSpace::Local) {
        _rotation = _rotation * quat;
    } else if (space == TransformSpace::Parent) {
        _rotation = quat * _rotation;
    } else if (space == TransformSpace::World) {
        _rotation = _rotation * Math::inverse(getAbsoluteRotation()) * quat * getAbsoluteRotation();
    }

    needUpdate();
}

void Node::scale(const Math::Vec3f& scale) {
    _scale *= scale;
    needUpdate();
}

void Node::setPosition(const Math::Vec3f& position, TransformSpace space) {
    if (space == TransformSpace::Local) {
        _position = _rotation.transform() * position;
    } else if (space == TransformSpace::Parent) {
        _position = position;
    } else if (space == TransformSpace::World) {
        if (_parent) {
            _position = (_parent->getAbsoluteRotation().transform() * position * _parent->getAbsoluteScale()) + _parent->getAbsolutePosition();
        } else {
            _position = position;
        }
    }

    needUpdate();
}

void Node::setRotation(const Math::Quatf& rotation, TransformSpace space) {
    if (space == TransformSpace::Local) {
        // TODO: Use the current local rotation to compute the new rotation
        _rotation = rotation;
    } else if (space == TransformSpace::Parent) {
        _rotation = rotation;
    } else if (space == TransformSpace::World) {
        if (_parent) {
            _rotation = Math::inverse(getAbsoluteRotation()) * rotation;
        } else {
            _rotation = rotation;
        }
    }

    needUpdate();
}

void Node::setRotation(float angle, const Math::Vec3f& axis, TransformSpace space) {
    setRotation(Math::Quatf(angle, axis), space);
}

void Node::setDirection(const Math::Vec3f& spaceTargetDirection, const Math::Vec3f& localDirectionVector, const Math::Vec3f& localUpVector, TransformSpace space) {
    if (spaceTargetDirection.length() == 0.0f) {
        return;
    }

    // The direction we want the local direction point to
    Math::Vec3f targetDirection = Math::normalize(spaceTargetDirection);

    // Transform target direction to world space
    if (space == TransformSpace::Local) {
        targetDirection = getAbsoluteRotation().transform() * targetDirection;
    } else if (space == TransformSpace::Parent) {
        if (_parent) {
            targetDirection = _parent->getAbsoluteRotation().transform() * targetDirection;
        }
    } else if (space == TransformSpace::World) {
        // Nothing to do here
    }

    Math::Vec3f xVec = Math::normalize(cross(localUpVector, targetDirection));
    Math::Vec3f yVec = Math::normalize(cross(targetDirection, xVec));
    Math::Quatf unitZToTarget = Math::Quatf::fromAxes(xVec, yVec, targetDirection);

    Math::Quatf targetOrientation;

    if (localDirectionVector == Math::Vec3f{0.0f, 0.0f, -1.0f}) {
        targetOrientation = Math::Quatf(-unitZToTarget.y(), -unitZToTarget.z(), unitZToTarget.w(), unitZToTarget.x());
    } else {
        targetOrientation = unitZToTarget * directionTo(localDirectionVector, Math::Vec3f{0.0f, 0.0f, 1.0f});
    }

    setRotation(targetOrientation, TransformSpace::Parent);
}

void Node::lookAt(const Math::Vec3f& targetPosition, const Math::Vec3f& localDirectionVector, const Math::Vec3f& localUpVector, TransformSpace space) {
    Math::Vec3f origin;

    if (space == TransformSpace::Local) {
        origin = Math::Vec3f(0.0f);
    } else if (space == TransformSpace::Parent) {
        origin = _position;
    } else if (space == TransformSpace::World) {
        origin = getAbsolutePosition();
    }

    setDirection(targetPosition - origin, localDirectionVector, localUpVector, space);
}

void Node::needUpdate() {
    _needUpdate = true;

    for (auto& child : _children) {
        child->needUpdate();
    }
}

void Node::update() {
    if (_parent) {
        _absolutePosition = _parent->getAbsoluteRotation().transform() * (_parent->getAbsoluteScale() * _position) + _parent->getAbsolutePosition();
        _absoluteRotation = _parent->getAbsoluteRotation() * _rotation;
        _absoluteScale = _parent->getAbsoluteScale() * _scale;
    } else {
        _absolutePosition = _position;
        _absoluteRotation = _rotation;
        _absoluteScale = _scale;
    }

    _absoluteRotation.normalize();

    _transform = Math::Geometry::translate(_absolutePosition) * _absoluteRotation.transform() * Math::Geometry::scale(_absoluteScale);

    _needUpdate = false;
}

} // Graphics
} // lug
