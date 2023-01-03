# Vulkan project
Simple vulkan app that renders a model Nefertiti bust. It implements features as:
- Multisampling,
- Mipmapping,
- Depth buffering.

![Main window](screenshots/main_window_7.png)

## Requirements
To compile this project, following libraries have to be installed:
- [Vulkan SDK](https://github.com/LunarG/VulkanTools)
- [Vulkan Validation Layers](https://github.com/KhronosGroup/Vulkan-ValidationLayers)
- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)

On arch-like systems following command takes care of it:
```bash
pacman -S vulkan-devel glfw-devel glm
```

## Resources
- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [Salmon Sculpture Texture](https://commons.wikimedia.org/wiki/File:Spawning_salmon_sculpture,_Wetherby_(16th_October_2020).jpg)
- [Efertiti's bust by C. Yamahata](https://sketchfab.com/3d-models/nefertitis-bust-like-in-the-museum-ce5b14926e494558ab584375a8d63ca7)

## External libraries
- [STB (stb_image.h)](https://github.com/nothings/stb/blob/master/stb_image.h)
- [Tiny obj loader (tiny_obj_loader.h)](https://github.com/tinyobjloader/tinyobjloader/blob/release/tiny_obj_loader.h)
