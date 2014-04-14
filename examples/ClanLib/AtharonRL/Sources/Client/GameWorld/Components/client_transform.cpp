
#include "precomp.h"
#include "client_transform.h"

using namespace Totem;
using namespace clan;

ClientTransform::ClientTransform(GameObject *owner, const std::string &name)
: Transform(owner, name)
{
}

ClientTransform::~ClientTransform()
{
}
