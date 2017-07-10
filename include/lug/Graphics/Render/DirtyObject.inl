inline void DirtyObject::setDirty() {
    _dirtyFlag = 0xFFFFFFFF;
}

inline void DirtyObject::setDirty(uint32_t currentFrame) {
    _dirtyFlag |= (1 << currentFrame);
}

inline void DirtyObject::clearDirty() {
    _dirtyFlag = 0x0;
}

inline void DirtyObject::clearDirty(uint32_t currentFrame) {
    _dirtyFlag &= ~(1 << currentFrame);
}

inline bool DirtyObject::isDirty() const {
    return _dirtyFlag;
}

inline bool DirtyObject::isDirty(uint32_t currentFrame) const {
    return _dirtyFlag & (1 << currentFrame);
}
