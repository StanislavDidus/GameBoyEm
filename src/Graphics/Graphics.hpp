#pragma once

#include "Utils/Log.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <glad/glad.h>

namespace dmg
{
    inline void error_callback(int error, const char* description)
    {
        DMG_ERROR("Error happened in Graphics Context: {}", description);
    }

    inline void input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    struct GraphicsContext
    {
        GLFWwindow* window = nullptr;
    };

    inline GraphicsContext InitializeGraphicsContext()
    {
        GraphicsContext graphics_context{};

        if (!glfwInit())
        {
            DMG_ERROR("GLFW Initialization failed.");
            throw std::runtime_error{"GLFW Initialization failed."};
        }

        glfwSetErrorCallback(error_callback);

        GLFWwindow* window = glfwCreateWindow(640, 480, "Game Boy Emulator", nullptr, nullptr);
        if (!window)
        {
            DMG_ERROR("GLFW Window could not be created.");
            throw std::runtime_error{"GLFW Window could not be created."};
        }

        graphics_context.window = window;

        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            DMG_ERROR("Failed to initialize GLFW Context.");
            throw std::runtime_error{"Failed to initialize GLFW Context."};
        }

        glfwSetKeyCallback(window, input_callback);

        DMG_INFO("Graphics Context was created.");

        return graphics_context;
    }

    inline void DestroyGraphicsContext(GraphicsContext& graphics_context)
    {
        glfwDestroyWindow(graphics_context.window);
        glfwTerminate();

        DMG_INFO("Graphics Context was destroyed.");
    }
}