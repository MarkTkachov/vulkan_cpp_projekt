#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

class HelloTriangleApplication
{
public:
  void run()
  {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  const uint32_t windowWidth = 800;
  const uint32_t windowHeight = 600;

  VkInstance instance;

  void initWindow()
  {
    if (!glfwInit())
      throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(windowWidth, windowHeight, "Vulkan", nullptr, nullptr);
  };

  void initVulkan()
  {
    createInstance();
  };

  void createInstance()
  {
    // Application info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Instance info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // list glfw extensions
    std::cout << "glfw extensions:\n";
    for (uint32_t i = 0; i < glfwExtensionCount; i++)
    {
      std::cout << "\t" << glfwExtensions[i] << std::endl;
    }

    // List available extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    std::cout << "available extensions:\n";

    for (const auto &extension : extensions)
    {
      std::cout << '\t' << extension.extensionName << '\n';
    }

    //check if glfw extensions are in the available extensions
    for (uint32_t i = 0; i < glfwExtensionCount; i++)
    {
      const char* glfwExtension = glfwExtensions[i];
      bool found = false;
      for (const auto &extension : extensions)
      {
        if (strcmp(glfwExtension, extension.extensionName) == 0)
        {
          found = true;
          break;
        }
      }
      if (!found) {
        char msg[256];
        snprintf(msg, sizeof(msg), "glfw extension %s not found", glfwExtension);
        throw std::runtime_error("glfw extension not found");
      }
    }
    std::cout << "All glfw extensions found\n";

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    // Create the Vulkan instance
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
      throw std::runtime_error("failed to create instance!");


  };

  void mainLoop()
  {
    while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
    }
  };

  void cleanup()
  {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
  };
};

int main()
{
  HelloTriangleApplication app;

  try
  {
    app.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
