#pragma once

namespace core
{
#if defined(APOLLO_USE_FLOAT)
    using Real = float;
#elif defined(APOLLO_USE_DOUBLE)
    using Real = double;
#endif
} // namespace core
