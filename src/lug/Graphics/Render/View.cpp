#include <lug/Graphics/Render/View.hpp>
#include <lug/Graphics/Render/Target.hpp>

namespace lug {
namespace Graphics {
namespace Render {

View::View(const Target* renderTarget) : _renderTarget{renderTarget} {}

void View::update() {
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

void View::init(View::InitInfo& initInfo) {
    _info = std::move(initInfo);
    _camera = std::move(_info.camera);

    update();
}

} // Render
} // Graphics
} // lug
