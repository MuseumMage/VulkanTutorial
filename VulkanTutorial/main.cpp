//
//  main.cpp
//  VulkanTutorial
//
//  Created by 张博 on 2023/5/24.
//

#include <iostream>
#include "HelloTriangleApplication.h"

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
