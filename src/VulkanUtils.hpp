#ifndef VULKANUTILS_H
#define VULKANUTILS_H

#include <vulkan/vulkan.hpp>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct error_codes {
    int value;
    const char* name;
};

const char* err2msg(int code);

bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);

#endif
