#pragma once

#include "Common.hpp"
#include "Actor/Actor.hpp"
#include "Component.hpp"
#include "Renderer/Lights.hpp"
#include <list>
#include "Actor/ActorFactory.hpp"

namespace vh {

class World : public Component
{
    COMPONENT_NAME(World);

public:
    World();

    template<typename T, typename... Args> T* SpawnActor(Args... args) { return AddActor(new T(args...)); }

    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    template<typename T> void DestroyActor(T*& actor)
    {
        auto iter = mActors.begin();
        while (iter != mActors.end())
        {
            if (iter->get() == actor)
            {
                iter = mActors.erase(iter);
                actor = nullptr;
            }
            else
            {
                ++iter;
            }
        }
    }

    const std::list<std::unique_ptr<Actor>>& GetActors()
    {
        return mActors;
    }

    template<typename T>
    T* GetActorByName(const std::string& name)
    {
        for (const std::unique_ptr<Actor>& actor : mActors)
        {
            if (actor->GetName() == name)
            {
                return reinterpret_cast<T*>(actor.get());
            }
        }
        return nullptr;
    }

    Actor* CreateActor(const std::string& name)
    {
        std::string tmp(name);
        tmp.append("_");
        tmp.append(std::to_string(mActors.size()));

        Actor* actor = new Actor(this);
        actor->SetName(tmp);
        mActors.push_back(std::unique_ptr<Actor>(actor));
        return actor;
    }

private:
    std::list<std::unique_ptr<Actor>> mActors;
    ActorFactory mActorFactory;

    UNCOPYABLE(World);
};

} // namespace vh
