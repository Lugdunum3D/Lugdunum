struct CmdCopyImage {
    const API::Image& srcImage;
    VkImageLayout srcImageLayout;

    const API::Image& dstImage;
    VkImageLayout dstImageLayout;

    std::vector<VkImageCopy> regions;
};

void copyImage(const CmdCopyImage& parameters) const;
