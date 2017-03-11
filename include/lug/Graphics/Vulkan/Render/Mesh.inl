inline const API::Buffer* Mesh::getVertexBuffer() const {
    return _vertexBuffer.get();
}

inline const API::Buffer* Mesh::getIndexBuffer() const {
    return _indexBuffer.get();
}
