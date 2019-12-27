#include "hielement.h"
namespace CXS{
    HiElement::HiElement(const char* className)
    {
        mHandle = 0;
        hiClassName = className;
    }
    HI_S32 HiElement::getHandle()
    {
        return mHandle;
    }
    const char*HiElement::getClassName()
    {
        return hiClassName.data();
    }
}
