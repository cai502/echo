#pragma once

#include "interface/FrameBuffer.h"
#include "vk_render_base.h"

namespace Echo
{
    class VKFramebuffer : public FrameBuffer
    {
    public:
        VKFramebuffer(ui32 id, ui32 width, ui32 height);
        virtual ~VKFramebuffer();

        // attach render view
        virtual void attach(Attachment attachment, RenderView* renderView) override;

        // begin render
        virtual bool begin(bool isClearColor, const Color& bgColor, bool isClearDepth, float depthValue, bool isClearStencil, ui8 stencilValue) override;
        virtual bool end() override;

        // on resize
        virtual void onSize(ui32 width, ui32 height);

    private:
        // create render pass
        void createVkRenderPass();

        // create vk frame buffer
        void createVkFramebuffer();

    private:
        VkRenderPass    m_vkRenderPass;
        VkFramebuffer   m_vkFramebuffer;
    };
}
