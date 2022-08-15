#ifndef VULKANUTILS_H
#define VULKANUTILS_H

#include <vulkan/vulkan.hpp>

#ifndef NDEBUG
const bool enableValidationLayers = false;
#elif
const bool enableValidationLayers = true;
#endif

struct error_codes {
    int value;
    const char* name;
};

const char* err2msg(int code);

bool checkValidationLayerSupport();

#endif
