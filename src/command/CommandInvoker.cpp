#include <CommandInvoker.hh>

void CommandInvoker::set_command(Command *c){
    command = c;
}

void CommandInvoker::execute_command(){
    command->execute();
}