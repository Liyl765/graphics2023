#pragma once
#include <vector>
#include <iostream>

#include <filesystem>

#include <cstdio>
#include <cstring>
#include <fstream>
inline std::vector<char> readFile(const std::string& filename);
/// <summary>
/// ����GLSL��SPIR-V,��������ɫ��ģ��
/// </summary>
/// <param name="device"></param>
/// <param name="path">��ɫ��Ŀ¼</param>
/// <param name="src">GLSL�ļ���</param>
/// <param name="dst">SPIR-V�ļ���</param>
/// <returns></returns>
inline VkShaderModule createShaderModule(VkDevice device, const std::string&path, const std::string& src, const std::string& dst)
{

	std::filesystem::path originalPath = std::filesystem::current_path();
	std::filesystem::current_path(path);
	const std::string command = " glslangValidator.exe -V "+ src +" -o "+ dst;
	FILE* output = _popen(command.c_str(), "r");
	

	if (output) {
		// ��ȡ���
		char buffer[128];
		std::string result;
		fgets(buffer, sizeof(buffer), output);
		if (buffer != src+'\n') {
			throw std::runtime_error(buffer);
		}
		while (fgets(buffer, sizeof(buffer), output) != nullptr) {
			result += buffer;
		}
		if (!result.empty()) {
			throw std::runtime_error(result);
		}
		// ��ӡ���

		// �ر��ļ�ָ��
		_pclose(output);
	}
	auto code= readFile(dst);

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
	std::filesystem::current_path(originalPath);
	return shaderModule;
}
inline std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}