#include "Common.hpp"
#include "CameraBehavior.hpp"
#include "Actor/Actor.hpp"

using namespace vh;

vh::M4 vh::CameraBehavior::GetView()
{
    // Default implementation: use actor's position and orientation
    return glm::lookAt(GetPos(), GetPos() + GetForward(), GetOwner()->GetUp());
}

vh::V3 vh::CameraBehavior::GetForward()
{
    return V3(glm::toMat4(GetRot()) * glm::vec4(0, 0, -1, 0));
}
