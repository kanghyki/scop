#ifndef INCLUDED_PTRDEF_H
#define INCLUDED_PTRDEF_H

#include <memory>

#define CLASS_PTR(className)                                                                                           \
    class className;                                                                                                   \
    using className##UPtr = std::unique_ptr<className>;                                                                \
    using className##Ptr = std::shared_ptr<className>;

#endif
