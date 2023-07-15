@ECHO OFF
%VULKAN_SDK%\Bin\glslc.exe %0\..\FragmentShader.frag -o %CD%/FragmentShader.spv || pause
%VULKAN_SDK%\Bin\glslc.exe %0\..\VertexShader.vert -o %CD%/VertexShader.spv || pause
exit