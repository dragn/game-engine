#pragma once

#include "Common.hpp"
#include "ActorBehavior.hpp"

namespace vh
{

/**
 * Actor is something that could be placed in Scene,
 * something with position and rotation, but not necessarily drawable or movable.
 */
class Actor
{
    friend class World;

public:
    virtual ~Actor();

    // -- transform
    const M4& GetTransform() const { return mTransform; }

    // -- pos
    const V3& GetPos() const;
    void SetPos(const V3& pos);
    void AddPos(const V3& pos);

    // -- scale
    const V3& GetScale() const;
    void SetScale(const V3& scale);
    void AddScale(const V3& scale);

    // -- rot
    Rot GetRot() const;
    void AddRot(Rot rot);
    void SetRot(Rot rot);

    // -- get Actor's forward vector
    V3 GetForward() const;

    // -- get Actor's up vector
    V3 GetUp() const;

    // -- yaw
    float GetYaw() const;
    void SetYaw(float yaw);
    void AddYaw(float deltaYaw);

    // -- pitch
    float GetPitch() const;
    void SetPitch(float pitch);
    void AddPitch(float deltaPitch);

    void SetName(const std::string& name)
    {
        mName = name;
    }

    void SetName(const char* name)
    {
        mName = name;
    }

    const std::string& GetName() const
    {
        return mName;
    }

    M4 GetView() const;

    template<typename BEHAVIOR_CLASS, typename... ARGS>
    BEHAVIOR_CLASS* AddBehavior(ARGS... args)
    {
        mBehaviors.push_back(std::make_unique<BEHAVIOR_CLASS>(this, args...));
        return static_cast<BEHAVIOR_CLASS*>(mBehaviors.back().get());
    }

    template<typename BEHAVIOR_CLASS, typename FUNC>
    void ForEachBehaviorOfType(FUNC func)
    {
        std::for_each(mBehaviors.begin(), mBehaviors.end(), [&func] (const std::unique_ptr<ActorBehavior>& behavior)
        {
            BEHAVIOR_CLASS* base = dynamic_cast<BEHAVIOR_CLASS*>(behavior.get());
            if (base) func(base);
        });
    }

    // -- Enable this actor
    void StartPlay();

    // -- Disable this actor
    void EndPlay();

protected:
    virtual void _UpdateTransform();

    M4 mTransform;

private:
    World* mOwner;
    bool mPlaying;

    V3 mPos;
    V3 mScale;

    float mYaw = 0;
    float mPitch = 0;

    std::string mName;

    std::list<std::unique_ptr<ActorBehavior>> mBehaviors;

    // Actor ticks are called from World
    void Tick(uint32_t delta);

    // Construction of Actor is only allowed in World
    Actor(World* world, V3 pos = V3(), Rot rot = Rot(), V3 scale = V3(1.0f, 1.0f, 1.0f));
};

} // namespace vh
