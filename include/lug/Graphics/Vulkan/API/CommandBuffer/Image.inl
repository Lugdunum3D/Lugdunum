struct CmdCopyImage {
    const API::Image& srcImage;
    VkImageLayout srcImageLayout;

    const API::Image& dstImage;
    VkImageLayout dstImageLayout;

    std::vector<VkImageCopy> regions;
};

struct CmdBlitImage {
    const API::Image& srcImage;
    VkImageLayout srcImageLayout;

    const API::Image& dstImage;
    VkImageLayout dstImageLayout;

    std::vector<VkImageBlit> regions;

    VkFilter filter;
};

void copyImage(const CmdCopyImage& parameters) const;
void blitImage(const CmdBlitImage& parameters) const;
