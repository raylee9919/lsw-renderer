@echo off

pushd shaders
set VULKAN_SDK=C:/VulkanSDK/1.4.309.0
for /r %%i in (*.frag, *.vert) do %VULKAN_SDK%/Bin/glslc %%i -o ../../data/shaders/%%~ni.spv
popd
