template <size_t maxSets>
inline DescriptorSetPool<maxSets>::DescriptorSetPool(Renderer& renderer) : _renderer(renderer) {}

template <size_t maxSets>
inline DescriptorSetPool<maxSets>::~DescriptorSetPool() {
    descriptorPool.destroy();
}

template <size_t maxSets>
inline bool DescriptorSetPool<maxSets>::init() {
    if (static_cast<VkDescriptorPool>(descriptorPool) == VK_NULL_HANDLE) {
        API::Builder::DescriptorPool descriptorPoolBuilder(_renderer.getDevice());

        // Use VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT to individually free descritors sets
        descriptorPoolBuilder.setFlags(0);

        descriptorPoolBuilder.setMaxSets(42); // TODO: Replace this arbitrary number

        std::vector<VkDescriptorPoolSize> poolSizes{ // TODO: Replace this initialization
            {
                /* poolSize.type            */ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                /* poolSize.descriptorCount */ 42
            },
            {
                /* poolSize.type            */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                /* poolSize.descriptorCount */ 42
            }
        };
        descriptorPoolBuilder.setPoolSizes(poolSizes);

        VkResult result{VK_SUCCESS};
        if (!descriptorPoolBuilder.build(descriptorPool, &result)) {
            LUG_LOG.error("DescriptorSetPool: Can't create the descriptor pool: {}", result);
            return false;
        }
    }
    return true;
}

template <size_t maxSets>
inline std::tuple<bool, const DescriptorSet*> DescriptorSetPool<maxSets>::allocate(size_t hash, const API::DescriptorSetLayout& descriptorSetLayout) {
    auto it = _descriptorSetsInUse.find(hash);
    if (it == _descriptorSetsInUse.end()) {
        DescriptorSet* descriptorSet = allocateNewDescriptorSet(descriptorSetLayout);

        if (!descriptorSet) {
            return std::make_tuple(false, nullptr);
        }

        descriptorSet->setHash(hash);
        descriptorSet->_referenceCount += 1;

        _descriptorSetsInUse[hash] = descriptorSet;

        return std::make_tuple(true, descriptorSet);
    }

    it->second->_referenceCount += 1;
    return std::make_tuple(false, it->second);
}

template <size_t maxSets>
inline void DescriptorSetPool<maxSets>::free(const DescriptorSet* descriptorSet) {
    if (!descriptorSet) {
        return;
    }

    const_cast<DescriptorSet*>(descriptorSet)->_referenceCount -= 1;

    const auto it = _descriptorSetsInUse.find(descriptorSet->getHash());
    if (descriptorSet->_referenceCount == 0) {
        if (it != _descriptorSetsInUse.end() && it->second == descriptorSet) {
            _descriptorSetsInUse.erase(descriptorSet->getHash());
        }

        _freeDescriptorSets[_freeDescriptorSetsCount] = const_cast<DescriptorSet*>(descriptorSet);
        ++_freeDescriptorSetsCount;
    }
}

template <size_t maxSets>
inline DescriptorSet* DescriptorSetPool<maxSets>::allocateNewDescriptorSet(const API::DescriptorSetLayout& descriptorSetLayout) {
    if (_freeDescriptorSetsCount) {
        --_freeDescriptorSetsCount;

        DescriptorSet* tmp = _freeDescriptorSets[_freeDescriptorSetsCount];
        _freeDescriptorSets[_freeDescriptorSetsCount] = nullptr;

        return tmp;
    } else if (_descriptorSetsCount < maxSets) {
        API::Builder::DescriptorSet descriptorSetBuilder(_renderer.getDevice(), descriptorPool);
        descriptorSetBuilder.setDescriptorSetLayouts({static_cast<VkDescriptorSetLayout>(descriptorSetLayout)});

        VkResult result{VK_SUCCESS};
        if (!descriptorSetBuilder.build(_descriptorSets[_descriptorSetsCount]._descriptorSet, &result)) {
            LUG_LOG.error("DescriptorSetPool: Can't create descriptor set: {}", result);
            return nullptr;
        }

        return &_descriptorSets[_descriptorSetsCount++];
    } else {
        LUG_LOG.error("DescriptorSetPool: Can't create descriptor set: Maximum number of sets reached");
    }

    return nullptr;
}
