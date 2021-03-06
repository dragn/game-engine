#pragma once

#include "Modules/Renderer3D/RenderableBehavior/RenderableBehavior.hpp"
#include "Modules/ResourceSystem/ResourceTypes/RSkyBox.hpp"
#include "Modules/standard.hpp"

namespace vh
{

    /*
          //        \\
        BBBB SkyBox BBBB
          \\        //

        SkyBox rendering behavior.
    */

    class BSkyBox : public ActorBehavior
    {
    public:
        BSkyBox() = default;
        BSkyBox(const char* filePath)
        {
            MResourceSystem* rs = App::Get<MResourceSystem>();
            CHECK(rs);

            mSkyBoxRes = rs->GetResource<RSkyBox>(filePath);
        }
        void operator=(BSkyBox const& other)
        {
            mSkyBoxRes = other.mSkyBoxRes;
        }

        std::shared_ptr<RSkyBox> const& GetRes() const { return mSkyBoxRes; }

    private:
        std::shared_ptr<RSkyBox> mSkyBoxRes;
    };

} // namespace vh
