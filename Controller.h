#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <map>
#include <algorithm>
#include <functional>

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Controller
{

    public:

        Controller();
        virtual ~Controller();

        void bind(const int key, std::function<void()> action);
        void unbind(const int key);
        void unbind();

        void check(GLFWwindow* w);

    private:

        std::map<const int, std::function<void()>> m__keyBindings;

};

#endif
