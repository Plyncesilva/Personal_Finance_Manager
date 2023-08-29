#pragma once

#include <iostream>
#include <gtkmm-3.0/gtkmm/window.h>
#include <gtkmm-3.0/gtkmm/button.h>

class ExpenseRegister : public Gtk::Window {

    public:
        ExpenseRegister();
        virtual ~ExpenseRegister();
    
    protected:
        void on_button_clicked();

        // Member widgets
        Gtk::Button m_button;
};

