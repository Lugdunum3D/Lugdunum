inline SceneNode* SceneNode::getSceneNode(const std::string& name) {
    return static_cast<SceneNode*>(Node::getNode(name));
}

inline const SceneNode* SceneNode::getSceneNode(const std::string& name) const {
    return static_cast<const SceneNode*>(Node::getNode(name));
}
