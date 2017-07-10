inline DescriptorSet::DescriptorSet(API::DescriptorSet descriptorSet) : _descriptorSet(std::move(descriptorSet)) {}

inline const API::DescriptorSet& DescriptorSet::getDescriptorSet() const {
    return _descriptorSet;
}

inline size_t DescriptorSet::getHash() const {
    return _hash;
}

inline void DescriptorSet::setHash(size_t hash) {
    _hash = hash;
}
