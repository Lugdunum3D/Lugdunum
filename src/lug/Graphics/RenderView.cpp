#include <lug/Graphics/RenderView.hpp>
#include <lug/Graphics/RenderTarget.hpp>

#include <lug/System/Logger.hpp>


namespace lug {
namespace Graphics {

RenderView::RenderView(const RenderTarget* renderTarget) : _renderTarget{renderTarget} {}


void RenderView::update() {

    _viewport = {
        {                                                               // offset
            _renderTarget->getWidth() * _info.viewport.offset.x,        // x
            _renderTarget->getHeight() * _info.viewport.offset.y        // y
        },

        {                                                               // extent
            _renderTarget->getWidth() * _info.viewport.extent.width,    // width
            _renderTarget->getHeight() * _info.viewport.extent.height   // height
        },

        _info.viewport.minDepth,                                        // minDepth
        _info.viewport.maxDepth                                         // maxDepth
    };

     LUG_LOG.info(" getWidth {}", _viewport.extent.width);
     LUG_LOG.info(" getheight {}", _viewport.extent.height);

     LUG_LOG.info(" _renderTarget->getWidth {}",  _renderTarget->getWidth());
     LUG_LOG.info(" _renderTarget->getHeight {}", _renderTarget->getHeight());


    _scissor = {
        {                                                               // offset
            _renderTarget->getWidth() * _info.scissor.offset.x,         // x
            _renderTarget->getHeight() * _info.scissor.offset.y         // y
        },

        {                                                               // extent
            _renderTarget->getWidth() * _info.scissor.extent.width,     // width
            _renderTarget->getHeight() * _info.scissor.extent.height    // height
        }
    };

    if (_camera) {
        _camera->needUpdateProj();
    }
}

void RenderView::init(RenderView::InitInfo& initInfo) {
    _info = std::move(initInfo);
    _camera = std::move(_info.camera);

    update();
}

} // Graphics
} // lug
