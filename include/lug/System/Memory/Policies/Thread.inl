inline void SingleThreadPolicy::enter() const {}
inline void SingleThreadPolicy::leave() const {}

template<class SynchronizationPrimitive>
inline void MultiThreadPolicy<SynchronizationPrimitive>::enter() {
    _primitive.lock();
}

template<class SynchronizationPrimitive>
inline void MultiThreadPolicy<SynchronizationPrimitive>::leave() {
    _primitive.unlock();
}
