#pragma once

#include <Command.hh>

class CommandInvoker {
    private:
        Command *command;

    public:
        
        void set_command(Command *c);
        void execute_command();
};