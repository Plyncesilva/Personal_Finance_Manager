#include <ExpenseRegister.hh>

#include <iostream>

ExpenseRegister::ExpenseRegister() : m_button("Hello World") {
  // Sets the border width of the window.
  set_border_width(1000);

  m_button.signal_clicked().connect(sigc::mem_fun(*this, &ExpenseRegister::on_button_clicked));

  add(m_button);
  m_button.show();
}

ExpenseRegister::~ExpenseRegister(){
}

void ExpenseRegister::on_button_clicked(){
  std::cout << "Hello World" << std::endl;
}