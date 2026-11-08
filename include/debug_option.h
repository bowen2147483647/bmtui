#pragma once
#ifdef DEBUG
#define PRIVATE_ON_RELEASE public
#else
#define PRIVATE_ON_RELEASE private
#endif
#define IGNORE(x) (void)(x)
