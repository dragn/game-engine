#include "ActorFactory.hpp"

#define REGISTER(name) Register(#name, &_Create<name>);

vh::ActorFactory::ActorFactory()
{
}
