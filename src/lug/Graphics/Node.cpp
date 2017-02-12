#include <lug/Graphics/Node.hpp>
#include <lug/Math/Geometry/Transform.hpp>

namespace lug {
namespace Graphics {

Node::Node(const std::string& name) : _name(name) {}

Node* Node::getNode(const std::string& name) {
    if (name == _name) {
        return this;
    }

    for (const auto& child : _children) {
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
        Node* tmp = child->getNode(name);

        if (tmp) {
            return tmp;
        }
    }

    return nullptr;
}

void Node::attachChild(std::unique_ptr<Node> child) {
    child->_parent = this;
    _children.push_back(std::move(child));
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
    Math::Quatf quat(angle, axis);

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

void Node::needUpdate() {
    _needUpdate = true;
    _dirty = true;

    for (const auto& child : _children) {
        child->needUpdate();
    }
}

void Node::update() {
    if (_parent) {
        _absolutePosition = _parent->getAbsoluteRotation().transform() * (_parent->getAbsoluteScale() * _position) + _parent->getAbsolutePosition();
        _absoluteRotation = _parent->getAbsoluteRotation() * _rotation;
        _absoluteScale = _parent->getAbsoluteScale() * _scale;
    }
    else {
        _absolutePosition = _position;
        _absoluteRotation = _rotation;
        _absoluteScale = _scale;
    }

    _transform = Math::Geometry::translate(_absolutePosition) * _absoluteRotation.transform() * Math::Geometry::scale(_absoluteScale);

    _needUpdate = false;
}

} // Graphics
} // lug
