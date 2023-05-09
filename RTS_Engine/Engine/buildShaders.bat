@ECHO OFF
%VULKAN_SDK%\Bin\glslc.exe FragmentShader.frag -o FragmentShader.spv || pause
%VULKAN_SDK%\Bin\glslc.exe VertexShader.vert -o VertexShader.spv || pause
exit